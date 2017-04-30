//
//  NSqlQueryModel.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 30/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "NSqlQueryModel.hpp"

NSqlQueryModel::NSqlQueryModel(QObject* parent) :
    QSqlQueryModel(parent)
{
    // Nothing to do here
}

void NSqlQueryModel::refresh(void)
{
    this->setQuery(this->query().executedQuery());

    QModelIndex topLeft = this->index(0, 0);
    QModelIndex bottomRight = this->index(
                this->rowCount() - 1, this->columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);
}
