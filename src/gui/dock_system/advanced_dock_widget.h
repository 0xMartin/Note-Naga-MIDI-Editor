#pragma once

#include <QDockWidget>
#include <QIcon>
#include <QPointer>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QWidget>
#include <QList>

// ---------- Forward declarations ----------
class CustomDockTitleBar;
class DockIndicatorOverlay;

// ---------- AdvancedDockWidget ----------
class AdvancedDockWidget : public QDockWidget {
    Q_OBJECT
public:
    explicit AdvancedDockWidget(
        const QString& title,
        const QIcon& icon = QIcon(),
        QWidget* customButtonWidget = nullptr, // optional, can be nullptr
        QWidget* parent = nullptr);

    void setTitleText(const QString& text);
    void setTitleIcon(const QIcon& icon);
    void setCustomButtonWidget(QWidget* widget);

    CustomDockTitleBar* getTitleBar() const { return titleBar; }

    // Dragging support
    void startDragFromTitleBar(const QPoint& from, const QPoint& current);
    void endDragFromTitleBar();

    // For overlay:
    void showDockOverlay();
    void hideDockOverlay();
    void updateDockOverlay(const QPoint& globalPos);
    void dockToArea(int area);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    friend class DockIndicatorOverlay;
    friend class CustomDockTitleBar;
    DockIndicatorOverlay* overlay = nullptr;

    // Dragging state
    bool dragging = false;
    bool dragOnTitleBar = false;
    QPoint dragStartPos;

    // Title bar
    CustomDockTitleBar* titleBar = nullptr;
};