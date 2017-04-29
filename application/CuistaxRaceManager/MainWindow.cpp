//
//  MainWindow.cpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 21/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

/* ------------------------------------------------------------------------- *
 * PUBLIC METHODS                                                            *
 * ------------------------------------------------------------------------- */

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), _stopWatch(NULL),
    _labelRaceList(NULL), _comboBoxRaceList(NULL)
{
    QCoreApplication::setOrganizationName("Nakim");
    QCoreApplication::setOrganizationDomain("nakim.be");
    QCoreApplication::setApplicationName("CuistaxRaceManager");

    // GUI Configuration (widgets)
    this->ui->setupUi(this);
    this->createToolBar();
    // TODO: createRaceTableContextMenu
    // TODO: createRankingModel

    // Restore previous MainWindows layout settings
    this->readSettings();

    // Connect to previous database if exists
    DatabaseManager::DatabaseType databaseType = DatabaseManager::DatabaseType::UNKNOWN;
    if (DatabaseManager::restorePreviousDataBase(databaseType))
    {
        this->refreshAllDatabaseModels();

        bool localDatabase = databaseType == DatabaseManager::DatabaseType::SQLite;

        // Display project file path in the main window title
        QFileInfo dbFile(QSqlDatabase::database().databaseName());
        this->setWindowTitle(tr("Cuistax Race Manager - Project: %1 ").arg(dbFile.baseName()));

        this->ui->statusBar->showMessage(
                    tr("Latest %1 project automatically loaded").arg(localDatabase ? "local" : "remote"), 4000);
    }
}

MainWindow::~MainWindow(void)
{
    // Widgets
    delete this->ui;
    delete this->_stopWatch;

    qDeleteAll(this->_spacersRaceList);
    delete this->_labelRaceList;
    delete this->_comboBoxRaceList;

    // Models
    delete this->_teamTableModel;
}

/* ------------------------------------------------------------------------- *
 * PROTECTED METHODS                                                         *
 * ------------------------------------------------------------------------- */

void MainWindow::createToolBar(void)
{
    // Mainly developed with Qt Designer

    /* ---------------------------------------------------------------------- *
     *                           Race list combobox                           *
     * ---------------------------------------------------------------------- */

    if (this->_comboBoxRaceList != NULL)
        delete this->_comboBoxRaceList;
    if (this->_labelRaceList != NULL)
        delete this->_labelRaceList;
    if (!this->_spacersRaceList.empty())
    {
        qDeleteAll(this->_spacersRaceList);
        this->_spacersRaceList.clear();
    }

    // Create combobox
    this->_comboBoxRaceList = new QComboBox(this);
    this->_comboBoxRaceList->setEditable(false);
    this->_comboBoxRaceList->setSizePolicy(QSizePolicy::Expanding,
                                           QSizePolicy::Maximum);
    // Create label
    this->_labelRaceList = new QLabel(tr("Races:"), this);

    // Create spacers
    NSpacer* labelLeadingSpacer = new NSpacer(12, 0, this);
    NSpacer* comboboxLeadingSpacer = new NSpacer(12, 0, this);
    this->_spacersRaceList.append(labelLeadingSpacer);
    this->_spacersRaceList.append(comboboxLeadingSpacer);

    // Add the label and the comboBox to the mainToolBar
    this->ui->mainToolBar->addWidget(labelLeadingSpacer);
    this->ui->mainToolBar->addWidget(this->_labelRaceList);
    this->ui->mainToolBar->addWidget(comboboxLeadingSpacer);
    this->ui->mainToolBar->addWidget(this->_comboBoxRaceList);

    // Update the current race id and update all the needed tables
    connect(this->_comboBoxRaceList, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentRaceChanged(int)));

    /* ---------------------------------------------------------------------- *
     *                                Stopwatch                               *
     * ---------------------------------------------------------------------- */
    if (this->_stopWatch != NULL)
        delete this->_stopWatch;

    // Create stopwatch
    this->_stopWatch = new NStopWatch(this);

    // Add the stopwatch to the mainToolBar
    this->ui->mainToolBar->addWidget(this->_stopWatch);
    //this->_stopWatch->setEnabled(false);

    // Informs the MainWindow that the race started
    connect(this->_stopWatch, SIGNAL(started()), this, SLOT(raceStarted()));

    // TODO : if the race changed, the stopwatch is stopped
}

