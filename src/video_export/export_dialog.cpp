#include "export_dialog.h"
#include "video_renderer.h"
#include "video_exporter.h"
#include <QtWidgets>

ExportDialog::ExportDialog(NoteNagaMidiSeq* sequence, NoteNagaEngine* engine, QWidget* parent)
    : QDialog(parent), m_engine(engine), m_sequence(sequence),
      m_renderer(new VideoRenderer(sequence)),
      m_currentTime(0.0), m_exportThread(nullptr), m_exporter(nullptr)
{
    setupUi();
    connectEngineSignals();

    m_totalDuration = nn_ticks_to_seconds(m_sequence->getMaxTick(), m_sequence->getPPQ(), m_sequence->getTempo());
    m_timeSlider->setRange(0, (int)(m_totalDuration * 100));

    connect(m_playPauseButton, &QPushButton::clicked, this, &ExportDialog::onPlayPauseClicked);
    connect(m_stopButton, &QPushButton::clicked, this, &ExportDialog::onStopClicked);
    connect(m_timeSlider, &QSlider::valueChanged, this, &ExportDialog::seek);
    connect(m_exportButton, &QPushButton::clicked, this, &ExportDialog::onExportClicked);
    
    connect(m_scaleSpinBox, &QDoubleSpinBox::valueChanged, this, &ExportDialog::updatePreviewSettings);

    // OPRAVA: Používáme checkStateChanged místo zastaralého stateChanged
    connect(m_renderNotesCheck, &QCheckBox::checkStateChanged, this, &ExportDialog::updatePreviewSettings);
    connect(m_renderKeyboardCheck, &QCheckBox::checkStateChanged, this, &ExportDialog::updatePreviewSettings);
    connect(m_renderParticlesCheck, &QCheckBox::checkStateChanged, this, &ExportDialog::updatePreviewSettings);
    connect(m_renderParticlesCheck, &QCheckBox::toggled, m_particleSettingsGroup, &QWidget::setEnabled);
    
    connect(m_particleTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ExportDialog::onParticleTypeChanged);
    connect(m_particleFileButton, &QPushButton::clicked, this, &ExportDialog::onSelectParticleFile);
    connect(m_particleCountSpin, &QSpinBox::valueChanged, this, &ExportDialog::updatePreviewSettings);
    connect(m_particleLifetimeSpin, &QDoubleSpinBox::valueChanged, this, &ExportDialog::updatePreviewSettings);
    connect(m_particleSpeedSpin, &QDoubleSpinBox::valueChanged, this, &ExportDialog::updatePreviewSettings);
    connect(m_particleGravitySpin, &QDoubleSpinBox::valueChanged, this, &ExportDialog::updatePreviewSettings);

    // --- PŘIDANÉ PROPOJENÍ (CONNECT) ---
    connect(m_particleStartSizeSpin, &QDoubleSpinBox::valueChanged, this, &ExportDialog::updatePreviewSettings);
    connect(m_particleEndSizeSpin, &QDoubleSpinBox::valueChanged, this, &ExportDialog::updatePreviewSettings);
    connect(m_particleTintCheck, &QCheckBox::checkStateChanged, this, &ExportDialog::updatePreviewSettings);
    // --- KONEC PŘIDANÝCH PROPOJENÍ ---


    // Nastavíme výchozí stav a spustíme první render
    onParticleTypeChanged(m_particleTypeCombo->currentIndex());
    updatePreviewSettings(); // Tím se nastaví i m_secondsVisible
    onPlaybackTickChanged(m_engine->getProject()->getCurrentTick());
}

ExportDialog::~ExportDialog()
{
    delete m_renderer;
    if (m_exportThread && m_exportThread->isRunning())
    {
        m_exportThread->quit();
        m_exportThread->wait();
    }
}

