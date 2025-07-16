#include "playback_worker.h"
#include <QDebug>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <chrono>
#include <thread>

// --- PlaybackWorker Implementation ---

PlaybackWorker::PlaybackWorker(AppContext* ctx, double timer_interval_ms,
                               std::function<void(const std::vector<MidiNote>&, const std::vector<MidiNote>&)> on_note_events,
                               std::function<void(int)> on_position_changed,
                               QObject* parent)
    : QObject(parent),
      ctx(ctx),
      timer_interval(timer_interval_ms / 1000.0),
      playing(false),
      thread(nullptr),
      worker(nullptr),
      should_stop(false),
      on_note_events(on_note_events),
      on_position_changed(on_position_changed)
{}

void PlaybackWorker::recalculate_worker_tempo()
{
    if (!worker) {
        qDebug() << "PlaybackWorker: Worker is not running, unable to recalculate tempo.";
        return;
    }
    worker->recalculate_tempo();
}

bool PlaybackWorker::play()
{
    if (playing) {
        qDebug() << "PlaybackWorker: Already playing.";
        return false;
    }
    if (!ctx->midi_file) {
        qDebug() << "PlaybackWorker: No MIDI file loaded.";
        return false;
    }
    qDebug() << "PlaybackWorker: Starting playback.";

    thread = new QThread;
    worker = new PlaybackThreadWorker(ctx, timer_interval, on_note_events, on_position_changed);
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &PlaybackThreadWorker::run);
    connect(worker, &PlaybackThreadWorker::finished_signal, thread, &QThread::quit);
    connect(worker, &PlaybackThreadWorker::finished_signal, this, &PlaybackWorker::cleanup_thread);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
    playing = true;
    emit playing_state_changed_signal(playing);
    return true;
}

bool PlaybackWorker::stop()
{
    if (!playing) {
        qDebug() << "PlaybackWorker: Not currently playing.";
        return false;
    }
    qDebug() << "PlaybackWorker: Stopping playback.";

    if (worker)
        worker->stop();
    if (thread) {
        thread->quit();
        thread->wait();
    }
    playing = false;
    emit playing_state_changed_signal(playing);
    return true;
}

void PlaybackWorker::cleanup_thread()
{
    if (worker) {
        worker->deleteLater();
        worker = nullptr;
    }
    if (thread) {
        thread->wait();
        thread->deleteLater();
        thread = nullptr;
    }
    playing = false;
    emit playing_state_changed_signal(playing);
}

// --- PlaybackThreadWorker Implementation ---

PlaybackThreadWorker::PlaybackThreadWorker(AppContext* ctx, double timer_interval,
    std::function<void(const std::vector<MidiNote>&, const std::vector<MidiNote>&)> on_note_events,
    std::function<void(int)> on_position_changed)
    : QObject(nullptr),
      ctx(ctx),
      timer_interval(timer_interval),
      ms_per_tick(1.0),
      on_note_events(on_note_events),
      on_position_changed(on_position_changed),
      should_stop(false)
{}

void PlaybackThreadWorker::recalculate_tempo()
{
    int current_tick = ctx->current_tick;
    double us_per_tick = static_cast<double>(ctx->tempo) / ctx->ppq;
    ms_per_tick = us_per_tick / 1000.0;
    start_time_point = std::chrono::high_resolution_clock::now();
    start_tick_at_start = current_tick;
}

void PlaybackThreadWorker::run()
{
    qDebug() << "PlaybackThreadWorker: run() start";
    int current_tick = ctx->current_tick;
    recalculate_tempo();

    using clock = std::chrono::high_resolution_clock;

    while (!should_stop) {
        auto now = clock::now();
        double elapsed_ms = std::chrono::duration<double, std::milli>(now - start_time_point).count();
        int target_tick = start_tick_at_start + static_cast<int>(elapsed_ms / ms_per_tick);
        int tick_advance = std::max(1, target_tick - current_tick);
        int last_tick = current_tick;
        current_tick += tick_advance;

        // Stop playback on reaching max tick
        if (current_tick >= ctx->max_tick) {
            current_tick = ctx->max_tick;
            should_stop = true;
        }

        // Collect notes to play/stop
        std::vector<MidiNote> notes_to_play;
        std::vector<MidiNote> notes_to_stop;
        for (const auto& track : ctx->tracks) {
            if (!track->playing) continue;
            for (const auto& note : track->midi_notes) {
                if (note.start.has_value() && note.length.has_value()) {
                    if (last_tick < note.start.value() && note.start.value() <= current_tick)
                        notes_to_play.push_back(note);
                    if (last_tick < note.start.value() + note.length.value() && note.start.value() + note.length.value() <= current_tick)
                        notes_to_stop.push_back(note);
                }
            }
        }

        // Callbacks for note events and position change
        if (on_note_events)
            on_note_events(notes_to_play, notes_to_stop);
        if (on_position_changed)
            on_position_changed(current_tick);

        std::this_thread::sleep_for(std::chrono::duration<double>(timer_interval));
    }

    qDebug() << "PlaybackThreadWorker: finished";

    emit finished_signal();
}

void PlaybackThreadWorker::stop()
{
    should_stop = true;
}