#pragma once

#include <QWidget>

class AdvancedDockWidget;

class DockIndicatorOverlay : public QWidget {
    Q_OBJECT
public:
    enum Area { Center = 0, Top = 1, Bottom = 2, Left = 3, Right = 4, None = -1 };

    explicit DockIndicatorOverlay(AdvancedDockWidget* parent);

    int areaAt(const QPoint& pos) const;
    void highlightArea(int area);
    void applyDocking();

protected:
    void paintEvent(QPaintEvent*) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void leaveEvent(QEvent*) override;

private:
    int highlightedArea;
    AdvancedDockWidget* advDock;
};