void ExportDialog::setupUi()
{
    setWindowTitle(tr("Export Video"));
    setMinimumSize(900, 700); // Zvětšíme okno pro nová nastavení

    QGridLayout* mainLayout = new QGridLayout(this);

    // --- Skupina pro náhled ---
    m_previewGroup = new QGroupBox(tr("Preview"));
    QVBoxLayout* previewLayout = new QVBoxLayout;
    m_previewLabel = new QLabel;
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setStyleSheet("background-color: black; border: 1px solid #444;");
    m_previewLabel->setMinimumHeight(300); // Trochu zvětšíme
    previewLayout->addWidget(m_previewLabel, 1);
    m_timeSlider = new QSlider(Qt::Horizontal);
    previewLayout->addWidget(m_timeSlider);
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    m_playPauseButton = new QPushButton(tr("Play"));
    m_stopButton = new QPushButton(tr("Stop"));
    controlsLayout->addWidget(m_playPauseButton);
    controlsLayout->addWidget(m_stopButton);
    controlsLayout->addStretch(1);
    previewLayout->addLayout(controlsLayout);
    m_previewGroup->setLayout(previewLayout);

    mainLayout->addWidget(m_previewGroup, 0, 0); // Vlevo

    m_settingsTabs = new QTabWidget;

    m_exportSettingsPage = new QWidget;
    QFormLayout *exportFormLayout = new QFormLayout(m_exportSettingsPage);
    
    m_resolutionCombo = new QComboBox;
    m_resolutionCombo->addItems({"1280x720 (720p)", "1920x1080 (1080p)"});
    m_fpsCombo = new QComboBox;
    m_fpsCombo->addItems({"30 FPS", "60 FPS"});
    m_scaleSpinBox = new QDoubleSpinBox;
    m_scaleSpinBox->setRange(1.0, 15.0);
    m_scaleSpinBox->setValue(5.0);
    m_scaleSpinBox->setSuffix(tr(" s"));
    m_scaleSpinBox->setToolTip(tr("How many seconds of notes are visible on screen at once."));

    exportFormLayout->addRow(tr("Resolution:"), m_resolutionCombo);
    exportFormLayout->addRow(tr("Framerate:"), m_fpsCombo);
    exportFormLayout->addRow(tr("Vertical Scale:"), m_scaleSpinBox);
    exportFormLayout->setContentsMargins(10, 10, 10, 10);

    m_settingsTabs->addTab(m_exportSettingsPage, tr("Export"));

    // --- Záložka 2: Render Settings ---
    m_renderSettingsPage = new QWidget;
    QVBoxLayout *renderLayout = new QVBoxLayout(m_renderSettingsPage);
    renderLayout->setContentsMargins(10, 10, 10, 10);
    
    // Checkboxy pro zapnutí/vypnutí (Bod 5)
    m_renderNotesCheck = new QCheckBox(tr("Render falling notes"));
    m_renderNotesCheck->setChecked(true);
    m_renderKeyboardCheck = new QCheckBox(tr("Render piano keyboard"));
    m_renderKeyboardCheck->setChecked(true);
    m_renderParticlesCheck = new QCheckBox(tr("Render particles"));
    m_renderParticlesCheck->setChecked(true);
    
    renderLayout->addWidget(m_renderNotesCheck);
    renderLayout->addWidget(m_renderKeyboardCheck);
    renderLayout->addWidget(m_renderParticlesCheck);
    renderLayout->addSpacing(10);

    // Skupina pro nastavení částic (Body 2, 4)
    m_particleSettingsGroup = new QGroupBox(tr("Particle Settings"));
    QFormLayout *particleForm = new QFormLayout(m_particleSettingsGroup);
    
    m_particleTypeCombo = new QComboBox;
    m_particleTypeCombo->addItems({tr("Default (Sparkle)"), tr("Circle"), tr("Custom Image")});
    particleForm->addRow(tr("Particle Type:"), m_particleTypeCombo);

    // Náhled a výběr vlastního obrázku (Bod 2)
    QHBoxLayout* fileLayout = new QHBoxLayout;
    m_particleFileButton = new QPushButton(tr("Select..."));
    m_particlePreviewLabel = new QLabel;
    m_particlePreviewLabel->setFixedSize(32, 32);
    m_particlePreviewLabel->setStyleSheet("border: 1px solid #555;");
    m_particlePreviewLabel->setAlignment(Qt::AlignCenter);
    fileLayout->addWidget(m_particleFileButton);
    fileLayout->addWidget(m_particlePreviewLabel);
    fileLayout->addStretch();
    particleForm->addRow(tr("Custom File:"), fileLayout);

    // Nastavení parametrů (Bod 4)
    m_particleCountSpin = new QSpinBox;
    m_particleCountSpin->setRange(1, 100);
    m_particleCountSpin->setValue(15);
    particleForm->addRow(tr("Count (per note):"), m_particleCountSpin);

    m_particleLifetimeSpin = new QDoubleSpinBox;
    m_particleLifetimeSpin->setRange(0.1, 5.0);
    m_particleLifetimeSpin->setValue(0.75);
    m_particleLifetimeSpin->setSuffix(" s");
    m_particleLifetimeSpin->setSingleStep(0.1);
    particleForm->addRow(tr("Lifetime:"), m_particleLifetimeSpin);

    m_particleSpeedSpin = new QDoubleSpinBox;
    m_particleSpeedSpin->setRange(10.0, 500.0);
    m_particleSpeedSpin->setValue(75.0);
    m_particleSpeedSpin->setSingleStep(5.0);
    particleForm->addRow(tr("Initial Speed:"), m_particleSpeedSpin);

    m_particleGravitySpin = new QDoubleSpinBox;
    m_particleGravitySpin->setRange(0.0, 1000.0);
    m_particleGravitySpin->setValue(200.0);
    m_particleGravitySpin->setSingleStep(10.0);
    particleForm->addRow(tr("Gravity:"), m_particleGravitySpin);
    
    // --- PŘIDÁNÍ NOVÝCH UI PRVKŮ ---
    m_particleStartSizeSpin = new QDoubleSpinBox;
    m_particleStartSizeSpin->setRange(0.1, 5.0);
    m_particleStartSizeSpin->setValue(0.5);
    m_particleStartSizeSpin->setSuffix("x");
    m_particleStartSizeSpin->setSingleStep(0.1);
    particleForm->addRow(tr("Start Size Multiplier:"), m_particleStartSizeSpin);

    m_particleEndSizeSpin = new QDoubleSpinBox;
    m_particleEndSizeSpin->setRange(0.1, 10.0);
    m_particleEndSizeSpin->setValue(1.0);
    m_particleEndSizeSpin->setSuffix("x");
    m_particleEndSizeSpin->setSingleStep(0.1);
    particleForm->addRow(tr("End Size Multiplier:"), m_particleEndSizeSpin);

    m_particleTintCheck = new QCheckBox(tr("Tint with note color"));
    m_particleTintCheck->setChecked(true);
    particleForm->addRow(m_particleTintCheck);
    // --- KONEC PŘIDANÝCH UI PRVKŮ ---

    renderLayout->addWidget(m_particleSettingsGroup);
    renderLayout->addStretch(1); // Natlačí vše nahoru

    m_settingsTabs->addTab(m_renderSettingsPage, tr("Render"));

    mainLayout->addWidget(m_settingsTabs, 0, 1); // Vpravo

    // --- Sekce pro export a progress ---
    QVBoxLayout* exportLayout = new QVBoxLayout;
    m_exportButton = new QPushButton(tr("Export to MP4"));
    m_exportButton->setFixedHeight(40);
    exportLayout->addWidget(m_exportButton);

    m_progressWidget = new QWidget;
    QFormLayout* progressFormLayout = new QFormLayout(m_progressWidget);
    progressFormLayout->setContentsMargins(0, 10, 0, 0);
    m_audioProgressBar = new QProgressBar;
    m_videoProgressBar = new QProgressBar;
    m_audioProgressLabel = new QLabel(tr("Audio Rendering:"));
    m_videoProgressLabel = new QLabel(tr("Video Rendering:"));
    progressFormLayout->addRow(m_audioProgressLabel, m_audioProgressBar);
    progressFormLayout->addRow(m_videoProgressLabel, m_videoProgressBar);
    exportLayout->addWidget(m_progressWidget);
    m_statusLabel = new QLabel;
    m_statusLabel->setAlignment(Qt::AlignCenter);
    exportLayout->addWidget(m_statusLabel);
    exportLayout->addStretch(1);

    mainLayout->addLayout(exportLayout, 1, 0, 1, 2); // Dole přes oba sloupce

    mainLayout->setColumnStretch(0, 3);
    mainLayout->setColumnStretch(1, 2); // Dáme nastavením trochu více místa

    m_progressWidget->setVisible(false); // Na začátku je skrytý
}

