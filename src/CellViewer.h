#ifndef NETLIST_CELLVIEWER_H
#define NETLIST_CELLVIEWER_H
#include<QMainWindow>
#pragma once


namespace Netlist
{
class Cell;
class CellWidget;
class SaveCellDialog;
class OpenCellDialog;
class InstancesWidget;
class CellsLib;

class CellViewer:public QMainWindow
{
    Q_OBJECT;
public:
    CellViewer ( QWidget* parent = NULL );
    virtual ~CellViewer ();
    Cell * getCell() const ;
    //inline CellWidget* getCellWidget()const{return cellWidget_;}
    inline  CellsLib* getCellsLib  ()
    {
        return cellsLib_;   // TME9+.
    }


public slots :
    void saveCell ();
    void openCell ();
    void setCell( Cell* );
    void      showCellsLib        ();  // TME9+.
    void      showInstancesWidget ();  // TME9+.
    void  about();

signals:
    void cellLoaded();

private :
    CellWidget * cellWidget_ ;
    SaveCellDialog * saveCellDialog_ ;
    InstancesWidget* instancesWidget_;  // TME9+.
    CellsLib*        cellsLib_;         // TME9+.
};

}
#endif