void MainWindow::createTeamTabeModel(void)
{
    if (this->_teamTableModel != NULL)
        delete this->_teamTableModel;

    // Create team table model
    this->_teamTableModel = new SqlTableModelColumnsEditable(this);
    this->_teamTableModel->setTable("team");
    this->_teamTableModel->setHeaderData(0, Qt::Horizontal, tr("Cuistax number"));
    this->_teamTableModel->setHeaderData(1, Qt::Horizontal, tr("Team"));
    this->_teamTableModel->setColumnEditable(0, false);

    // Assign table model to table view
    this->ui->tableViewTeams->setModel(this->_teamTableModel);

    // Update data
    this->_teamTableModel->select();
}

void MainWindow::centerOnScreen(void)
{
    QDesktopWidget screen;

    QRect screenGeom = screen.screenGeometry(this);

    int screenCenterX = screenGeom.center().x();
    int screenCenterY = screenGeom.center().y();

    this->move(screenCenterX - width () / 2, screenCenterY - height() / 2);
}

void MainWindow::readSettings(void)
{
    // Restore MainWindow settings
    this->readLayoutSettings();

    // Other settings to restore ...
}

void MainWindow::writeSettings(void) const
{
    // Save MainWindow settings
    this->writeLayoutSettings();

    // Other settings to save ...
}

void MainWindow::readLayoutSettings(void)
{
    QSettings settings;

    settings.beginGroup(SETTINGS_GROUP_MAINWINDOW_LAYOUT);

    this->restoreGeometry(settings.value("geometry").toByteArray());
    this->ui->splitterMain->restoreState(
                settings.value("splitterMain").toByteArray());

    // Other MainWindow Settings ...

    settings.endGroup();
}

void MainWindow::writeLayoutSettings(void) const
{
    QSettings settings;

    settings.beginGroup(SETTINGS_GROUP_MAINWINDOW_LAYOUT);

    //settings.setValue("isMaximized",this->isMaximized());
    settings.setValue("geometry", this->saveGeometry());
    settings.setValue("splitterMain", this->ui->splitterMain->saveState());

    // Other MainWindow Settings ...

    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // Save the state of the MainWindow and its widgets
    this->writeSettings();

    QMainWindow::closeEvent(event);
}

void MainWindow::refreshAllDatabaseModels(void)
{
    // Create all sql models base on batabase's table(s)
    this->createTeamTabeModel();

    // TODO : Manage other models
}

void MainWindow::on_actionNewLocalProject_triggered(void)
{
    // Get new project file path
    QString dbFilePath = QFileDialog::getSaveFileName(
                this, tr("Create new local project"), QDir::homePath(),
                tr("Projet Magnee Cuistax (*.db)"));

    // User canceled (nothing to do)
    if (dbFilePath.isEmpty())
        return;

    try
    {
        if (!DatabaseManager::createLocalDatabase(dbFilePath))
            throw NException("Local database not created");

        this->statusBar()->showMessage(tr("Local project successfully created"), 4000);
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Error when creating local project"),
                             exception.message());
    }
}

void MainWindow::on_actionNewRemoteProject_triggered(void)
{
    DialogRemoteServerInformation dialog;

    // Open settings dialog
    if (dialog.exec() != QDialog::Accepted) // User canceled
        return;

    try
    {
        // Check remote server credentials
        if (dialog.hostName().isEmpty())
            throw NException("Invalid host name");
        if (dialog.port() < 0)
            throw NException("Invalid port");
        if (dialog.databaseName().isEmpty())
            throw NException("Invalid database name");
        if (dialog.userName().isEmpty())
            throw NException("Invalid user name");
        if (dialog.password().isEmpty())
            throw NException("Invalid password");

        // Create remote database
        ConnectionOptions connectionOptions;
        connectionOptions.setHostName(dialog.hostName());
        connectionOptions.setPort(dialog.port());
        connectionOptions.setDatabaseName(dialog.databaseName());
        connectionOptions.setUserName(dialog.userName());
        connectionOptions.setPassword(dialog.password());

        if (!DatabaseManager::createRemoteDatabase(connectionOptions))
            throw NException("Remote database not created");

        this->statusBar()->showMessage(tr("Remote project successfully created"), 4000);
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Error when creating remote project"),
                             exception.message());
    }
}

