#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QScrollArea>
#include <QMessageBox>
#include <QVector>
#include "../core/app_context.h"
#include "../core/mixer.h"
#include "../widgets/audio_dial.h"
#include "../widgets/audio_dial_centered.h"
#include "../widgets/multi_channel_volume_bar.h"
#include "routing_entry_widget.h"

class TrackMixerWidget : public QWidget {
    Q_OBJECT
public:
    explicit TrackMixerWidget(AppContext* ctx, Mixer* mixer, QWidget* parent = nullptr);

    void refresh_routing_table();

public slots:
    void on_min_note_changed(float value);
    void on_max_note_changed(float value);
    void on_global_offset_changed(float value);
    void on_global_volume_changed(float value);
    void on_global_pan_changed(float value);

private slots:
    void _on_add_entry();
    void _on_remove_selected_entry();
    void _on_clear_routing_table();
    void _on_default_entries();
    void _handle_playing_note(const MidiNote& note);

private:
    void set_channel_output_value(int channel_idx, float value, int time_ms = -1);
    void _init_ui();
    void _update_entry_selection(int idx);

    AppContext* ctx;
    Mixer* mixer;
    int selected_entry_index;
    int selected_row;

    std::vector<RoutingEntryWidget*> entry_widgets;

    AudioDial* dial_min;
    AudioDial* dial_max;
    AudioDialCentered* dial_offset;
    AudioDial* dial_vol;
    AudioDialCentered* dial_pan;
    MultiChannelVolumeBar* channel_volume_bar;

    QVBoxLayout* routing_entries_layout;
    QWidget* routing_entries_container;
    QScrollArea* routing_scroll;
};