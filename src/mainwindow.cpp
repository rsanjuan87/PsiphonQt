#include "mainwindow.h"
#include "QAutostart"
#include "ui_mainwindow.h"
#include "defs.h"
#include "CoreProcessControler.h"
#include "Params.h"

#include <QDir>
#include <QDesktopWidget>

#include <src/objects/countrydata.h>
#define DETECT_TRAY_BG_COLOR "detect"

MainWindow::MainWindow(QProcess *process, QSystemTrayIcon *tray, QMenu *menu, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle(qAppName());
    this->process = process;
    this->tray = tray;
    this->trayMenu = menu;
    connect(ui->actionToggleWindow, &QAction::triggered, this, &MainWindow::toggleWindow);
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::startStopTunnel);

    trayMenu->addAction(ui->actionConnect);
    trayMenu->addSeparator();
    trayMenu->addAction(ui->actionToggleWindow);
    trayMenu->addAction(ui->actionSettings);
    trayMenu->addAction(ui->actionTunel_settings);
    trayMenu->addAction(ui->actionAbout);
    trayMenu->addSeparator();
    process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process, &QProcess::readyRead, this, &MainWindow::readout);
    connect(process, SIGNAL(finished(int)), this, SLOT(setTunelStoped(int)));

    connect(&timerConnecting, SIGNAL(timeout()), this, SLOT(timerTick()));

    connect(tray, &QSystemTrayIcon::messageClicked, this, &MainWindow::toggleWindow);

    winPopup = new QMenu();
    widPopup = new QWidgetAction(winPopup);
    signaler.installOn(ui->logo);
      connect(&signaler, SIGNAL(mouseButtonEvent(QWidget *, QMouseEvent *)),
              this, SLOT(onGeneralClick(QWidget *, QMouseEvent *)));

    loadconfig();
    createIconsMenu();
    ui->logo->setAttribute(Qt::WA_Hover, true);
    ui->chkTrayDialog->setVisible(QSystemTrayIcon::isSystemTrayAvailable());
    if(!QSystemTrayIcon::isSystemTrayAvailable()){
        ui->chkTrayDialog->setChecked(false);
        on_chkTrayDialog_clicked();
    }

#ifdef Q_OS_MACOS
       QList<QString> list;
       QDir d = QFileInfo(qApp->applicationFilePath()).absoluteDir();
       d.cdUp();
       list << "read" << d.path()+"/Info.plist"
            << "LSUIElement";
       QProcess p;
       p.start("defaults", list);
       p.waitForFinished();
       QString s(p.readAllStandardOutput().simplified().toLower());
       if(s=="yes"|| s=="true" || s=="1")
           ui->chkHideFromDockMac->setChecked(true);
       dockMenu = new QMenu();
       dockMenu->addAction(ui->actionConnect);
       dockMenu->addSeparator();
       dockMenu->addAction(ui->actionToggleWindow);
       dockMenu->addAction(ui->actionSettings);
       dockMenu->addAction(ui->actionTunel_settings);
       dockMenu->addAction(ui->actionAbout);
       dockMenu->setAsDockMenu();
#else
    setMenuBar(new QMenuBar);
    ui->chkHideFromDockMac->setVisible(false);
#endif
    detectSetWindowsTheme();

    ui->version->setText(tr("Versión: ")+VERSION);

}

void MainWindow::detectSetWindowsTheme(){
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",QSettings::NativeFormat);
    if(settings.value("AppsUseLightTheme")==0){
        qApp->setStyle(QStyleFactory::create("Fusion"));
        QPalette darkPalette;
        QColor darkColor = QColor(10,10,10);
        QColor disabledColor = QColor(127,127,127);
        darkPalette.setColor(QPalette::Window, darkColor);
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(18,18,18));
        darkPalette.setColor(QPalette::AlternateBase, darkColor);
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
        darkPalette.setColor(QPalette::Button, darkColor);
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

        qApp->setPalette(darkPalette);
        QFile file(":/res/winpopup.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = QString::fromLatin1(file.readAll());
        styleSheet=styleSheet.replace("#001133", darkColor.name());
        qApp->setStyleSheet(styleSheet);
    }
#endif
}

void MainWindow::setVisible(bool visible)
{
    ui->actionToggleWindow->setText(tr(!visible? "Show":"Hide"));
    QMainWindow::setVisible(visible);
}

