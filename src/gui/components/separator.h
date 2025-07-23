#pragma once

#include <QWidget>

class Separator : public QWidget
{
    Q_OBJECT
public:
    enum Orientation {
        Horizontal,
        Vertical
    };

    explicit Separator(Orientation orientation, const QColor &color = QColor("#19191f"), QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Orientation m_orientation;
    QColor m_color;
};