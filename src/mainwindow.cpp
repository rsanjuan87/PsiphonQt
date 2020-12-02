#include "mainwindow.h"
#include "QAutostart"
#include "ui_mainwindow.h"
#include "defs.h"
#include "CoreProcessControler.h"

#include <QDir>

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

#if defined (Q_OS_MAC)
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
}

void MainWindow::setVisible(bool visible)
{
    ui->actionToggleWindow->setText(tr(visible? "Show":"Hide"));
    QMainWindow::setVisible(visible);
}

void MainWindow::createIconsMenu(){
    iconMenu = new QMenu(winPopup);

    wid1 = new QWidgetAction(iconMenu);
    wid2 = new QWidgetAction(iconMenu);
    wid3 = new QWidgetAction(iconMenu);
    wid4 = new QWidgetAction(iconMenu);
    btn1 = new QToolButton(iconMenu);
    btn2 = new QToolButton(iconMenu);
    btn3 = new QToolButton(iconMenu);
    btn4 = new QToolButton(iconMenu);
    btn1->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn2->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn3->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn4->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn1->setToolTip(":/imgs/on.png");
    btn2->setToolTip(":/imgs/logo.png");
    btn3->setToolTip(":/imgs/dark.png");
    btn4->setToolTip(":/imgs/light.png");
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

    signaler.installOn(btn1);
    signaler.installOn(btn2);
    signaler.installOn(btn3);
    signaler.installOn(btn4);

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
    QIcon icon =  QPixmap::fromImage(setOpacity( QImage(trayIcon), 0.3));
    ui->logo->setPixmap(QPixmap(":/imgs/on"));
    if (QTime::currentTime().second()%2==0){
        icon =  QPixmap::fromImage(setOpacity( gray(QImage(trayIcon)), 0.3));
        ui->logo->setPixmap(QPixmap(":/imgs/normal"));
    }
        qApp->setWindowIcon(icon);
        tray->setIcon(icon);
    }

void MainWindow::setTunnelConnected(){
    QIcon icon(trayIcon);
    qApp->setWindowIcon(icon);
    tray->setIcon(icon);
    ////TODO si esta conectado no mostrar la notificacion
//    ui->btnStartStop->setText(tr("Stop"));
    ui->logo->setPixmap(QPixmap(":/imgs/on"));
    ui->actionConnect->setText(tr("Stop"));
    ui->lblCnxStatus->setText(tr("Connected"));
    if(ui->chkTunnelNotifications->isChecked())
        tray->showMessage(qAppName(), tr("Connected"), icon);
    //ui->commandLinkButton->click();
    ui->logsview->appendPlainText(tr("Connected"));
    if(ui->chkSetProxyConfig->isChecked())
        systemProxy->setProxy();
    ui->tab_config->setEnabled(false);
}


void MainWindow::setTunnelConnecting(){
    timerConnecting.start(300);
//    ui->btnStartStop->setText(tr("Stop"));
    ui->actionConnect->setText(tr("Stop"));
    if(ui->chkTunnelNotifications->isChecked())
        tray->showMessage(qAppName(), tr("Connecting"));
    ui->lblCnxStatus->setText(tr("Connecting"));
    ui->logsview->appendPlainText(tr("Connecting"));
    ui->tab_config->setEnabled(false);
}

void MainWindow::setTunelStoped(int){
    if(wasConnected && ui->chkSetProxyConfig->isChecked())
        systemProxy->restoreProxy();
    wasConnected = false;
    timerConnecting.stop();
    QIcon icon =  QPixmap::fromImage(setOpacity( gray(QImage(trayIcon)), 0.3));
    qApp->setWindowIcon(icon);
//    ui->btnStartStop->setText(tr("Connect"));
    ui->logo->setPixmap(QPixmap(":/imgs/off"));
    ui->actionConnect->setText(tr("Connect"));
    ui->lblCnxStatus->setText(tr("Stoped"));
    if(ui->chkTunnelNotifications->isChecked())
        tray->showMessage(qAppName(), tr("Stoped"), icon);
    tray->setIcon(icon);
    ui->logsview->appendPlainText(tr("Disconnected"));
    ui->tab_config->setEnabled(true);
}

void MainWindow::readout(){
    QString readed = process->readAll();
    for(QString line : readed.split("\n")){
        QJsonDocument loadDoc(QJsonDocument::fromJson(readed.toUtf8()));

        QJsonObject json = loadDoc.object();
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
        qDebug() << line;
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
        popup(QSystemTrayIcon::Trigger);
    }else{
        QMainWindow::show();
        QRect screenGeometry = QApplication::primaryScreen()->geometry();
        int x = (screenGeometry.width()-width()) / 2;
        int y = (screenGeometry.height()-height()) / 2;
        move(x, y);
        raise();
    }
}