void MainWindow::createIconsMenu(){
    iconMenu = new QMenu(winPopup);

    wid1 = new QWidgetAction(iconMenu);
    wid2 = new QWidgetAction(iconMenu);
    wid3 = new QWidgetAction(iconMenu);
    wid4 = new QWidgetAction(iconMenu);
    wid5 = new QWidgetAction(iconMenu);
    btn1 = new QToolButton(iconMenu);
    btn2 = new QToolButton(iconMenu);
    btn3 = new QToolButton(iconMenu);
    btn4 = new QToolButton(iconMenu);
    btn5 = new QToolButton(iconMenu);
    btn1->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn2->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn3->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn4->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn5->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn1->setToolTip(":/imgs/on.png");
    btn2->setToolTip(":/imgs/logo.png");
    btn3->setToolTip(":/imgs/dark.png");
    btn4->setToolTip(":/imgs/light.png");
    btn5->setToolTip(DETECT_TRAY_BG_COLOR);

    btn1->setIconSize(QSize(32, 32));
    btn1->setIcon(QIcon(":/imgs/on.png"));
    wid1->setDefaultWidget(btn1);
    iconMenu->addAction(wid1);

    btn2->setIconSize(QSize(32, 32));
    btn2->setIcon(QIcon(":/imgs/logo.png"));
    wid2->setDefaultWidget(btn2);
    iconMenu->addAction(wid2);

    btn3->setIconSize(QSize(32, 32));
    btn3->setIcon(QIcon(":/imgs/dark.png"));
    wid3->setDefaultWidget(btn3);
    iconMenu->addAction(wid3);

    btn4->setIconSize(QSize(32, 32));
    btn4->setIcon(QIcon(":/imgs/light.png"));
    wid4->setDefaultWidget(btn4);
    iconMenu->addAction(wid4);

    btn5->setIconSize(QSize(32, 32));
    btn5->setIcon(QIcon(":/imgs/detect.png"));
    wid5->setDefaultWidget(btn5);
    iconMenu->addAction(wid5);

    signaler.installOn(btn1);
    signaler.installOn(btn2);
    signaler.installOn(btn3);
    signaler.installOn(btn4);
    signaler.installOn(btn5);

    ui->btnIconLogo->setMenu(iconMenu);
}

void MainWindow::onGeneralClick(QWidget *widget, QMouseEvent *event){
    if(event->type() == QEvent::MouseButtonPress) {
        if (widget == ui->logo){
            startStopTunnel();
        }else if(widget == btn1){
            setIcon(btn1->toolTip());
            iconMenu->close();
            writeAppConfig();
        }else if(widget == btn2){
            setIcon(btn2->toolTip());
            iconMenu->close();
            writeAppConfig();
        }else if(widget == btn3){
            setIcon(btn3->toolTip());
            iconMenu->close();
            writeAppConfig();
        }else if(widget == btn4){
            setIcon(btn4->toolTip());
            iconMenu->close();
            writeAppConfig();
        }else if(widget == btn5){
            setIcon(btn5->toolTip());
            iconMenu->close();
            writeAppConfig();
        }
    }else if(event->type() == QEvent::Enter){
        if (widget == ui->logo){
            if (process->state()==QProcess::NotRunning){
                ui->logo->setPixmap(QPixmap(":imgs/off_over"));
            }else{
                if(wasConnected)
                    ui->logo->setPixmap(QPixmap(":imgs/on_over"));
            }
        }
    }else if(event->type() == QEvent::Leave){
        if (widget == ui->logo){
            if (process->state()==QProcess::NotRunning){
                ui->logo->setPixmap(QPixmap(":imgs/off"));
            }else{
                if(wasConnected)
                    ui->logo->setPixmap(QPixmap(":imgs/on"));
            }
        }
    }
}

void MainWindow::timerTick(){
    QIcon icon =  QPixmap::fromImage(setOpacity( getImageTrayIcon(), 0.3));
    ui->logo->setPixmap(QPixmap(":/imgs/on"));
    if (QTime::currentTime().second()%2==0){
        icon =  QPixmap::fromImage(setOpacity( gray(getImageTrayIcon()), 0.3));
        ui->logo->setPixmap(QPixmap(":/imgs/normal"));
    }
        qApp->setWindowIcon(icon);
        tray->setIcon(icon);
    }

void MainWindow::setTunnelConnected(){
    QIcon icon(trayIcon);
    qApp->setWindowIcon(icon);
    tray->setIcon(icon);
    ui->logo->setPixmap(QPixmap(":/imgs/on"));
    ui->actionConnect->setText(tr("Stop"));
    ui->lblCnxStatus->setText(tr("Connected"));
    if(ui->chkTunnelNotifications->isChecked())
        tray->showMessage(qAppName(), tr("Connected"), icon);
    ui->logsview->appendPlainText(tr("Connected"));
    if(ui->chkSetProxyConfig->isChecked())
        systemProxy->setProxy();
    ui->tab_config->setEnabled(false);
    ui->btnCountryFlag->setEnabled(false);
}


void MainWindow::setTunnelConnecting(){
    timerConnecting.start(300);
    ui->actionConnect->setText(tr("Stop"));
    if(ui->chkTunnelNotifications->isChecked())
        tray->showMessage(qAppName(), tr("Connecting"));
    ui->lblCnxStatus->setText(tr("Connecting"));
    ui->logsview->appendPlainText(tr("Connecting"));
    ui->tab_config->setEnabled(false);
    ui->btnCountryFlag->setEnabled(false);
}

