//
//  NSqlQueryModel.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 30/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#ifndef __NSQLQUERYMODEL_HPP__
#define __NSQLQUERYMODEL_HPP__

#include <QSqlQueryModel>
#include <QSqlQuery>

class NSqlQueryModel : public QSqlQueryModel
{
    public:

        explicit NSqlQueryModel(QObject* parent = 0);

        void refresh(void);
};

#endif /* __NSQLQUERYMODEL_HPP__ */
