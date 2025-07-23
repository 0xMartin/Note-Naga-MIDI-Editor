#pragma once

#include <QColor>
#include <QWidget>
#include <QTimer>

/**
 * @brief The IndicatorLedWidget class provides a simple LED indicator widget.
 * It can be used to visually indicate the status of a process or component.
 */
class IndicatorLedWidget : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Constructor for the IndicatorLedWidget.
     * @param color The color of the LED indicator. Default is green.
     * @param parent The parent widget. Default is nullptr.
     */
    explicit IndicatorLedWidget(const QColor &color = Qt::green,
                                QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    /**
     * @brief Gets the current color of the LED indicator.
     * @return The current color.
     */
    QColor getColor() const { return led_color; }

    /**
     * @brief Gets the active state of the LED indicator.
     * @return True if the LED is active, false otherwise.
     */
    bool isActive() const { return is_active; }

public slots:
    /**
     * @brief Sets the LED state directly (on/off).
     * @param state True for on, false for off.
     */
    void setState(bool state);

    /**
     * @brief Temporarily sets LED state; after time_ms (max 50ms) returns to previous state.
     * @param state State to set (true = on, false = off).
     * @param end_active State to restore after timeout.
     * @param time_ms Duration in ms (will be clamped to max 50ms).
     */
    void setState(bool state, bool end_active, int time_ms);

    /**
     * @brief Sets the color of the LED indicator.
     * @param color The new color for the LED indicator.
     */
    void setColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor led_color;
    bool is_active;

    QTimer timer;
    bool end_active = false;

private slots:
    void restorePreviousState();
};