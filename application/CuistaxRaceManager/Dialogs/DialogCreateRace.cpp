//
//  DialogCreateRace.cpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 29/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "DialogCreateRace.hpp"
#include "ui_DialogCreateRace.h"

DialogCreateRace::DialogCreateRace(QWidget* parent) :
    QDialog(parent), ui(new Ui::DialogCreateRace),
    _regex("^[a-zA-Z0-9àáâãäåçèéêëìíîïðòóôõöùúûüýÿ ]{3,80}$"),
    _validName(false), _validLocation(false), _validLength(false)
{
    // GUI Configuration
    this->ui->setupUi(this);

    this->enableFields(false);

    // Custon calendar
    this->ui->calendarWidgetDate->setMinimumDate(QDate::currentDate());

    // Get data in an other thread
    //QtConcurrent::run(this, &DialogCreateRace::readDataFromDatabase);
    readDataFromDatabase();
}

DialogCreateRace::~DialogCreateRace(void)
{
    delete this->ui;
}

QString DialogCreateRace::raceName(void) const
{
    return this->ui->lineEditName->text();
}

QDate DialogCreateRace::raceDate(void) const
{
    return this->ui->calendarWidgetDate->selectedDate();
}

QString DialogCreateRace::raceLocation(void) const
{
    return this->ui->lineEditLocation->text();
}

double DialogCreateRace::raceLength(void) const
{
    return this->ui->doubleSpinBoxLength->value();
}

void DialogCreateRace::updateSaveButtonVisibility(void)
{
    this->ui->buttonBox->setStandardButtons(
        (this->_validName && this->_validLocation && this->_validLength) ?
            QDialogButtonBox::Cancel | QDialogButtonBox::Save :
            QDialogButtonBox::Cancel);
}

void DialogCreateRace::enableFields(bool enabled)
{
    // Race's name
    this->ui->labelName->setEnabled(enabled);
    this->ui->lineEditName->setEnabled(enabled);
    this->ui->labelNameMessage->setEnabled(enabled);

    // Race's location
    this->ui->labelLocation->setEnabled(enabled);
    this->ui->lineEditLocation->setEnabled(enabled);
    this->ui->labelLocationMessage->setEnabled(enabled);

    // Race's length
    this->ui->labelLength->setEnabled(enabled);
    this->ui->doubleSpinBoxLength->setEnabled(enabled);

    // Race's date
    this->ui->labelDate->setEnabled(enabled);
    this->ui->calendarWidgetDate->setEnabled(enabled);
}

void DialogCreateRace::on_lineEditName_textEdited(QString const& raceName)
{
    try
    {
        // Check if the race name match the regex
        if(!this->_regex.exactMatch(raceName))
            throw NException(tr("Min 3 characters. Only letters and numbers"));

        // Checks if a race with the same name already exists
        QMutexLocker locker(&this->_mutexExistingRaceNames); // Unlock at dispose
        if (this->_existingRaceNames.contains(raceName, Qt::CaseInsensitive))
            throw NException(tr("A race with the same name already exists"));

        // Team name is valid
        this->_validName = true;
        this->ui->labelNameMessage->setText(tr("Name available"));
    }
    catch(NException const& exception)
    {
        this->_validName = false;
        this->ui->labelNameMessage->setText(exception.message());
    }

    // Update message text color
    this->ui->labelNameMessage->setStyleSheet(
                QString("QLabel { color : %1; }")
                    .arg(this->_validName ? "green" : "red"));

    this->updateSaveButtonVisibility();
}

void DialogCreateRace::on_lineEditLocation_textEdited(QString const& raceLocation)
{
    // Regex match
    this->_validLocation = this->_regex.exactMatch(raceLocation);

    // Update message depending on location validity
    this->ui->labelLocationMessage->setText(
        this->_validLocation ? tr("Valid location") :
                               tr("Min 3 characters. Only letters and numbers"));
    // Update message text color
    this->ui->labelLocationMessage->setStyleSheet(
                QString("QLabel { color : %1; }")
                    .arg(this->_validLocation ? "green" : "red"));

    this->updateSaveButtonVisibility();
}

void DialogCreateRace::on_doubleSpinBoxLength_valueChanged(double length)
{
    this->_validLength = length > 0.0;
    this->updateSaveButtonVisibility();
}

void DialogCreateRace::readDataFromDatabase(void)
{
    // Clear database data
    this->_existingRaceNames.clear();

    QSqlQuery query = DatabaseManager::execQuery("SELECT name FROM race");

    // Critical section
    QMutexLocker locker(&this->_mutexExistingRaceNames); // Unlock at dispose

    // Extract all existing race names
    while (query.next()) {
        this->_existingRaceNames.append(query.value(0).toString());
    }

    this->enableFields(true);
}

