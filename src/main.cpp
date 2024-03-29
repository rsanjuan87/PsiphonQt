#include "Params.h"
#include "defs.h"
#include "mainwindow.h"
#include "application.h"

#include <QDir>
#include <QFile>
#include <QMenu>


void showMan(){
    qInfo() << qAppName() + " command line help";
    qInfo() << "";
    qInfo() << "Tunnel params";
    qInfo() << Params::START_TUNNEL << " Start tunnel connection on alredy running app or open app with started connection";
    qInfo() << Params::STOP_TUNNEL << " Stop tunnel connection on already running app";
    qInfo() << Params::TOGGLE_TUNNEL << " Start or stop tunnel connection on already running app";
    qInfo() << "";
    qInfo() << "App params";
    qInfo() << Params::SHOW_PAGE+"#" << " Start or raise app showing # page index ";
    qInfo() << Params::HELP << " Show this help and exit this instance ";
    qInfo() << Params::QUIT_APP << " Close and stop the tunnel and app ";
}

int main(int argc, char *argv[])
{
    QString arg0(argv[0]);
    qputenv("QT_QPA_PLATFORM_PLUGIN_PATH", QString(QFileInfo(argv[0]).dir().path()+"/Plugins").toUtf8());
    Application app(argc, argv, true);
    QStringList argl = app.arguments();

    QString args = argl.join(' ').remove(arg0).trimmed();
    if(args == Params::HELP){
        showMan();
        exit(0);
    }else if (argl.last().endsWith("config.json") && QFile::exists(argl.last())) {
        int i = QMessageBox::question(0, qAppName(),
                                      QObject::tr("Va a remplazar la configuración actual con la de este archivo\n"
                                      "Desea continuar?"),
                                          QMessageBox::Yes,
                                          QMessageBox::No);
        if( QMessageBox::Yes == i && app.isSecondary() && app.sendMessage(Params::QUIT_APP.toUtf8())){
            QFile::copy(args, Defs::configDirPath());
        }
    }else if( app.isSecondary() ) {
        qInfo() << "Sending message " << app.arguments().join(' ').remove(arg0);
        app.sendMessage((args + Params::FROM_SECOND).toUtf8(), 30000);
        qInfo() << "Done";
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
#ifdef Q_OS_MACOS
    QObject::connect(&app, SIGNAL(clickedOnDock()), &w, SLOT(show()));
#endif
    menu.addAction("Quit", &w, SLOT(quit()));
    w.parceReceivedMessage(0, args.toUtf8());

    return app.exec();
}

