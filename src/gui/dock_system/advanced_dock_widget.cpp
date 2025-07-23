#include "advanced_dock_widget.h"

#include <QApplication>
#include <QMainWindow>
#include <QMouseEvent>
#include <QStyleOptionDockWidget>
#include <QStylePainter>

#include "dock_indicator_overlay.h"
#include "dock_title_bar.h"

AdvancedDockWidget::AdvancedDockWidget(const QString &title, const QIcon &icon,
                                       QWidget *customButtonWidget, QWidget *parent)
    : QDockWidget(title, parent) {
    setFeatures(DockWidgetMovable | DockWidgetFloatable | DockWidgetClosable);
    titleBar = new CustomDockTitleBar(this, title, icon, customButtonWidget);
    setTitleBarWidget(titleBar);
    setMouseTracking(true);
    setStyleSheet("QDockWidget { border: 1px solid #19191f; }");
}

void AdvancedDockWidget::setTitleText(const QString &text) {
    if (titleBar) titleBar->setTitleText(text);
}

void AdvancedDockWidget::setTitleIcon(const QIcon &icon) {
    if (titleBar) titleBar->setTitleIcon(icon);
}

void AdvancedDockWidget::setCustomButtonWidget(QWidget *widget) {
    if (titleBar) titleBar->setCustomButtonWidget(widget);
}

void AdvancedDockWidget::startDragFromTitleBar(const QPoint &from,
                                               const QPoint &current) {
    if (!dragging) {
        dragging = true;
        dragOnTitleBar = true;
        dragStartPos = from;
    }
    QMouseEvent fakeMove(QEvent::MouseMove, mapFromGlobal(current), current,
                         Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    mouseMoveEvent(&fakeMove);
}

void AdvancedDockWidget::endDragFromTitleBar() {
    for (QWidget *w : QApplication::topLevelWidgets()) {
        QMainWindow *mw = qobject_cast<QMainWindow *>(w);
        if (!mw) continue;
        auto docks = mw->findChildren<AdvancedDockWidget *>();
        for (auto dock : docks)
            dock->hideDockOverlay();
    }
    dragging = false;
    dragOnTitleBar = false;
}

void AdvancedDockWidget::mousePressEvent(QMouseEvent *event) {
    QDockWidget::mousePressEvent(event);
}

void AdvancedDockWidget::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && dragOnTitleBar && (event->buttons() & Qt::LeftButton)) {
        for (QWidget *w : QApplication::topLevelWidgets()) {
            QMainWindow *mw = qobject_cast<QMainWindow *>(w);
            if (!mw) continue;
            auto docks = mw->findChildren<AdvancedDockWidget *>();
            for (auto dock : docks) {
                if (dock == this) continue;
                QRect gr = dock->rect();
                QPoint rel = dock->mapFromGlobal(event->globalPosition().toPoint());
                if (gr.contains(rel)) {
                    dock->showDockOverlay();
                    dock->updateDockOverlay(event->globalPosition().toPoint());
                } else {
                    dock->hideDockOverlay();
                }
            }
        }
    }

    QDockWidget::mouseMoveEvent(event);
}

void AdvancedDockWidget::mouseReleaseEvent(QMouseEvent *event) {
    dragging = false;
    QDockWidget::mouseReleaseEvent(event);
}

void AdvancedDockWidget::paintEvent(QPaintEvent *event) {
    QStyleOptionDockWidget opt;
    opt.rect = rect();
    opt.title = windowTitle();
    opt.closable = features() & QDockWidget::DockWidgetClosable;
    opt.floatable = features() & QDockWidget::DockWidgetFloatable;
    opt.movable = features() & QDockWidget::DockWidgetMovable;
    opt.verticalTitleBar = false;
    QStylePainter p(this);
    p.drawPrimitive(QStyle::PE_Widget, opt);
    QDockWidget::paintEvent(event);
}

void AdvancedDockWidget::setFloating(bool floating) {
    QDockWidget::setFloating(floating);

    if (floating) {
        setWindowFlags(Qt::Window |
                       Qt::WindowTitleHint |
                       Qt::WindowSystemMenuHint |
                       Qt::WindowMinMaxButtonsHint |
                       Qt::WindowCloseButtonHint);
        show();
    } else {
        setWindowFlags(Qt::Widget);
        show();
    }
}

void AdvancedDockWidget::showDockOverlay() {
    if (!overlay) {
        overlay = new DockIndicatorOverlay(this);
        overlay->setGeometry(rect());
        overlay->raise();
        overlay->show();
    }
}

void AdvancedDockWidget::hideDockOverlay() {
    if (overlay) {
        overlay->applyDocking();
        overlay->hide();
        overlay->deleteLater();
        overlay = nullptr;
    }
}

void AdvancedDockWidget::updateDockOverlay(const QPoint &globalPos) {
    if (overlay) {
        overlay->setGeometry(rect());
        overlay->raise();
        overlay->update();
        QPoint rel = mapFromGlobal(globalPos);
        int a = overlay->areaAt(rel);
        overlay->highlightArea(a);
        overlay->update();
    }
}

void AdvancedDockWidget::dockToArea(AdvancedDockWidget::Area area) {
    QMainWindow *mw = qobject_cast<QMainWindow *>(parentWidget());
    if (!mw) return;

    AdvancedDockWidget *dragged_dock = nullptr;
    auto docks = mw->findChildren<AdvancedDockWidget *>();
    for (auto d : docks) {
        if (d->dragging) {
            dragged_dock = d;
            break;
        }
    }

    QString names[] = {"Center", "Top", "Bottom", "Left", "Right"};
    QString msg = QString(">>>>>>> Mouse released at area %1")
                      .arg(area >= 0 ? names[area] : "None");
    qDebug() << msg;
    qDebug() << "Dragged dock: " << dragged_dock->objectName();
    qDebug() << "Area dock: " << this->objectName();

    if (!dragged_dock || dragged_dock == this) return;

    if (dragged_dock->isFloating()) { dragged_dock->setFloating(false); }

    switch (area) {
    case AdvancedDockWidget::Area::Center:
        mw->tabifyDockWidget(this, dragged_dock);
        break;
    case AdvancedDockWidget::Area::Top:
        mw->splitDockWidget(dragged_dock, this, Qt::Vertical);
        break;
    case AdvancedDockWidget::Area::Bottom:
        mw->splitDockWidget(this, dragged_dock, Qt::Vertical);
        break;
    case AdvancedDockWidget::Area::Left:
        mw->splitDockWidget(dragged_dock, this, Qt::Horizontal);
        break;
    case AdvancedDockWidget::Area::Right:
        mw->splitDockWidget(this, dragged_dock, Qt::Horizontal);
        break;
    }
}

void AdvancedDockWidget::toggleMaximizeRestoreFloating() {
    if (!isFloating()) return;
    QWidget *floatWin = this->window();
    if (!floatingMaximized) {
        floatingRestoreGeometry = floatWin->geometry();
        auto screen = floatWin->screen()->availableGeometry();
        floatWin->setGeometry(screen);
        floatingMaximized = true;
    } else {
        floatWin->setGeometry(floatingRestoreGeometry);
        floatingMaximized = false;
    }
}