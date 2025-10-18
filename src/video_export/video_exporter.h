#pragma once

#include <QObject>
#include <QSize>
#include <QString>
#include <QFuture>
#include <QFutureWatcher>
#include <QFutureSynchronizer> // For waiting on batches
#include <QAtomicInt>
#include <note_naga_engine/core/types.h>
#include <note_naga_engine/note_naga_engine.h>
#include "video_renderer.h"

class VideoExporter : public QObject
{
    Q_OBJECT

public:
    explicit VideoExporter(NoteNagaMidiSeq *sequence, QString outputPath,
                           QSize resolution, int fps, NoteNagaEngine *m_engine,
                           double secondsVisible,
                           const VideoRenderer::RenderSettings& settings, // We receive settings, not a renderer
                           QObject *parent = nullptr);
    ~VideoExporter();

public slots:
    void doExport();

signals:
    void audioProgressUpdated(int percentage);
    void videoProgressUpdated(int percentage);
    void statusTextChanged(const QString &status);
    void finished();
    void error(const QString &errorMessage);

private slots:
    void onTaskFinished();

private:
    NoteNagaEngine *m_engine;
    NoteNagaMidiSeq *m_sequence;
    QString m_outputPath;
    QSize m_resolution;
    int m_fps;
    
    // --- Stored settings ---
    double m_secondsVisible;
    VideoRenderer::RenderSettings m_settings;

    QFutureWatcher<bool> m_audioWatcher;
    QFutureWatcher<bool> m_videoWatcher;
    QAtomicInt m_finishedTaskCount;

    QString m_tempAudioPath;
    QString m_tempVideoPath;
    
    // --- Counter for parallel rendering ---
    QAtomicInt m_framesRendered;
    int m_totalFrames;

    // --- New methods for parallel export ---
    
    /**
     * @brief Phase 1: Runs simulation and Phase 2: Parallel rendering.
     */
    bool exportVideoBatched(const QString &outputPath);
    
    /**
     * @brief Renders one batch of frames (runs in its own thread).
     * @return Path to the temporary .mp4 file for this batch.
     */
    QString renderVideoBatch(int startFrame, int endFrame, 
                             const std::vector<VideoRenderer::FrameState>& allFrameStates);
    
    /**
     * @brief Joins multiple video files into one using FFmpeg concat.
     */
    bool concatenateVideos(const QStringList& videoFiles, const QString& outputPath);
    
    // --- Old methods ---
    bool exportAudio(const QString &outputPath);
    bool combineAudioVideo(const QString &videoPath, const QString &audioPath, const QString &finalPath);
    void writeWavHeader(std::ofstream &file, int sampleRate, int numSamples);
    void cleanup();
};