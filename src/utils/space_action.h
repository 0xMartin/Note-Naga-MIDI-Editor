#pragma once

#include <QObject>
#include <QEvent>
#include <QAction>

class SpaceAction : public QObject {
    Q_OBJECT
public:
    explicit SpaceAction(QObject* parent, QAction* action);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    QAction* action;
};