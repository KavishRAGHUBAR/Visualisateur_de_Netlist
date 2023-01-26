#include "InstancesModel.h"
#include "Cell.h"
#include "Instance.h"


namespace Netlist
{
InstancesModel::InstancesModel (QObject* parent)
    : QAbstractTableModel(parent)
    , cell_ (NULL)
{

}

InstancesModel::~InstancesModel()
{

}

void InstancesModel::setCell( Cell* cell )
{
    emit layoutAboutToBeChanged();
    cell_ = cell ;
    emit layoutChanged ();
}

int InstancesModel::rowCount(const QModelIndex& parent) const
{
    return ( cell_ ) ? cell_ -> getInstances().size() : 0;
}

int InstancesModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

QVariant InstancesModel::data ( const QModelIndex& index , int role ) const
{
    if (not cell_ or not index.isValid()) return QVariant();
    if (role == Qt::DisplayRole )
    {
        int row = index.row ();
        switch ( index.column() )
        {
        case 0:
            return cell_->getInstances ()[row]->getName().c_str();
        case 1:
            return cell_->getInstances ()[row]->getMasterCell()->getName().c_str();
        }
    }
    return QVariant ();
}

QVariant InstancesModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Vertical ) return QVariant ();
    if ( role != Qt::DisplayRole ) return QVariant ();
    switch ( section )
    {
    case 0:
        return "Instance" ;
    case 1:
        return "MasterCell" ;
    }
    return QVariant ();
}


Cell* InstancesModel::getModel (int row)
{
    if ( not cell_ ) return NULL ;
    if ( row >= (int)cell_->getInstances().size ()) return NULL ;
    return cell_->getInstances()[row]->getMasterCell();
}
}
