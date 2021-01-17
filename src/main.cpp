#include "Params.h"
#include "mainwindow.h"

#include <QApplication>
#include <../QSingleApp/SingleApplication>
#include <QDir>
#include <QMenu>

int main(int argc, char *argv[])
{
    QString arg0(argv[0]);
    qputenv("QT_QPA_PLATFORM_PLUGIN_PATH", QString(QFileInfo(argv[0]).dir().path()+"/Plugins").toUtf8());
    SingleApplication app(argc, argv, true);
    if( app.isSecondary() ) {

        app.sendMessage((app.arguments().join(' ').remove(arg0) + Params::FROM_SECOND).toUtf8(), 30000);
        return 0;
    }
    app.setQuitOnLastWindowClosed(!QSystemTrayIcon::isSystemTrayAvailable());
    QSystemTrayIcon tray;
    tray.setIcon(QIcon(":/imgs/logo"));
    QProcess p;
    p.setReadChannelMode(QProcess::MergedChannels);
    QMenu menu;
    MainWindow w(&p, &tray, &menu);
    QObject::connect(&app, SIGNAL(instanceStarted()), &w, SLOT(show()));
    QObject::connect(&app, SIGNAL(receivedMessage(quint32, QByteArray)), &w, SLOT(parceReceivedMessage(quint32, QByteArray)));

    QObject::connect(&app, SIGNAL(aboutToQuit()), &p, SLOT(terminate()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), &p, SLOT(kill()));

    menu.addAction("Quit", &app, SLOT(quit()));
    w.parceReceivedMessage(0, QString(app.arguments().join(' ')).remove(arg0).toUtf8());

    return app.exec();
}

