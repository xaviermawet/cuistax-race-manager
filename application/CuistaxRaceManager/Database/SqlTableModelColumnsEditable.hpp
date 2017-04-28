//
//  SqlTableModelColumnsEditable.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 26/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#ifndef __SQLTABLEMODELCOLUMNSEDITABLE_HPP__
#define __SQLTABLEMODELCOLUMNSEDITABLE_HPP__

#include <QSqlTableModel>
#include <QList>

class SqlTableModelColumnsEditable : public QSqlTableModel
{
    public:

        explicit SqlTableModelColumnsEditable(
                QObject* parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());

        void setColumnEditable(int columnIndex, bool editable);
        bool isColumnEditable(int columnIndex) const;
        void resetColumnEditable(void);

        virtual Qt::ItemFlags flags(const QModelIndex& index) const;

    protected:

        QList<int> _notEditableColumns;
};

#endif /* __SQLTABLEMODELCOLUMNSEDITABLE_HPP__ */
