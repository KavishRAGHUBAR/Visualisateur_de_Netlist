#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "OpenAbout.h"
#include "CellViewer.h"

namespace Netlist
{

OpenAbout :: OpenAbout ( QWidget * parent )//CTOR pour OpenAbout
    : QDialog ( parent )//Declarer QDialog comme parent
{
    setWindowTitle ( tr ( " About " ) );//Fonction de set windowtitle
    QLabel * label = new QLabel ();
    label -> setText ( tr ( " Projet MOBJ : Visualisateur de Netlist. \n\n Realise par le binome : Weiyi GONG  et Kavish RAGHUBAR \n\n M1 SESI - Sorbonne Universite - 2022/2023 \n\n https://www-soc.lip6.fr/master-sesi/ue-mobj/") );//Insert les nouvelles texts
    QFrame * separator = new QFrame ();
    separator -> setFrameShape ( QFrame :: HLine );
    separator -> setFrameShadow ( QFrame :: Sunken );
    QVBoxLayout * vLayout = new QVBoxLayout ();
    vLayout -> setSizeConstraint ( QLayout :: SetFixedSize );
    vLayout -> addWidget ( label );
    setLayout ( vLayout );//Fonction pour set layout en vertical
}


bool OpenAbout :: run ()
{
    OpenAbout* oc=new OpenAbout();//Creer une class OpenABout
    int dialogResult = oc -> exec ();
    delete oc;//Supprimer la class pour eviter la fuite memoire
    return ( dialogResult == Accepted );
}

}
