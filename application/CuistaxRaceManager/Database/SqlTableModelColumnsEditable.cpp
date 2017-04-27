//
//  SqlTableModelColumnsEditable.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 26/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "SqlTableModelColumnsEditable.hpp"

SqlTableModelColumnsEditable::SqlTableModelColumnsEditable(QObject* parent, QSqlDatabase db) :
    QSqlTableModel(parent, db), _notEditableColumns()
{
    // Nothing to do here
}

void SqlTableModelColumnsEditable::setColumnEditable(int columnIndex, bool editable)
{
    if (editable)
    {
        this->_notEditableColumns.removeOne(columnIndex);
    }
    else
    {
        if (!this->_notEditableColumns.contains(columnIndex))
            this->_notEditableColumns.append(columnIndex);
    }
}

bool SqlTableModelColumnsEditable::isColumnEditable(int columnIndex) const
{
    return this->_notEditableColumns.contains(columnIndex);
}

void SqlTableModelColumnsEditable::resetColumnEditable(void)
{
    this->_notEditableColumns.clear();
}

Qt::ItemFlags SqlTableModelColumnsEditable::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QSqlTableModel::flags(index);
    if (this->_notEditableColumns.contains(index.column()))
        flags &= ~Qt::ItemIsEditable;
    return flags;
}
