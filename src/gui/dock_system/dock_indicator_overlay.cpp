#include "dock_indicator_overlay.h"

#include "advanced_dock_widget.h"
#include <QMouseEvent>
#include <QPainter>

DockIndicatorOverlay::DockIndicatorOverlay(AdvancedDockWidget* parent)
    : QWidget(parent), advDock(parent)
{
    highlightedArea = -1;
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    setGeometry(parent->rect());
    raise();
    show();
}

void DockIndicatorOverlay::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.setBrush(QColor(40, 60, 120, 40));
    p.setPen(Qt::NoPen);
    p.drawRect(rect());

    int cx = width() / 2, cy = height() / 2, iconR = 28;
    QPoint centers[5] = {
        QPoint(cx, cy),                 // center
        QPoint(cx, 36),                 // top
        QPoint(cx, height() - 36),      // bottom
        QPoint(36, cy),                 // left
        QPoint(width() - 36, cy)        // right
    };
    for (int i = 0; i < 5; ++i) {
        QPoint center = centers[i];
        QColor c = (highlightedArea == i) ? QColor(255, 255, 200, 200)
                                          : QColor(230, 230, 255, 90);
        p.setBrush(c);
        p.setPen(QPen(Qt::white, (highlightedArea == i) ? 3 : 1));
        p.drawEllipse(center, iconR, iconR);

        p.setPen(Qt::NoPen);
        p.setBrush(QColor(60, 80, 120, 200));
        if (i == 0) {
            p.drawEllipse(center, 8, 8);
        } else {
            QPoint arr[3];
            switch (i) {
            case 1: arr[0] = center + QPoint(0, -12); arr[1] = center + QPoint(-10, 8); arr[2] = center + QPoint(10, 8); break;
            case 2: arr[0] = center + QPoint(0, 12); arr[1] = center + QPoint(-10, -8); arr[2] = center + QPoint(10, -8); break;
            case 3: arr[0] = center + QPoint(-12, 0); arr[1] = center + QPoint(8, -10); arr[2] = center + QPoint(8, 10); break;
            case 4: arr[0] = center + QPoint(12, 0); arr[1] = center + QPoint(-8, -10); arr[2] = center + QPoint(-8, 10); break;
            }
            p.drawPolygon(arr, 3);
        }
    }
}

int DockIndicatorOverlay::areaAt(const QPoint& pos) const {
    int cx = width() / 2, cy = height() / 2;
    QPoint centers[5] = {
        QPoint(cx, cy),
        QPoint(cx, 36),
        QPoint(cx, height() - 36),
        QPoint(36, cy),
        QPoint(width() - 36, cy)
    };
    for (int i = 0; i < 5; ++i) {
        if (QLineF(pos, centers[i]).length() < 28) return i;
    }
    return -1;
}

void DockIndicatorOverlay::highlightArea(int area) {
    if (area == highlightedArea) return;
    highlightedArea = area;
    update();
}

void DockIndicatorOverlay::mouseMoveEvent(QMouseEvent* e) {
    int a = areaAt(e->pos());
    if (a != highlightedArea) {
        highlightedArea = a;
        update();
    }
}

void DockIndicatorOverlay::leaveEvent(QEvent*) {
    highlightedArea = -1;
    update();
}

void DockIndicatorOverlay::applyDocking() {
    if (highlightedArea == -1) return;
    if (advDock) {
        advDock->dockToArea(static_cast<AdvancedDockWidget::Area>(highlightedArea));
        advDock->dragging = false;
        advDock->drag_on_title_bar = false;
    }
}