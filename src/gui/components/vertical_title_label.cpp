#include "vertical_title_label.h"

#include <QPainter>
#include <QFontMetrics>
#include <QStyleOption>

VerticalTitleLabel::VerticalTitleLabel(const QString& text, QWidget* parent)
    : QWidget(parent), m_text(text)
{
    m_font = QFont("Segoe UI", 18, QFont::Bold);
    setMinimumHeight(40);
    setMinimumWidth(32);
}

void VerticalTitleLabel::setText(const QString& text) {
    m_text = text;
    update();
}

void VerticalTitleLabel::setFont(const QFont& font) {
    m_font = font;
    update();
}

QSize VerticalTitleLabel::sizeHint() const {
    QFontMetrics fm(m_font);
    // Since it will be drawn vertically, width = text height, height = text width
    int w = fm.height() + 8;
    int h = fm.horizontalAdvance(m_text) + 8;
    return QSize(w, h);
}

void VerticalTitleLabel::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(m_font);

    QFontMetrics fm(m_font);
    int textWidth = fm.horizontalAdvance(m_text);
    int textHeight = fm.height();

    // Center horizontally (X), align to top vertically (Y=0)
    int x = width() / 2;
    int y = 0;

    painter.save();
    painter.translate(x, y);
    painter.rotate(-90); // rotate counterclockwise
    painter.setPen(QColor("#79b8ff"));
    // Draw text horizontally, but rotated
    painter.drawText(QRect(-textWidth / 2, 0, textWidth, textHeight), Qt::AlignCenter, m_text);
    painter.restore();
}