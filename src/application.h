#ifndef APPLICATION_H
#define APPLICATION_H

#include <../QSingleApp/SingleApplication>


class Application : public SingleApplication
{
    Q_OBJECT
public:
    Application( int &argc, char *argv[],
                 bool allowSecondary = false,
                 Options options = Mode::User,
                 int timeout = 1000 );

#ifdef Q_OS_MACOS
public:
    bool eventFilter(QObject *watched, QEvent *event);
protected:
    Qt::ApplicationState _prevAppState;
signals:
    void clickedOnDock();
#endif
};

#endif // APPLICATION_H
