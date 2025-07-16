#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cstdint>
#include <map>

// --- MIDI Event Types ---
enum class MidiEventType {
    NoteOn,
    NoteOff,
    PolyAftertouch,
    ControlChange,
    ProgramChange,
    ChannelAftertouch,
    PitchBend,
    Meta,
    SysEx,
    Unknown
};

struct MidiEvent {
    uint32_t delta_time = 0;
    MidiEventType type = MidiEventType::Unknown;
    uint8_t channel = 0; // 0-15 for channel events
    std::vector<uint8_t> data; // Event-specific data

    // Meta-specific
    uint8_t meta_type = 0;
    std::vector<uint8_t> meta_data;

    // For SysEx
    std::vector<uint8_t> sysex_data;
};

struct MidiTrack {
    std::vector<MidiEvent> events;
};

struct MidiFileHeader {
    uint16_t format = 1; // 0, 1 or 2
    uint16_t nTracks = 0;
    uint16_t division = 480; // ticks per quarter note
};

class MidiFile {
public:
    MidiFile();
    bool load(const std::string& filename);
    bool save(const std::string& filename) const;

    void clear();
    int getNumTracks() const;
    const MidiTrack& getTrack(int idx) const;
    MidiTrack& getTrack(int idx);

    MidiFileHeader header;

    std::vector<MidiTrack> tracks;

    // Helper: create simple file (single track, simple note events)
    static MidiFile createTestFile();

private:
    static uint32_t readVarLen(std::istream &in);
    static void writeVarLen(std::ostream &out, uint32_t value);

    static uint16_t readBE16(std::istream &in);
    static uint32_t readBE32(std::istream &in);
    static void writeBE16(std::ostream &out, uint16_t value);
    static void writeBE32(std::ostream &out, uint32_t value);

    bool parseHeader(std::istream &in);
    bool parseTrack(std::istream &in, MidiTrack& track);

    bool writeHeader(std::ostream &out) const;
    bool writeTrack(std::ostream &out, const MidiTrack& track) const;
};