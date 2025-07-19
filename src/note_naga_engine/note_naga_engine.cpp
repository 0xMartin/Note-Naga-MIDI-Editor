#include "note_naga_engine.h"


NoteNagaEngine::NoteNagaEngine(QObject* parent)
    : QObject(parent)
    , project_data(std::make_shared<NoteNagaProjectData>())
    , mixer(std::make_unique<Mixer>())
    , playback_worker(std::make_unique<PlaybackWorker>(this))
{
}

NoteNagaEngine::~NoteNagaEngine()
{
    // Cleanup if necessary
}

bool NoteNagaEngine::init()
{
    // (Re)initialize engine state, connections, etc.
    // Possibly connect playback_worker signals to this engine for relaying
    // Connect playback note events to signals
    connect(playback_worker.get(), &PlaybackWorker::playing_note,
            this, [this](const MidiNote& note, int track_id) {
                NN_QT_EMIT(playing_note_signal(note, track_id));
            });
    connect(playback_worker.get(), &PlaybackWorker::mixer_playing_note,
            this, [this](const MidiNote& note, const QString& device_name, int channel) {
                NN_QT_EMIT(mixer_playing_note_signal(note, device_name, channel));
            });

    // Initialize mixer and playback worker with project data if needed
    // ...

    return true;
}

bool NoteNagaEngine::load_project(const QString &midi_file_path)
{
    return false;
}

void NoteNagaEngine::start_playback()
{
    if (playback_worker)
        playback_worker->start();
}

void NoteNagaEngine::stop_playback()
{
    if (playback_worker)
        playback_worker->stop();
}

void NoteNagaEngine::set_playback_position(int tick)
{
    if (playback_worker)
        playback_worker->set_position(tick);
}

void NoteNagaEngine::mute_track(int track_id, bool mute)
{
    if (mixer)
        mixer->mute_track(track_id, mute);
}

void NoteNagaEngine::solo_track(int track_id, bool solo)
{
    if (mixer)
        mixer->solo_track(track_id, solo);
}

NoteNagaProjectData* NoteNagaEngine::get_project_data()
{
    return project_data.get();
}

void NoteNagaEngine::set_project_data(const std::shared_ptr<NoteNagaProjectData>& data)
{
    project_data = data;
    // Reinitialize mixer and playback worker with new project data if needed
}

Mixer* NoteNagaEngine::get_mixer()
{
    return mixer.get();
}

PlaybackWorker* NoteNagaEngine::get_playback_worker()
{
    return playback_worker.get();
}

std::optional<int> NoteNagaEngine::get_active_sequence_id() const
{
    if (project_data)
        return project_data->active_sequence_id;
    return std::nullopt;
}

void NoteNagaEngine::set_active_sequence_id(int sequence_id)
{
    if (project_data)
        project_data->active_sequence_id = sequence_id;
    NN_QT_EMIT(selected_track_changed_signal(sequence_id));
}

std::shared_ptr<NoteNagaMIDISequence> NoteNagaEngine::get_active_sequence() const
{
    if (!project_data)
        return nullptr;
    auto id = project_data->active_sequence_id;
    if (id && *id < static_cast<int>(project_data->sequences.size()))
        return project_data->sequences[*id];
    return nullptr;
}

std::shared_ptr<Track> NoteNagaEngine::get_track_by_id(int track_id) const
{
    auto seq = get_active_sequence();
    if (seq)
        return seq->get_track_by_id(track_id);
    return nullptr;
}