#ifndef COREPROCESSCONTROLER_H
#define COREPROCESSCONTROLER_H

#include <QProcess>
#include <QDebug>


class CoreControler
{
public:
    static QString corePid(QString path);
    static bool coreIsRunning(QString path);
    static void killCore(QString path);
};


#endif // COREPROCESSCONTROLER_H



bool CoreControler::coreIsRunning(QString path){
    return !corePid(path).isEmpty();
}

QString CoreControler::corePid(QString path){
    QProcess p;
#if not defined (Q_OS_WIN)
    p.start("ps aux");
    p.waitForFinished();
    QString out =  p.readAllStandardOutput();
    for(QString s: out.split("\n")){
        if(s.contains(path)){
            s=s.simplified();
            out = s.split(" ").at(1);
        }
    }
    return out;
#else
    p.start("tasklist /fi \"IMAGENAME eq "+path+"\" /nh /fo:CSV");
    p.waitForFinished();
    QString out = p.readAllStandardOutput();
    qDebug() << out;
    if (out.contains(",")) {
        qDebug() << "este "<< out.split(",").at(1).simplified();
        return out.split(",").at(1).simplified();
    } else {
        return "";
    }
#endif
    return "";
}

void CoreControler::killCore(QString path){
    QProcess p;
#if not defined (Q_OS_WIN)
    p.start("kill -kill "+ corePid(path));
#else
    p.start("taskkill /pid " +corePid(path));
#endif
    p.waitForFinished();
}
