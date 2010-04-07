#include "logging.h"

Logging::Logging()
{
}

void Logging::clearLogs(void){
    QSqlQuery query;
    query.exec("DELETE FROM logging");
    if (!query.exec()){
        qDebug()<<"clearLogs.SqlError: "<<query.lastError()<<query.executedQuery();
    }
    query.exec("VACUUM");
    if (!query.exec()){
        qDebug()<<"clearLogs.SqlError: "<<query.lastError()<<query.executedQuery();
    }
    return;
}

void Logging::deleteLogs(QString prefix_name){
    QSqlQuery query;
    query.prepare("DELETE FROM logging WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name);");
    query.bindValue(":prefix_name", prefix_name);

    if (!query.exec()){
        qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
    }
    return;
}

void Logging::deleteLogs(QString prefix_name, QString app_name){
    QSqlQuery query;
    query.prepare("DELETE FROM logging WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND name=:app_name;");
    query.bindValue(":prefix_name", prefix_name);
    query.bindValue(":app_name", app_name);

    if (!query.exec()){
        qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
    }
    return;
}

void Logging::deleteLogs(QString prefix_name, QString app_name, QString date){
    QSqlQuery query;
    query.prepare("DELETE FROM logging WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND name=:app_name AND date=:date;");
    query.bindValue(":prefix_name", prefix_name);
    query.bindValue(":app_name", app_name);
    query.bindValue(":date", QDateTime::fromString(date, "dd.MM.yyyy hh:mm:ss").toTime_t());

    if (!query.exec()){
        qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
    }
    return;
}

void Logging::addLogRecord(int prefix_id, QString program, int exit, QString stdout, uint date){
    QSqlQuery query;

    query.prepare("INSERT INTO logging(name, exit, stdout, prefix_id, date) VALUES (:name, :exit, :stdout, :prefix_id, :date);");
    query.bindValue(":name", program);
    query.bindValue(":exit", exit);
    query.bindValue(":stdout", stdout);
    query.bindValue(":prefix_id", prefix_id);
    query.bindValue(":date", date);

    if (!query.exec()){
        qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
    }
    return;
}

QStringList Logging::getApps(QString prefix_name){
    QStringList list;

    QSqlQuery query;
    query.prepare("SELECT name FROM logging WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) GROUP BY name ORDER BY name");
    query.bindValue(":prefix_name", prefix_name);

    if (query.exec()){
        while (query.next()) {
            if (query.value(0).isValid())
                list.append(query.value(0).toString());
        }
    } else {
        qDebug()<<"SqlError: "<<query.lastError();
    }
    query.clear();

    return list;
}

QHash<uint, int> Logging::getDates(QString prefix_name, QString app_name){
    QHash<uint, int> list;

    QSqlQuery query;
    query.prepare("SELECT date, exit FROM logging WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND name=:app_name ORDER BY date DESC");
    query.bindValue(":prefix_name", prefix_name);
    query.bindValue(":app_name", app_name);

    if (query.exec()){
        while (query.next()) {
            if (query.value(0).isValid()){
                list.insert(query.value(0).toUInt(), query.value(1).toInt());
            }
        }
    } else {
        qDebug()<<"SqlError: "<<query.lastError();
    }
    query.clear();

    return list;
}

QString Logging::getLogs(QString prefix_name, QString app_name, QString date){
    QString list;

    QSqlQuery query;
    query.prepare("SELECT stdout FROM logging WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND name=:app_name AND date=:date");
    query.bindValue(":prefix_name", prefix_name);
    query.bindValue(":app_name", app_name);
    query.bindValue(":date", QDateTime::fromString(date, "dd.MM.yyyy hh:mm:ss").toTime_t());

    if (query.exec()){
        query.first();
        if (query.value(0).isValid())
            list = query.value(0).toString();
    } else {
        qDebug()<<"SqlError: "<<query.lastError();
    }
    query.clear();

    return list;
}
