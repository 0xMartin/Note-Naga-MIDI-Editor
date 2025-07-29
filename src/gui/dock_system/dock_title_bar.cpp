#include "dock_title_bar.h"

#include "advanced_dock_widget.h"
#include <QStyle>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QTransform>

CustomDockTitleBar::CustomDockTitleBar(
    AdvancedDockWidget* dock, const QString& title, const QIcon& icon, QWidget* customButtonWidget,
    bool horizontal)
    : QFrame(dock), dockWidget(dock), horizontal(horizontal)
{
    setObjectName("CustomDockTitleBar");
    setStyleSheet("QFrame#CustomDockTitleBar { background: #2b2f37; border: 1px solid #19191f; margin-bottom: 0px; }");

    if (horizontal) {
        // horizontal layout
        QHBoxLayout* header_layout = new QHBoxLayout(this);
        header_layout->setContentsMargins(10, 0, 0, 0);
        header_layout->setSpacing(6);

        // title icon
        iconLabel = new QLabel(this);
        iconLabel->setPixmap(icon.pixmap(20, 20));
        iconLabel->setFixedSize(20, 20);
        header_layout->addWidget(iconLabel, 0, Qt::AlignVCenter);

        // title label
        titleLabel = new QLabel(title, this);
        titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #79b8ff; letter-spacing: 1.2px;");
        header_layout->addWidget(titleLabel, 0, Qt::AlignVCenter);

        header_layout->addStretch(1);

        // custom buttons
        customButtons = nullptr;
        setCustomButtonWidget(customButtonWidget);
        if (customButtons) {
            header_layout->addWidget(customButtons, 0, Qt::AlignVCenter);
        }

        // default buttons
        QPushButton* floatBtn = createDefaultButton(QIcon(":/icons/maximize.svg"), "Undock / Dock", SLOT(onFloatClicked()));
        header_layout->addWidget(floatBtn, 0, Qt::AlignVCenter);
        QPushButton* closeBtn = createDefaultButton(QIcon(":/icons/close.svg"), "Close", SLOT(onCloseClicked()));
        header_layout->addWidget(closeBtn, 0, Qt::AlignVCenter);

    } else {
        // vertical layout
        QVBoxLayout* main_layout = new QVBoxLayout(this);
        main_layout->setContentsMargins(0, 8, 0, 4);
        main_layout->setSpacing(0);

        // title icon
        iconLabel = new QLabel(this);
        QPixmap pm = icon.pixmap(24, 24);
        QTransform tr;
        tr.rotate(-90);
        pm = pm.transformed(tr);
        iconLabel->setPixmap(pm);
        iconLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        iconLabel->setFixedSize(24, 24);
        main_layout->addWidget(iconLabel, 0, Qt::AlignHCenter | Qt::AlignTop);

        // title label
        titleLabel = new QLabel(title, this);
        titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #79b8ff; letter-spacing: 1.2px;");
        titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        titleLabel->setMinimumHeight(60);
        main_layout->addWidget(titleLabel, 0, Qt::AlignHCenter | Qt::AlignTop);

        main_layout->addSpacing(16);

        // custom buttons
        customButtons = nullptr;
        setCustomButtonWidget(customButtonWidget);
        if (customButtons) {
            main_layout->addWidget(customButtons, 0, Qt::AlignHCenter | Qt::AlignBottom);
        }

        // default buttons
        QPushButton* floatBtn = createDefaultButton(QIcon(":/icons/maximize.svg"), "Undock / Dock", SLOT(onFloatClicked()));
        main_layout->addWidget(floatBtn, 0, Qt::AlignHCenter);
        QPushButton* closeBtn = createDefaultButton(QIcon(":/icons/close.svg"), "Close", SLOT(onCloseClicked()));
        main_layout->addWidget(closeBtn, 0, Qt::AlignHCenter);
    }
}

void CustomDockTitleBar::setTitleText(const QString& text) {
    titleLabel->setText(text);
    update();
}

