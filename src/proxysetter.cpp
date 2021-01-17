#include "defs.h"
#include "proxysetter.h"

#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QDebug>

ProxySetter::ProxySetter(QString httpPort, QString sockPort)
{
    this->httpPort = httpPort;
    this->sockPort = sockPort;
    confList = new QMap<QString, ProxyConf>();

}

void ProxySetter::exportShellSetter(){

    QFile file(":/res/setProxy.bash");
    file.open(QFile::ReadOnly);
    QString bash = QString(file.readAll()).replace("$$PORT$$", httpPort);
    file.close();

    file.setFileName(Defs::configDirPath()+"/setProxy.bash");
    file.open(QFile::WriteOnly);
    file.resize(0);
    file.write(bash.toUtf8());
    file.close();
    system(("chmod +X "+Defs::configDirPath()+"/setProxy.bash").toUtf8());
}

#ifdef Q_OS_MAC

void ProxySetter::setProxy(){
    /// set in zsh

    exportShellSetter();
    QFile file(QDir::homePath()+"/.zprofile");
    file.open(QFile::Append);
    file.write("\n"+Defs::configDirPath().toUtf8()+"/setProxy.bash ###PsiphonQt\n");
    file.close();

    file.setFileName(QDir::homePath()+"/.bash_profile");
    file.open(QFile::Append);
    file.write("\n"+Defs::configDirPath().toUtf8()+"/setProxy.bash ###PsiphonQt\n");
    file.close();

    QString interface;
    QProcess p;
    p.start("scutil --nwi");
    p.waitForFinished();
    for(QString s: p.readAllStandardOutput().split('\n')){
        s = s.simplified();
        if(s.contains(": flags :")){
            interface = s.split(':').at(0).simplified();//en0
            break;
        }
    }
    p.start("networksetup -listnetworkserviceorder ");
    p.waitForFinished();
    QStringList list = QString(p.readAllStandardOutput()).split('\n');
    for (int i = 1; i < list.size(); i++) {
        if (list.at(i).contains("Device: "+interface)){
            interface = list.at(i-1).split(") ").at(1).simplified();
            break;
        }
    }

    p.start("scutil --proxy");
    p.waitForFinished();

    ProxyConf systemProxy;
    QStringList out = QString(p.readAllStandardOutput()).split('\n');
    for(QString s: out){
        s = s.trimmed();
        if(s.contains("HTTPEnable")){
            systemProxy.status = s.split(":").at(1);
        }else if(s.contains("HTTPPort")){
            systemProxy.port = s.split(":").at(1);
        }else if(s.contains("HTTPProxy")){
            systemProxy.host = s.split(":").at(1);
        }else if(s.contains("HTTPUser")){
            systemProxy.user = s.split(":").at(1);
        }
    }
    if(!confList->keys().contains("-setwebproxy \""+interface+"\""))
        confList->insert("-setwebproxy \""+interface+"\"", systemProxy);

    ProxyConf systemProxyS;
    for(QString s: out){
        s = s.trimmed();
        if(s.contains("HTTPSEnable")){
            systemProxyS.status = s.split(":").at(1);
        }else if(s.contains("HTTPSPort")){
            systemProxyS.port = s.split(":").at(1);
        }else if(s.contains("HTTPSProxy")){
            systemProxyS.host = s.split(":").at(1);
        }else if(s.contains("HTTPSUser")){
            systemProxyS.user = s.split(":").at(1);
        }
    }
    if(!confList->keys().contains("-setsecurewebproxy \""+interface+"\""))
        confList->insert("-setsecurewebproxy \""+interface+"\"", systemProxyS);

    ProxyConf systemProxyO;
    for(QString s: out){
        s = s.trimmed();
        if(s.contains("SOCKSEnable")){
            systemProxyS.status = s.split(":").at(1);
        }else if(s.contains("SOCKSPort")){
            systemProxyS.port = s.split(":").at(1);
        }else if(s.contains("SOCKSProxy")){
            systemProxyS.host = s.split(":").at(1);
        }else if(s.contains("SOCKSUser")){
            systemProxyS.user = s.split(":").at(1);
        }
    }
    if(!confList->keys().contains("-setsocksfirewallproxy \""+interface+"\""))
        confList->insert("-setsocksfirewallproxy \""+interface+"\"", systemProxyO);

    p.startDetached("networksetup -setsecurewebproxy \""+interface+"\" 127.0.0.1 "+httpPort);
    p.startDetached("networksetup -setwebproxy \""+interface+"\" 127.0.0.1 "+httpPort);
    p.startDetached("networksetup -setsocksfirewallproxy \""+interface+"\" 127.0.0.1 "+sockPort);

}

