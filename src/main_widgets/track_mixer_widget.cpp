#include "track_mixer_widget.h"
#include <QIcon>

TrackMixerWidget::TrackMixerWidget(AppContext* ctx_, Mixer* mixer_, QWidget* parent)
    : QWidget(parent), ctx(ctx_), mixer(mixer_), selected_entry_index(-1), selected_row(-1)
{
    setObjectName("TrackMixerWidget");
    connect(ctx, &AppContext::mixer_playing_note_signal, this, &TrackMixerWidget::_handle_playing_note);
    connect(mixer, &Mixer::routing_entry_stack_changed_signal, this, &TrackMixerWidget::refresh_routing_table);
    _init_ui();
}

void TrackMixerWidget::_init_ui()
{
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(12, 5, 5, 5);
    main_layout->setSpacing(2);

    QLabel* title = new QLabel("🎚️ Track Mixer");
    title->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #79b8ff; letter-spacing: 1.2px; margin-bottom: 2px;");
    main_layout->addWidget(title, Qt::AlignLeft);

    QFrame* controls_frame = new QFrame();
    controls_frame->setStyleSheet("QFrame { background: #2F3139; border: 1px solid #494d56; border-radius: 10px; padding: 2px 10px 0px 10px; }");
    QHBoxLayout* controls_layout = new QHBoxLayout(controls_frame);
    controls_layout->setSpacing(28);
    controls_layout->setContentsMargins(0, 0, 0, 0);

    dial_min = new AudioDial();
    dial_min->setLabel("Note Min");
    dial_min->setRange(0, 127);
    dial_min->setValue(mixer->master_min_note);
    dial_min->setDefaultValue(0);
    dial_min->showValue(true);
    dial_min->setValueDecimals(0);
    connect(dial_min, &AudioDial::valueChanged, this, &TrackMixerWidget::on_min_note_changed);

    dial_max = new AudioDial();
    dial_max->setLabel("Note Max");
    dial_max->setRange(0, 127);
    dial_max->setValue(mixer->master_max_note);
    dial_max->setDefaultValue(127);
    dial_max->showValue(true);
    dial_max->setValueDecimals(0);
    connect(dial_max, &AudioDial::valueChanged, this, &TrackMixerWidget::on_max_note_changed);

    dial_offset = new AudioDialCentered();
    dial_offset->setLabel("Note Offset");
    dial_offset->setRange(-24, 24);
    dial_offset->setValue(mixer->master_note_offset);
    dial_offset->setDefaultValue(0);
    dial_offset->showValue(true);
    dial_offset->setValueDecimals(0);
    connect(dial_offset, &AudioDialCentered::valueChanged, this, &TrackMixerWidget::on_global_offset_changed);

    dial_vol = new AudioDial();
    dial_vol->setLabel("Volume");
    dial_vol->setRange(0, 100);
    dial_vol->setValueDecimals(1);
    dial_vol->setValue(mixer->master_volume * 100);
    dial_vol->setDefaultValue(100);
    dial_vol->setValuePostfix(" %");
    dial_vol->showValue(true);
    connect(dial_vol, &AudioDial::valueChanged, this, &TrackMixerWidget::on_global_volume_changed);

    dial_pan = new AudioDialCentered();
    dial_pan->setLabel("Pan");
    dial_pan->setRange(-1.0, 1.0);
    dial_pan->setValueDecimals(2);
    dial_pan->setValue(mixer->master_pan);
    dial_pan->setDefaultValue(0.0);
    connect(dial_pan, &AudioDialCentered::valueChanged, this, &TrackMixerWidget::on_global_pan_changed);

    controls_layout->addWidget(dial_min, Qt::AlignVCenter);
    controls_layout->addWidget(dial_max, Qt::AlignVCenter);
    controls_layout->addWidget(dial_offset, Qt::AlignVCenter);
    controls_layout->addWidget(dial_vol, Qt::AlignVCenter);
    controls_layout->addWidget(dial_pan, Qt::AlignVCenter);

    main_layout->addWidget(controls_frame, Qt::AlignHCenter);

    QLabel* channel_output_label = new QLabel("Channel Output");
    channel_output_label->setStyleSheet("font-size: 13px; font-weight: bold; color: #7eb8f9;");
    main_layout->addWidget(channel_output_label, Qt::AlignLeft);

    channel_volume_bar = new MultiChannelVolumeBar(16);
    channel_volume_bar->setRange(0, 1);
    channel_volume_bar->setMinimumHeight(90);
    main_layout->addWidget(channel_volume_bar);

    QLabel* routing_label = new QLabel("Routing Table");
    routing_label->setStyleSheet("font-size: 13px; font-weight: bold; color: #7eb8f9;");
    main_layout->addWidget(routing_label, Qt::AlignLeft);

    routing_scroll = new QScrollArea(this);
    routing_scroll->setWidgetResizable(true);
    routing_scroll->setFrameShape(QFrame::NoFrame);
    routing_scroll->setMinimumHeight(220);
    routing_scroll->setStyleSheet("QScrollArea { background: transparent; padding: 0px; }");
    main_layout->addWidget(routing_scroll, 1);

    routing_entries_container = new QWidget();
    routing_entries_layout = new QVBoxLayout(routing_entries_container);
    routing_entries_layout->setContentsMargins(3, 3, 3, 3);
    routing_entries_layout->setSpacing(4);
    routing_entries_layout->addStretch(1);
    routing_scroll->setWidget(routing_entries_container);

    QHBoxLayout* routing_controls = new QHBoxLayout();
    routing_controls->setSpacing(6);
    routing_controls->setContentsMargins(6, 6, 6, 6);

    QPushButton* btn_add = new QPushButton();
    btn_add->setObjectName("RoutingAddButton");
    btn_add->setIcon(QIcon(":/icons/add.svg"));
    btn_add->setToolTip("Add new routing entry");
    btn_add->setFixedSize(QSize(28, 28));
    connect(btn_add, &QPushButton::clicked, this, &TrackMixerWidget::_on_add_entry);
    routing_controls->addWidget(btn_add);

    QPushButton* btn_remove = new QPushButton();
    btn_remove->setObjectName("RoutingRemoveButton");
    btn_remove->setIcon(QIcon(":/icons/remove.svg"));
    btn_remove->setToolTip("Remove selected routing entry");
    btn_remove->setFixedSize(QSize(28, 28));
    connect(btn_remove, &QPushButton::clicked, this, &TrackMixerWidget::_on_remove_selected_entry);
    routing_controls->addWidget(btn_remove);

    QPushButton* btn_clear = new QPushButton();
    btn_clear->setObjectName("RoutingClearButton");
    btn_clear->setIcon(QIcon(":/icons/clear.svg"));
    btn_clear->setToolTip("Clear all routing entries");
    btn_clear->setFixedSize(QSize(28, 28));
    connect(btn_clear, &QPushButton::clicked, this, &TrackMixerWidget::_on_clear_routing_table);
    routing_controls->addWidget(btn_clear);

    QPushButton* btn_default = new QPushButton();
    btn_default->setObjectName("RoutingDefaultButton");
    btn_default->setIcon(QIcon(":/icons/reload.svg"));
    btn_default->setToolTip("Set default routing (one entry per track, Fluidsynth)");
    btn_default->setFixedSize(QSize(28, 28));
    connect(btn_default, &QPushButton::clicked, this, &TrackMixerWidget::_on_default_entries);
    routing_controls->addWidget(btn_default);

    routing_controls->addStretch(1);
    main_layout->addLayout(routing_controls);

    setStyleSheet(
        "QWidget#TrackMixerWidget { background: transparent; border: none; padding: 0px; }"
        "QPushButton { min-width: 24px; max-width: 24px; min-height: 24px; max-height: 24px; padding: 0px; }"
    );

    refresh_routing_table();
}

