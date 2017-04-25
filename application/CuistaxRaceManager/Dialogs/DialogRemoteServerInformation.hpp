//
//  DialogRemoteServerInformation.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 24/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#ifndef __DIALOGREMOTESERVERINFORMATION_HPP__
#define __DIALOGREMOTESERVERINFORMATION_HPP__

#include <QtWidgets>

#define SETTINGS_GROUP_REMOTE_DATABASE_CONNECTION "Remote_Database_Connection"

namespace Ui {
    class DialogRemoteServerInformation;
}

class DialogRemoteServerInformation : public QDialog
{
        Q_OBJECT

    public:

        /*!
         * \brief DialogRemoteServerInformation: Default constructor
         * \param parent: Pointer to a parent widget if exists
         */
        explicit DialogRemoteServerInformation(QWidget* parent = 0);

        /*!
         * \brief ~DialogRemoteServerInformation: Virtual destructor
         */
        virtual ~DialogRemoteServerInformation(void);

        /* setters */
        void setHostName(QString const& hostName);
        void setPort(int port);
        void setDatabaseName(QString const& databaseName);
        void setUserName(QString const& userName);
        void setPassword(QString const& password);

        /* Getters */

        QString hostName(void) const;
        int port(void) const;
        QString databaseName(void) const;
        QString userName(void) const;
        QString password(void) const;

    private:

        void saveSettings(void) const;
        void closeEvent(QCloseEvent* event);

    private slots:

        void on_buttonBox_clicked(QAbstractButton *button);

    private:

        Ui::DialogRemoteServerInformation* ui;
};

#endif /* __DIALOGREMOTESERVERINFORMATION_HPP__ */
