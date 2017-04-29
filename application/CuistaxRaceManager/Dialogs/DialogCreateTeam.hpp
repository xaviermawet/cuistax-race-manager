//
//  DialogCreateTeam.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 28/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#ifndef __DIALOGCREATETEAM_HPP__
#define __DIALOGCREATETEAM_HPP__

#include <QDialog>
#include <QtConcurrent>
#include <QMutex>
#include "Database/DatabaseManager.hpp"
#include "Utils/NException.hpp"

namespace Ui {
    class DialogCreateTeam;
}

class DialogCreateTeam : public QDialog
{
        Q_OBJECT

    public:

        /*!
         * \brief DialogCreateTeam: Default constructor
         * \param parent: Pointer to a parent widget if exists
         */
        explicit DialogCreateTeam(QWidget* parent = 0);

        /*!
         * \brief ~DialogCreateTeam: Virtual destructor
         */
        virtual ~DialogCreateTeam(void);

        /*!
         * \brief teamName: Get the team name
         * \return The team name
         */
        QString teamName(void) const;

        /*!
         * \brief cuistaxNumber: Get the cuistax number
         * \return The cuistax number
         */
        int cuistaxNumber(void) const;

    protected:

        /*!
         * \brief updateSaveButtonState: Hide or show the save button depending
         * on the team name and the cuistax number validity
         */
        void updateSaveButtonVisibility(void);

    private:

        /*!
         * \brief readDataFromDatabase: Get the list of all existing team names
         * and cuistax number
         */
        void readDataFromDatabase(void);

    private slots:

        // Auto-connect
        void on_lineEditTeamName_textEdited(QString const& teamName);
        void on_spinBoxCuistaxNumber_valueChanged(int cuistaxNumber);

    private:

        Ui::DialogCreateTeam*   ui;
        QRegExp                 _regex;
        bool                    _teamNameValid;
        bool                    _cuistaxNumberValid;

        QMutex                  _mutexExistingTeamData;
        QStringList             _existingTeamNames;
        QList<int>              _existingCuistaxNumbers;
};

#endif /* __DIALOGCREATETEAM_HPP__ */
