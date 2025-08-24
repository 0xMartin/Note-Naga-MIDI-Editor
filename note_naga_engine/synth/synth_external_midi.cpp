#include "note_naga_engine/core/types.h"
#include <note_naga_engine/synth/synth_external_midi.h>
#include <note_naga_engine/logger.h>

#include <algorithm>
#include <cmath>

NoteNagaSynthExternalMidi::NoteNagaSynthExternalMidi(const std::string &name, 
                                                     const std::string &port_name)
    : NoteNagaSynthesizer(name), 
      midi_out_(std::make_unique<RtMidiOut>(RtMidi::UNSPECIFIED, "NoteNagaEngine")),
      is_connected_(false),
      current_port_name_(port_name)
{
    // Initialize RtMidi
    try {
        if (!port_name.empty()) {
            setMidiOutputPort(port_name);
        } else {
            // Try to connect to the first available MIDI port
            if (midi_out_->getPortCount() > 0) {
                midi_out_->openPort(0);
                current_port_name_ = midi_out_->getPortName(0);
                is_connected_ = true;
                NOTE_NAGA_LOG_INFO("External MIDI synthesizer connected to port: " + current_port_name_);
            } else {
                NOTE_NAGA_LOG_WARNING("No MIDI output ports available");
            }
        }
    } catch (RtMidiError &error) {
        NOTE_NAGA_LOG_ERROR("RtMidi error: " + error.getMessage());
        is_connected_ = false;
    }

    // Initialize channel and program mappings
    for (int i = 0; i < 16; ++i) {
        channel_programs_[i] = -1;
        channel_pan_[i] = 0.0f;
    }
}

NoteNagaSynthExternalMidi::~NoteNagaSynthExternalMidi() {
    std::lock_guard<std::mutex> lock(synth_mutex_);
    
    // Stop all sounds before terminating
    stopAllNotes();
    
    // Close MIDI port and release resources
    if (midi_out_ && is_connected_) {
        midi_out_->closePort();
    }
}

bool NoteNagaSynthExternalMidi::ensureMidiOutput() {
    if (is_connected_) return true;
    
    try {
        // If the synthesizer is not connected, try to connect to the first available port
        if (midi_out_->getPortCount() > 0) {
            midi_out_->openPort(0);
            current_port_name_ = midi_out_->getPortName(0);
            is_connected_ = true;
            NOTE_NAGA_LOG_INFO("External MIDI synthesizer connected to port: " + current_port_name_);
            return true;
        } else {
            NOTE_NAGA_LOG_WARNING("No MIDI output ports available");
            return false;
        }
    } catch (RtMidiError &error) {
        NOTE_NAGA_LOG_ERROR("RtMidi error during connection: " + error.getMessage());
        is_connected_ = false;
        return false;
    }
}

std::vector<std::string> NoteNagaSynthExternalMidi::getAvailableMidiOutputPorts() {
    std::vector<std::string> ports;
    
    try {
        RtMidiOut midi;
        unsigned int port_count = midi.getPortCount();
        
        for (unsigned int i = 0; i < port_count; ++i) {
            ports.push_back(midi.getPortName(i));
        }
    } catch (RtMidiError &error) {
        NOTE_NAGA_LOG_ERROR("RtMidi error while getting port list: " + error.getMessage());
    }
    
    return ports;
}

bool NoteNagaSynthExternalMidi::setMidiOutputPort(const std::string &port_name) {
    if (is_connected_) {
        midi_out_->closePort();
        is_connected_ = false;
    }
    
    try {
        // Find port by name
        unsigned int port_count = midi_out_->getPortCount();
        for (unsigned int i = 0; i < port_count; ++i) {
            if (midi_out_->getPortName(i) == port_name) {
                midi_out_->openPort(i);
                current_port_name_ = port_name;
                is_connected_ = true;
                NOTE_NAGA_LOG_INFO("External MIDI synthesizer connected to port: " + port_name);
                return true;
            }
        }
        
        NOTE_NAGA_LOG_WARNING("MIDI port '" + port_name + "' not found");
        return false;
    } catch (RtMidiError &error) {
        NOTE_NAGA_LOG_ERROR("RtMidi error when setting port: " + error.getMessage());
        is_connected_ = false;
        return false;
    }
}

void NoteNagaSynthExternalMidi::sendControlChange(int channel, int controller, int value) {
    if (!ensureMidiOutput()) return;
    
    std::vector<unsigned char> message;
    message.push_back(0xB0 + channel);  // Control Change on specified channel
    message.push_back(controller);      // Controller number
    message.push_back(std::clamp(value, 0, 127)); // Value (0-127)
    
    try {
        midi_out_->sendMessage(&message);
    } catch (RtMidiError &error) {
        NOTE_NAGA_LOG_ERROR("RtMidi error when sending CC: " + error.getMessage());
        is_connected_ = false;
    }
}

void NoteNagaSynthExternalMidi::sendProgramChange(int channel, int program) {
    if (!ensureMidiOutput()) return;
    
    std::vector<unsigned char> message;
    message.push_back(0xC0 + channel);  // Program Change on specified channel
    message.push_back(std::clamp(program, 0, 127)); // Program number (0-127)
    
    try {
        midi_out_->sendMessage(&message);
    } catch (RtMidiError &error) {
        NOTE_NAGA_LOG_ERROR("RtMidi error when sending Program Change: " + error.getMessage());
        is_connected_ = false;
    }
}

