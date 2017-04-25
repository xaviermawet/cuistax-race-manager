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

/* Setting keys */
#define SETTINGKEY_SQLITE_DATABASE_FILEPATH "database"

class DatabaseManager
{
    enum DatabaseType
    {
        SQLite,
        MySQL
    };

    public:

        static bool createLocalDatabase(QString const& databaseFilePath);
        static bool createLocalDatabase(QDir const& databaseDir = QDir::current(),
                                        QString const& databaseName = DEFAULT_DATABASE_NAME);

        static bool createRemoteDatabase(ConnectionOptions const& connectionOptions);

        static bool openExistingDatabase(QString const& databaseFilePath);
        static bool openExistingDatabase(QDir const& databaseDir = QDir::current(),
                                         QString const& databaseName = DEFAULT_DATABASE_NAME);

    private:

        /*!
         * \brief closeConnection: Close existing database connection
         * \return
         */
        static bool closeConnection(void);       

        static bool openLocalDatabase(QString const& databaseFilePath);
        static bool openRemoteDatabase(ConnectionOptions const& connectionOptions, bool create = false);

        static bool createSchema(DatabaseType databaseType);
};

#endif /* __DATABASEMANAGER_HPP__ */