void ExportDialog::connectEngineSignals() {
    connect(m_engine->getPlaybackWorker(), &NoteNagaPlaybackWorker::currentTickChanged, this, &ExportDialog::onPlaybackTickChanged);
    connect(m_engine->getPlaybackWorker(), &NoteNagaPlaybackWorker::playingStateChanged, this, [this](bool playing){
        m_playPauseButton->setText(playing ? tr("Pause") : tr("Play"));
    });
}

void ExportDialog::onPlayPauseClicked()
{
    if (m_engine->isPlaying()) {
        m_engine->stopPlayback();
    } else {
        m_engine->startPlayback();
    }
}

void ExportDialog::onStopClicked() {
    m_engine->stopPlayback();
    m_engine->setPlaybackPosition(0);
}

void ExportDialog::seek(int value) {
    if (m_engine->isPlaying()) {
        m_engine->stopPlayback();
    }
    m_currentTime = value / 100.0;
    int tick = nn_seconds_to_ticks(m_currentTime, m_sequence->getPPQ(), m_sequence->getTempo());
    m_engine->setPlaybackPosition(tick);
    onPlaybackTickChanged(tick);
}

// AKTUALIZOVANÁ METODA
VideoRenderer::RenderSettings ExportDialog::getCurrentRenderSettings()
{
    VideoRenderer::RenderSettings settings;

    // Toggles
    settings.renderNotes = m_renderNotesCheck->isChecked();
    settings.renderKeyboard = m_renderKeyboardCheck->isChecked();
    settings.renderParticles = m_renderParticlesCheck->isChecked();

    // Nastavení částic
    settings.particleType = (VideoRenderer::RenderSettings::ParticleType)m_particleTypeCombo->currentIndex();
    if (settings.particleType == VideoRenderer::RenderSettings::Custom && !m_particleFilePath.isEmpty()) {
        settings.customParticleImage = QImage(m_particleFilePath); // Používáme QImage
    }
    
    settings.particleCount = m_particleCountSpin->value();
    settings.particleLifetime = m_particleLifetimeSpin->value();
    settings.particleSpeed = m_particleSpeedSpin->value();
    settings.particleGravity = m_particleGravitySpin->value();

    // --- PŘIDANÉ NAČÍTÁNÍ HODNOT ---
    settings.tintParticles = m_particleTintCheck->isChecked();
    settings.particleStartSize = m_particleStartSizeSpin->value();
    settings.particleEndSize = m_particleEndSizeSpin->value();
    // --- KONEC PŘIDANÝCH HODNOT ---

    return settings;
}

