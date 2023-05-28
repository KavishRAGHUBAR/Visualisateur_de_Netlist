#include  "Node.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"
#include  "Cell.h"
#include "Indentation.h"
#include  "Point.h"
#include  "Line.h"
#include  "Shape.h"


using namespace std;


namespace Netlist
{
Net:: Net( Cell* cell, const std::string& name, Term::Type t)//CTOR de la classe Net
    : owner_ (cell)
    , name_  (name)
    ,id_     ()
    ,type_   (t)
    ,nodes_  ()
{
    cell->add(this);//Ajouter dans cell
}


Net::~Net()//DTOR de la classe Net
{
    if(getCell()) getCell()->remove(this); //par securite on verifie si c'est la bonne cell
}

std::string Net::getName() const
{
    return name_;
}

Cell* Net::getCell() const
{
    return owner_;
}

unsigned int Net::getId() const
{
    return id_;
}

Term::Type Net::getType()
{
    return type_;
}

const std::vector<Node*>& Net::getNodes() const
{
    return nodes_;
}

size_t Net::getFreeNodeId()//Trouver la premiere case vide du vecteur nodes
{
    size_t i;
    for(i=0; i<nodes_.size(); ++i) //parce qu'il peut y avoir des "trous" dans Node*
    {
        if(nodes_[i]==NULL)
        {
            return i;//Si i est plus petit que la capacite du vecteur, il y a une case vide dans le vecteur

        }

    }
    return nodes_.size();//Si i est egale a la capacita, donc il n'a pas une case vide
}

void Net::add(Node* n)
{
    if (not n) return;

    size_t id=n ->getId();
    if (id == Node::noid)
    {
        id=getFreeNodeId();
        n->setId(id);
    }

    if (id <nodes_.size())
    {
        if (nodes_[id] != NULL)
        {
            cerr << "Overwrite "<< id <<endl;
            nodes_[id] -> setId(Node::noid);
        }

        nodes_[id] = n;
    }
    else
    {
        for(size_t i=nodes_.size(); i<id; ++i)
        {
            nodes_.push_back(NULL);
        }
        nodes_.push_back(n);
    }

}

bool Net::remove(Node* n)
{
    for ( vector<Node*>::iterator inode=nodes_.begin() ; inode != nodes_.end() ; ++inode )
    {
        if (*inode == n) nodes_.erase( inode );
        return true;
    }
    return false;
}

void  Net::add ( Line* line )
{
    if (line) lines_.push_back( line );
}

bool  Net::remove ( Line* line )
{
    if (line)
    {
        for ( vector<Line*>::iterator il = lines_.begin()
                                           ; il != lines_.end() ; ++il )
        {
            if (*il == line)
            {
                lines_.erase( il );
                return true;
            }
        }
    }
    return false;
}

Node* Net::getNode(int id)const
{
    return nodes_[id];
}

void  Net::toXml ( ostream& stream )const
{
    stream << indent++<<"<net name=\"" << name_ << "\" "<< "type=\""<<Term::toString(type_)<< "\"/>"<<endl;

    for ( vector<Node*>::const_iterator it=nodes_.begin() ; it != nodes_.end() ; ++it )//Itérateur pour obtenir l'element dans le vecteur node
    {
        if ( (*it) == NULL )
        {
            continue ;
        }
        (*it)->toXml(cout); //qu'on affichera par la suite sur la console

    }

    for ( vector<Line*>::const_iterator it=lines_.begin() ; it != lines_.end() ; ++it )//Itérateur pour obtenir l'element dans le vecteur line
    {
        (*it)->toXml(cout); //qu'on affichera par la suite sur la console
    }
    stream << --indent <<"</net>\n";
}

Net* Net::fromXml ( Cell* C,xmlTextReaderPtr reader )
{
    //Net* net   = NULL;//Initialiser net � vide
    const xmlChar* NetTag      = xmlTextReaderConstString( reader, (const xmlChar*)"net" );//Trouver le tag net


    string netName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );//Appeler reader pour trouver la valeur apres name
    string netType = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"type" ) );//Appeler reader pour trouver la valeur apres type


    if(not netName.empty() && not netType.empty())
    {

        Net* net= new Net(C,netName,Term::toType(netType));
        const xmlChar* nodeTag = xmlTextReaderConstString( reader, (const xmlChar*)"node" );
        const xmlChar* lineTag = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
        //const xmlChar* arcTag = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
        //const xmlChar* ellipseTag = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
        //const xmlChar* termTag = xmlTextReaderConstString( reader, (const xmlChar*)"term" );

        while ( true )
        {

            xmlTextReaderRead(reader);

            switch ( xmlTextReaderNodeType(reader) )
            {
            case XML_READER_TYPE_COMMENT:
            case XML_READER_TYPE_WHITESPACE:
            case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                continue;
            }

            const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );//Trouver nodename par reader
            //cerr<<nodeName<<endl;
            if (nodeName == nodeTag) //Si reader a trouve tag de node
            {
                if(Node::fromXml(net,reader))//Appeler la methode de fromxml dans node
                {
                    continue;
                }
            }

            else if ( nodeName == lineTag ) //Si reader a trouve le tag de line
            {
                if(Line::fromXml(net,reader))//Appeler la methode de fromxml dans line
                {
                    continue ;
                }
            }

            else if ( (nodeName == NetTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) )
            {
                break;
            }
        }
        return net;
    }
    return NULL;
}
}
