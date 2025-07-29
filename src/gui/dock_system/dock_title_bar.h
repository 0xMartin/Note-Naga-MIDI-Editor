#pragma once

#include <QWidget>
#include <QIcon>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

// Forward declaration of AdvancedDockWidget to avoid circular dependency
class AdvancedDockWidget;

/**
 * @brief CustomDockTitleBar is a custom title bar for AdvancedDockWidget.
 * It includes the title, icon, and optional custom buttons.
 */
class CustomDockTitleBar : public QFrame {
    Q_OBJECT
public:
    explicit CustomDockTitleBar(
        AdvancedDockWidget* dock,
        const QString& title,
        const QIcon& icon,
        QWidget* customButtonWidget = nullptr,
        bool horizontal = true);

    void setTitleText(const QString& text);
    void setTitleIcon(const QIcon& icon);
    void setCustomButtonWidget(QWidget* widget);

    QString getTitleText() const { return titleLabel->text(); }
    QWidget* getCustomButtonWidget() const { return customButtons; }

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onFloatClicked();
    void onCloseClicked();

private:
    AdvancedDockWidget* dockWidget = nullptr;
    QLabel* iconLabel = nullptr;
    QLabel* titleLabel = nullptr;
    QWidget* customButtons = nullptr;
    QWidget* defaultButtons = nullptr;

    bool horizontal = true;
    bool dragging = false;
    QPoint dragStartPos;

    QPushButton* createDefaultButton(const QIcon& icon, const QString& tooltip, const char* slotName);
    void updateDefaultButtons();
};