void TrackMixerWidget::on_min_note_changed(float value) {
    mixer->master_min_note = int(value);
}
void TrackMixerWidget::on_max_note_changed(float value) {
    mixer->master_max_note = int(value);
}
void TrackMixerWidget::on_global_offset_changed(float value) {
    mixer->master_note_offset = int(value);
}
void TrackMixerWidget::on_global_volume_changed(float value) {
    mixer->master_volume = float(value / 100.0f);
}
void TrackMixerWidget::on_global_pan_changed(float value) {
    mixer->master_pan = value;
}

void TrackMixerWidget::set_channel_output_value(int channel_idx, float value, int time_ms) {
    channel_volume_bar->setValue(channel_idx, value, time_ms);
}

void TrackMixerWidget::refresh_routing_table() {
    QVBoxLayout* layout = routing_entries_layout;
    selected_entry_index = -1;
    // Remove all widgets except last stretch
    for (int i = layout->count() - 2; i >= 0; --i) {
        QLayoutItem* item = layout->itemAt(i);
        QWidget* widget = item->widget();
        if (widget) {
            widget->setParent(nullptr);
            widget->deleteLater();
            layout->removeWidget(widget);
        } else {
            layout->removeItem(item);
        }
    }
    entry_widgets.clear();
    for (int idx = 0; idx < mixer->routing_entries.size(); ++idx) {
        RoutingEntryWidget* widget = new RoutingEntryWidget(mixer->routing_entries[idx], mixer, ctx);
        // Selection: connect via event filter or signal, here to be implemented:
        widget->installEventFilter(this);
        widget->setProperty("_select_idx", idx);
        widget->setMouseTracking(true);
        // Custom selection handler
        connect(widget, &RoutingEntryWidget::clicked, this, [this, idx]() {
            this->_update_entry_selection(idx);
        });
        layout->insertWidget(layout->count() - 1, widget);
        entry_widgets.push_back(widget);
    }
}

