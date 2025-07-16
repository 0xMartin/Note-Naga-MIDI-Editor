#pragma once

#include <QObject>
#include <QThread>
#include <functional>
#include <chrono>
#include "app_context.h"

// PlaybackWorker manages playback in a separate thread.
// It emits signals when playback finishes and playing state changes.

class PlaybackWorker : public QObject
{
    Q_OBJECT
public:
    explicit PlaybackWorker(AppContext* ctx, double timer_interval_ms,
                           std::function<void(const std::vector<MidiNote>&, const std::vector<MidiNote>&)> on_note_events = nullptr,
                           std::function<void(int)> on_position_changed = nullptr,
                           QObject* parent = nullptr);

    bool is_playing() const { return playing; }
    void recalculate_worker_tempo();
    bool play();
    bool stop();

signals:
    void finished_signal();
    void playing_state_changed_signal(bool playing);

private slots:
    void cleanup_thread();

private:
    AppContext* ctx;
    double timer_interval;
    bool playing;
    QThread* thread;
    class PlaybackThreadWorker* worker;
    bool should_stop;
    std::function<void(const std::vector<MidiNote>&, const std::vector<MidiNote>&)> on_note_events;
    std::function<void(int)> on_position_changed;
};

class PlaybackThreadWorker : public QObject
{
    Q_OBJECT
public:
    PlaybackThreadWorker(AppContext* ctx, double timer_interval,
                        std::function<void(const std::vector<MidiNote>&, const std::vector<MidiNote>&)> on_note_events = nullptr,
                        std::function<void(int)> on_position_changed = nullptr);
    void recalculate_tempo();
    void stop();

public slots:
    void run();

signals:
    void finished_signal();

private:
    AppContext* ctx;
    double timer_interval;
    double ms_per_tick;
    std::chrono::high_resolution_clock::time_point start_time_point;
    int start_tick_at_start;
    std::function<void(const std::vector<MidiNote>&, const std::vector<MidiNote>&)> on_note_events;
    std::function<void(int)> on_position_changed;
    bool should_stop;
};