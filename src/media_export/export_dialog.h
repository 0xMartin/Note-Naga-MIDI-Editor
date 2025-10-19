#pragma once

#include <QDialog>
#include <QColor>
#include <note_naga_engine/core/types.h>
#include <note_naga_engine/note_naga_engine.h>

#include "../gui/components/midi_seq_progress_bar.h" 
#include "media_exporter.h" 
#include "media_renderer.h" 
#include "preview_worker.h" 

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QComboBox;
class QProgressBar;
class QThread;
class QDoubleSpinBox;
class QGroupBox;
class QCheckBox;
class QSpinBox;
class QScrollArea;
class QSplitter;
QT_END_NAMESPACE

/**
 * @brief Dialog for configuring and exporting a video rendering of a MIDI sequence.
 */
class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(NoteNagaMidiSeq *sequence, NoteNagaEngine *engine, QWidget *parent = nullptr);
    ~ExportDialog();

private slots:
    // Playback controls
    void onPlayPauseClicked();
    void onPlaybackTickChanged(int tick);
    void seek(float seconds); 
    
    // Export process
    void onExportClicked();
    void onExportFinished();

    // Progress updates
    void updateAudioProgress(int percentage);
    void updateVideoProgress(int percentage);
    void updateStatusText(const QString &status);
    
    // Settings changes
    void onParticleTypeChanged(int index);
    void onSelectParticleFile();
    void updatePreviewSettings(); 
    void onSelectBgColor();
    void onSelectBgImage();
    void onClearBg();
    void updateBgLabels();
    
    /**
     * @brief Receives the finished frame from the PreviewWorker thread and displays it.
     */
    void onPreviewFrameReady(const QImage& frame);

private:
    // Engine and data
    NoteNagaEngine *m_engine;
    NoteNagaMidiSeq *m_sequence;
    
    // --- Added for Preview thread ---
    QThread* m_previewThread;
    PreviewWorker* m_previewWorker;

    // --- UI Components ---
    QSplitter *m_mainSplitter;
    QWidget *m_leftWidget;
    QWidget *m_rightWidget;

    // Preview components
    QLabel *m_previewLabel;
    QPushButton *m_playPauseButton;
    MidiSequenceProgressBar *m_progressBar; 
    QPushButton *m_exportButton;
    
    // Progress components
    QProgressBar *m_audioProgressBar;
    QProgressBar *m_videoProgressBar;
    QLabel *m_audioProgressLabel;
    QLabel *m_videoProgressLabel;
    QLabel *m_statusLabel;
    
    QGroupBox *m_previewGroup;
    QWidget *m_progressWidget;
        
    // Settings components
    QScrollArea *m_settingsScrollArea;
    QWidget *m_settingsWidget; 
    
    // Export settings
    QComboBox *m_resolutionCombo;
    QComboBox *m_fpsCombo;
    QDoubleSpinBox *m_scaleSpinBox;

    // Background settings
    QPushButton *m_bgColorButton;
    QPushButton *m_bgImageButton;
    QPushButton *m_bgClearButton;
    QLabel *m_bgColorPreview;
    QLabel *m_bgImagePreview;
    QCheckBox *m_bgShakeCheck;
    QDoubleSpinBox *m_bgShakeSpin;

    // Render settings 
    QCheckBox *m_renderNotesCheck;
    QCheckBox *m_renderKeyboardCheck;
    QCheckBox *m_renderParticlesCheck;
    QCheckBox *m_pianoGlowCheck;
    QDoubleSpinBox *m_noteStartOpacitySpin;
    QDoubleSpinBox *m_noteEndOpacitySpin;
    
    // Particle settings
    QGroupBox *m_particleSettingsGroup; 
    QComboBox *m_particleTypeCombo;
    QPushButton *m_particleFileButton;
    QLabel *m_particlePreviewLabel;
    QSpinBox *m_particleCountSpin;
    QDoubleSpinBox *m_particleLifetimeSpin;
    QDoubleSpinBox *m_particleSpeedSpin;
    QDoubleSpinBox *m_particleGravitySpin;
    QCheckBox *m_particleTintCheck;
    QDoubleSpinBox *m_particleStartSizeSpin;
    QDoubleSpinBox *m_particleEndSizeSpin;

    // State variables
    QString m_particleFilePath; 
    QColor m_backgroundColor;
    QString m_backgroundImagePath;
    double m_currentTime;
    double m_totalDuration;
    QSize m_lastRenderSize; // Size for rendering the preview

    // Export threading
    QThread *m_exportThread;
    MediaExporter *m_exporter;

    MediaRenderer::RenderSettings getCurrentRenderSettings();
    void setupUi();
    void connectEngineSignals();
    void setControlsEnabled(bool enabled);
    QSize getTargetResolution();
    void updatePreviewRenderSize();
    
protected:
    // --- Catching resize events ---
    virtual void resizeEvent(QResizeEvent *event) override;
};