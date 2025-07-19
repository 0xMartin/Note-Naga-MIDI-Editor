#include "project_data.h"

#include <QFileInfo>
#include <QString>
#include <memory>
#include <iostream>
#include <algorithm>
#include <QDebug>

// ---------- Note Naga MIDI Sequence ----------
NoteNagaMIDISeq::NoteNagaMIDISeq()
{
    this->sequence_id = rand();
    this->clear();
}

NoteNagaMIDISeq::NoteNagaMIDISeq(int sequence_id)
{
    this->sequence_id = sequence_id;
    this->clear();
}

NoteNagaMIDISeq::NoteNagaMIDISeq(int sequence_id, std::vector<std::shared_ptr<NoteNagaTrack>> tracks)
{
    this->sequence_id = sequence_id;
    this->clear();
    this->tracks = std::move(tracks);
}

void NoteNagaMIDISeq::clear() {
    std::cout << "AppContext: Clearing context" << std::endl;
    tracks.clear();
    ppq = 480;
    tempo = 500000;
    midi_file = nullptr;
    active_track_id.reset();
    current_tick = 0;
    max_tick = 0;
}

void NoteNagaMIDISeq::set_active_track_id(std::optional<int> track_id)
{
    if (!track_id.has_value()) {
        this->active_track_id.reset();
        return;
    }
    if (track_id < 0 || track_id >= tracks.size()) {
        std::cerr << "Invalid track ID: " << track_id.value() << std::endl;
        return;
    }
    this->active_track_id = track_id;
    NN_QT_EMIT(active_track_changed_signal(track_id.value()));
}

std::shared_ptr<NoteNagaTrack> NoteNagaMIDISeq::get_active_track()
{
    if (active_track_id.has_value()) {
        return get_track_by_id(*active_track_id);
    }
    return nullptr;
}

std::shared_ptr<NoteNagaTrack> NoteNagaMIDISeq::get_track_by_id(int track_id)
{
    auto it = std::find_if(tracks.begin(), tracks.end(), [track_id](const std::shared_ptr<NoteNagaTrack>& tr) {
        return tr->get_id() == track_id;
    });
    if (it != tracks.end()) return *it;
    return nullptr;
}

int NoteNagaMIDISeq::compute_max_tick() {
    max_tick = 0;
    for (const auto& track : tracks) {
        for (const auto& note : track->get_notes()) {
            if (note.start.has_value() && note.length.has_value())
                max_tick = std::max(max_tick, note.start.value() + note.length.value());
        }
    }
    return max_tick;
}

void NoteNagaMIDISeq::load_from_midi(const QString& midi_file_path) {
    // Check for empty path
    if (midi_file_path.isEmpty()) {
        std::cout << "AppContext: No MIDI file path provided." << std::endl;
        return;
    }
    QFileInfo fi(midi_file_path);
    if (!fi.exists()) {
        std::cout << "AppContext: MIDI file " << midi_file_path.toStdString() << " does not exist." << std::endl;
        return;
    }

    std::cout << "AppContext: Loading MIDI file from " << midi_file_path.toStdString() << std::endl;
    clear();

    std::shared_ptr<MidiFile> midiFile = std::make_shared<MidiFile>();
    if (!midiFile->load(midi_file_path.toStdString())) {
        std::cout << "AppContext: Failed to load MIDI file." << std::endl;
        return;
    }
    this->midi_file = midiFile;
    this->ppq = midiFile->header.division;

    // Split logic for type 0 and type 1 into helper methods
    std::vector<std::shared_ptr<NoteNagaTrack>> tracks_tmp;
    if (midiFile->header.format == 0 && midiFile->getNumTracks() == 1) {
        tracks_tmp = load_type0_tracks(*midiFile);
    } else {
        tracks_tmp = load_type1_tracks(*midiFile);
    }

    // Set the tracks
    this->tracks = tracks_tmp;
    compute_max_tick();

    // Set the active track
    if (!tracks.empty()) {
        active_track_id = tracks[0]->get_id();
    } else {
        active_track_id.reset();
    }

    // signals
    for (const std::shared_ptr<NoteNagaTrack>& track : this->tracks) {
        connect(track.get(), &NoteNagaTrack::meta_changed_signal, this, &NoteNagaMIDISeq::track_meta_changed_signal);
    }
}

