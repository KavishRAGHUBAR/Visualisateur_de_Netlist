#include "CellsLib.h"
#include "Cell.h"
#include "CellViewer.h"
#include "CellsModel.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>



namespace Netlist
{


CellsLib::CellsLib( QWidget* parent)
    : QWidget(parent)
    , cellViewer_(NULL)
    , baseModel_(new CellsModel(this))
    , view_(new QTableView(this))
    , load_(new QPushButton(this))
{
    setAttribute(Qt::WA_QuitOnClose,false);
    setAttribute(Qt::WA_DeleteOnClose,false);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    view_->setShowGrid(false);
    view_->setAlternatingRowColors(true);
    view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    view_->setSelectionMode(QAbstractItemView::SingleSelection);
    view_->setSortingEnabled(true);
    view_->setModel(baseModel_);

    QHeaderView* horizontalHeader=view_->horizontalHeader();
    horizontalHeader->setDefaultAlignment(Qt::AlignHCenter);
    horizontalHeader->setMinimumSectionSize(300);
    horizontalHeader->setStretchLastSection(true);

    QHeaderView* verticalHeader=view_->verticalHeader();
    verticalHeader->setVisible(false);

    load_->setText("Load");


    QHBoxLayout * hLayout = new QHBoxLayout ();
    hLayout -> addStretch ();
    hLayout -> addWidget ( load_ );
    hLayout -> addStretch ();
    QFrame * separator = new QFrame ();
    separator -> setFrameShape ( QFrame :: HLine );
    separator -> setFrameShadow ( QFrame :: Sunken );
    QVBoxLayout * vLayout = new QVBoxLayout ();
    //vLayout -> setSizeConstraint ( QLayout :: SetFixedSize );
    vLayout -> addWidget (view_);
    vLayout -> addLayout ( hLayout );
    setLayout ( vLayout );

    connect(load_,SIGNAL(clicked()),this,SLOT(load()));
}

void        CellsLib::setCellViewer  ( CellViewer* cellviewer)
{
    cellViewer_=cellviewer;
}

int         CellsLib::getSelectedRow () const
{
    QModelIndexList selecteds=view_->selectionModel()->selection().indexes();

    if(selecteds.empty()) return -1;
    return selecteds.first().row();
}


void        CellsLib::load ()
{
    int selectedRow=getSelectedRow();
    if(selectedRow<0) return;
    //std::cerr << "cellViewer - " << cellViewer_ << std::endl;
    cellViewer_->setCell(baseModel_->getModel(selectedRow));
}



}
