#pragma once

#include <QWidget>
#include <QColor>
#include <QPointF>
#include <QRectF>
#include <QString>

class AudioDialCentered : public QWidget {
    Q_OBJECT
public:
    explicit AudioDialCentered(QWidget* parent = nullptr);

    float value() const;
    void setValue(float value);

    void setDefaultValue(float value);
    void setRange(float min_val, float max_val);
    void setGradient(const QColor& color_start, const QColor& color_end);

    void setLabel(const QString& label);
    void showLabel(bool show);
    void showValue(bool show);
    void setValuePrefix(const QString& prefix);
    void setValuePostfix(const QString& postfix);
    void setValueDecimals(int decimals);

signals:
    void valueChanged(float);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void updateGeometryCache();
    std::tuple<int, int, int, QPointF, float, float> getCircleGeometry();
    float angleToValue(float angle_deg) const;
    bool inCircleArea(const QPoint& pos);

    float _min;
    float _max;
    float _value;
    float _default_value;

    int _start_angle;
    int _angle_range;

    QColor bg_color;
    QColor inner_color;
    QColor inner_outline;
    QColor arc_bg_color;
    QColor dot_color;
    QColor dot_end_color;
    QColor gradient_start;
    QColor gradient_end;

    bool _pressed;

    QString _label;
    bool _show_label;
    bool _show_value;
    QString _value_prefix;
    QString _value_postfix;
    int _value_decimals;

    // Geometry cache
    std::tuple<int, int, int, QPointF, float, float> _geometry_cache;
    std::tuple<int, int, bool, bool, QString, int> _geometry_cache_size;
};