void ExportDialog::updatePreviewSettings()
{
    // Nastavíme škálování (dříve to bylo v lambdě)
    m_renderer->setSecondsVisible(m_scaleSpinBox->value());
    
    // Nastavíme všechna ostatní nastavení
    m_renderer->setRenderSettings(getCurrentRenderSettings());
    
    // Překreslíme náhled, pokud engine nehraje
    if (!m_engine->isPlaying()) {
        onPlaybackTickChanged(m_engine->getProject()->getCurrentTick());
    }
}

void ExportDialog::onParticleTypeChanged(int index)
{
    bool isCustom = (index == (int)VideoRenderer::RenderSettings::Custom); // 2 je "Custom Image"
    m_particleFileButton->setVisible(isCustom);
    m_particlePreviewLabel->setVisible(isCustom);

    // --- PŘIDÁNÍ/ZOBRAZENÍ NOVÝCH NASTAVENÍ ---
    // Tint a velikost dává smysl jen pro obrázky (ne kruhy)
    bool isPixmap = (index == (int)VideoRenderer::RenderSettings::Resource || isCustom);
    m_particleTintCheck->setVisible(isPixmap);
    m_particleStartSizeSpin->setVisible(isPixmap);
    m_particleEndSizeSpin->setVisible(isPixmap);
    // --- KONEC PŘIDÁNÍ ---

    // Aktualizujeme náhled obrázku v labelu
    if (isCustom && !m_particleFilePath.isEmpty()) {
        QPixmap pixmap(m_particleFilePath);
        m_particlePreviewLabel->setPixmap(pixmap.scaled(m_particlePreviewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else if (index == (int)VideoRenderer::RenderSettings::Resource) {
        QPixmap pixmap(":/images/sparkle.png"); // Ukážeme výchozí
        m_particlePreviewLabel->setPixmap(pixmap.scaled(m_particlePreviewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_particlePreviewLabel->setVisible(true); // Ukážeme i výchozí
    } else {
        m_particlePreviewLabel->clear(); // Pro kruh neukazujeme nic
        m_particlePreviewLabel->setVisible(false); // A skryjeme
    }

    updatePreviewSettings();
}

void ExportDialog::onSelectParticleFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select Particle Image"), "", tr("Images (*.png *.jpg *.bmp)"));
    if (!path.isEmpty()) {
        m_particleFilePath = path;
        QPixmap pixmap(path);
        m_particlePreviewLabel->setPixmap(pixmap.scaled(m_particlePreviewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        
        // Aktualizujeme náhled
        updatePreviewSettings();
    }
}


void ExportDialog::onExportClicked()
{
    QString outputPath = QFileDialog::getSaveFileName(this, tr("Save Video"), "", tr("MPEG-4 Video (*.mp4)"));
    if (outputPath.isEmpty())
        return;

    QSize resolution = (m_resolutionCombo->currentIndex() == 0) ? QSize(1280, 720) : QSize(1920, 1080);
    int fps = (m_fpsCombo->currentIndex() == 0) ? 30 : 60;
    double secondsVisible = m_scaleSpinBox->value();

    VideoRenderer::RenderSettings settings = getCurrentRenderSettings();

    setControlsEnabled(false);

    m_exportThread = new QThread;
    m_exporter = new VideoExporter(m_sequence, outputPath, resolution, fps, this->m_engine, secondsVisible, settings);
    
    m_exporter->moveToThread(m_exportThread);

    // Propojení nových, specifických signálů a slotů
    connect(m_exportThread, &QThread::started, m_exporter, &VideoExporter::doExport);
    connect(m_exporter, &VideoExporter::finished, this, &ExportDialog::onExportFinished);
    connect(m_exporter, &VideoExporter::error, this, [this](const QString &msg) {
        QMessageBox::critical(this, tr("Error"), msg);
        onExportFinished();
    });

    connect(m_exporter, &VideoExporter::audioProgressUpdated, this, &ExportDialog::updateAudioProgress);
    connect(m_exporter, &VideoExporter::videoProgressUpdated, this, &ExportDialog::updateVideoProgress);
    connect(m_exporter, &VideoExporter::statusTextChanged, this, &ExportDialog::updateStatusText);

    connect(m_exporter, &VideoExporter::finished, m_exportThread, &QThread::quit);
    connect(m_exporter, &VideoExporter::finished, m_exporter, &VideoExporter::deleteLater);
    connect(m_exportThread, &QThread::finished, m_exportThread, &QThread::deleteLater);

    m_exportThread->start();
}

void ExportDialog::updateAudioProgress(int percentage)
{
    m_audioProgressBar->setValue(percentage);
}

void ExportDialog::updateVideoProgress(int percentage)
{
    m_videoProgressBar->setValue(percentage);
}

void ExportDialog::updateStatusText(const QString &status)
{
    m_statusLabel->setText(status);
}

void ExportDialog::onExportFinished()
{
    setControlsEnabled(true);
    if (!m_statusLabel->text().contains(tr("Error"), Qt::CaseInsensitive)) {
        QMessageBox::information(this, tr("Success"), tr("Video export finished successfully."));
    }
    m_exportThread = nullptr;
    m_exporter = nullptr;
}

void ExportDialog::setControlsEnabled(bool enabled)
{
    m_previewGroup->setEnabled(enabled);
    m_settingsTabs->setEnabled(enabled); 
    m_exportButton->setEnabled(enabled);
    m_progressWidget->setVisible(!enabled);

    if (enabled)
    {
        m_audioProgressBar->setValue(0);
        m_audioProgressBar->setMaximum(100);
        m_videoProgressBar->setValue(0);
        m_videoProgressBar->setMaximum(100);
        m_statusLabel->clear();
    }
}

void ExportDialog::onPlaybackTickChanged(int tick) {
    m_currentTime = nn_ticks_to_seconds(tick, m_sequence->getPPQ(), m_sequence->getTempo());

    QImage frame = m_renderer->renderFrame(m_currentTime, m_previewLabel->size());
    m_previewLabel->setPixmap(QPixmap::fromImage(frame.scaled(m_previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));

    m_timeSlider->blockSignals(true);
    m_timeSlider->setValue((int)(m_currentTime * 100));
    m_timeSlider->blockSignals(false);
}