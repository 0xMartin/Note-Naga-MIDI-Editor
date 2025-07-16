#pragma once

#include <QWidget>
#include <QSize>
#include <QRect>
#include <QColor>
#include <QPainter>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <vector>
#include <map>
#include <memory>
#include "../core/app_context.h"
#include "../core/shared.h"

class MidiEditorWidget : public QWidget {
    Q_OBJECT
public:
    explicit MidiEditorWidget(AppContext* ctx, QWidget* parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    double get_time_scale() const { return time_scale; }
    int get_key_height() const { return key_height; }

signals:
    void set_play_position_signal(int tick);

public slots:
    void repaint_slot();
    void set_time_scale_slot(double scale);
    void set_key_height_slot(int h);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void recalculate_content_size();
    void _reload_notes();
    void set_time_scale(double scale);
    void set_key_height(int h);
    void draw_note(QPainter& painter, const MidiNote& note, const QColor& note_color,
                   int visible_x0, int visible_x1, int visible_y0, int visible_y1, bool active);

    AppContext* ctx;
    std::map<int, std::vector<int>> notes_starts; // track_id -> vector of starts
    std::map<int, std::vector<int>> notes_ends;   // track_id -> vector of ends
    bool has_file;

    double time_scale;
    int key_height;
    int _content_width;
    int _content_height;
    int ppq;

    // Colors
    QColor bg_color;
    QColor fg_color;
    QColor line_color;
    QColor subline_color;

    static constexpr int MIN_NOTE = 0;
    static constexpr int MAX_NOTE = 127;
};