#ifndef COUNTRYDATA_H
#define COUNTRYDATA_H

#include <QObject>

class CountryData : public QObject
{
    Q_OBJECT
public:
    explicit CountryData(QObject *parent = nullptr);

public slots:
    static QString iso2name(QString iso);
    static QString iso2code(QString iso);
    static QString iso2flag(QString iso);
signals:

private slots:
    void fill();
    QString iso2line(QString iso);

private:
    QStringList country_codes;
};

#endif // COUNTRYDATA_H
