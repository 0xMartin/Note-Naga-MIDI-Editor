#pragma once

#include <QWidget>
#include <QFont>
#include <QColor>
#include <QRect>
#include <QPen>

class MidiTactRuler : public QWidget {
    Q_OBJECT
public:
    explicit MidiTactRuler(int ticks_per_beat, double time_scale, int max_tick, QWidget* parent = nullptr);

    void set_params(int ticks_per_beat, double time_scale, int max_tick);
    void set_horizontal_scroll(int val);

signals:
    void play_position_set_signal(int tick);

public slots:
    void set_horizontal_scroll_slot(int val);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    int ticks_per_beat;
    double time_scale;
    int max_tick;
    int horizontalScroll;
    QFont font;

    // Colors
    QColor bg_color;
    QColor fg_color;
    QColor subline_color;
    QColor tact_bg_color;
    QColor tact_line_color;
};