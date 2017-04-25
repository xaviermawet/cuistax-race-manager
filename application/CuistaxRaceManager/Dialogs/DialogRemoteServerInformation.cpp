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

    // Read settings
    QSettings settings;
    settings.beginGroup(SETTINGS_GROUP_REMOTE_DATABASE_CONNECTION);

    this->setHostName(settings.value("hostname", QString()).toString());
    this->setPort(settings.value("port", 0).toInt());
    this->setDatabaseName(settings.value("databasename", QString()).toString());
    this->setUserName(settings.value("username", QString()).toString());
    this->setPassword(settings.value("password", QString()).toString());

    settings.endGroup();

    // Check save checkbox if at lease one setting exists
    this->ui->checkBoxSaveCredentials->setChecked(
                !this->hostName().isEmpty() ||
                !(this->port() == 0) ||
                !this->databaseName().isEmpty() ||
                !this->userName().isEmpty() ||
                !this->password().isEmpty()
               );
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

void DialogRemoteServerInformation::saveSettings(void) const
{
    QSettings settings;
    settings.beginGroup(SETTINGS_GROUP_REMOTE_DATABASE_CONNECTION);

    // Save settings
    if (this->ui->checkBoxSaveCredentials->isChecked())
    {
        settings.setValue("hostname", this->hostName());
        settings.setValue("port", this->port());
        settings.setValue("databasename", this->databaseName());
        settings.setValue("username", this->userName());
        settings.setValue("password", this->password());
    }
    // Delete settings
    else
    {
        // Removes the group, and all it keys
        settings.remove("");
    }

    settings.endGroup();
}

void DialogRemoteServerInformation::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event)

    this->saveSettings();
}

void DialogRemoteServerInformation::on_buttonBox_clicked(QAbstractButton* button)
{
    Q_UNUSED(button)

    this->saveSettings();
}