void MainWindow::receivedMessage(quint32 instanceId, QByteArray message)
{
    Q_UNUSED(instanceId);
    QList<QString> list = QString(message).split(' ');
    for(QString s: list){
        if(s.startsWith("--show-")){
            show();
            setPage(s.remove("--show-").toInt());
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
    qDebug() << tunelCorePath << list;
    if(process->state() == QProcess::NotRunning){
        startTunnel();
    }else{
        process->terminate();
        timerConnecting.singleShot(1000, process, SLOT(kill()));
    }
}


void MainWindow::startTunnel(){
    disconnect(process, SIGNAL(finished(int)), this, SLOT(startTunnel()));
    QList<QString> list;
    list << "-config" << configPath;
    qDebug() << tunelCorePath << list;
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
    trayIcon = ":/imgs/logo.png";

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
        setTrayMenu(false);
    }
    if(!QFileInfo::exists(configPath)){
          QFile r(":/res/config.json");
          r.open(QFile::ReadOnly);
          QString config = r.readAll();
          QDir().mkpath(QFileInfo(configPath).dir().path());
          config.replace("$$HOME$$", Defs::configDirPath().replace("\\", "\\\\"));
          QFile file(configPath);
          if(file.open(QFile::WriteOnly)){
              file.write(config.toUtf8());
          }
          file.close();
    }

    ui->btnIconLogo->setIcon(QIcon(trayIcon));
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

    QJsonDocument saveDoc(json);
    saveFile.resize(0);
    saveFile.write(saveDoc.toJson());
}


void MainWindow::on_pushButton_2_clicked(){
    if (process->state() != QProcess::NotRunning) {
        startStopTunnel();
    }
    tray->hide();
    exit(0);
}

void MainWindow::on_btnSaveTunnelConfig_clicked(){
    writeTunelConfig();
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
        qApp->quit();
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

void MainWindow::on_btnIconLogo_clicked()
{
    setIcon(":/imgs/logo.png");
}

void MainWindow::setIcon(QString icon){
    ui->btnIconLogo->setIcon(QIcon(icon));
    trayIcon = icon;
    writeAppConfig();
    if(process->state() == QProcess::NotRunning){
        setTunelStoped();
    }else{
        if(!timerConnecting.isActive())
            setTunnelConnected();
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
        disconnect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(popup(QSystemTrayIcon::ActivationReason)));
    }else{
        widPopup->setDefaultWidget(ui->centralwidget);
        winPopup->addAction(widPopup);
#if defined (Q_OS_MAC)
        tray->setContextMenu(winPopup);
#endif
        if(showPage){
            winPopup->popup(QCursor::pos());
            close();
        }
    }
#if not defined (Q_OS_MAC)
       connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(popup(QSystemTrayIcon::ActivationReason)));
#endif

}

void MainWindow::popup(QSystemTrayIcon::ActivationReason reason){
    if (reason == QSystemTrayIcon::Context)
        return;
    if (ui->chkTrayDialog->isChecked()) {
#if defined (Q_OS_WIN)

            QScreen *screen = QGuiApplication::primaryScreen();
            if (const QWindow *window = windowHandle())
                screen = window->screen();
            if (!screen)
                return;

            QImage img = screen->grabWindow(0).toImage();
            QPair<Position, int> p = getTaskData();
            QPoint c = QCursor::pos();
            QColor bg;
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
            QOperatingSystemVersion version = QOperatingSystemVersion::current();
            QString path = ":/res/winpopup8.qss";
            if(version.majorVersion()==6 && version.minorVersion()==1)
                path = ":/res/winpopup7.qss" ;
            else if (version.majorVersion()>=10)
                path = ":/res/winpopup.qss";
            QFile file(path);
            file.open(QFile::ReadOnly);
            QString styleSheet = QString::fromLatin1(file.readAll());

            QColor fg;
            if(bg.red()>127 && bg.green() > 127 && bg.blue() > 127){
                fg = Qt::black;
            }else{
                fg = Qt::white;
                styleSheet=styleSheet.remove("_dark");
            }

            styleSheet=styleSheet.replace("#001133", bg.name().insert(1, "ee"))
                                 .replace("#ffffff", fg.name());

            winPopup->setStyleSheet(styleSheet);
            winPopup->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
            winPopup->setAttribute(Qt::WA_TranslucentBackground);
           //ui->centralwidget->setStyleSheet("background-color: rgb( 255, 255, 255, 0);");

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

QPair<MainWindow::Position, int> MainWindow::getTaskData(){
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
