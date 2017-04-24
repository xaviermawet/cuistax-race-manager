//
//  ConnectionOptions.cpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 24/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "ConnectionOptions.hpp"

ConnectionOptions::ConnectionOptions(void)
{
    // Nothing to do here ...
}

ConnectionOptions::ConnectionOptions(QString const& hostName, int port,
    QString const& databaseName, QString const& userName, QString const& password) :
    _hostName(hostName), _port(port), _databaseName(databaseName), _userName(userName), _password(password)
{
    // Nothing to do here ...
}

ConnectionOptions::~ConnectionOptions(void)
{
    // Nothing to do here ...
}

bool ConnectionOptions::isValid(void) const
{
    return !this->_hostName.isEmpty() &&
           this->port() > 0 &&
           !this->_databaseName.isEmpty() &&
           !this->_userName.isEmpty() &&
           !this->_password.isEmpty();
}

void ConnectionOptions::setHostName(QString const& hostName)
{
    this->_hostName = hostName;
}

void ConnectionOptions::setPort(int port)
{
    this->_port = port;
}

void ConnectionOptions::setDatabaseName(QString const& databaseName)
{
    this->_databaseName = databaseName;
}

void ConnectionOptions::setUserName(QString const& userName)
{
    this->_userName = userName;
}

void ConnectionOptions::setPassword(QString const& password)
{
    this->_password = password;
}

QString ConnectionOptions::hostName(void) const
{
    return this->_hostName;
}

int ConnectionOptions::port(void) const
{
    return this->_port;
}

QString ConnectionOptions::databaseName(void) const
{
    return this->_databaseName;
}

QString ConnectionOptions::userName(void) const
{
    return this->_userName;
}

QString ConnectionOptions::password(void) const
{
    return this->_password;
}
