#ifndef PROXYSETTER_H
#define PROXYSETTER_H

#include <QMap>
#include <QObject>

class ProxyConf{
public:
    QString status = "";
    QString host = "";
    QString user = "";
    QString pass = "";
    QString port = "";
    QString toString(){
        return  status + user + pass + host + port;
    }
};

class ProxySetter
{
public:
    ProxySetter(QString httpPort, QString sockPort);

public slots:
    void setProxy();
    void restoreProxy();

#if not defined(Q_OS_MAC) and not defined (Q_OS_WIN)
    void setProxyKde();
    void setProxyGnome();
    void restoreProxyGnome();
    void restoreProxyKde();
#endif
private:
    QString sockPort;
    QString httpPort;
    QMap<QString, ProxyConf> *confList;
};

#endif // PROXYSETTER_H
