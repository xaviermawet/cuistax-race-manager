//
//  NStopWatch.cpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 29/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "NStopWatch.hpp"
#include "ui_NStopWatch.h"

NStopWatch::NStopWatch(QWidget* parent) :
    QWidget(parent), ui(new Ui::NStopWatch)
{
    // GUI Configuration
    this->ui->setupUi(this);

    // Timer configuration
    this->_refreshTimer = new QTimer(this);
    this->_refreshTimer->setInterval(1000);
    connect(this->_refreshTimer, SIGNAL(timeout()), this, SLOT(refreshWatch()));
}

NStopWatch::~NStopWatch(void)
{
    delete this->ui;
    delete this->_refreshTimer;
}

QTime NStopWatch::elapsedTime(void) const
{
    if(this->_pause)
        return QTime(0, 0).addMSecs(this->_watchTime.msecsTo(this->_startingPauseTime));
    else
        return QTime(0, 0, 0, 0).addMSecs(this->_watchTime.elapsed());
}

bool NStopWatch::isActive(void) const
{
    return this->_refreshTimer->isActive();
}

bool NStopWatch::isInPause(void) const
{
    return this->_pause;
}

void NStopWatch::setButtonsVisible(bool visible)
{
    this->ui->pushButtonStart->setVisible(visible);
    this->ui->pushButtonStop->setVisible(visible);
    this->ui->pushButtonReset->setVisible(visible);
}

bool NStopWatch::areButtonsVisible(void) const
{
    return this->ui->pushButtonStart->isVisible();
}

void NStopWatch::start(void)
{
    if (!this->_refreshTimer->isActive())
        this->_refreshTimer->start();

    // Save starting time or add pause time to watch time
    if (this->_pause)
        this->_watchTime =
            this->_watchTime.addMSecs(this->_startingPauseTime.restart());
    else
    {
        this->_watchTime.start();
        emit this->started();
    }

    this->_pause = false;
    this->refreshWatch();

    this->ui->pushButtonStart->setText(tr("Restart"));
    this->ui->pushButtonStart->setIcon(QIcon(":/icons/skip-to-start"));
}

void NStopWatch::stop(void)
{
    // Nothing to do if already stopped
    if (!this->_refreshTimer->isActive())
        return;

    // Save time at the beginning of the pause
    this->_startingPauseTime.start();
    this->_pause = true;
    this->_refreshTimer->stop();

    this->ui->pushButtonStart->setText(tr("Continue"));
    this->ui->pushButtonStart->setIcon(QIcon(":/icons/resume"));

    emit this->stopped(this->_startingPauseTime);
}

void NStopWatch::reset(void)
{
    this->_pause = false;
    this->_refreshTimer->stop();
    this->ui->timeEdit->setTime(QTime(0, 0));

    this->_watchTime = QTime();

    this->ui->pushButtonStart->setText(tr("Start"));
    this->ui->pushButtonStart->setIcon(QIcon(":/icons/play"));

    emit this->reseted();
}

void NStopWatch::refreshWatch(void)
{
    this->ui->timeEdit->setTime(
                QTime(0, 0).addMSecs(this->_watchTime.elapsed()));
}
