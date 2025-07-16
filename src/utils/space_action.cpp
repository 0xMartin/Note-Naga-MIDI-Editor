#include "space_action.h"

#include <QKeyEvent>
#include <Qt>

SpaceAction::SpaceAction(QObject* parent, QAction* action)
    : QObject(parent), action(action)
{
}

bool SpaceAction::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Space) {
            if (action) {
                action->trigger();
            }
            return true;
        }
    }
    return false;
}