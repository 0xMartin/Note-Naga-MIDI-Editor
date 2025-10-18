#pragma once

#include <QDialog>
#include <note_naga_engine/core/types.h>
#include <note_naga_engine/note_naga_engine.h>
#include "video_renderer.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QSlider;
class QComboBox;
class QProgressBar; 
class QThread;
class QDoubleSpinBox;
class QGroupBox;
class QCheckBox;
class QSpinBox;
class QTabWidget;
QT_END_NAMESPACE

class VideoExporter;

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(NoteNagaMidiSeq *sequence, NoteNagaEngine *engine, QWidget *parent = nullptr);
    ~ExportDialog();

private slots:
    void onPlayPauseClicked();
    void onStopClicked();
    void onPlaybackTickChanged(int tick);
    void seek(int value);
    void onExportClicked();
    void onExportFinished();

    void updateAudioProgress(int percentage);
    void updateVideoProgress(int percentage);
    void updateStatusText(const QString &status);
    
    void onParticleTypeChanged(int index);
    void onSelectParticleFile();
    void updatePreviewSettings(); 

private:
    VideoRenderer::RenderSettings getCurrentRenderSettings();
    void setupUi();
    void connectEngineSignals();
    void setControlsEnabled(bool enabled);

    NoteNagaEngine *m_engine;
    NoteNagaMidiSeq *m_sequence;
    VideoRenderer *m_renderer;

    QLabel *m_previewLabel;
    QPushButton *m_playPauseButton;
    QPushButton *m_stopButton;
    QSlider *m_timeSlider;
    QPushButton *m_exportButton;
    
    QProgressBar *m_audioProgressBar;
    QProgressBar *m_videoProgressBar;
    QLabel *m_audioProgressLabel;
    QLabel *m_videoProgressLabel;
    QLabel *m_statusLabel;
    
    QGroupBox *m_previewGroup;
    QWidget *m_progressWidget;
        
    // Nahradí m_settingsGroup
    QTabWidget *m_settingsTabs;
    QWidget *m_exportSettingsPage; 
    QWidget *m_renderSettingsPage; 
    
    // Prvky pro m_exportSettingsPage
    QComboBox *m_resolutionCombo;
    QComboBox *m_fpsCombo;
    QDoubleSpinBox *m_scaleSpinBox;

    // Prvky pro m_renderSettingsPage 
    QCheckBox *m_renderNotesCheck;
    QCheckBox *m_renderKeyboardCheck;
    QCheckBox *m_renderParticlesCheck;
    
    QGroupBox *m_particleSettingsGroup; 

    // Prvky pro m_particleSettingsGroup
    QComboBox *m_particleTypeCombo;
    QPushButton *m_particleFileButton;
    QLabel *m_particlePreviewLabel;
    QSpinBox *m_particleCountSpin;
    QDoubleSpinBox *m_particleLifetimeSpin;
    QDoubleSpinBox *m_particleSpeedSpin;
    QDoubleSpinBox *m_particleGravitySpin;

    // --- PŘIDANÉ UI PRVKY ---
    QCheckBox *m_particleTintCheck;
    QDoubleSpinBox *m_particleStartSizeSpin;
    QDoubleSpinBox *m_particleEndSizeSpin;
    // --- KONEC PŘIDANÝCH PRVKŮ ---

    QString m_particleFilePath; 
    double m_currentTime;
    double m_totalDuration;

    QThread *m_exportThread;
    VideoExporter *m_exporter;
};