#-------------------------------------------------
#
# Project created by QtCreator 2017-04-21T00:47:35
#
#-------------------------------------------------

QT       += core gui sql concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CuistaxRaceManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        MainWindow.cpp \
    Utils/NException.cpp \
    Database/DatabaseManager.cpp \
    Dialogs/DialogRemoteServerInformation.cpp \
    Database/ConnectionOptions.cpp \
    Database/SqlTableModelColumnsEditable.cpp \
    Dialogs/DialogCreateTeam.cpp \
    Widgets/NStopWatch.cpp \
    Widgets/NSpacer.cpp \
    Dialogs/DialogCreateRace.cpp \
    Database/NSqlQueryModel.cpp

HEADERS  += MainWindow.hpp \
    Utils/NException.hpp \
    Database/DatabaseManager.hpp \
    Dialogs/DialogRemoteServerInformation.hpp \
    Database/ConnectionOptions.hpp \
    Database/SqlTableModelColumnsEditable.hpp \
    Dialogs/DialogCreateTeam.hpp \
    Widgets/NStopWatch.hpp \
    Widgets/NSpacer.hpp \
    Dialogs/DialogCreateRace.hpp \
    Database/NSqlQueryModel.hpp

FORMS    += MainWindow.ui \
    Dialogs/DialogRemoteServerInformation.ui \
    Dialogs/DialogCreateTeam.ui \
    Widgets/NStopWatch.ui \
    Dialogs/DialogCreateRace.ui

RESOURCES += \
    Resources.qrc
