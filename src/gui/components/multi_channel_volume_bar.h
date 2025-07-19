#pragma once

#include <QWidget>
#include <QColor>
#include <QTimer>
#include <QElapsedTimer>
#include <vector>
#include <QString>

class MultiChannelVolumeBar : public QWidget {
    Q_OBJECT
public:
    explicit MultiChannelVolumeBar(int channels = 16,
                                   const QString& start_color = "#00ff00",
                                   const QString& end_color = "#ff0000",
                                   bool dynamic_mode = true,
                                   QWidget* parent = nullptr);

    void setChannelCount(int channels);
    int getChannelCount() const;
    void setValue(int channel_idx, float value, int time_ms = -1);
    void setRange(float min_value, float max_value);
    void setLabels(const std::vector<QString>& labels);

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onAnimTick();

private:
    int channels;
    QColor start_color;
    QColor end_color;
    bool dynamic_mode;

    float min_value;
    float max_value;
    int bar_width_min;
    int bar_width_max;
    int bar_space_min;
    int bar_space_max;
    int bar_bottom_margin;
    int bar_top_margin;
    std::vector<QString> labels;

    // Animation
    std::vector<float> current_values;
    std::vector<float> initial_decay_values;
    std::vector<int> decay_times;
    std::vector<QElapsedTimer*> anim_elapsed;
    std::vector<bool> anim_active;
    float decay_steepness;
    std::vector<float> target_values;
    QTimer* timer;

    static float exponential_decay(float progress, float steepness = 4.0f);
};