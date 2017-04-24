//
//  ConnectionOptions.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 24/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#ifndef __CONNECTIONOPTIONS_HPP__
#define __CONNECTIONOPTIONS_HPP__

#include <QString>

class ConnectionOptions
{
    public:

        explicit ConnectionOptions(void);
        explicit ConnectionOptions(QString const& hostName,
                                   int port,
                                   QString const& databaseName,
                                   QString const& userName,
                                   QString const& password);


        virtual ~ConnectionOptions(void);

        bool isValid(void) const;

        /* Setters */
        void setHostName(QString const& hostName);
        void setPort(int port);
        void setDatabaseName(QString const& databaseName);
        void setUserName(QString const& userName);
        void setPassword(QString const& password);

        /* Getters */
        QString hostName(void) const;
        int port(void) const;
        QString databaseName(void) const;
        QString userName(void) const;
        QString password(void) const;

    protected:

        QString _hostName;
        int     _port;
        QString _databaseName;
        QString _userName;
        QString _password;
};

#endif /* __CONNECTIONOPTIONS_HPP__ */
