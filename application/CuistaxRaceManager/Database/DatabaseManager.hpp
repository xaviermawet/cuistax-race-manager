//
//  DatabaseManager.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 23/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#ifndef __DATABASEMANAGER_HPP__
#define __DATABASEMANAGER_HPP__

#include <QtSql>
#include <QFile>

#include "ConnectionOptions.hpp"
#include "Utils/NException.hpp"

/* Common */
#define DEFAULT_DATABASE_NAME "Cuistax"

/* Local database (SQLite) */
#define DEFAULT_SQLITE_DATABASE_EXTENSION ".db"

/* Remote database (MySQL) */

/* Settings Groups & Keys */
#define SETTINGS_GROUP_DATABASEMANAGER "DatabaseManager"
#define SETTING_KEY_LAST_OPENED_DATABASE_TYPE "type"

#define SETTINGS_GROUP_SQLITE "SQLite"
#define SETTINGKEY_SQLITE_DATABASE_FILEPATH "database"

#define SETTINGS_GROUP_MYSQL "MySQL"
#define SETTINGS_GROUP_MYSQL "MySQL"
#define SETTING_KEY_MYSQL_HOSTNAME "hostname"
#define SETTING_KEY_MYSQL_PORT "port"
#define SETTING_KEY_MYSQL_DATABASE_NAME "database_name"
#define SETTING_KEY_MYSQL_USERNAME "username"
#define SETTING_KEY_MYSQL_PASSWORD "password"

class DatabaseManager
{
    public:

        enum DatabaseType
        {
            SQLite = 0,
            MySQL = 1,
            UNKNOWN = 99
        };

        static bool restorePreviousDataBase(DatabaseType& databaseType);

        static bool createLocalDatabase(QString const& databaseFilePath);
        static bool createLocalDatabase(QDir const& databaseDir = QDir::current(),
                                        QString const& databaseName = DEFAULT_DATABASE_NAME);

        static bool createRemoteDatabase(ConnectionOptions const& connectionOptions);

        static bool openExistingLocalDatabase(QString const& databaseFilePath);
        static bool openExistingLocalDatabase(QDir const& databaseDir = QDir::current(),
                                         QString const& databaseName = DEFAULT_DATABASE_NAME);

        static bool openExistingRemoteDatabase(ConnectionOptions const& connectionOptions);

        /* To bind value to a previously prepared query,
         * parameters have to be lists of variants */
        template <typename T>
        static QVariantList toVariantList( const QList<T> &list )
        {
            QVariantList newList;
            foreach( const T &item, list )
                newList << item;

            return newList;
        }

        template <typename T>
        static QVariantList toVariantList( const QVector<T> &vector )
        {
            QVariantList newList;
            foreach( const T &item, vector )
                newList << item;

            return newList;
        }

    private:

        /*!
         * \brief closeConnection: Close existing database connection
         * \return
         */
        static bool closeConnection(void);       

        static bool openLocalDatabase(QString const& databaseFilePath);
        static bool openRemoteDatabase(ConnectionOptions const& connectionOptions, bool create = false);

        static bool createSchema(DatabaseType databaseType);

        static void saveDatabaseManagerSettings(void);
};

#endif /* __DATABASEMANAGER_HPP__ */