void NoteNagaSynthExternalMidi::playNote(const NN_Note_t &note, int channel, float pan) {
    std::lock_guard<std::mutex> lock(synth_mutex_);

    if (!note.velocity.has_value() || note.velocity.value() <= 0) return;
    
    NoteNagaTrack *track = note.parent;
    if (!track) return;
    
    // If not connected to MIDI, try to establish connection
    if (!ensureMidiOutput()) return;
    
    // Get program for the track (parent of note)
    int prog = track->getInstrument().value_or(0);
    
    // Set program change if needed
    if (channel_programs_[channel] != prog) {
        sendProgramChange(channel, prog);
        channel_programs_[channel] = prog;
    }
    
    // Set pan if needed
    if (std::abs(channel_pan_[channel] - pan) > 0.01f) {
        int midiPan = static_cast<int>(std::round(pan * 63.5 + 63.5));
        sendControlChange(channel, 10, std::clamp(midiPan, 0, 127));
        channel_pan_[channel] = pan;
    }
    
    // Check if note is already playing
    if (playing_notes_[track].find(note.id) != playing_notes_[track].end()) {
        return;
    }
    
    // Send Note On message
    std::vector<unsigned char> message;
    message.push_back(0x90 + channel); // Note On on specified channel
    message.push_back(note.note);      // Note number
    message.push_back(note.velocity.value_or(100)); // Velocity
    
    try {
        midi_out_->sendMessage(&message);
        
        // Store the note in playing_notes_ for later stop
        playing_notes_[track][note.id] = PlayedNote_t{note, channel};
    } catch (RtMidiError &error) {
        NOTE_NAGA_LOG_ERROR("RtMidi error when sending Note On: " + error.getMessage());
        is_connected_ = false;
    }
}

void NoteNagaSynthExternalMidi::stopNote(const NN_Note_t &note) {
    NoteNagaTrack *track = note.parent;
    if (!track) return;
    
    if (!ensureMidiOutput()) return;
    
    // Find note in playing notes by ID
    TrackNotesMap &playingTrackNotes = playing_notes_[track];
    auto it = playingTrackNotes.find(note.id);
    
    // Retrieve note parameters and stop it
    if (it != playingTrackNotes.end()) {
        const PlayedNote_t &pn = it->second;
        
        // Send Note Off message
        std::vector<unsigned char> message;
        message.push_back(0x80 + pn.channel); // Note Off on specified channel
        message.push_back(pn.note.note);      // Note number
        message.push_back(0);                 // Velocity for Note Off
        
        try {
            midi_out_->sendMessage(&message);
            playingTrackNotes.erase(it);
        } catch (RtMidiError &error) {
            NOTE_NAGA_LOG_ERROR("RtMidi error when sending Note Off: " + error.getMessage());
            is_connected_ = false;
        }
    }
}

void NoteNagaSynthExternalMidi::stopAllNotes(NoteNagaMidiSeq *seq, NoteNagaTrack *track) {
    if (!ensureMidiOutput()) return;
    
    // Stop notes according to function parameters
    if (track) {
        // Stop notes for specified track
        for (const auto &[id, pn] : playing_notes_[track]) {
            std::vector<unsigned char> message;
            message.push_back(0x80 + pn.channel); // Note Off on specified channel
            message.push_back(pn.note.note);      // Note number
            message.push_back(0);                 // Velocity for Note Off
            
            try {
                midi_out_->sendMessage(&message);
            } catch (RtMidiError &error) {
                NOTE_NAGA_LOG_ERROR("RtMidi error when sending Note Off: " + error.getMessage());
                is_connected_ = false;
                break;
            }
        }
        playing_notes_[track].clear();
    } else if (seq) {
        // Stop notes for the entire sequence
        for (auto &tr : seq->getTracks()) {
            if (tr) stopAllNotes(nullptr, tr);
        }
    } else {
        // Stop all notes
        for (auto &[track, notes] : playing_notes_) {
            for (const auto &[id, pn] : notes) {
                std::vector<unsigned char> message;
                message.push_back(0x80 + pn.channel); // Note Off on specified channel
                message.push_back(pn.note.note);      // Note number
                message.push_back(0);                 // Velocity for Note Off
                
                try {
                    midi_out_->sendMessage(&message);
                } catch (RtMidiError &error) {
                    NOTE_NAGA_LOG_ERROR("RtMidi error when sending Note Off: " + error.getMessage());
                    is_connected_ = false;
                    break;
                }
            }
            notes.clear();
        }
        
        // Alternatively, we can send MIDI "All Notes Off" message to all channels
        if (is_connected_) {
            for (int channel = 0; channel < 16; ++channel) {
                sendControlChange(channel, 123, 0); // CC 123 = All Notes Off
            }
        }
    }
}

std::string NoteNagaSynthExternalMidi::getConfig(const std::string &key) const {
    if (key == "port") {
        return current_port_name_;
    }
    return "";
}

bool NoteNagaSynthExternalMidi::setConfig(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> lock(synth_mutex_);

    if (key == "port") {
        NN_QT_EMIT(synthUpdated(this));
        return setMidiOutputPort(value);
    }
    return false;
}

std::vector<std::string> NoteNagaSynthExternalMidi::getSupportedConfigKeys() const {
    return {"port"};
}