void ProxySetter::restoreProxy(){
    /// set in zsh
    QFile file(QDir::homePath()+"/.zprofile");
    file.open(QFile::ReadWrite);
    QString text = QString(file.readAll()).remove("\n"+Defs::configDirPath()+"/setProxy.bash ###PsiphonQt");
    file.resize(0);
    file.write(text.toUtf8());
    file.close();

    QProcess p;
    QList<QString> interfaces = confList->keys();
    for(int i = 0; i < interfaces.size(); i++){
        QString interCMD = interfaces.at(i);
        ProxyConf conf = confList->take(interfaces.at(i));
        if (conf.host.isEmpty()) {
            QString param = interCMD.insert(interCMD.indexOf(" "), "state");
            p.startDetached("networksetup " +param  + " off");
        }else {
            p.startDetached("networksetup " + interfaces.at(i) + " " + conf.host + " " + conf.port );
        }
    }
    confList->clear();

}

#elif defined (Q_OS_WIN)

void ProxySetter::setProxy(){

    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", QSettings::NativeFormat);
    ProxyConf systemProxy;
    systemProxy.host = settings.value("ProxyServer").toString();
    systemProxy.status = settings.value("ProxyEnable").toString();
    systemProxy.user = settings.value("ProxyUser").toString();
    systemProxy.pass = settings.value("ProxyPass").toString();

    if(!confList->keys().contains("system"))
        confList->insert("system", systemProxy);

    settings.setValue("ProxyServer", "127.0.0.1:"+httpPort);
    settings.setValue("ProxyEnable", "1");
    settings.setValue("ProxyUser", "");
    settings.setValue("ProxyPass", "");
    QProcess::startDetached("netsh winhttp import proxy source=ie");
}

void ProxySetter::restoreProxy(){
        QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", QSettings::NativeFormat);
        ProxyConf systemProxy = confList->take("system");
        settings.setValue("ProxyServer", systemProxy.host);
        settings.setValue("ProxyEnable", systemProxy.status.toInt());
        settings.setValue("ProxyUser", systemProxy.user);
        settings.setValue("ProxyPass", systemProxy.pass);
        QProcess::startDetached("netsh winhttp import proxy source=ie");
        confList->clear();
}

#else

void ProxySetter::setProxy(){
//    set in bash

    exportShellSetter();

    QFile file(QDir::homePath()+"/.bash_profile");
    file.open(QFile::Append);
    file.write("\n"+Defs::configDirPath().toUtf8()+"/setProxy.bash ###PsiphonQt\n");
    file.close();

    QString desk = qgetenv("XDG_CURRENT_DESKTOP");

    if(desk.toUpper().contains("GNOME") || desk.toUpper().contains("DEEPIN") ){
        ///GNOME
        setProxyGnome();
    }else if(desk == "KDE"){
        ///KDE
        setProxyKde();

    }
}

void ProxySetter::restoreProxy(){
//    set in bash

        QFile file(QDir::homePath()+"/.bash_profile");
        file.open(QFile::ReadWrite);
        QString text = QString(file.readAll()).remove("\n"+Defs::configDirPath()+"/setProxy.bash ###PsiphonQt");
        file.resize(0);
        file.write(text.toUtf8());
        file.close();

        QString desk = qgetenv("XDG_CURRENT_DESKTOP");

        if(desk.toUpper().contains("GNOME") || desk.toUpper().contains("DEEPIN") ){
            ///GNOME
            restoreProxyGnome();
        }else if(desk == "KDE"){
            ///KDE
            restoreProxyKde();

        }
        confList->clear();
}

void ProxySetter::setProxyKde(){
    QProcess p;
    ProxyConf systemProxy;
    p.start("kreadconfig5 --file kioslaverc --group \"Proxy Settings\" --key ProxyType");
    p.waitForFinished();
    QString status = p.readAllStandardOutput().simplified();

    systemProxy.status = status;
    p.start("kreadconfig5 --file kioslaverc --group \"Proxy Settings\" --key httpProxy");
    p.waitForFinished();
    systemProxy.host = p.readAllStandardOutput().simplified();
    
    if(!confList->keys().contains("httpProxy"))
        confList->insert("httpProxy", systemProxy);
    
    ProxyConf systemProxys;
    systemProxys.status = status;
    p.start("kreadconfig5 --file kioslaverc --group \"Proxy Settings\" --key httpsProxy");
    p.waitForFinished();
    systemProxys.host = p.readAllStandardOutput().simplified();
    
    if(!confList->keys().contains("httpsProxy"))
        confList->insert("httpProxy", systemProxys);

    ProxyConf systemProxyo;
    systemProxyo.status = status;
    p.start("kreadconfig5 --file kioslaverc --group \"Proxy Settings\" --key socksProxy");
    p.waitForFinished();
    systemProxyo.host = p.readAllStandardOutput().simplified();
    
    if(!confList->keys().contains("sockProxy"))
        confList->insert("sockProxy", systemProxyo);

    p.start("kwriteconfig5 --file kioslaverc --group \"Proxy Settings\" --key ProxyType 1");
    p.waitForFinished();
    p.start("kwriteconfig5 --file kioslaverc --group \"Proxy Settings\" --key httpProxy 127.0.0.1:"+httpPort);
    p.waitForFinished();
    p.start("kwriteconfig5 --file kioslaverc --group \"Proxy Settings\" --key httpsProxy 127.0.0.1:"+httpPort);
    p.waitForFinished();
    p.start("kwriteconfig5 --file kioslaverc --group \"Proxy Settings\" --key socksProxy 127.0.0.1:"+sockPort);
    p.waitForFinished();
    p.start("dbus-send --type=signal /KIO/Scheduler org.kde.KIO.Scheduler.reparseSlaveConfiguration string:''");
    p.waitForFinished();
}

