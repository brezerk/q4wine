#ifndef LOGGING_H
#define LOGGING_H

#include <config.h>

#include <memory>

#include <QList>
#include <QString>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QDateTime>

class Logging
{
public:
    Logging();

    void addLogRecord(int prefix_id, QString program, int exit, QString stdout, uint date);

    void clearLogs(void);
    void deleteLogs(QString prefix_name);
    void deleteLogs(QString prefix_name, QString app_name);
    void deleteLogs(QString prefix_name, QString app_name, QString date);

    QStringList getApps(QString prefix_name);
    QHash<uint, int> getDates(QString prefix_name, QString app_name);
    QString getLogs(QString prefix_name, QString app_name, QString date);

};

#endif // LOGGING_H
