#ifndef NETLIST_OpenABOUT_H
#define NETLIST_OpenABOUT_H
#include <QDialog>
#include <QLineEdit>
#pragma once

//gestion d'affichage de about

namespace Netlist
{

class OpenAbout: public QDialog
{

    Q_OBJECT ;
public :
    OpenAbout ( QWidget * parent = NULL );
    static bool run ();

};


}
#endif
