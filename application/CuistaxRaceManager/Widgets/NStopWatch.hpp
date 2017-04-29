//
//  NStopWatch.cpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 29/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#ifndef __NSTOPWATCH_HPP__
#define __NSTOPWATCH_HPP__

#include <QtWidgets>

namespace Ui {
    class NStopWatch;
}

class NStopWatch : public QWidget
{
        Q_OBJECT

    public:

        explicit NStopWatch(QWidget* parent = 0);
        virtual ~NStopWatch(void);

        QTime elapsedTime(void) const;

        bool isActive(void) const;
        bool isInPause(void) const;

        void setButtonsVisible(bool visible);
        bool areButtonsVisible(void) const;

    public slots:

        void start(void);
        void stop(void);
        void reset(void);

    private slots:

        void refreshWatch(void);

    signals:

        void started(void);
        void stopped(QTime const& time);
        void reseted(void);

    private:

        Ui::NStopWatch* ui;

        bool    _pause;
        QTime   _watchTime;
        QTime   _startingPauseTime;
        QTimer* _refreshTimer;
};

#endif /* __NSTOPWATCH_HPP__ */
