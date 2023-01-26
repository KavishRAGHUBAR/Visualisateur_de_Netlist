#include "CellsModel.h"
#include "Cell.h"
#include "Instance.h"


namespace Netlist
{
CellsModel::CellsModel (QObject* parent)
    : QAbstractTableModel(parent)
    , cell_ ()
{

}

CellsModel::~CellsModel()
{

}

/*void CellsModel::setCell() {
    emit layoutAboutToBeChanged();
    cell_ = (Cell::getAllCells()) ;
    emit layoutChanged ();
}*/

int CellsModel::rowCount(const QModelIndex& parent) const
{
    return ( cell_.size() );
}

int CellsModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant CellsModel::data ( const QModelIndex& index , int role ) const
{
    if (not cell_.size() or not index.isValid()) return QVariant();
    if (role == Qt::DisplayRole )
    {
        int row = index.row ();
        //if(index.column()==0 ) {
        return cell_[row]->getName().c_str();
        //}
    }
    return QVariant ();
}

QVariant CellsModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Vertical ) return QVariant ();
    if ( role != Qt::DisplayRole ) return QVariant ();
    //if ( section==0 ) {
    return "Cell" ;
    //}
    return QVariant ();
}


Cell* CellsModel::getModel (int row)
{
    if ( not cell_.size() ) return NULL ;
    if ( row >= (int)cell_.size ()) return NULL ;
    return cell_[row];
}

void CellsModel::updateDatas()
{
    //setCell();
    emit layoutAboutToBeChanged();
    cell_ = (Cell::getAllCells()) ;
    emit layoutChanged ();
}



}
