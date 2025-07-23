#pragma once

#include <QDockWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QList>
#include <QPointer>
#include <QPushButton>
#include <QWidget>

// ---------- Forward declarations ----------
class CustomDockTitleBar;
class DockIndicatorOverlay;

/**
 * @brief AdvancedDockWidget is a custom dock widget that extends QDockWidget
 *        with a custom title bar and drag-and-drop functionality.
 */
class AdvancedDockWidget : public QDockWidget {
    Q_OBJECT
public:
    /**
     * @brief Area enum defines the possible docking areas for the dock widget.
     */
    enum Area { Center = 0, Top = 1, Bottom = 2, Left = 3, Right = 4, None = -1 };

    /**
     * @brief Constructs an AdvancedDockWidget with a title, icon, and optional custom
     * button widget.
     * @param title The title of the dock widget.
     * @param icon The icon for the dock widget.
     * @param customButtonWidget Optional custom button widget to be added to the title
     * bar.
     * @param parent Parent widget.
     */
    explicit AdvancedDockWidget(
        const QString &title, const QIcon &icon = QIcon(),
        QWidget *customButtonWidget = nullptr, // optional, can be nullptr
        QWidget *parent = nullptr);

    /**
     * @brief Sets the title text of the dock widget.
     * @param text The new title text.
     */
    void setTitleText(const QString &text);

    /**
     * @brief Sets the icon of the dock widget.
     * @param icon The new icon.
     */
    void setTitleIcon(const QIcon &icon);

    /**
     * @brief Sets a custom button widget in the title bar.
     * @param widget The custom button widget to set.
     */
    void setCustomButtonWidget(QWidget *widget);

    /**
     * @brief Gets the title bar widget.
     * @return Pointer to the CustomDockTitleBar.
     */
    CustomDockTitleBar *getTitleBar() const { return titleBar; }

    /**
     * @brief Shows the dock overlay for docking.
     * This is used to indicate where the dock can be dropped.
     * It will show the overlay with docking indicators.
     */
    void showDockOverlay();

    /**
     * @brief Hides the dock overlay.
     * This is called when the dock is dropped or no longer needed.
     */
    void hideDockOverlay();

    /**
     * @brief Updates the dock overlay position based on the global mouse position.
     * @param globalPos Position of dock.
     */
    void dockToArea(AdvancedDockWidget::Area area);

    /**
     * @brief Sets the floating state of the dock widget.
     * @param floating True to set the dock widget as floating, false to dock it.
     */
    void setFloating(bool floating);

public slots:
    void toggleMaximizeRestoreFloating();

private:
    bool floatingMaximized = false;
    QRect floatingRestoreGeometry;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    friend class DockIndicatorOverlay;
    friend class CustomDockTitleBar;
    DockIndicatorOverlay *overlay = nullptr;

    // Dragging state
    bool dragging = false;
    bool dragOnTitleBar = false;
    QPoint dragStartPos;

    // Title bar
    CustomDockTitleBar *titleBar = nullptr;

    // Dragging support (for docking)
    void startDragFromTitleBar(const QPoint &from, const QPoint &current);
    void endDragFromTitleBar();
    void updateDockOverlay(const QPoint &globalPos);
};