void MainWindow::setTunelStoped(int){
    if(wasConnected && ui->chkSetProxyConfig->isChecked())
        systemProxy->restoreProxy();
    wasConnected = false;
    timerConnecting.stop();
    QIcon icon =  QPixmap::fromImage(setOpacity( gray(getImageTrayIcon()), 0.3));
    qApp->setWindowIcon(icon);
    ui->logo->setPixmap(QPixmap(":/imgs/off"));
    ui->actionConnect->setText(tr("Connect"));
    ui->lblCnxStatus->setText(tr("Stoped"));
    if(ui->chkTunnelNotifications->isChecked())
        tray->showMessage(qAppName(), tr("Stoped"), icon);
    tray->setIcon(icon);
    ui->logsview->appendPlainText(tr("Disconnected"));
    ui->tab_config->setEnabled(true);
    ui->btnCountryFlag->setEnabled(true);
}

void MainWindow::updateRegions(){
    ui->cmbRegions->clear();
    ui->cmbRegions->addItem(QIcon(":/imgs/fastCountry"),tr("Fastest"));
    for(QString s: regionsList){
        ui->cmbRegions->addItem(QIcon(CountryData::iso2flag(s)), CountryData::iso2name(s));
    }
}

void MainWindow::readout(){
    QString readed = process->readAll();
    for(QString line : readed.split("\n")){
        QJsonDocument loadDoc(QJsonDocument::fromJson(readed.toUtf8()));

        QJsonObject json = loadDoc.object();


        if(!json["data"].toObject()["regions"].toArray().isEmpty()){
            QJsonArray arr = json["data"].toObject()["regions"].toArray();
            regionsList.clear();
            for(QJsonValue o: arr){
                regionsList<< o.toString();
            }
            updateRegions();
            writeAppConfig();
        }
        QString type = json["noticeType"].toString();
        if(type == "Tunnels") {
            if(json["data"].toObject()["count"].toInt() > 0 ){
                timerConnecting.stop();
                if(!wasConnected){
                    wasConnected=true;
                    ui->commandLinkButton->click();
                }
                setTunnelConnected();
            }else{
                setTunnelConnecting();
            }
        }else if (type == ("SocksProxyPortInUse")) {
            if (CoreControler::coreIsRunning(tunelCorePath)){
                CoreControler::killCore(tunelCorePath);
                connect(process, SIGNAL(finished(int)), this, SLOT(startTunnel()));
                break;
            }else{
                QMessageBox(QMessageBox::Critical, qAppName(), qAppName() +"\n"+ tr("Socks port in use, please set another port or close the app that is using this port"), QMessageBox::Ok).exec();
                ui->tabWidget->setCurrentWidget(ui->tab_config);
                ui->editLocalSocksPort->setFocus();
                break;
            }
        }else if (type == ("HttpProxyPortInUse")) {
            if (CoreControler::coreIsRunning(tunelCorePath)){
                CoreControler::killCore(tunelCorePath);
                connect(process, SIGNAL(finished(int)), this, SLOT(startTunnel()));
                break;
            }else{
                QMessageBox(QMessageBox::Critical, qAppName(), qAppName() +"\n"+ tr("Http port in use, please set another port or close the app that is using this port"), QMessageBox::Ok).exec();
                ui->tabWidget->setCurrentWidget(ui->tab_config);
                ui->editLocalHttpPort->setFocus();
                break;
            }
        }else if (type == "EstablishTunnelTimeout") {
            QMessageBox(QMessageBox::Critical, qAppName(), qAppName() +"\n"+ tr("Couldn't connect, please check the tunnel internet connection configuration"), QMessageBox::Ok).exec();
            ui->tabWidget->setCurrentWidget(ui->tab_config);
            break;
        }

        ///for windows
        else if (line.contains("bolt.Close(): funlock error:")) {
                   if (CoreControler::coreIsRunning(tunelCorePath)){
                       CoreControler::killCore(tunelCorePath);
                       connect(process, SIGNAL(finished(int)), this, SLOT(startTunnel()));
                       break;
                   }else{
                       QMessageBox(QMessageBox::Critical, qAppName(), qAppName() +"\n"+ tr("Http port in use, please set another port or close the app that is using this port"), QMessageBox::Ok).exec();
                       ui->tabWidget->setCurrentWidget(ui->tab_config);
                       ui->editLocalHttpPort->setFocus();
                       break;
                   }
               }
    }
    ui->logsview->appendPlainText(readed);

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::toggleWindow(){
    if(isVisible()){
        close();
    }else{
        show();
    }
}

void MainWindow::show(){
    ui->actionToggleWindow->setText(tr("Hide"));
    ui->tabWidget->setCurrentWidget(ui->tab_main);
    if (ui->chkTrayDialog->isChecked()) {
        popup(QSystemTrayIcon::MiddleClick);
    }else{
        QMainWindow::show();
        QRect screenGeometry = QApplication::primaryScreen()->geometry();
        int x = (screenGeometry.width()-width()) / 2;
        int y = (screenGeometry.height()-height()) / 2;
        move(x, y);
        raise();
    }
    detectSetWindowsTheme();
}

void MainWindow::parceReceivedMessage(quint32 instanceId, QByteArray message)
{
    Q_UNUSED(instanceId);
    QString msg = QString(message).remove(qApp->applicationFilePath());
    if(msg == Params::FROM_SECOND){
        tray->showMessage(qAppName(), tr("Already running"));
        setPage(0);
        return;
    }
    if (msg.contains(Params::FROM_SECOND)){
        msg = msg.remove(Params::FROM_SECOND);
        qInfo() << "Message received" << msg;
    }
    QList<QString> list = msg.remove(Params::FROM_SECOND).split(' ');
    for(QString s: list){
        if(s.startsWith(Params::SHOW_PAGE)){
            show();
            setPage(s.remove(Params::SHOW_PAGE).toInt());
        }else if(s == Params::START_TUNNEL){
            startTunnel();
        }else if(s == Params::STOP_TUNNEL){
            stopTunnel();
        }else if(s == Params::TOGGLE_TUNNEL){
            startStopTunnel();
        }else if(s == Params::QUIT_APP){
            quit();
        }
    }
}
bool MainWindow::close(){
    ui->actionToggleWindow->setText(tr("Show"));
    QMainWindow::close();
    return true;
}


void MainWindow::startStopTunnel(){
    QList<QString> list;
    list << "-config" << configPath;
    if(process->state() == QProcess::NotRunning){
        startTunnel();
    }else{
        stopTunnel();
    }
}

void MainWindow::stopTunnel(){
    process->terminate();
    timerConnecting.singleShot(1000, process, SLOT(kill()));
}


void MainWindow::startTunnel(){
    disconnect(process, SIGNAL(finished(int)), this, SLOT(startTunnel()));
    QList<QString> list;
    list << "-config" << configPath;
    if(!QFile::exists(tunelCorePath)){
        tray->showMessage(qAppName(), tr("Core executable not found"));
        ui->logsview->appendPlainText(tr("Core executable not found, please set custom core executable "));
        setTunelStoped();
    }else if(process->state() == QProcess::NotRunning){
        systemProxy = new ProxySetter(ui->editLocalHttpPort->text(), ui->editLocalSocksPort->text());
        process->setWorkingDirectory(tunelCoreDir);
        process->start(tunelCorePath, list);
        setTunnelConnecting();
    }
}

void MainWindow::loadconfig(){
    tunelCorePath = QApplication::applicationDirPath()+"/core";
    configPath = Defs::tunelConfigFilePath();
    trayIcon = DETECT_TRAY_BG_COLOR;

    ui->chkShowWindowOnStart->setChecked(true);
    ui->chkTunnelNotifications->setChecked(true);
    tunelCoreDir = (QFileInfo(tunelCorePath).dir().absolutePath());
    readAppConfig();
    readTunelConfig();
    setTrayMenu();
    if(ui->chkConnectOnStart->isChecked())
        ui->actionConnect->trigger();
    else
        setTunelStoped();
}

void MainWindow::readTunelConfig(){
    QFile loadFile(configPath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject json = loadDoc.object();

    if (json.contains("UpstreamProxyUrl") && json["UpstreamProxyUrl"].isString()){
        QString proxyConfig = (json["UpstreamProxyUrl"].toString());
        QUrl *url = new QUrl(proxyConfig);
        ui->chkUseProxy->setChecked((url->scheme() == "http"));
        ui->editProxyHost->setText(url->host());
        ui->editProxyPort->setValue((url->port()));
        if (proxyConfig.contains("@")) {
            ui->chkProxyAuth->setChecked(true);
            ui->editProxyUser->setText(url->userName());
            ui->editProxyPass->setText(url->password());
        }
    }else
        if (json.contains(".UpstreamProxyUrl") && json[".UpstreamProxyUrl"].isString()){
            QString proxyConfig = (json[".UpstreamProxyUrl"].toString());
            QUrl *url = new QUrl(proxyConfig);
            ui->editProxyHost->setText(url->host());
            ui->editProxyPort->setValue((url->port()));
            if (proxyConfig.contains("@")) {
                ui->chkProxyAuth->setChecked(true);
                ui->editProxyUser->setText(url->userName());
                ui->editProxyPass->setText(url->password());
            }
        }
    if(json.contains("LocalSocksProxyPort") ){
        ui->editLocalSocksPort->setValue(json["LocalSocksProxyPort"].toInt());
    }
    if(json.contains("LocalHttpProxyPort") ){
        ui->editLocalHttpPort->setValue(json["LocalHttpProxyPort"].toInt());
    }

    QString iso = json["EgressRegion"].toString();
    int i = regionsList.indexOf(iso);
    ui->cmbRegions->setCurrentIndex(iso.isEmpty()?0:i+1);
    ui->btnCountryFlag->setIcon(QIcon(iso==0 ? ":/imgs/fastCountry"
                                           : CountryData::iso2flag(iso)));
}

void MainWindow::readAppConfig(){
    QFile loadFile(Defs::appConfigFilePath());

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open read file.");
    }else{
        QByteArray saveData = loadFile.readAll();

        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

        QJsonObject json = loadDoc.object();

        if (json.contains("CustomCoreExe")) {
            ui->chkCustomCoreExe->setChecked(true);
            tunelCorePath = json["CustomCoreExe"].toString();
            tunelCoreDir = (QFileInfo(tunelCorePath).dir().absolutePath());
            ui->editCustomCoreExe->setText(tunelCorePath);
        }
        if (json.contains(".CustomCoreExe")) {
            ui->editCustomCoreExe->setText(json[".CustomCoreExe"].toString());
        }

        if (json.contains("CustomCoreConfig")) {
            ui->chkCustomCoreConfig->setChecked(true);
            configPath = json["CustomCoreConfig"].toString();
            ui->editCustomCoreConfig->setText(configPath);
        }
        if (json.contains(".CustomCoreConfig")) {
            ui->editCustomCoreConfig->setText(json[".CustomCoreConfig"].toString());
        }

        if (json.contains(("trayIcon"))) {
            trayIcon = json["trayIcon"].toString();
        }

        if(json.contains("SetProxyConfig"))
            ui->chkSetProxyConfig->setChecked(json["SetProxyConfig"].toBool());
        ui->chkConnectOnStart->setChecked(json["connectOnStart"].toBool());
        ui->chkShowWindowOnStart->setChecked(json["showWindowOnStart"].toBool());
        ui->chkStratupOnLogin->setChecked(QAutostart().isAutostart());
        ui->chkTrayDialog->setChecked(json["TrayDialog"].toBool());
        ui->chkTunnelNotifications->setChecked(json["TunnelNotifications"].toBool());
        if(json.contains("noShowScreenCapPermission") ){
            noShowScreenCapPermission = json["noShowScreenCapPermission"].toBool();
        }
        if(!json["regions"].toArray().isEmpty()){
            QJsonArray arr = json["regions"].toArray();
            regionsList.clear();
            for(QJsonValue o: arr){
                regionsList<< o.toString();
            }
            updateRegions();
        }

        setTrayMenu(false);
    }
          QFile r(!QFileInfo::exists(configPath)?":/res/config.json":configPath);
          r.open(QFile::ReadOnly);
          QString config = r.readAll();
          QDir().mkpath(QFileInfo(configPath).dir().path());
          config.replace("$$HOME$$", Defs::configDirPath().replace("\\", "\\\\"));
          QFile file(configPath);
          if(file.open(QFile::WriteOnly)){
              file.write(config.toUtf8());
          }
          file.close();

    ui->btnIconLogo->setIcon(QIcon(trayIcon==DETECT_TRAY_BG_COLOR ? QString(":/imgs/detect")
                                                       : QPixmap::fromImage(getImageTrayIcon())));
    tray->show();
    if(ui->chkShowWindowOnStart->isChecked()){
        setPage(0);
    }
}

void MainWindow::writeTunelConfig(){
    QFile saveFile(configPath);

    if (!saveFile.open(QIODevice::ReadWrite)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray saveData = saveFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject json = loadDoc.object();

    QString host = ui->editProxyHost->text() + ":" + ui->editProxyPort->text();
    QString user = ui->editProxyUser->text() + ":" + ui->editProxyPass->text();
    bool auth = ui->chkProxyAuth->isChecked();
    if (ui->chkUseProxy->isChecked()) {
        json.remove(".UpstreamProxyUrl");
        json["UpstreamProxyUrl"] = "http://"+(auth ? user+"@" : "") + host;
    }else{
        json.remove("UpstreamProxyUrl");
        json[".UpstreamProxyUrl"] = "http://"+(auth ? user+"@" : "") + host;
    }

    json["LocalSocksProxyPort"] = ui->editLocalSocksPort->value();
    json["LocalHttpProxyPort"] = ui->editLocalHttpPort->value();

    int t = ui->cmbRegions->currentIndex();
    QString iso = t==0?"":regionsList.at(t-1);
    json["EgressRegion"] = t==0?"":iso;
    ui->btnCountryFlag->setIcon(QIcon(t==0 ? ":/imgs/fastCountry"
                                           : CountryData::iso2flag(iso)));

    QJsonDocument saveDoc(json);
    saveFile.resize(0);
    saveFile.write(saveDoc.toJson());
    ui->tabWidget->setCurrentWidget(ui->tab_main);
}

void MainWindow::writeAppConfig(){
    QFile saveFile(Defs::appConfigFilePath());

    if (!saveFile.open(QIODevice::ReadWrite)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray saveData = saveFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject json = loadDoc.object();

    if (ui->chkCustomCoreExe->isChecked()) {
        json["CustomCoreExe"] = ui->editCustomCoreExe->text();
        json.remove(".CustomCoreExe");
    }else{
        json[".CustomCoreExe"] = ui->editCustomCoreExe->text();
        json.remove("CustomCoreExe");
    }

    if (ui->chkCustomCoreConfig->isChecked()) {
        json["CustomCoreConfig"] = ui->editCustomCoreConfig->text();
        json.remove(".CustomCoreConfig");
    }else{
        json[".CustomCoreConfig"] = ui->editCustomCoreConfig->text();
        json.remove("CustomCoreConfig");
    }


    json["trayIcon"] = trayIcon;

    json["connectOnStart"]= ui->chkConnectOnStart->isChecked();
    json["SetProxyConfig"]= ui->chkSetProxyConfig->isChecked();
    json["showWindowOnStart"]= ui->chkShowWindowOnStart->isChecked();
    json["TrayDialog"]= ui->chkTrayDialog->isChecked();
    json["TunnelNotifications"]= ui->chkTunnelNotifications->isChecked();
    json["noShowScreenCapPermission"]= noShowScreenCapPermission;

    json["regions"] = QJsonArray::fromStringList(regionsList);

    QJsonDocument saveDoc(json);
    saveFile.resize(0);
    saveFile.write(saveDoc.toJson());
}

void MainWindow::quit(){
    if (process->state() != QProcess::NotRunning) {
        stopTunnel();
        systemProxy->restoreProxy();
    }
    tray->hide();
    exit(0);
}


void MainWindow::on_pushButton_2_clicked(){
    quit();
}

void MainWindow::on_btnSaveTunnelConfig_clicked(){
    writeTunelConfig();
    writeAppConfig();
}

void MainWindow::on_btnCustomCoreExe_clicked(){
    QFileDialog dialog(this, tr("Select tunnel core executable"), QDir::homePath());
    if( dialog.exec() ){
        ui->chkCustomCoreExe->setChecked(true);
        ui->editCustomCoreExe->setText(dialog.selectedFiles().at(0));
    }else{
        ui->chkCustomCoreExe->setChecked(false);
    }
    writeAppConfig();
    if(process->state() != QProcess::NotRunning){
        process->terminate();
        startStopTunnel();
    }
}

void MainWindow::on_chkCustomCoreExe_clicked(bool checked){
    if (checked && ui->editCustomCoreExe->text().isEmpty()) {
        ui->btnCustomCoreExe->click();
    }
    writeAppConfig();
}

void MainWindow::on_btnCustomCoreConfig_clicked(){
    QFileDialog dialog(this, tr("Select tunnel core configuration"), QDir::homePath());
    if( dialog.exec() ){
        ui->chkCustomCoreConfig->setChecked(true);
        ui->editCustomCoreConfig->setText(dialog.selectedFiles().at(0));
    }else{
        ui->chkCustomCoreConfig->setChecked(false);
    }
    writeAppConfig();
    readTunelConfig();
}

void MainWindow::on_chkCustomCoreConfig_clicked(bool checked){
    if (checked && ui->editCustomCoreConfig->text().isEmpty()) {
        ui->btnCustomCoreConfig->click();
    }
    writeAppConfig();
}

void MainWindow::askForReset(){
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(qAppName());
    msgBox.setInformativeText(tr("The app may need restart to to set the right behavior"));
    msgBox.setStandardButtons(QMessageBox::Reset | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Reset);
    if(msgBox.exec() == QMessageBox::Reset) {
        QList<QString> list;
        list << "--show-3";
        QProcess::startDetached(qApp->applicationFilePath(), list);
        quit();
    }
}

void MainWindow::setPage(int index){
    show();
    ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::on_chkHideFromDockMac_clicked(){
    QList<QString> list;
    QDir d = QFileInfo(qApp->applicationFilePath()).absoluteDir();
    d.cdUp();
    list << "write" << d.path()+"/Info.plist"
         << "LSUIElement" << (ui->chkHideFromDockMac->isChecked() ? "1" : "0");
    QProcess p;
    p.setReadChannelMode(QProcess::MergedChannels);
    p.start("defaults", list);
    p.waitForFinished();

    askForReset();
}

QImage MainWindow::gray(QImage original){
    QImage img(original);
    if (!img.isNull())
    {
        int pixels = img.width() * img.height();
        if (pixels*(int)sizeof(QRgb) <= img.sizeInBytes())
        {
            QRgb *data = (QRgb *)img.bits();
            for (int i = 0; i < pixels; i++)
            {
                int val = qGray(data[i]);
                data[i] = qRgba(val, val, val, qAlpha(data[i]));
            }
        }
    }
    return img;
}

QImage MainWindow::setOpacity(QImage image, qreal opacity){
    QImage newImg(image.size(), QImage::Format_ARGB32);
    newImg.fill(Qt::transparent);

    QPainter painter(&newImg);
    painter.setOpacity(opacity);
    painter.drawImage(QRect(0, 0, image.width(), image.height()), image);

    return newImg;
}

void MainWindow::setIcon(QString icon){
    trayIcon = icon;
    ui->btnIconLogo->setIcon(QIcon(trayIcon==DETECT_TRAY_BG_COLOR ? QString(":/imgs/detect")
                                                       : QPixmap::fromImage(getImageTrayIcon())));
    writeAppConfig();
    if(process->state() == QProcess::NotRunning){
        setTunelStoped();
    }else{
        if(!timerConnecting.isActive())
            setTunnelConnected();
    }
}

QImage MainWindow::getImageTrayIcon(){
    if(trayIcon == DETECT_TRAY_BG_COLOR){

        QScreen *screen = qApp->primaryScreen();
        if (const QWindow *window = windowHandle())
            screen = window->screen();
        if (NULL == screen) {
#ifdef Q_OS_MACOS
            if (!noShowScreenCapPermission && askPermission != QMessageBox::Cancel){
                QMessageBox msgBox;
                 msgBox.setText(tr("Se necesita el permiso de captura de pantalla"));
                 msgBox.setInformativeText(tr("Abrir preferencias para otorfar el permiso"));
                 msgBox.setDetailedText(tr("Este permiso es necesitado para detectar el color del fondo detrás del icono de estado o notificación"));
                 msgBox.setIcon(QMessageBox::Question);
                 msgBox.setStandardButtons(QMessageBox::Open | QMessageBox::Cancel);
                 msgBox.setDefaultButton(QMessageBox::Open);
                 QPushButton *connectButton = msgBox.addButton(tr("Nunca"), QMessageBox::ActionRole);

                 askPermission = msgBox.exec();

                 if (msgBox.clickedButton() == connectButton) {
                     noShowScreenCapPermission = true;
                     writeAppConfig();
                 }else if (askPermission == QMessageBox::Open) {
                     system("open \"x-apple.systempreferences:com.apple.preference.security?Privacy_ScreenCapture\"");
                     QMessageBox wait;
                     wait.setText(tr("En espera del permiso necesario"));
                     wait.setInformativeText(tr("Presione 'Aceptar' cuando lo haya otorgado"));
                     wait.setModal(Qt::ApplicationModal);
                     wait.setIcon(QMessageBox::Warning);
                     wait.exec();
                     return getImageTrayIcon();
                 }
            }
#endif
            return QImage(":/imgs/light.png");
        }else{
                QSize s = screen->size();
                QImage img = screen->grabWindow(0,0,0,s.width(),s.height()).toImage();
                QPair<Position, int> p = getWinTaskbarData();
                QColor bg;
                QPoint c = QPoint(2,2);
                switch (p.first) {
                case BOTTOM:
                    bg = img.pixel(c.x(), img.height()-2);
                    break;
                case RIGHT:
                    bg = img.pixel(img.width()-2, c.y());
                    break;
                case TOP:
                    bg = img.pixel(c.x(), 2);
                    break;
                case LEFT:
                    bg = img.pixel(2, c.y());
                    break;
                }
                if(bg.red()>127 && bg.green() > 127 && bg.blue() > 127){
                    return QImage(":/imgs/dark.png");
                }else{
                    return QImage(":/imgs/light.png");
                }
        }
    }else {
        return QImage(trayIcon);
    }
}

void MainWindow::on_btnAbout_clicked()
{
    ui->actionAbout->trigger();
}

void MainWindow::on_actionAbout_triggered()
{
    setPage(4);
}

void MainWindow::on_actionTunel_settings_triggered()
{
    setPage(2);
}

void MainWindow::on_actionSettings_triggered()
{
    setPage(3);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_commandLinkButton_clicked()
{
#ifdef Q_OS_MAC
    QDesktopServices::openUrl(QUrl("https://sanmail.app/PsiphonQt/#mac"));
#elif defined(Q_OS_WIN)
    QDesktopServices::openUrl(QUrl("https://sanmail.app/PsiphonQt/#win"));
#else
    QDesktopServices::openUrl(QUrl("https://sanmail.app/PsiphonQt/#linux"));
#endif
}

void MainWindow::on_chkConnectOnStart_clicked()
{
    writeAppConfig();
}

void MainWindow::on_chkShowWindowOnStart_clicked()
{
    writeAppConfig();
}

void MainWindow::on_chkStratupOnLogin_toggled(bool checked)
{
    QAutostart().setAutostart(checked);
}

void MainWindow::on_chkTrayDialog_clicked()
{
    setTrayMenu(true);
    writeAppConfig();
}

void MainWindow::setTrayMenu(bool showPage){
    tray->setContextMenu(trayMenu);
    if(!ui->chkTrayDialog->isChecked()){
        setCentralWidget(ui->centralwidget);
        if(showPage)
            setPage(3);
        disconnect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                   this, SLOT(popup(QSystemTrayIcon::ActivationReason)));
    }else{
        widPopup->setDefaultWidget(ui->centralwidget);
        winPopup->addAction(widPopup);
#ifdef Q_OS_MACOS
        tray->setContextMenu(winPopup);
#endif
        if(showPage){
            winPopup->popup(QCursor::pos());
            close();
        }
    }
#ifndef Q_OS_MACOS
       connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
               this, SLOT(popup(QSystemTrayIcon::ActivationReason)));
#endif
}

void MainWindow::popup(QSystemTrayIcon::ActivationReason reason){
    if (reason == QSystemTrayIcon::Context)
        return;
    if (ui->chkTrayDialog->isChecked()) {
#ifndef Q_OS_MACOS
            QScreen *screen = QGuiApplication::primaryScreen();
            if (const QWindow *window = windowHandle())
                screen = window->screen();
            if (!screen)
                return;
            QImage img = screen->grabWindow(0).toImage();
            QPair<Position, int> p = getWinTaskbarData();
            QColor bg;
            QPoint c = QCursor::pos();
            switch (p.first) {
            case BOTTOM:
                bg = img.pixel(c.x(), img.height()-4);
                break;
            case RIGHT:
                bg = img.pixel(img.width()-2, c.y());
                break;
            case TOP:
                bg = img.pixel(c.x(), 2);
                break;
            case LEFT:
                bg = img.pixel(2, c.y());
                break;
           }
            QColor fg;
            if(bg.red()>127 && bg.green() > 127 && bg.blue() > 127){
                fg = Qt::black;
            }else{
                fg = Qt::white;
            }
#ifdef Q_OS_WIN
            QOperatingSystemVersion version = QOperatingSystemVersion::current();
            QString path = ":/res/winpopup8.qss";
            if(version.majorVersion()==6 && version.minorVersion()==1)
                path = ":/res/winpopup7.qss" ;
            else if (version.majorVersion()>=10)
                path = ":/res/winpopup.qss";
            QFile file(path);
            file.open(QFile::ReadOnly);
            QString styleSheet = QString::fromLatin1(file.readAll());


            styleSheet=styleSheet.replace("#001133", bg.name().insert(1, "ee"))
                                 .replace("#ffffff", fg.name());

            winPopup->setStyleSheet(styleSheet);
            winPopup->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
            winPopup->setAttribute(Qt::WA_TranslucentBackground);
           //ui->centralwidget->setStyleSheet("background-color: rgb( 255, 255, 255, 0);");
#else
        QString desk = qgetenv("XDG_CURRENT_DESKTOP");
        if(desk.toUpper().contains("DEEPIN") ){
            QString path = ":/res/deepin.qss";
            QFile file(path);
            file.open(QFile::ReadOnly);
            QString styleSheet = QString::fromLatin1(file.readAll());

            styleSheet=styleSheet.replace("#001133", bg.name().insert(1, "ee"))
                                 .replace("#ffffff", fg.name());

            winPopup->setStyleSheet(styleSheet);
            winPopup->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
            winPopup->setAttribute(Qt::WA_TranslucentBackground);
        }
#endif
#endif

       winPopup->setWindowOpacity(0.9);
        winPopup->popup(QCursor::pos());
    }else{
        ui->centralwidget->setStyleSheet("");
        toggleWindow();
    }
}

void MainWindow::on_chkTunnelNotifications_clicked()
{
    writeAppConfig();
}

QPair<MainWindow::Position, int> MainWindow::getWinTaskbarData(){
    QPair<MainWindow::Position, int> data;
    const QRect displayRect = QGuiApplication::primaryScreen()->geometry();
    const QRect deskRect = QGuiApplication::primaryScreen()->availableGeometry();

    if (deskRect.height() < displayRect.height()) {
        data.second = abs(deskRect.height() - displayRect.height());
        if (deskRect.y() > displayRect.y()) {
            data.first = (TOP);
        } else {
            data.first = (BOTTOM);
        }
    } else {
        data.second = abs(deskRect.width() - displayRect.width());
        if (deskRect.x() > displayRect.x()) {
            data.first = (LEFT);
        } else {
            data.first = (RIGHT);
        }
    }
    return data;
}

void MainWindow::on_btnClearLogs_clicked()
{
    ui->logsview->clear();
}

void MainWindow::on_btnSaveLogs_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                QDir::homePath()+"/logs.log");
    QFile saveFile(fileName);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }
    saveFile.resize(0);
    saveFile.write(ui->logsview->toPlainText().toUtf8());
}

void MainWindow::on_chkSetProxyConfig_clicked()
{
    writeAppConfig();
}

void MainWindow::on_btnCountryFlag_clicked()
{
    setPage(2);
    ui->cmbRegions->showPopup();
}
