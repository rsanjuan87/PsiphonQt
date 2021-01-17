#include <QCoreApplication>
#include <QTextStream>
#include <QFileInfo>
#include <QSettings>
#include <QProcess>
#include <QString>
#include <QFile>
#include <QDir>
#include "qautostart.h"

#ifdef Q_OS_WIN
#define REG_KEY "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"

bool QAutostart::isAutostart() const {
    QSettings settings(REG_KEY, QSettings::NativeFormat);

    if (settings.value(appName()).isNull()) {
        return false;
    }

    return true;
}

void QAutostart::setAutostart(bool autostart) {
    QSettings settings(REG_KEY, QSettings::NativeFormat);

    if (autostart) {
        settings.setValue(appName() , appPath().replace('/','\\'));
    } else {
        settings.remove(appName());
    }
}

#elif defined (Q_OS_MAC)
#define PLIST_PATH QDir::homePath()+"/Library/LaunchAgents/"+appName()+".plist"

bool QAutostart::isAutostart() const {
    return QFile(PLIST_PATH).exists();
}

void QAutostart::setAutostart(bool autostart) {
    QString plistPath = PLIST_PATH;
    QFile::remove(plistPath);
    if (autostart) {
        QSettings plist(plistPath, QSettings::NativeFormat);
        plist.setValue("Label", appName());
        plist.setValue("ProgramArguments", QStringList()<<appPath());
        plist.setValue("ProcessType", "Interactive");
        plist.setValue("RunAtLoad", true);
        plist.setValue("KeepAlive", false);
    }
}

#elif defined (Q_OS_LINUX)
#define DESK_PATH QDir::homePath() + "/.config/autostart/" + appName() +".desktop"

bool QAutostart::isAutostart() const {
    QFileInfo check_file(DESK_PATH);

    if (check_file.exists() && check_file.isFile()) {
        return true;
    }

    return false;
}

void QAutostart::setAutostart(bool autostart) {
    QFile file(DESK_PATH);

    file.remove();
    QString path = QFileInfo(DESK_PATH).absoluteDir().path();
    if(autostart) {
        QDir dir(path);
        if(!dir.exists()) {
            dir.mkpath(path);
        }

        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "[Desktop Entry]" << endl;
            stream << "Exec=" << appPath() << endl;
            stream << "Type=Application" << endl;
        }
    }
}

#else

bool QAutostart::isAutostart() const {
    return false;
}

void QAutostart::setAutostart(bool autostart) {
    Q_UNUSED(autostart);
}

QString QAutostart::appPath() const {
    return QString();
}
#endif

QString QAutostart::appPath() const {
    return QCoreApplication::applicationFilePath();
}

QString QAutostart::appName() const {
    return QCoreApplication::applicationName();
}
