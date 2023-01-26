#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "OpenCellDialog.h"
#include "CellViewer.h"

namespace Netlist
{

OpenCellDialog :: OpenCellDialog ( QWidget * parent )//CTOR pour OpenCellDialog
    : QDialog ( parent )//Declarer QDialog comme parent
    , lineEdit_ ( NULL )//Pas de lineEdit_
{
    setWindowTitle ( tr ( " Open Cell " ) );//Fonction de set windowtitle
    QLabel * label = new QLabel ();
    label -> setText ( tr ( " Enter Cell name ( without extention ) " ) );//Insertion du nom de Cell
    lineEdit_ = new QLineEdit ();
    lineEdit_ -> setMinimumWidth ( 400 );
    QPushButton * okButton = new QPushButton ();//Creer PushBottom
    okButton -> setText ( " OK " );//Initialiser Button en mot OK
    okButton -> setDefault ( true );
    QPushButton * cancelButton = new QPushButton ();//Creer PushBottom
    cancelButton -> setText ( " Cancel " );//Initialiser Bottom en mot Cancel
    QHBoxLayout * hLayout = new QHBoxLayout ();//Creer un layout en horizontal
    hLayout -> addStretch ();
    hLayout -> addWidget ( okButton );//Mettre okButton en layout horizontal
    hLayout -> addStretch ();
    hLayout -> addWidget ( cancelButton );//Mettre cancelButton en layout horizontal
    hLayout -> addStretch ();
    QFrame * separator = new QFrame ();
    separator -> setFrameShape ( QFrame :: HLine );
    separator -> setFrameShadow ( QFrame :: Sunken );
    QVBoxLayout * vLayout = new QVBoxLayout ();
    vLayout -> setSizeConstraint ( QLayout :: SetFixedSize );
    vLayout -> addWidget ( label );
    vLayout -> addWidget ( lineEdit_ );
    vLayout -> addLayout ( hLayout );
    setLayout ( vLayout );//Fonction pour set layout en haut
    connect ( okButton , SIGNAL ( clicked ()) , this , SLOT ( accept ()) );//Connecter le button avec signal d'action clicked et slot accept
    connect ( cancelButton , SIGNAL ( clicked ()) , this , SLOT ( reject ()) );//Connecter le button avec signal d'action clicked et slot reject
}

//GETTER
const QString OpenCellDialog :: getCellName () const
{
    return lineEdit_-> text ();
}

//SETTER
void OpenCellDialog :: setCellName ( const QString & name )
{
    return lineEdit_ -> setText ( name );
}

bool OpenCellDialog :: run ( QString & name )
{
    OpenCellDialog* oc=new OpenCellDialog();//Creer une class OpenCellDialog
    oc->setCellName ( name );//Initialiser cette class avec le nom d'insertion
    int dialogResult = oc -> exec ();
    name = oc->getCellName ();//Verifier la class est le bon choix
    delete oc;//Supprimer la class pour eviter la fuite memoire
    return ( dialogResult == Accepted );
}
}
