#ifndef AUTOSTART_H
#define AUTOSTART_H

#include <QString>

class QAutostart
{
public:
    bool isAutostart() const;
    void setAutostart(bool autostart);

protected:
    QString appPath() const;
    QString appName() const;
 };

#endif // AUTOSTART_H
