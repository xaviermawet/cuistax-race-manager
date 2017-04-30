//
//  MainWindow.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 21/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

// Header file that contains the definition of all classes in the Qt Core,
// Qt GUI and Qt Widgets modules
#include <QtWidgets>

// Utils
#include "Utils/NException.hpp"

// Database
#include "Database/DatabaseManager.hpp"
#include "Database/SqlTableModelColumnsEditable.hpp"

// Dialogs
#include "Dialogs/DialogRemoteServerInformation.hpp"
#include "Dialogs/DialogCreateTeam.hpp"
#include "Dialogs/DialogCreateRace.hpp"
#include "Dialogs/DialogTest.hpp"

// Widgete
#include "Widgets/NStopWatch.hpp"
#include "Widgets/NSpacer.hpp"

#define SETTINGS_GROUP_MAINWINDOW_LAYOUT "MainWindow_Layout"
#define SETTINGS_GROUP_REMOTE_CONNECTION "Remote_Server_Connection"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:

        /*!
         * \brief MainWindow: default constructor
         * \param parent: pointer to a parent widget if exists
         */
        explicit MainWindow(QWidget* parent = 0);

        /*!
         * \brief ~MainWindow: virtual destructor
         */
        virtual ~MainWindow(void);

    protected:

        /*!
         * \brief createToolBar : create all the widgets for the mainToolBar
         */
        void createToolBar(void);

        /*!
         * \brief createTeamListModel : create a list model based on table TEAM
         */
        void createTeamTabeModel(void);

        /*!
         * \brief centerOnScreen: center the MainWindow
         */
        void centerOnScreen(void);

        /*!
         * \brief readSettings: restore all previous MainWindow settings
         */
        void readSettings(void);

        /*!
         * \brief writeSettings: save all current MainWindow settings
         */
        void writeSettings(void) const;

        /*!
         * \brief readLayoutSettings: restore previous MainWindow layout settings
         */
        void readLayoutSettings(void);

        /*!
         * \brief writeLayoutSettings: save current MainWindow layout settings
         */
        void writeLayoutSettings(void) const;

        /*!
         * \brief closeEvent: close event handler. Overloaded method for writing settings
         * \param event: the close event
         */
        virtual void closeEvent(QCloseEvent* event);

        /*!
         * \brief refreshAllDatabaseModels: Deletes and creates all models that depend on database
         */
        void refreshAllDatabaseModels(void);

    private slots:

        // File menu
        void on_actionNewLocalProject_triggered(void);
        void on_actionNewRemoteProject_triggered(void);
        void on_actionOpenLocalProject_triggered(void);
        void on_actionOpenRemoteProject_triggered(void);
        void on_actionHelpProject_triggered(void);

        // Team management
        void on_actionCreateTeam_triggered(void);
        void on_actionCreateRace_triggered(void);

        // Race Management
        void raceStarted(void);
        void currentRaceChanged(int currentRaceIndex);

    private:

        // Widgets
        Ui::MainWindow  *ui;
        NStopWatch*     _stopWatch;
        QList<NSpacer*> _spacersRaceList;
        QLabel*         _labelRaceList;
        QComboBox*      _comboBoxRaceList;

        // Models
        SqlTableModelColumnsEditable* _teamTableModel;
};

#endif /* __MAINWINDOW_HPP__ */
