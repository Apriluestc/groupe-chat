#include "sqlite.h"
/*
bool CSqlite::sqliteInit()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(m_dbName);
    if (!m_database.open()) {

        qDebug() << m_database.lastError();
        return false;
    } else {

        return true;
    }
}

bool CSqlite::printTable()
{
    QSqlQuery sqlQuery(m_database);
    if (!sqlQuery.exec("select * from userInfo")) {

        qDebug() << sqlQuery.lastError();
        return false;
    }
    int tmpId;
    QString tmpPasswd;
    while (sqlQuery.next()) {

        tmpId = sqlQuery.value(0).toInt();
        tmpPasswd = sqlQuery.value(2).toString();
        cout << "id=" << tmpId << ", passwd=" << tmpPasswd.toStdString() << endl;
    }
    return true;
}

bool CSqlite::checkoutPasswd(char *name, char *passwd)
{
    cout << "name:" << name << "passwd:" << passwd << endl;
    return true;
}

int CSqlite::getUserIdFromSqlite(char *name)
{
    cout << "name:" << name << endl;
    return 1;
}
*/





