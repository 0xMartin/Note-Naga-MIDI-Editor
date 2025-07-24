#pragma once

#include <QFrame>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include <QPropertyAnimation>

#include <note_naga_engine/note_naga_engine.h>
#include "../components/midi_seq_progress_bar.h"

/**
 * @brief The MidiControlBarWidget class provides a control bar for MIDI playback.
 * It includes buttons for play/pause, navigation, and tempo control...
 */
class MidiControlBarWidget : public QFrame {
    Q_OBJECT
public:
    /**
     * @brief Constructor for MidiControlBarWidget.
     * @param engine Pointer to the NoteNagaEngine instance.
     * @param parent Parent widget.
     */
    explicit MidiControlBarWidget(NoteNagaEngine* engine, QWidget* parent = nullptr);

public slots:
    /**
     * @brief Set playing state of the control bar.
     */
    void setPlaying(bool is_playing);

signals:
    /**
     * @brief Signal emitted when the play button is toggled.
     */
    void playToggled();

    /**
     * @brief Signal emitted when the user navigates to the start of the sequence.
     */
    void goToStart();

    /**
     * @brief Signal emitted when the user navigates to the end of the sequence.
     */
    void goToEnd();

    /**
     * @brief Signal emitted when the tempo is changed.
     * @param tempo New tempo in BPM.
     */
    void tempoChanged(int tempo);

    /**
     * @brief Signal emitted when the metronome is toggled.
     * @param state True if metronome is on, false otherwise.
     */
    void metronomeToggled(bool state);

private slots:
    void updateBPM();
    void updateProgressBar();
    void metronomeBtnClicked();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    NoteNagaEngine* engine;
    
    int ppq;
    int tempo;
    int max_tick;
    bool metronome_on;

    QLabel* tempo_label;
    QLabel* tempo_icon;
    MidiSequenceProgressBar* progress_bar;
    QPushButton* play_btn;
    QPushButton* to_start_btn;
    QPushButton* to_end_btn;
    QPushButton* metronome_btn;

    void initUI();
    void editTempo(QMouseEvent* event);
    static QString format_time(double sec);
};