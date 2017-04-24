//
//  DatabaseManager.cpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 23/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "DatabaseManager.hpp"


bool DatabaseManager::createLocalDatabase(const QString &databaseFilePath)
{
    if (!DatabaseManager::installLocalDatabase(databaseFilePath))
        return false;

    // Save database filepath for restoring purposes
    QSettings settings;
    settings.setValue(SETTINGKEY_SQLITE_DATABASE_FILEPATH, databaseFilePath);

    // TODO : Save type of database opened

    return true;
}

bool DatabaseManager::createLocalDatabase(QDir const& databaseDir, QString const& databaseName)
{

    QString validDatabaseName = databaseName;

    // Check if the file have the right extension
    QFileInfo databasefileInfo(databaseName);
    if (databasefileInfo.suffix().isEmpty() || databasefileInfo.suffix() != DEFAULT_SQLITE_DATABASE_EXTENSION)
    {
        validDatabaseName = databasefileInfo.fileName().append(DEFAULT_SQLITE_DATABASE_EXTENSION);
    }

    return DatabaseManager::createLocalDatabase(databaseDir.filePath(validDatabaseName));
}

bool DatabaseManager::createRemoteDatabase(ConnectionOptions const& connectionOptions)
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

    if (!db.open())
        throw NException(QObject::tr("Error when opening connection at ") +
                         connectionOptions.hostName() + " : " +
                         db.lastError().text(), db.lastError().NoError);

    /* ---------------------------------------------------------------------- *
     *                             Create Database                            *
     * ---------------------------------------------------------------------- */

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

    /* ---------------------------------------------------------------------- *
     *                              Create Schema                             *
     * ---------------------------------------------------------------------- */

    return createSchema(DatabaseType::MySQL);
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

bool DatabaseManager::installLocalDatabase(const QString &databaseFilePath)
{
    /* --------------------------------------------------------------------- *
     *                             Open Database                             *
     * --------------------------------------------------------------------- */

    if (DatabaseManager::openLocalDatabase(databaseFilePath))
        return DatabaseManager::createSchema(DatabaseType::SQLite);
    return false;
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
