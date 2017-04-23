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

    settings.beginGroup(QSETTINGS_GROUP_MAINWINDOW_LAYOUT);

    this->restoreGeometry(settings.value("geometry").toByteArray());

    // Other MainWindow layout settings ...

    settings.endGroup();
}

void MainWindow::writeLayoutSettings(void) const
{
    QSettings settings;

    settings.beginGroup(QSETTINGS_GROUP_MAINWINDOW_LAYOUT);

    //settings.setValue("isMaximized",this->isMaximized());
    settings.setValue("geometry", this->saveGeometry());

    // Other MainWindow layout settings ...

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
    {
        return;
    }

    try
    {
        throw NException("Not implemented yet");
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Error when creating local project"),
                             exception.message());
    }
}

void MainWindow::on_actionNewRemoteProject_triggered(void)
{

}

void MainWindow::on_actionOpenLocalProject_triggered(void)
{
    // Get existing project file path
    QString dbFilePath = QFileDialog::getOpenFileName(
                this, tr("Open local project"), QDir::homePath(),
                tr("Projet Magnee Cuistax (*.db)"));

    // User canceled (nothing to do)
    if (dbFilePath.isEmpty())
    {
        return;
    }

    try
    {
        throw NException("Not implemented yet");
    }
    catch (NException const& exception)
    {
        QMessageBox::warning(this, tr("Error when openning local project"),
                             exception.message());
    }
}

void MainWindow::on_actionOpenRemoteProject_triggered(void)
{

}

void MainWindow::on_actionHelpProject_triggered(void)
{

}
