//
//  DialogCreateTeam.cpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 28/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "DialogCreateTeam.hpp"
#include "ui_DialogCreateTeam.h"

DialogCreateTeam::DialogCreateTeam(QWidget* parent) :
    QDialog(parent), ui(new Ui::DialogCreateTeam),
    _regex("^[a-zA-Z0-9àáâãäåçèéêëìíîïðòóôõöùúûüýÿ ]{3,80}$"),
    _teamNameValid(false), _cuistaxNumberValid(false)
{
    // GUI Configuration
    this->ui->setupUi(this);

    // TODO : threading get data
    QtConcurrent::run(this, &DialogCreateTeam::readDataFromDatabase);
}

DialogCreateTeam::~DialogCreateTeam(void)
{
    delete this->ui;
}

QString DialogCreateTeam::teamName(void) const
{
    return this->ui->lineEditTeamName->text();
}

int DialogCreateTeam::cuistaxNumber(void) const
{
    return this->ui->spinBoxCuistaxNumber->value();
}

void DialogCreateTeam::updateSaveButtonVisibility(void)
{
    this->ui->buttonBox->setStandardButtons(
        (this->_teamNameValid && this->_cuistaxNumberValid) ?
            QDialogButtonBox::Cancel | QDialogButtonBox::Save :
            QDialogButtonBox::Cancel);
}

void DialogCreateTeam::readDataFromDatabase(void)
{
    // Clear databse data
    this->_existingTeamNames.clear();
    this->_existingCuistaxNumbers.clear();

    QSqlQuery query = DatabaseManager::execQuery("SELECT cuistax, name FROM team");

    // Critical section
    QMutexLocker locker(&this->_mutexExistingTeamData); // Unlock at dispose

    while (query.next()) {
        this->_existingCuistaxNumbers.append(query.value(0).toInt());
        this->_existingTeamNames.append(query.value(1).toString());
    }

    qDebug() << "[DialogCreateTeam] readDataFromDatabase DONE";
}

void DialogCreateTeam::on_lineEditTeamName_textEdited(QString const& teamName)
{
    try
    {
        // Check if the team name match the regex
        if (!this->_regex.exactMatch(teamName))
            throw NException(tr("Min 3 characters. Only letters and numbers"));

        // Check if a team with the same name already exists
        QMutexLocker locker(&this->_mutexExistingTeamData); // Unlock at dispose
        if (this->_existingTeamNames.contains(teamName, Qt::CaseInsensitive))
            throw NException(tr("A team with the same name already exists"));

        // Team name is valid
        this->_teamNameValid = true;
        this->ui->labelTeamNameMessage->setText(tr("Team name available"));
    }
    catch(NException const& exception)
    {
        this->_teamNameValid = false;
        this->ui->labelTeamNameMessage->setText(exception.message());
    }

    // Update message text color
    this->ui->labelTeamNameMessage->setStyleSheet(
                QString("QLabel { color : %1; }")
                    .arg(this->_teamNameValid ? "green" : "red"));

    // Update buttons visibility
    this->updateSaveButtonVisibility();
}

void DialogCreateTeam::on_spinBoxCuistaxNumber_valueChanged(int cuistaxNumber)
{
    try
    {
        // Check if a team with the same cuistax number already exists
        QMutexLocker locker(&this->_mutexExistingTeamData);
        if (this->_existingCuistaxNumbers.contains(cuistaxNumber))
            throw NException(tr("A team with the same cuistax number already exists"));

        // Cuistax number is valid
        this->_cuistaxNumberValid = true;
        this->ui->labelCuistaxNumberMessage->setText(tr("Cuistax number available"));
    }
    catch(NException const& exception)
    {
        this->_cuistaxNumberValid = false;
        this->ui->labelCuistaxNumberMessage->setText(exception.message());
    }

    // Update message text color
    this->ui->labelCuistaxNumberMessage->setStyleSheet(
                QString("QLabel { color : %1; }")
                    .arg(this->_cuistaxNumberValid ? "green" : "red"));

    // Update buttons visibility
    this->updateSaveButtonVisibility();
}
