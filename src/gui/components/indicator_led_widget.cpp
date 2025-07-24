#include "indicator_led_widget.h"
#include <QBrush>
#include <QPainter>
#include <QPen>

IndicatorLedWidget::IndicatorLedWidget(const QColor &color, QWidget *parent)
    : QWidget(parent), led_color(color), is_active(false)
{
    setMinimumSize(18, 18);
    setMaximumSize(100, 100);
    connect(&timer, &QTimer::timeout, this, &IndicatorLedWidget::restorePreviousState);
}

QSize IndicatorLedWidget::sizeHint() const { return QSize(22, 22); }

QSize IndicatorLedWidget::minimumSizeHint() const { return QSize(18, 18); }

void IndicatorLedWidget::setState(bool state) {
    if (timer.isActive()) {
        timer.stop();
    }
    if (is_active != state) {
        is_active = state;
        update();
    }
}

void IndicatorLedWidget::setState(bool state, bool end_active, int time_ms) {
    // Clamp time to max 50ms, min 0
    int duration = qBound(50, time_ms, 999999);
    if (timer.isActive()) {
        timer.stop();
    }
    this->end_active = end_active;
    this->is_active = state;
    update();
    if (duration > 0) {
        timer.start(duration);
    } else {
        // If time is 0, restore immediately
        restorePreviousState();
    }
}

void IndicatorLedWidget::restorePreviousState() {
    timer.stop();
    this->is_active = this->end_active;
    update();
}

void IndicatorLedWidget::setColor(const QColor &color) {
    if (led_color != color) {
        led_color = color;
        update();
    }
}

void IndicatorLedWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    float w = width();
    float h = height();
    float s = qMin(w, h) - 5;

    QRectF ledRect((w - s) / 2, (h - s) / 2, s, s);

    // Draw dark border
    QPen borderPen(QColor(20, 20, 20), 4);
    p.setPen(borderPen);
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(ledRect);

    // LED fill (off = dark, on = color)
    QRadialGradient grad(ledRect.center(), s / 2);
    if (is_active) {
        grad.setColorAt(0.0, led_color.lighter(160));
        grad.setColorAt(0.7, led_color);
        grad.setColorAt(1.0, led_color.darker(180));
    } else {
        QColor offColor = led_color.darker(400);
        grad.setColorAt(0.0, offColor.lighter(110));
        grad.setColorAt(1.0, offColor);
    }
    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(grad));
    p.drawEllipse(ledRect);
}