#pragma once

#include <QWidget>
#include <QColor>
#include <QTimer>
#include <QElapsedTimer>
#include <QFont>
#include <vector>

class VolumeBar : public QWidget {
    Q_OBJECT
public:
    explicit VolumeBar(float value = 0.0f,
                       const QString& start_color = "#00ff00",
                       const QString& end_color = "#ff0000",
                       bool dynamic_mode = true,
                       QWidget* parent = nullptr);

    void setValue(float value, int time_ms = -1);
    void setRange(float min_value, float max_value);
    void setLabels(const std::vector<QString>& labels);

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onAnimTick();

private:
    QColor start_color;
    QColor end_color;
    bool dynamic_mode;

    float min_value;
    float max_value;
    int bar_height;
    std::vector<QString> labels;

    // Animation
    float current_value;
    float target_value;
    float initial_decay_value;
    int decay_time;
    int min_decay_time;
    QTimer* timer;
    QElapsedTimer anim_elapsed;
    bool anim_active;
    float decay_steepness;
};

float exponential_decay(float progress, float steepness = 4.0f);