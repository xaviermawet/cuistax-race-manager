//
//  MainWindow.cpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 21/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    // GUI Configuration
    this->ui->setupUi(this);
}

MainWindow::~MainWindow(void)
{
    delete this->ui;
}
