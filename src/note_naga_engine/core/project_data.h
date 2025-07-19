#pragma once

#include <QColor>
#include <QString>
#include <vector>
#include <memory>
#include <optional>
#include <QVariant>

#include "../midi/midi_file.h"
#include "types.h"

// ---------- Note Naga MIDI Sequence ----------
class NoteNagaMIDISequence {

public:
    NoteNagaMIDISequence();
    NoteNagaMIDISequence(int sequence_id);
    NoteNagaMIDISequence(int sequence_id, std::vector<std::shared_ptr<Track>> tracks);

    void clear();
    std::shared_ptr<Track> get_track_by_id(int track_id);
    int compute_max_tick();

    void load_from_midi(const QString& midi_file_path);
    std::vector<std::shared_ptr<Track>> load_type0_tracks(const MidiFile& midiFile);
    std::vector<std::shared_ptr<Track>> load_type1_tracks(const MidiFile& midiFile);

    int get_sequence_id() const { return sequence_id; }

    int get_ppq() const { return ppq; }
    void set_ppq(int ppq) { this->ppq = ppq; }

    int get_tempo() const { return tempo; }
    void set_tempo(int tempo) { this->tempo = tempo; }

    int get_current_tick() const { return current_tick; }
    void set_current_tick(int tick) { current_tick = tick; }

    int get_max_tick() const { return max_tick; }

    std::optional<int> get_active_track_id() const { return active_track_id; }
    void set_active_track_id(int track_id) { active_track_id = track_id; }

    std::vector<std::shared_ptr<Track>> get_tracks() const { return tracks; }

    std::shared_ptr<MidiFile> get_midi_file() const { return midi_file; }

protected:
    int sequence_id;
    
    std::vector<std::shared_ptr<Track>> tracks;
    std::optional<int> active_track_id;
    std::optional<int> solo_track_id;
    std::shared_ptr<MidiFile> midi_file; 

    int ppq;
    int tempo;
    int current_tick;
    int max_tick;
};

// ---------- Note Naga Project Data ----------
class NoteNagaProjectData {
public:
    NoteNagaProjectData();

    bool load_project(const QString& project_path);

    void add_sequence(const std::shared_ptr<NoteNagaMIDISequence>& sequence);
    void remove_sequence(const std::shared_ptr<NoteNagaMIDISequence>& sequence);

    std::optional<int> get_active_sequence_id() const { return active_sequence_id; }

    std::vector<std::shared_ptr<NoteNagaMIDISequence>> get_sequences() const { return sequences; }

protected:
    std::vector<std::shared_ptr<NoteNagaMIDISequence>> sequences;
    std::optional<int> active_sequence_id;
};