#ifndef DEFS_H
#define DEFS_H

#define VERSION "1.1.1"

#include <QDir>

class Defs
{
public:
    Defs() {}
    static inline QString configDirPath(){
        return QDir::toNativeSeparators(QDir::homePath()+"/.PsiphonQt/");
    }
    static inline QString appConfigFilePath(){
        return QDir::toNativeSeparators(QDir::homePath()+"/.PsiphonQt/appconfig.json");
    }
    static inline QString tunelConfigFilePath(){
        return QDir::toNativeSeparators(QDir::homePath()+"/.PsiphonQt/config.json");
    }
};

#endif // DEFS_H
