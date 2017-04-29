//
//  DatabaseManager.cpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 23/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "DatabaseManager.hpp"

bool DatabaseManager::restorePreviousDataBase(DatabaseType& databaseType)
{
    QSettings settings;
    settings.beginGroup(SETTINGS_GROUP_DATABASEMANAGER);

    if (settings.contains(SETTING_KEY_LAST_OPENED_DATABASE_TYPE))
    {
        databaseType = (DatabaseType)settings.value(SETTING_KEY_LAST_OPENED_DATABASE_TYPE).toInt();

        switch (databaseType)
        {
            case DatabaseType::SQLite:
            {
                settings.beginGroup(SETTINGS_GROUP_SQLITE);
                QString dbFilePath = settings.value(SETTINGKEY_SQLITE_DATABASE_FILEPATH, QString()).toString();
                return DatabaseManager::openExistingLocalDatabase(dbFilePath);
            }
            case DatabaseType::MySQL:
            {
                settings.beginGroup(SETTINGS_GROUP_MYSQL);
                ConnectionOptions connectionOptions;
                connectionOptions.setHostName(settings.value(SETTING_KEY_MYSQL_HOSTNAME, QString()).toString());
                connectionOptions.setPort(settings.value(SETTING_KEY_MYSQL_PORT, -1).toInt());
                connectionOptions.setDatabaseName(settings.value(SETTING_KEY_MYSQL_DATABASE_NAME, QString()).toString());
                connectionOptions.setUserName(settings.value(SETTING_KEY_MYSQL_USERNAME, QString()).toString());
                connectionOptions.setPassword(settings.value(SETTING_KEY_MYSQL_PASSWORD, QString()).toString());
                return DatabaseManager::openExistingRemoteDatabase(connectionOptions);
            }
            default:
                break;
        }
    }

    settings.endGroup();
    return false;
}

bool DatabaseManager::createLocalDatabase(QString const& databaseFilePath)
{
    QString validFilePath = databaseFilePath;

    // Check if the file have the right extension
    QFileInfo databasefileInfo(databaseFilePath);
    if (databasefileInfo.suffix().isEmpty())
        validFilePath = databasefileInfo.fileName().append(DEFAULT_SQLITE_DATABASE_EXTENSION);
    else if (databasefileInfo.suffix() != DEFAULT_SQLITE_DATABASE_EXTENSION)
        validFilePath = databasefileInfo.path() + QDir::separator() +
                databasefileInfo.baseName() + DEFAULT_SQLITE_DATABASE_EXTENSION;

    /* --------------------------------------------------------------------- *
     *                    Open Database and create schema                    *
     * --------------------------------------------------------------------- */

    DatabaseManager::openLocalDatabase(databaseFilePath);
    if (!DatabaseManager::createSchema(DatabaseType::SQLite))
        return false;

    // Save database settings
    DatabaseManager::saveDatabaseManagerSettings();

    return true;
}

bool DatabaseManager::createLocalDatabase(QDir const& databaseDir, QString const& databaseName)
{
    return DatabaseManager::createLocalDatabase(databaseDir.filePath(databaseName));
}

bool DatabaseManager::createRemoteDatabase(ConnectionOptions const& connectionOptions)
{
    DatabaseManager::openRemoteDatabase(connectionOptions, true);
    if (!DatabaseManager::createSchema(DatabaseType::MySQL))
        return false;

    // Save database settings
    DatabaseManager::saveDatabaseManagerSettings();

    return true;
}

bool DatabaseManager::openExistingLocalDatabase(QString const& databaseFilePath)
{
    if (!QFile::exists(databaseFilePath))
        return false;

    if (!DatabaseManager::openLocalDatabase(databaseFilePath))
        return false;

    // Save database settings
    DatabaseManager::saveDatabaseManagerSettings();

    return true;
}

bool DatabaseManager::openExistingLocalDatabase(QDir const& databaseDir, QString const& databaseName)
{
    return DatabaseManager::openExistingLocalDatabase(databaseDir.filePath(databaseName));
}

