#pragma once

#include <note_naga_engine/core/note_naga_synthesizer.h>
#include <note_naga_engine/core/types.h>
#include <RtMidi.h>
#include <string>
#include <vector>
#include <memory>

/**
 * External MIDI synthesizer for NoteNagaEngine.
 * Communicates with external MIDI devices using RtMidi.
 */
class NoteNagaSynthExternalMidi : public NoteNagaSynthesizer {
public:
    /**
     * @brief Constructor for external MIDI synthesizer
     * @param name Name of the synthesizer
     * @param port_name MIDI port name to connect to (empty string = automatic selection)
     */
    NoteNagaSynthExternalMidi(const std::string &name, const std::string &port_name = "");
    ~NoteNagaSynthExternalMidi() override;

    void playNote(const NN_Note_t &note, int channel = 0, float pan = 0.0) override;
    void stopNote(const NN_Note_t &note) override;
    void stopAllNotes(NoteNagaMidiSeq *seq = nullptr, NoteNagaTrack *track = nullptr) override;

    virtual std::string getConfig(const std::string &key) const override;
    virtual bool setConfig(const std::string &key, const std::string &value) override;
    virtual std::vector<std::string> getSupportedConfigKeys() const override;
    
    /**
     * @brief Get a list of available MIDI output ports
     * @return Vector of MIDI port names
     */
    static std::vector<std::string> getAvailableMidiOutputPorts();
    
    /**
     * @brief Set MIDI port for output
     * @param port_name MIDI port name
     * @return True if connection was successful, otherwise False
     */
    bool setMidiOutputPort(const std::string &port_name);

    /**
     * @brief Get current MIDI port name
     * @return Name of the currently connected MIDI port or empty if not connected
     */
    std::string getCurrentPortName() const { return current_port_name_; }

protected:
    /**
     * @brief Ensure that MIDI output is initialized
     * @return True if MIDI output is ready
     */
    bool ensureMidiOutput();
    
    /**
     * @brief Send MIDI Control Change message
     * @param channel MIDI channel (0-15)
     * @param controller Controller number
     * @param value Value (0-127)
     */
    void sendControlChange(int channel, int controller, int value);
    
    /**
     * @brief Send MIDI Program Change message
     * @param channel MIDI channel (0-15)
     * @param program Program number (0-127)
     */
    void sendProgramChange(int channel, int program);

private:
    std::unique_ptr<RtMidiOut> midi_out_; ///< RtMidi output interface
    bool is_connected_;                   ///< Connection status to MIDI port
    std::string current_port_name_;       ///< Currently used MIDI port
};