// --- Helper: load type 0 MIDI file (split channels) ---
std::vector<std::shared_ptr<NoteNagaTrack>> NoteNagaMIDISeq::load_type0_tracks(const MidiFile& midiFile) {
    qDebug() << "NoteNagaMIDISequence: Loading type 0 MIDI file with " << midiFile.getNumTracks() << " tracks.";
    std::vector<std::shared_ptr<NoteNagaTrack>> tracks_tmp;

    // Only one track - need to split by MIDI channel
    const MidiTrack& track = midiFile.getTrack(0);
    int abs_time = 0;
    std::map<std::pair<int,int>, std::pair<int,int>> notes_on; // (note, channel) -> (start, velocity)
    std::map<int, std::vector<NoteNagaNote>> channel_note_buffers;
    std::map<int, int> channel_instruments;
    std::map<int, QString> channel_names;

    int tempo = 500000;

    // Parse all events and group notes per channel
    for (const auto& evt : track.events) {
        abs_time += evt.delta_time;
        // Track name: store for all channels
        if (evt.type == MidiEventType::Meta && evt.meta_type == MIDI_META_TRACK_NAME) {
            std::string track_name(evt.meta_data.begin(), evt.meta_data.end());
            size_t endpos = track_name.find_last_not_of('\0');
            if (endpos != std::string::npos)
                track_name = track_name.substr(0, endpos + 1);
            for (int ch = 0; ch < 16; ++ch) {
                channel_names[ch] = QString::fromStdString(track_name);
            }
        }
        // Program change: store instrument per channel
        if (evt.type == MidiEventType::ProgramChange && !evt.data.empty()) {
            channel_instruments[evt.channel] = evt.data[0];
        }
        // Tempo change: only once
        if (evt.type == MidiEventType::Meta && evt.meta_type == MIDI_META_SET_TEMPO) {
            if (evt.meta_data.size() == 3) {
                tempo = (evt.meta_data[0] << 16) | (evt.meta_data[1] << 8) | evt.meta_data[2];
            }
        }
        // Note on: register note start per channel
        if (evt.type == MidiEventType::NoteOn && !evt.data.empty() && evt.data[1] > 0) {
            int note = evt.data[0];
            int velocity = evt.data[1];
            int channel = evt.channel;
            notes_on[std::make_pair(note, channel)] = std::make_pair(abs_time, velocity);
        }
        // Note off: finish note per channel
        else if ((evt.type == MidiEventType::NoteOff) || (evt.type == MidiEventType::NoteOn && !evt.data.empty() && evt.data[1] == 0)) {
            int note = evt.data[0];
            int channel = evt.channel;
            auto key = std::make_pair(note, channel);
            auto it = notes_on.find(key);
            if (it != notes_on.end()) {
                int start = it->second.first;
                int velocity = it->second.second;
                channel_note_buffers[channel].push_back(
                    NoteNagaNote(note, start, abs_time - start, velocity)
                );
                notes_on.erase(it);
            }
        }
    }

    // Create Track for each used channel
    int t_id = 0;
    for (auto& pair : channel_note_buffers) {
        int channel = pair.first;
        std::vector<NoteNagaNote>& note_buffer = pair.second;
        if (note_buffer.empty()) continue;

        QString name = channel_names.count(channel) ? channel_names[channel] : QString("Channel %1").arg(channel + 1);
        int instrument = channel_instruments.count(channel) ? channel_instruments[channel] : 0;

        qDebug() << "> Track ID:" << t_id
                 << "Channel:" << channel
                 << "Name:" << name
                 << "Instrument:" << instrument
                 << "Notes:" << note_buffer.size();
        auto track_info = std::make_shared<NoteNagaTrack>(
            t_id,
            name,
            instrument,
            channel
        );
        std::sort(note_buffer.begin(), note_buffer.end(), [](const NoteNagaNote& a, const NoteNagaNote& b) { 
            return a.start < b.start; 
        });
        track_info->set_notes(note_buffer);
        tracks_tmp.push_back(track_info);
        ++t_id;
    }
    this->tempo = tempo;
    return tracks_tmp;
}

