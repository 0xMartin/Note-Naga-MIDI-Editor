#include "separator.h"
#include <QPainter>

Separator::Separator(Orientation orientation, const QColor &color, QWidget *parent)
    : QWidget(parent),
      m_orientation(orientation),
      m_color(color)
{
    if (m_orientation == Vertical) {
        setFixedWidth(12);
        setMinimumHeight(8);
    } else {
        setFixedHeight(12);
        setMinimumWidth(8);
    }
    setAttribute(Qt::WA_TranslucentBackground);
}

void Separator::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);
    QPen pen(m_color);
    pen.setWidth(1);
    p.setPen(pen);

    if (m_orientation == Vertical) {
        int x = width() / 2;
        p.drawLine(x, 0, x, height());
    } else {
        int y = height() / 2;
        p.drawLine(0, y, width(), y);
    }
}