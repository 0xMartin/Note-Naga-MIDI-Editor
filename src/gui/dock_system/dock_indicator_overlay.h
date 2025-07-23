#pragma once

#include <QWidget>

// ---------- Forward declarations ----------
class AdvancedDockWidget;

/**
 * @brief The DockIndicatorOverlay class provides visual feedback for docking areas
 * when dragging a dock widget.
 */
class DockIndicatorOverlay : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Constructs a DockIndicatorOverlay for the given AdvancedDockWidget.
     * @param parent Pointer to the AdvancedDockWidget that this overlay belongs to.
     */
    explicit DockIndicatorOverlay(AdvancedDockWidget* parent);

    /**
     * @brief Get index of area at the given position.
     * @param pos Mouse position to check.
     * @return Area index (DockIndicatorOverlay::Area).
     */
    int areaAt(const QPoint& pos) const;

    /**
     * @brief Highlights the area at the given position.
     * @param area Area index (DockIndicatorOverlay::Area).
     */
    void highlightArea(int area);

    /**
     * @brief Clears the highlighted area.
     */
    void applyDocking();

protected:
    void paintEvent(QPaintEvent*) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void leaveEvent(QEvent*) override;

private:
    int highlightedArea;
    AdvancedDockWidget* advDock;
};