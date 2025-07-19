#pragma once

#include <QObject>
#include <vector>
#include <memory>
#include <optional>
#include <QString>

#include "core/project_data.h"
#include "core/types.h"
#include "core/mixer.h"
#include "worker/playback_worker.h"

class NoteNagaEngine : public QObject
{
    Q_OBJECT

public:
    explicit NoteNagaEngine(QObject* parent = nullptr);
    ~NoteNagaEngine();

    // --- Initialization ---
    bool init();
    bool load_project(const QString& midi_file_path);

    // --- Playback Control ---
    void start_playback();
    void stop_playback();
    void set_playback_position(int tick);

    // --- Mixer Control ---
    void mute_track(int track_id, bool mute = true);
    void solo_track(int track_id, bool solo = true);

    // --- Getters & Setters ---
    NoteNagaProjectData* get_project_data();
    void set_project_data(const std::shared_ptr<NoteNagaProjectData>& data);

    Mixer* get_mixer();
    PlaybackWorker* get_playback_worker();

    std::optional<int> get_active_sequence_id() const;
    void set_active_sequence_id(int sequence_id);

    // --- Utility ---
    std::shared_ptr<NoteNagaMIDISequence> get_active_sequence() const;
    std::shared_ptr<Track> get_track_by_id(int track_id) const;

    // --- Signals ---
Q_SIGNALS:
    void project_file_loaded_signal();

    void sequence_meta_changed_signal(int sequence_id);
    void selected_sequence_changed_signal(int sequence_id);

    void track_meta_changed_signal(int track_id);
    void selected_track_changed_signal(int track_id);

    void playing_note_signal(const MidiNote& note, int track_id);
    void mixer_playing_note_signal(const MidiNote& note, const QString& device_name, int channel);

protected:
    std::shared_ptr<NoteNagaProjectData> project_data;
    std::unique_ptr<Mixer> mixer;
    std::unique_ptr<PlaybackWorker> playback_worker;
};