bool DatabaseManager::openExistingRemoteDatabase(const ConnectionOptions &connectionOptions)
{
    if (!DatabaseManager::openRemoteDatabase(connectionOptions))
        return false;

    // Save database settings
    DatabaseManager::saveDatabaseManagerSettings();

    return true;
}

QSqlQuery DatabaseManager::execQuery(QString const& queryString, QVariantList const& values, bool forwardOnly)
{
    QSqlQuery query(queryString);

    // bind values
    foreach (QVariant value, values)
        query.addBindValue(value);

    query.setForwardOnly(forwardOnly);

    if (!query.exec())
        throw NException(QObject::tr("Error when executing query : ")
                         + query.lastQuery() + " " + query.lastError().text());

    return query; // Implicit sharing
}

void DatabaseManager::execTransaction(QSqlQuery& query)
{
    QSqlDriver* sqlDriver = QSqlDatabase::database().driver();

    sqlDriver->beginTransaction();

    if(!query.exec())
    {
        sqlDriver->rollbackTransaction();
        throw NException(QObject::tr("Request failed : ")
                         + query.lastQuery() + query.lastError().text());
    }

    // Try to commit transaction
    if(!sqlDriver->commitTransaction())
        throw NException(QObject::tr("Data validation failed"));
}

QSqlQuery DatabaseManager::execTransaction(QString const& queryString, QVariantList const& values, bool forwardOnly)
{
    QSqlQuery query(queryString);

    // bind values
    foreach (QVariant value, values)
        query.addBindValue(value);

    query.setForwardOnly(forwardOnly);

    DatabaseManager::execTransaction(query);

    return query;
}

void DatabaseManager::execBatch(QSqlQuery &query, QSqlQuery::BatchExecutionMode mode)
{
    QSqlDriver* sqlDriver = QSqlDatabase::database().driver();

    sqlDriver->beginTransaction();

    if(!query.execBatch(mode))
    {
        sqlDriver->rollbackTransaction();
        throw NException(QObject::tr("Request failed : ")
                         + query.lastQuery() + query.lastError().text());
    }

    // Try to commit transaction
    if(!sqlDriver->commitTransaction())
        throw NException(QObject::tr("Data validation failed"));
}

bool DatabaseManager::closeConnection(void)
{
    if(QSqlDatabase::database().isValid() &&
            QSqlDatabase::database().isOpen())
    {
        QString dbConnectionName =
                QSqlDatabase::database().connectionName();

        QSqlDatabase::database().close();
        QSqlDatabase::removeDatabase(dbConnectionName);
    }

    return true;
}

bool DatabaseManager::openLocalDatabase(const QString &dataBaseFilePath)
{
    /* ---------------------------------------------------------------------- *
     *                   Close existing database connection                   *
     * ---------------------------------------------------------------------- */
    DatabaseManager::closeConnection();

    /* ---------------------------------------------------------------------- *
     *                              Open Database                             *
     * ---------------------------------------------------------------------- */

    // SQLite version 3 or above
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dataBaseFilePath);

    if (!db.open())
        throw NException(QObject::tr("Error when opening local database ") +
                         dataBaseFilePath + " : " + db.lastError().text());

    db.exec("PRAGMA foreign_keys = ON");
    db.exec("PRAGMA journal_mode = MEMORY");
    db.exec("PRAGMA synchronous  = OFF");

    return true;
}

