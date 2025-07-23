#pragma once

#include <QWidget>
#include <QIcon>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

// ---------- Forward declarations ----------
class AdvancedDockWidget;

/**
 * @brief CustomDockTitleBar is a custom title bar for AdvancedDockWidget.
 * It includes the title, icon, and optional custom buttons.
 */
class CustomDockTitleBar : public QFrame {
    Q_OBJECT
public:
    /**
     * @brief Constructs a CustomDockTitleBar with a title, icon, and optional custom
     * button widget.
     * @param dock Pointer to the AdvancedDockWidget this title bar belongs to.
     * @param title The title text for the dock widget.
     * @param icon The icon for the dock widget.
     * @param customButtonWidget Optional custom button widget to be added to the title
     * bar.
     */
    explicit CustomDockTitleBar(
        AdvancedDockWidget* dock,
        const QString& title,
        const QIcon& icon,
        QWidget* customButtonWidget = nullptr);

    /**
     * @brief Sets the title text of the title bar.
     * @param text The new title text.
     */
    void setTitleText(const QString& text);

    /**
     * @brief Sets the icon of the title bar.
     * @param icon The new icon.
     */
    void setTitleIcon(const QIcon& icon);

    /**
     * @brief Sets a custom button widget to be displayed in the title bar.
     * @param widget The custom button widget to set.
     */
    void setCustomButtonWidget(QWidget* widget);

    /**
     * @brief Gets the current title text of the title bar.
     * @return The title text.
     */
    QString getTitleText() const {
        return titleLabel->text();
    }

    /**
     * @brief Gets the current icon of the title bar.
     * @return The icon.
     */
    QWidget* getCustomButtonWidget() const {
        return customButtons;
    }

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private slots:
    void onFloatClicked();
    void onCloseClicked();

private:
    AdvancedDockWidget* dockWidget = nullptr;

    QLabel* iconLabel = nullptr;
    QLabel* titleLabel = nullptr;
    QWidget* customButtons = nullptr;
    QWidget* defaultButtons = nullptr;

    // Drag tracking
    bool dragging = false;
    QPoint dragStartPos;

    QPushButton* createDefaultButton(const QIcon& icon, const QString& tooltip, const char* slotName);
    void updateDefaultButtons();
};