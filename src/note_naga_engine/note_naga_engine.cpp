#include "note_naga_engine.h"

NoteNagaEngine::NoteNagaEngine(QObject* parent)
    : QObject(parent)
{
    // set default configuration ...
}

NoteNagaEngine::~NoteNagaEngine()
{
    if (playback_worker) playback_worker->stop();
    if (mixer) mixer->close();
}

bool NoteNagaEngine::init()
{
    this->project_data = std::make_shared<NoteNagaProjectData>();
    this->mixer = std::make_unique<Mixer>(this->project_data);
    this->playback_worker = std::make_unique<PlaybackWorker>(this->project_data, this->mixer.get(), 30.0);
    // configuration loading ...
    return true;
}

bool NoteNagaEngine::load_project(const QString &midi_file_path)
{
    return this->project_data->load_project(midi_file_path);
}

void NoteNagaEngine::start_playback()
{
    if (playback_worker)playback_worker->play();
}

void NoteNagaEngine::stop_playback()
{
    if (playback_worker) playback_worker->stop();
}

void NoteNagaEngine::set_playback_position(int tick)
{
    if (playback_worker) playback_worker->set_(tick);
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

std::shared_ptr<NoteNagaProjectData> NoteNagaEngine::get_project_data()
{
    return project_data;
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