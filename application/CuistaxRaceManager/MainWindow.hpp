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

    private:

        // Widgets
        Ui::MainWindow *ui;
};

#endif /* __MAINWINDOW_HPP__ */