void CustomDockTitleBar::setTitleIcon(const QIcon& icon) {
    if (horizontal) {
        iconLabel->setPixmap(icon.pixmap(23, 23));
    } else {
        QPixmap pm = icon.pixmap(20, 20);
        QTransform tr;
        tr.rotate(-90);
        pm = pm.transformed(tr);
        iconLabel->setPixmap(pm);
    }
    update();
}

void CustomDockTitleBar::setCustomButtonWidget(QWidget* widget) {
    if (customButtons && customButtons->parent() == this) {
        customButtons->hide();
        customButtons->deleteLater();
    }
    customButtons = widget;
    if (customButtons) {
        customButtons->setParent(this);
        customButtons->setStyleSheet("QWidget { background: transparent; }");
        customButtons->show();
    }
}

QPushButton* CustomDockTitleBar::createDefaultButton(const QIcon& icon, const QString& tooltip, const char* slotName) {
    QPushButton* btn = new QPushButton(icon, "", this);
    btn->setToolTip(tooltip);
    btn->setFixedSize(24, 24);
    btn->setFlat(true);
    btn->setStyleSheet("QPushButton { background: transparent; border: none; border-radius: 0px; min-width: 24px; max-width: 24px; min-height: 24px; max-height: 24px; padding: 0px;}"
                       "QPushButton:hover { background: #5a35a0; color: #fff; }");
    connect(btn, SIGNAL(clicked()), this, slotName);
    return btn;
}

void CustomDockTitleBar::onFloatClicked() {
    if (dockWidget) dockWidget->setFloating(!dockWidget->isFloating());
}

void CustomDockTitleBar::onCloseClicked() {
    if (dockWidget) dockWidget->close();
}

void CustomDockTitleBar::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        setCursor(Qt::SizeAllCursor);
        dragging = true;
        dragStartPos = event->globalPos();
        event->accept();
    } else {
        QFrame::mousePressEvent(event);
    }
}

void CustomDockTitleBar::mouseMoveEvent(QMouseEvent* event) {
    if (dragging && (event->buttons() & Qt::LeftButton)) {
        if (dockWidget && dockWidget->isFloating()) {
            QPoint delta = event->globalPos() - dragStartPos;
            dockWidget->window()->move(dockWidget->window()->pos() + delta);
            dockWidget->startDragFromTitleBar(dragStartPos, event->globalPos());
            dragStartPos = event->globalPos();
        } else if (dockWidget) {
            dockWidget->startDragFromTitleBar(dragStartPos, event->globalPos());
        }
        event->accept();
    }
}

void CustomDockTitleBar::mouseReleaseEvent(QMouseEvent* event) {
    unsetCursor();
    if (dragging) {
        dragging = false;
        if (dockWidget) dockWidget->endDragFromTitleBar();
        event->accept();
    }
}

void CustomDockTitleBar::mouseDoubleClickEvent(QMouseEvent* event) {
    if (dockWidget && dockWidget->isFloating()) {
        dockWidget->toggleMaximizeRestoreFloating();
        event->accept();
        return;
    }
    QFrame::mouseDoubleClickEvent(event);
}

void CustomDockTitleBar::paintEvent(QPaintEvent* event) {
    QFrame::paintEvent(event);
    if (!horizontal) {
        // Vykreslení rotovaného textu přesně uprostřed titleLabel
        QPainter p(this);

        QRect labelRect = titleLabel->geometry();
        QString text = titleLabel->text();

        QFontMetrics fm(titleLabel->font());
        int textWidth = fm.horizontalAdvance(text);
        int textHeight = fm.height();

        // Vypočítej střed labelu
        int cx = labelRect.x() + labelRect.width() / 2;
        int cy = labelRect.y() + labelRect.height() / 2;

        p.save();

        // Posuň na střed labelu
        p.translate(cx, cy);
        // Otoč o -90°
        p.rotate(-90);

        // Vykresli text centrovaně (center na rotated labelu)
        QRect rotatedRect(-labelRect.height()/2, -labelRect.width()/2, labelRect.height(), labelRect.width());
        p.setFont(titleLabel->font());
        p.setPen(QColor("#79b8ff"));
        p.drawText(rotatedRect, Qt::AlignCenter, text);

        p.restore();

        titleLabel->hide();
    } else {
        titleLabel->show();
    }
}