void ProxySetter::restoreProxyKde(){
    if(confList->isEmpty())
        return;
    QProcess p;
    for (QString s : confList->keys()) {
        ProxyConf systemProxy = confList->take(s);
        p.start("kwriteconfig5 --file kioslaverc --group \"Proxy Settings\" --key ProxyType "+systemProxy.status);
        p.waitForFinished();
        p.start("kwriteconfig5 --file kioslaverc --group \"Proxy Settings\" --key "+s+" "+systemProxy.host.replace(" ", ":"));
        p.waitForFinished();
    }

    p.start("dbus-send --type=signal /KIO/Scheduler org.kde.KIO.Scheduler.reparseSlaveConfiguration string:''");
    p.waitForFinished();
    confList->clear();
}

void ProxySetter::setProxyGnome(){
    ProxyConf systemProxy;
    QString mode;
    QProcess p;
    p.start("gsettings get org.gnome.system.proxy mode");
    p.waitForFinished();
    mode = p.readAllStandardOutput().simplified().simplified();

    systemProxy.status = mode;
    p.start("gsettings get org.gnome.system.proxy.http host");
    p.waitForFinished();
    systemProxy.host = p.readAllStandardOutput().simplified().simplified();
    p.start("gsettings get org.gnome.system.proxy.http port");
    p.waitForFinished();
    systemProxy.port = p.readAllStandardOutput().simplified().simplified();

    if(!confList->keys().contains("org.gnome.system.proxy.http"))
        confList->insert("org.gnome.system.proxy.http", systemProxy);

    ProxyConf systemProxys;
    systemProxys.status = mode;
    p.start("gsettings get org.gnome.system.proxy.https host");
    p.waitForFinished();
    systemProxys.host = p.readAllStandardOutput().simplified().simplified();
    p.start("gsettings get org.gnome.system.proxy.https port");
    p.waitForFinished();
    systemProxys.port = p.readAllStandardOutput().simplified().simplified();

    if(!confList->keys().contains("org.gnome.system.proxy.https"))
        confList->insert("org.gnome.system.proxy.https", systemProxys);

    ProxyConf systemProxyo;
    systemProxyo.status = mode;
    p.start("gsettings get org.gnome.system.proxy.socks host");
    p.waitForFinished();
    systemProxyo.host = p.readAllStandardOutput().simplified().simplified();
    p.start("gsettings get org.gnome.system.proxy.socks port");
    p.waitForFinished();
    systemProxyo.port = p.readAllStandardOutput().simplified().simplified();
    if(!confList->keys().contains("org.gnome.system.proxy.socks"))
        confList->insert("org.gnome.system.proxy.socks", systemProxyo);

    p.startDetached("gsettings set org.gnome.system.proxy mode 'manual'");
    p.startDetached("gsettings set org.gnome.system.proxy.http host '127.0.0.1'");
    p.startDetached("gsettings set org.gnome.system.proxy.http port "+httpPort);

    p.startDetached("gsettings set org.gnome.system.proxy.https host '127.0.0.1'");
    p.startDetached("gsettings set org.gnome.system.proxy.https port "+httpPort);

    p.startDetached("gsettings set org.gnome.system.proxy.socks host '127.0.0.1'");
    p.startDetached("gsettings set org.gnome.system.proxy.socks port "+sockPort);
}

void ProxySetter::restoreProxyGnome(){
    if(confList->isEmpty())
        return;
    for(QString s: confList->keys()){
        ProxyConf systemProxy = confList->take(s);
        QProcess::startDetached("gsettings set org.gnome.system.proxy mode "+systemProxy.status);
        QProcess::startDetached("gsettings set "+s+" host "+systemProxy.host);
        QProcess::startDetached("gsettings set "+s+" port "+systemProxy.port);
    }
    confList->clear();
}

#endif



//void ProxySetter::setProxy(){
//#ifdef Q_OS_MAC)
//    setProxyMac();
//#elif defined (Q_OS_WIN)
//    setProxyWin();
//#else
//    setProxyLinux();
//#endif
//}

//void ProxySetter::restoreProxy(){
//#ifdef Q_OS_MAC)
//    restoreProxyMac();
//#elif defined (Q_OS_WIN)
//    restoreProxyWin();
//#else
//    restoreProxyLinux();
//#endif
//}