// --- Helper: load type 1 MIDI file (one track per chunk) ---
std::vector<std::shared_ptr<NoteNagaTrack>> NoteNagaMIDISeq::load_type1_tracks(const MidiFile& midiFile) {
    qDebug() << "NoteNagaMIDISequence: Loading type 1 MIDI file with " << midiFile.getNumTracks() << " tracks.";
    std::vector<std::shared_ptr<NoteNagaTrack>> tracks_tmp;

    int tempo = 500000;

    for (int track_idx = 0; track_idx < midiFile.getNumTracks(); ++track_idx) {
        const MidiTrack& track = midiFile.getTrack(track_idx);

        std::map<std::pair<int,int>, std::pair<int,int>> notes_on; // (note, channel) -> (start, velocity)
        int abs_time = 0;
        int instrument = 0;
        std::optional<int> channel_used;
        QString name;
        std::vector<NoteNagaNote> note_buffer;

        // Parse events for this track
        for (const auto& evt : track.events) {
            abs_time += evt.delta_time;

            // Track name
            if (evt.type == MidiEventType::Meta && evt.meta_type == MIDI_META_TRACK_NAME) {
                std::string track_name(evt.meta_data.begin(), evt.meta_data.end());
                size_t endpos = track_name.find_last_not_of('\0');
                if (endpos != std::string::npos)
                    track_name = track_name.substr(0, endpos + 1);
                name = QString::fromStdString(track_name); 
            }
            // Program change: store instrument
            if (evt.type == MidiEventType::ProgramChange) {
                if (!evt.data.empty()) {
                    instrument = evt.data[0];
                    if (!channel_used.has_value())
                        channel_used = evt.channel;
                }
            }
            // Tempo change: only from first track
            if (evt.type == MidiEventType::Meta && evt.meta_type == MIDI_META_SET_TEMPO && track_idx == 0) {
                if (evt.meta_data.size() == 3) {
                    tempo = (evt.meta_data[0] << 16) | (evt.meta_data[1] << 8) | evt.meta_data[2];
                }
            }
            // Note on
            if (evt.type == MidiEventType::NoteOn && !evt.data.empty() && evt.data[1] > 0) {
                int note = evt.data[0];
                int velocity = evt.data[1];
                int channel = evt.channel;
                if (!channel_used.has_value())
                    channel_used = channel;
                notes_on[std::make_pair(note, channel)] = std::make_pair(abs_time, velocity);
            }
            // Note off
            else if ((evt.type == MidiEventType::NoteOff) || (evt.type == MidiEventType::NoteOn && !evt.data.empty() && evt.data[1] == 0)) {
                int note = evt.data[0];
                int channel = evt.channel;
                auto key = std::make_pair(note, channel);
                auto it = notes_on.find(key);
                if (it != notes_on.end()) {
                    int start = it->second.first;
                    int velocity = it->second.second;
                    note_buffer.push_back(
                        NoteNagaNote(note, start, abs_time - start, velocity)
                    );
                    notes_on.erase(it);
                }
            }
        }

        auto track_info = std::make_shared<NoteNagaTrack>(
            track_idx,
            name.isEmpty() ? QString("Track %1").arg(track_idx + 1) : name,
            instrument,
            channel_used
        );
        std::sort(note_buffer.begin(), note_buffer.end(), [](const NoteNagaNote& a, const NoteNagaNote& b) { 
            return a.start < b.start; 
        });
        track_info->set_notes(note_buffer);
        tracks_tmp.push_back(track_info);
    }
    this->tempo = tempo;
    return tracks_tmp;
}

void NoteNagaMIDISeq::set_id(int new_id) {
    if (this->sequence_id == new_id)
        return;
    sequence_id = new_id;
    NN_QT_EMIT(meta_changed_signal(this->sequence_id, "id"));
}