void TrackMixerWidget::_on_add_entry() {
    mixer->add_routing_entry();
}

void TrackMixerWidget::_on_remove_selected_entry() {
    if (selected_entry_index >= 0) {
        mixer->remove_routing_entry(selected_entry_index);
    } else {
        QMessageBox::warning(this, "No Entry Selected", "Please select a routing entry to remove.", QMessageBox::Ok);
    }
}

void TrackMixerWidget::_on_clear_routing_table() {
    auto reply = QMessageBox::question(
        this, "Clear Routing Table", "Are you sure you want to clear all routing entries?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No
    );
    if (reply == QMessageBox::Yes) {
        mixer->clear_routing_table();
    }
}

void TrackMixerWidget::_on_default_entries() {
    auto reply = QMessageBox::question(
        this, "Set Default Routing", "This will clear all routing entries and set default routing. Continue?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No
    );
    if (reply == QMessageBox::Yes) {
        mixer->clear_routing_table();
        mixer->create_default_routing();
    }
}

void TrackMixerWidget::_handle_playing_note(const MidiNote& note) {
    int time_ms = int(note_time_ms(note, ctx->ppq, ctx->tempo));
    for (auto& entry : mixer->routing_entries) {
        if (entry.track_id == note.track && note.velocity.has_value() && note.velocity.value() > 0) {
            if (entry.channel < channel_volume_bar->getChannelCount()) {
                set_channel_output_value(entry.channel, note.velocity.value() / 127.0f, time_ms);
            }
        }
    }
}

void TrackMixerWidget::_update_entry_selection(int idx) {
    selected_row = idx;
    for (size_t i = 0; i < entry_widgets.size(); ++i) {
        entry_widgets[i]->refresh_style(int(i) == idx);
        if (int(i) == idx) {
            selected_entry_index = int(i);
        }
    }
}