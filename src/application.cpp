#include "application.h"
#include <QApplicationStateChangeEvent>
#include <QDebug>

Application::Application(int &argc, char *argv[], bool allowSecondary, Options options, int timeout)
    :SingleApplication(argc, argv, allowSecondary, options, timeout)
{
#ifdef Q_OS_MACOS
    installEventFilter(this);
#endif // Q_OS_MACOS
}

#ifdef Q_OS_MACOS
bool Application::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == this && event->type() == QEvent::ApplicationStateChange) {
        auto ev = static_cast<QApplicationStateChangeEvent*>(event);
        if (_prevAppState == Qt::ApplicationActive
                && ev->applicationState() == Qt::ApplicationActive) {
            emit clickedOnDock();
        }
        _prevAppState = ev->applicationState();
    }
    return SingleApplication::eventFilter(watched, event);
}
#endif // Q_OS_MACOS
