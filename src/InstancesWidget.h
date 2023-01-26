#ifndef NETLIST_INSTANCESWIDGET_H
#define NETLIST_INSTANCESWIDGET_H
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QDialog>
#include <QLineEdit>
#include <iostream>
#include "InstancesModel.h"
#include "CellViewer.h"
#pragma once



namespace Netlist
{
class Cell;
class CellViewer;
class InstancesModel;

class InstancesWidget : public QWidget
{
    Q_OBJECT;
public:
    InstancesWidget ( QWidget* parent=NULL );
    void  setCellViewer   ( CellViewer* );
    int   getSelectedRow  () const;
    inline  void  setCell         ( Cell* c)
    {
        return baseModel_->setCell(c);
    }
public slots:
    void  load            ();
private:
    CellViewer*     cellViewer_;
    InstancesModel* baseModel_;
    QTableView*     view_;
    QPushButton*    load_;
};


}
#endif
