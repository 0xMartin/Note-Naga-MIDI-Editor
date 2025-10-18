#pragma once

#include <QObject>
#include <QSize>
#include <QTimer>
#include <QMutex>
#include "video_renderer.h"

class NoteNagaMidiSeq;

/**
 * @brief This worker runs in a separate thread and handles preview rendering.
 * It receives requests from the GUI thread and sends back finished QImages.
 */
class PreviewWorker : public QObject
{
    Q_OBJECT

public:
    explicit PreviewWorker(NoteNagaMidiSeq* sequence);
    ~PreviewWorker();

signals:
    /**
     * @brief Emitted when a new preview frame is ready.
     * @param frame The finished QImage frame.
     */
    void frameReady(const QImage& frame);

public slots:
    /**
     * @brief Initializes the worker (must be called AFTER moving to the thread).
     */
    void init();
    
    /**
     * @brief Updates the target time for rendering.
     */
    void updateTime(double time);
    
    /**
     * @brief Updates the rendering settings.
     */
    void updateSettings(const VideoRenderer::RenderSettings& settings);
    
    /**
     * @brief Updates the visible time range (scale).
     */
    void updateScale(double secondsVisible);
    
    /**
     * @brief Updates the preview size.
     */
    void updateSize(const QSize& size);

private slots:
    /**
     * @brief Performs the actual rendering. Called by a timer to
     * merge multiple requests (e.g., during fast slider scrubbing).
     */
    void doRender();

private:
    VideoRenderer* m_renderer;
    NoteNagaMidiSeq* m_sequence;
    QTimer* m_renderTimer;

    // Mutex to protect shared data between slots and doRender()
    QMutex m_mutex; 
    
    // Last requested values
    double m_time;
    QSize m_size;
    VideoRenderer::RenderSettings m_settings;
    double m_scale;
    bool m_pendingRender;
};