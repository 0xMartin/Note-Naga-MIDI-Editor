#pragma once

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QColorDialog>
#include <QSizePolicy>
#include <QLineEdit>
#include <QDialog>
#include <QIcon>
#include <QColor>
#include <QString>

#include "../core/app_context.h"
#include "../widgets/volume_bar.h"
#include "../dialogs/instrument_selector_dialog.h"
#include "../core/shared.h"
#include "../core/icons.h"

class TrackWidget : public QFrame {
    Q_OBJECT
public:
    explicit TrackWidget(int track_index, AppContext* ctx, QWidget* parent = nullptr);

    int get_track_index() const { return track_index; }
    VolumeBar* get_volume_bar() const { return volume_bar; }

    void refresh_style(bool selected);

signals:
    void instrument_changed_signal(int track_index, int instrument_index);
    void visibility_changed_signal(int track_index, bool visible);
    void playback_changed_signal(int track_index, bool playing);
    void color_changed_signal(int track_index, QColor color);
    void name_changed_signal(int track_index, QString new_name);
    void clicked(int track_index);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void _toggle_visibility();
    void _toggle_solo();
    void _toggle_play();
    void _choose_color();
    void _name_edited();
    void _on_instrument_btn_clicked();

private:
    void _update_track_info();

    AppContext* ctx;
    int track_index;

    QPushButton* instrument_btn;
    QLabel* index_lbl;
    QLineEdit* name_edit;
    QPushButton* color_btn;
    QPushButton* vis_btn;
    QPushButton* solo_btn;
    QPushButton* play_btn;
    VolumeBar* volume_bar;
};