#ifndef NETLIST_CELLSLIB_H
#define NETLIST_CELLSLIB_H
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QDialog>
#include <QLineEdit>
#include <iostream>
#pragma once

namespace Netlist
{
class CellViewer;
class CellsModel;
class CellsLib : public QWidget
{
    Q_OBJECT;
public:
    CellsLib       ( QWidget* parent=NULL );
    void        setCellViewer  ( CellViewer* );
    int         getSelectedRow () const;
    inline CellsModel* getBaseModel   ()
    {
        return baseModel_;
    }
public slots:
    void        load           ();
private:
    CellViewer*  cellViewer_;
    CellsModel*  baseModel_;
    QTableView*  view_;
    QPushButton* load_;
};
}
#endif
