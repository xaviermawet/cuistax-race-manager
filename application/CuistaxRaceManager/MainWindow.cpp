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
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("Nakim");
    QCoreApplication::setOrganizationDomain("nakim.be");
    QCoreApplication::setApplicationName("CuistaxRaceManager");

    // GUI Configuration
    this->ui->setupUi(this);

    // Restore previous MainWindows layout settings
    this->readSettings();
}

MainWindow::~MainWindow(void)
{
    delete this->ui;
}

/* ------------------------------------------------------------------------- *
 * PROTECTED METHODS                                                         *
 * ------------------------------------------------------------------------- */

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

}