void NoteNagaMIDISeq::set_ppq(int ppq) {
    if (this->ppq == ppq)
        return;
    this->ppq = ppq;
    NN_QT_EMIT(meta_changed_signal(this->sequence_id, "ppq"));
}

void NoteNagaMIDISeq::set_tempo(int tempo) {
    if (this->tempo == tempo)
        return;
    this->tempo = tempo;
    NN_QT_EMIT(meta_changed_signal(this->sequence_id, "tempo"));
}

void NoteNagaMIDISeq::set_current_tick(int tick) {
    if (this->current_tick == tick)
        return;
    current_tick = tick;
    NN_QT_EMIT(meta_changed_signal(this->sequence_id, "current_tick"));
}

void NoteNagaMIDISeq::set_active_track_id(std::optional<int> track_id) {
    if (this->active_track_id == track_id)
        return;
    active_track_id = track_id;
    NN_QT_EMIT(meta_changed_signal(this->sequence_id, "active_track_id"));
}

void NoteNagaMIDISeq::set_solo_track_id(std::optional<int> track_id) {
    if (this->solo_track_id == track_id)
        return;
    solo_track_id = track_id;
    NN_QT_EMIT(meta_changed_signal(this->sequence_id, "solo_track_id"));
}

// ---------- Note Naga Project Data ----------

NoteNagaProject::NoteNagaProject() {
    // Initialize with empty sequences
    sequences.clear();
    active_sequence_id.reset();
    current_tick = 0;
    max_tick = 0;
}

bool NoteNagaProject::load_project(const QString &project_path)
{
    // Check for empty path
    if (project_path.isEmpty()) 
    {
        return false;
    }

    // Create a new MIDI sequence
    std::shared_ptr<NoteNagaMIDISeq> sequence = std::make_shared<NoteNagaMIDISeq>();
    sequence->load_from_midi(project_path);
    add_sequence(sequence);

    // Set the active sequence to the first one
    if (!sequences.empty()) {
        active_sequence_id = sequences[0]->get_id();
    } else {
        active_sequence_id.reset();
    }

    // signals
    connect(sequence.get(), &NoteNagaMIDISeq::meta_changed_signal, this, &NoteNagaProject::sequence_meta_changed_signal);

    NN_QT_EMIT(this->project_file_loaded_signal());

    return true;
}

void NoteNagaProject::add_sequence(const std::shared_ptr<NoteNagaMIDISeq>& sequence) {
    if (sequence) {
        sequences.push_back(sequence);
        if (!active_sequence_id.has_value()) {
            active_sequence_id = sequence->get_track_by_id(0)->get_id(); 
        }
    }
}

void NoteNagaProject::remove_sequence(const std::shared_ptr<NoteNagaMIDISeq>& sequence) {
    if (sequence) {
        auto it = std::remove(sequences.begin(), sequences.end(), sequence);
        if (it != sequences.end()) {
            sequences.erase(it, sequences.end());
            // Reset active sequence if it was removed
            if (active_sequence_id.has_value() && *active_sequence_id == sequence->get_track_by_id(0)->get_id()) {
                active_sequence_id.reset();
            }
        }
    }
}

int NoteNagaProject::compute_max_tick()
{
    // implement
    return 0.0;
}

void NoteNagaProject::set_active_sequence_id(std::optional<int> sequence_id) 
{ 
    if (!sequence_id.has_value()) {
        active_sequence_id.reset();
        return;
    }
    if (sequence_id < 0 || sequence_id >= sequences.size()) {
        std::cerr << "Invalid sequence ID: " << sequence_id.value() << std::endl;
        return;
    }
    active_sequence_id = sequence_id; 
    NN_QT_EMIT(active_sequence_changed_signal(sequence_id.value()));
}

std::shared_ptr<NoteNagaMIDISeq> NoteNagaProject::get_active_sequence() const
{
    if (active_sequence_id.has_value()) {
        auto it = std::find_if(sequences.begin(), sequences.end(),
            [this](const std::shared_ptr<NoteNagaMIDISeq>& seq) {
                return seq->get_id() == *active_sequence_id;
            });
        if (it != sequences.end()) {
            return *it;
        }
    }
    return nullptr;
}
