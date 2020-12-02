#include "mainwindow.h"

#include <QApplication>
#include <../QSingleApp/SingleApplication>
#include <QDir>
#include <QMenu>

int main(int argc, char *argv[])
{
    QString arg0(argv[0]);
    qputenv("QT_QPA_PLATFORM_PLUGIN_PATH", QString(QFileInfo(argv[0]).dir().path()+"/Plugins").toUtf8());
    SingleApplication app(argc, argv, false);
    if( app.isSecondary() ) {
        app.sendMessage(app.arguments().join(' ').toUtf8(), 30000);
        app.exit( 0 );
    }
    app.setQuitOnLastWindowClosed(!QSystemTrayIcon::isSystemTrayAvailable());
    QSystemTrayIcon tray;
    QProcess p;
    p.setReadChannelMode(QProcess::MergedChannels);
    QMenu menu;
    MainWindow w(&p, &tray, &menu);
    QObject::connect(&app, SIGNAL(instanceStarted()), &w, SLOT(show()));
    QObject::connect(&app, SIGNAL(receivedMessage(quint32, QByteArray)), &w, SLOT(receivedMessage(quint32, QByteArray)));

    QObject::connect(&app, SIGNAL(aboutToQuit()), &p, SLOT(terminate()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), &p, SLOT(kill()));

    menu.addAction("Quit", &app, SLOT(quit()));
    w.receivedMessage(0, app.arguments().join(' ').toUtf8());

    return app.exec();
}

