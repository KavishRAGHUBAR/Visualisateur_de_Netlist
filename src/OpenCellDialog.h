#ifndef NETLIST_OPENCELLDIALOG_H
#define NETLIST_OPENCELLDIALOG_H
#include <QDialog>
#include <QLineEdit>
#pragma once



namespace Netlist
{

class OpenCellDialog: public QDialog
{

    Q_OBJECT ;
public :
    OpenCellDialog ( QWidget * parent = NULL );
    static bool run ( QString & name );
    const QString getCellName () const ;
    void setCellName ( const QString & );
protected :
    QLineEdit * lineEdit_ ;
};


}
#endif