bool DatabaseManager::openRemoteDatabase(const ConnectionOptions &connectionOptions, bool create)
{
    if (!connectionOptions.isValid())
        throw NException(QObject::tr("Invalid Remote Server Connection"));

    /* ---------------------------------------------------------------------- *
     *                   Close existing database connection                   *
     * ---------------------------------------------------------------------- */

    DatabaseManager::closeConnection();

    /* ---------------------------------------------------------------------- *
     *                              Open Database                             *
     * ---------------------------------------------------------------------- */

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(connectionOptions.hostName());
    db.setPort(connectionOptions.port());
    db.setUserName(connectionOptions.userName());
    db.setPassword(connectionOptions.password());

    // If database already exists
    if (!create)
        db.setDatabaseName(connectionOptions.databaseName());

    if (!db.open())
        throw NException(QObject::tr("Error when opening connection at ") +
                         connectionOptions.hostName() + " : " +
                         db.lastError().text(), db.lastError().NoError);

    /* ---------------------------------------------------------------------- *
     *                       Create and Use new Database                      *
     * ---------------------------------------------------------------------- */
    if (create)
    {
        bool execSucced(false);
        QSqlQuery query(db);

        // Delete database if exists
        execSucced = query.exec("DROP DATABASE IF EXISTS " + connectionOptions.databaseName());
        if (!execSucced)
        {
            throw NException(QObject::tr("Error when deleting existing database ") +
                             connectionOptions.databaseName() + " : " +
                             db.lastError().text(), db.lastError().NoError);
        }

        // Create database
        execSucced = query.exec("CREATE DATABASE IF NOT EXISTS " + connectionOptions.databaseName());
        if (!execSucced)
        {
            throw NException(QObject::tr("Error when creating database ") +
                             connectionOptions.databaseName() + " : " +
                             db.lastError().text(), db.lastError().NoError);
        }

        // Use the newly created database
        query.exec("USE " + connectionOptions.databaseName());
    }

    return true;
}

bool DatabaseManager::createSchema(DatabaseType databaseType)
{
    // Use a script to create tables
    QFile schemaFile;
    switch (databaseType) {
        case DatabaseType::SQLite:
            schemaFile.setFileName(":/sql/SQLite-database-schema"); // Resource file
            break;
        case DatabaseType::MySQL:
            schemaFile.setFileName(":/sql/MySQL-database-schema"); // Resource file
            break;
        default:
            throw NException("Unsupported database");
    }

    if (schemaFile.fileName().isEmpty())
        throw NException("Error when creating database schema : no schema specified");

    QSqlDatabase db = QSqlDatabase::database();

    // Get all table creation scripts
    schemaFile.open(QFile::ReadOnly);
    QStringList schemaTableList = QString(
           schemaFile.readAll()).split(";", QString::SkipEmptyParts);

    db.driver()->beginTransaction();

    // Create all tables from script
    foreach(const QString schemaTable, schemaTableList)
        if(!schemaTable.trimmed().isEmpty())
            db.exec(schemaTable);

    bool commitSucced = db.driver()->commitTransaction();

    schemaFile.close();

    return commitSucced;
}

void DatabaseManager::saveDatabaseManagerSettings(void)
{
    QSqlDatabase db = QSqlDatabase::database();
    DatabaseType dbType = db.driverName() == "QSQLITE" ? DatabaseType::SQLite : DatabaseType::MySQL;

    QSettings settings;

    settings.beginGroup(SETTINGS_GROUP_DATABASEMANAGER);

    // Save current database type
    settings.setValue(SETTING_KEY_LAST_OPENED_DATABASE_TYPE, dbType);

    // Save database type depending settings
    switch (dbType) {
        case DatabaseType::SQLite:
            settings.beginGroup(SETTINGS_GROUP_SQLITE);
            settings.setValue(SETTINGKEY_SQLITE_DATABASE_FILEPATH, db.databaseName());
            settings.endGroup(); // End of SETTINGS_GROUP_SQLITE
            break;
        case DatabaseType::MySQL:
            settings.beginGroup(SETTINGS_GROUP_MYSQL);
            settings.setValue(SETTING_KEY_MYSQL_HOSTNAME, db.hostName());
            settings.setValue(SETTING_KEY_MYSQL_PORT, db.port());
            settings.setValue(SETTING_KEY_MYSQL_DATABASE_NAME, db.databaseName());
            settings.setValue(SETTING_KEY_MYSQL_USERNAME, db.userName());
            settings.setValue(SETTING_KEY_MYSQL_PASSWORD, db.password());
            settings.endGroup(); // End of SETTINGS_GROUP_MYSQL
            break;
        default:
            // Unknow database type - Nothing to do ...
            break;
    }

    settings.endGroup(); // end of SETTINGS_GROUP_DATABASEMANAGER
}
