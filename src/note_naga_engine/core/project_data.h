#pragma once

#include <QObject>
#include <QColor>
#include <QString>
#include <vector>
#include <memory>
#include <optional>
#include <QVariant>

#include "types.h"
#include "../io/midi_file.h"

// ---------- Note Naga MIDI file Sequence ----------
class NoteNagaMIDISeq : public QObject {
    Q_OBJECT

public:
    NoteNagaMIDISeq();
    NoteNagaMIDISeq(int sequence_id);
    NoteNagaMIDISeq(int sequence_id, std::vector<std::shared_ptr<NoteNagaTrack>> tracks);

    void clear();
    int compute_max_tick();

    void load_from_midi(const QString& midi_file_path);
    std::vector<std::shared_ptr<NoteNagaTrack>> load_type0_tracks(const MidiFile& midiFile);
    std::vector<std::shared_ptr<NoteNagaTrack>> load_type1_tracks(const MidiFile& midiFile);


    int get_id() const { return sequence_id; }
    int get_ppq() const { return ppq; }
    int get_tempo() const { return tempo; }
    int get_current_tick() const { return current_tick; }
    int get_max_tick() const { return max_tick; }
    std::optional<int> get_active_track_id() const { return active_track_id; }
    std::shared_ptr<NoteNagaTrack> get_active_track();
    std::optional<int> get_solo_track_id() const { return solo_track_id; }
    std::vector<std::shared_ptr<NoteNagaTrack>> get_tracks() const { return tracks; }
    std::shared_ptr<NoteNagaTrack> get_track_by_id(int track_id);
    std::shared_ptr<MidiFile> get_midi_file() const { return midi_file; }

    void set_id(int new_id);
    void set_ppq(int ppq);
    void set_tempo(int tempo);
    void set_current_tick(int tick);
    void set_active_track_id(std::optional<int> track_id);
    void set_solo_track_id(std::optional<int> track_id);

Q_SIGNALS:
    void meta_changed_signal(int sequence_id, const QString& param);
    void track_meta_changed_signal(int track_id, const QString& param);
    void active_track_changed_signal(int track_id);

protected:
    int sequence_id;
    
    std::vector<std::shared_ptr<NoteNagaTrack>> tracks;
    std::optional<int> active_track_id;
    std::optional<int> solo_track_id;
    std::shared_ptr<MidiFile> midi_file; 

    int ppq;
    int tempo;
    int current_tick;
    int max_tick;
};

// ---------- Note Naga Project file ----------
class NoteNagaProject : public QObject {
    Q_OBJECT

public:
    NoteNagaProject();

    bool load_project(const QString& project_path);

    void add_sequence(const std::shared_ptr<NoteNagaMIDISeq>& sequence);
    void remove_sequence(const std::shared_ptr<NoteNagaMIDISeq>& sequence);

    int compute_max_tick();

    int get_ppq() const { return ppq; }
    void set_ppq(int ppq) { this->ppq = ppq; }

    int get_tempo() const { return tempo; }
    void set_tempo(int tempo) { this->tempo = tempo; }

    std::optional<int> get_active_sequence_id() const { return active_sequence_id; }
    void set_active_sequence_id(std::optional<int> sequence_id);
    std::shared_ptr<NoteNagaMIDISeq> get_active_sequence() const;

    std::vector<std::shared_ptr<NoteNagaMIDISeq>> get_sequences() const { return sequences; }

Q_SIGNALS:
    void project_file_loaded_signal();

    void sequence_meta_changed_signal(int sequence_id, const QString& param);
    void active_sequence_changed_signal(int sequence_id);
    
protected:
    std::vector<std::shared_ptr<NoteNagaMIDISeq>> sequences;
    std::optional<int> active_sequence_id;

    int ppq;
    int tempo;
    int current_tick;
    int max_tick;
};