#include "CellViewer.h"
#include "Cell.h"
#include "SaveCellDialog.h"
#include "OpenCellDialog.h"
#include "CellWidget.h"
#include "InstancesWidget.h"
#include "CellsLib.h"
#include "CellsModel.h"
#include "OpenAbout.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDialog>
#include <QMessageBox>
#include <QWidget>

using namespace std;

namespace Netlist
{

CellViewer :: CellViewer ( QWidget * parent )
    : QMainWindow ( parent )
    , cellWidget_ ( NULL )
    , saveCellDialog_ ( NULL )
{
    cellWidget_ = new CellWidget();
    saveCellDialog_ = new SaveCellDialog ( this );
    setCentralWidget (cellWidget_);

    QMenu * fileMenu = menuBar () -> addMenu ( "&File " );
    QMenu * aboutMenu = menuBar () -> addMenu ( "&About " );
    QAction * aaction = new QAction ( "&About" , this );//L'action de about as
    aaction -> setStatusTip ( " Affichier about " );
    aaction -> setShortcut ( QKeySequence ( "CTRL+a" ) );//shortcut de save as
    aaction -> setVisible ( true );
    aboutMenu -> addAction ( aaction );
    connect ( aaction , SIGNAL ( triggered ()) , this , SLOT ( about()) );//Connect l'action avec le slot savecell()
    



    QAction * action = new QAction ( "&Save As" , this );//L'action de save as
    action -> setStatusTip ( " Save to disk ( rename ) the Cell " );
    action -> setShortcut ( QKeySequence ( "CTRL+S" ) );//shortcut de save as
    action -> setVisible ( true );
    fileMenu -> addAction ( action );
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( saveCell ()) );//Connect l'action avec le slot savecell()

    instancesWidget_=new InstancesWidget();
    instancesWidget_->setCellViewer(this);//Il faut recuperer le bon view d'instance
    QAction * iaction = new QAction ( "&Open Instance" , this );//L'action de Open Instance
    iaction -> setStatusTip ( " Open Instance " );
    iaction -> setShortcut ( QKeySequence ( "CTRL+I" ) );
    iaction -> setVisible ( true );
    fileMenu -> addAction ( iaction );
    connect ( iaction , SIGNAL ( triggered ()) , this , SLOT ( showInstancesWidget ()) );//Connect l'action avec le slot showInstancesWidger()

    cellsLib_=new CellsLib();
    cellsLib_->setCellViewer(this);//Il faut recuperer le bon view d'instance
    QAction * caction = new QAction ( "&Open Cell" , this );//L'action de open cell
    caction -> setStatusTip ( " Open Cell " );
    caction -> setShortcut ( QKeySequence ( "CTRL+C" ) );
    caction -> setVisible ( true );
    fileMenu -> addAction ( caction );
    connect ( caction , SIGNAL ( triggered ()) , this , SLOT ( showCellsLib ()) );//Connect l'action avec slot showCellsLib()


    QAction * oaction = new QAction ( "&Open" , this);//L'action de Open
    oaction -> setStatusTip ( " Open the Cell " );
    oaction -> setShortcut ( QKeySequence ( "CTRL+O" ) );
    oaction -> setVisible ( true );
    fileMenu -> addAction ( oaction );
    connect ( oaction , SIGNAL ( triggered ()) , this , SLOT ( openCell ()) );//Connect l'action avec slot openCell()


    action = new QAction ( "&Quit " , this );
    action -> setStatusTip ( " Exit the Netlist Viewer " );
    action -> setShortcut ( QKeySequence ( "CTRL+Q" ) );
    action -> setVisible ( true );
    fileMenu -> addAction ( action );
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( close ()) );
    //cellsLib_->getBaseModel()->setCell();

    connect (this, SIGNAL(cellLoaded()), cellsLib_->getBaseModel(),SLOT(updateDatas()));//Connect le signal cellLoaded() avec slot updateDatas()

    emit cellLoaded();//Appeler le signal donc appeler le slot updateDatas()
}


CellViewer::~ CellViewer ()
{
    delete cellWidget_;
}


void CellViewer::setCell ( Cell * c)
{
    //std::cerr << "cell widget = " <<cellWidget_ <<std::endl;
    cellWidget_->setCell(c);//Appeler la methode de setCell dans cellWidget
    instancesWidget_->setCell(c);//Appeler la methode de setCell dans instancesWidget


}

Cell * CellViewer::getCell () const
{
    return cellWidget_->getCell();
}

void CellViewer :: saveCell ()
{
    Cell * cell = getCell ();
    if ( cell == NULL ) return ;
    QString cellName = cell -> getName().c_str();
    if ( saveCellDialog_ -> run (cellName))
    {
        cell -> setName ( cellName.toStdString () );
        cell -> save ( cellName.toStdString () );
    }
}

void   CellViewer::openCell ()
{

    Cell* cell;
    QString cellName;
    //OpenCellDialog OC;
    cell= Cell::find(cellName.toStdString ());//Trouver le cell de meme nom
    //cerr<<cellName.toStdString ()<<endl;
    if(OpenCellDialog:: run(cellName))
    {


        setCell(cell -> load(cellName.toStdString ()));//Initialiser les cell en chargant les elements
        //cell -> setName ( cellName.toStdString () );
        //setCell(cell);
        emit cellLoaded();

    }
}

void      CellViewer::showInstancesWidget ()
{
    std::cout << instancesWidget_ << std::endl;
    instancesWidget_->show();//Affichier l'instanceWidget
}

void      CellViewer::showCellsLib        ()
{
    cellsLib_->show();//Affichier le cellsLib
}

//void CellViewer::cellLoaded()
//{
//    QString cellName;
//    Cell::load(cellName.toStdString ());
//}

void      CellViewer::about       ()
{
    OpenAbout:: run();
}

}
