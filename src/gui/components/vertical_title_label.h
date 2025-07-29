#pragma once

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QFont>

class VerticalTitleLabel : public QWidget {
    Q_OBJECT
public:
    explicit VerticalTitleLabel(const QString& text, QWidget* parent = nullptr);

    void setText(const QString& text);
    QString text() const { return m_text; }
    void setFont(const QFont& font);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString m_text;
    QFont m_font;
};