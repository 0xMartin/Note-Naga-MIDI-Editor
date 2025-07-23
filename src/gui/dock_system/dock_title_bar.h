#pragma once

#include <QWidget>
#include <QIcon>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

class AdvancedDockWidget;

class CustomDockTitleBar : public QFrame {
    Q_OBJECT
public:
    explicit CustomDockTitleBar(
        AdvancedDockWidget* dock,
        const QString& title,
        const QIcon& icon,
        QWidget* customButtonWidget = nullptr);

    void setTitleText(const QString& text);
    void setTitleIcon(const QIcon& icon);
    void setCustomButtonWidget(QWidget* widget);

    QString getTitleText() const {
        return titleLabel->text();
    }
    QWidget* getCustomButtonWidget() const {
        return customButtons;
    }

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

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