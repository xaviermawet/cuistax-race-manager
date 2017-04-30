//
//  DialogCreateRace.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 29/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#ifndef __DIALOGCREATERACE_HPP__
#define __DIALOGCREATERACE_HPP__

#include <QtWidgets>
#include <QtConcurrent>
#include <QMutex>
#include <QStringList>

#include "Database/DatabaseManager.hpp"
#include "Utils/NException.hpp"

namespace Ui {
    class DialogCreateRace;
}

class DialogCreateRace : public QDialog
{
        Q_OBJECT

    public:

        /*!
         * \brief DialogCreateRace: Default constructor
         * \param parent: Pointer to a parent widget if exists
         */
        explicit DialogCreateRace(QWidget* parent = 0);

        /*!
         * \brief ~DialogCreateRace: Virtual destructor
         */
        virtual ~DialogCreateRace(void);

        /*!
         * \brief raceName: Get the race's name
         * \return The race's name
         */
        QString raceName(void) const;

        /*!
         * \brief raceDate: Get the race's date
         * \return The race's date
         */
        QDate   raceDate(void) const;

        /*!
         * \brief racePlace: Get the race's location
         * \return The race's location
         */
        QString raceLocation(void) const;

        /*!
         * \brief raceDistance: Get the race's length
         * \return The race's length
         */
        double  raceLength(void) const;

    protected:

        /*!
         * \brief updateSaveButtonState: Hide or show the save button depending
         * on the team name and the cuistax number validity
         */
        void updateSaveButtonVisibility(void);

        /*!
         * \brief enableFields: Enable or disable form fields
         * \param enabled: enable state
         */
        void enableFields(bool enabled);

    private slots:

        // Autoconnect
        void on_lineEditName_textEdited(QString const& raceName);
        void on_lineEditLocation_textEdited(QString const& raceLocation);
        void on_doubleSpinBoxLength_valueChanged(double length);

    private:

        void readDataFromDatabase(void);

    private:

        Ui::DialogCreateRace* ui;

        QRegExp     _regex;
        QMutex      _mutexExistingRaceNames;
        QStringList _existingRaceNames;
        bool        _validName;
        bool        _validLocation;
        bool        _validLength;
};

#endif /* __DIALOGCREATERACE_HPP__ */