void MainWindow::on_actionOpenLocalProject_triggered(void)
{
    // Get existing project file path
    QString dbFilePath = QFileDialog::getOpenFileName(
                this, tr("Open local project"), QDir::homePath(),
                tr("Projet Magnee Cuistax (*.db)"));

    // User canceled (nothing to do)
    if (dbFilePath.isEmpty())
        return;

    try
    {
        // Open the database
        if (!DatabaseManager::openExistingLocalDatabase(dbFilePath))
            throw NException("Make sure the project file exists");

        this->refreshAllDatabaseModels();

        this->statusBar()->showMessage(
                    tr("Local project successfully opened"), 4000);
    }
    catch (NException const& exception)
    {
        QMessageBox::warning(this, tr("Error when openning local project"),
                             exception.message());
    }
}

void MainWindow::on_actionOpenRemoteProject_triggered(void)
{
    DialogRemoteServerInformation dialog;

    // Open settings dialog
    if (dialog.exec() != QDialog::Accepted) // User canceled
        return;

    try
    {
        // Check remote server credentials
        if (dialog.hostName().isEmpty())
            throw NException("Invalid host name");
        if (dialog.port() < 0)
            throw NException("Invalid port");
        if (dialog.databaseName().isEmpty())
            throw NException("Invalid database name");
        if (dialog.userName().isEmpty())
            throw NException("Invalid user name");
        if (dialog.password().isEmpty())
            throw NException("Invalid password");

        // Create remote database
        ConnectionOptions connectionOptions;
        connectionOptions.setHostName(dialog.hostName());
        connectionOptions.setPort(dialog.port());
        connectionOptions.setDatabaseName(dialog.databaseName());
        connectionOptions.setUserName(dialog.userName());
        connectionOptions.setPassword(dialog.password());

        if (!DatabaseManager::openExistingRemoteDatabase(connectionOptions))
            throw NException("Remote database not opened");

        this->statusBar()->showMessage(tr("Remote project successfully opened"), 4000);
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Error when creating remote project"),
                             exception.message());
    }
}

void MainWindow::on_actionHelpProject_triggered(void)
{
    // TODO
}

void MainWindow::on_actionCreateTeam_triggered(void)
{
    DialogCreateTeam dial;

    if (dial.exec() != QDialog::Accepted) // User canceled
        return;

    // Create the insertion query
    QSqlQuery insertQuery("INSERT INTO team (cuistax, name) values (?, ?)");
    insertQuery.addBindValue(dial.cuistaxNumber());
    insertQuery.addBindValue(dial.teamName());

    try
    {
        // Insert new team in database
        DatabaseManager::execQuery(insertQuery);
        this->statusBar()->showMessage(
                    tr("Team \"%1\" created").arg(dial.teamName()), 4000);

        // If the race has already started
        if (this->_stopWatch->isActive() || this->_stopWatch->isInPause())
        {
            // TOOD: this->_previousLapsInformation[dial.cuistaxNumber()] = Lap();
        }

        // TODO : refresh other ???

        // Refresh team list (table)
        this->_teamTableModel->select();
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(
                    this, tr("Unable to create team %1").arg(dial.teamName()),
                    exception.message());
    }
}

void MainWindow::raceStarted(void)
{
    // TODO: Delete all information about "previous laps"

    // TODO : create lap object for each team that will save the last lap information
}

void MainWindow::currentRaceChanged(int currentRaceIndex)
{
    // TODO
}
