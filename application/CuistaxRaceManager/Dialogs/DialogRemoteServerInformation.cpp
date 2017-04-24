//
//  DialogRemoteServerInformation.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 24/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "DialogRemoteServerInformation.hpp"
#include "ui_DialogRemoteServerInformation.h"

DialogRemoteServerInformation::DialogRemoteServerInformation(QWidget* parent) :
    QDialog(parent), ui(new Ui::DialogRemoteServerInformation)
{
    // GUI Configuration
    this->ui->setupUi(this);
}

DialogRemoteServerInformation::~DialogRemoteServerInformation(void)
{
    delete this->ui;
}
void DialogRemoteServerInformation::setHostName(QString const& hostName)
{
    this->ui->lineEditHostName->setText(hostName);
}

void DialogRemoteServerInformation::setPort(int port)
{
    this->ui->spinBoxPort->setValue(port);
}

void DialogRemoteServerInformation::setDatabaseName(QString const& databaseName)
{
    this->ui->lineEditProjectName->setText(databaseName);
}

void DialogRemoteServerInformation::setUserName(QString const& userName)
{
    this->ui->lineEditUserName->setText(userName);
}

void DialogRemoteServerInformation::setPassword(QString const& password)
{
    this->ui->lineEditPassword->setText(password);
}

QString DialogRemoteServerInformation::hostName(void) const
{
    return this->ui->lineEditHostName->text();
}

int DialogRemoteServerInformation::port(void) const
{
    return this->ui->spinBoxPort->value();
}

QString DialogRemoteServerInformation::databaseName(void) const
{
    return this->ui->lineEditProjectName->text();
}

QString DialogRemoteServerInformation::userName(void) const
{
    return this->ui->lineEditUserName->text();
}

QString DialogRemoteServerInformation::password(void) const
{
    return this->ui->lineEditPassword->text();
}
