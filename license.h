#ifndef LICENSE_H
#define LICENSE_H

#include <QObject>

#define __LICENSE_TIMESTAMP_FROMAT__ "yyyyMMddhhmmss"

class License : public QObject
{
    Q_OBJECT
public:
    explicit License(QObject *parent = nullptr);

    Q_INVOKABLE QString createLicense(const QString &userInfo, const QString &endTime, QString lid, const QString &productType);

    Q_INVOKABLE bool checkLicense(QString license);

signals:

public slots:

private:
    QString getKernelTypeByName(const QString &name);

};

#endif // LICENSE_H
