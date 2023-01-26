#include  <libxml/xmlreader.h>
#include  <string>
#include  <vector>
#include  "Box.h"
#include  "Symbol.h"
#include  "Term.h"
#include  "Shape.h"
#include  "Cell.h"

using namespace std;

namespace Netlist
{
Symbol::Symbol ( Cell* c):owner_(c),shapes_()
{

}

Symbol:: ~Symbol()
{

}

Cell* Symbol::getCell         () const
{
    return owner_;
}

Box Symbol::getBoundingBox  () const
{
    Box bb;
    for(vector<Shape*>::const_iterator it=shapes_.begin() ; it != shapes_.end() ; ++it)//Utiliser iterator pour recuperer chaque element dans le vecteur shapes_
    {
        Shape* shape = dynamic_cast<Shape*> (*it) ;//Recuperer chaque element
        bb=shape->getBoundingBox();//Donc on peut utiliser la methode getBoundingBox dans la classe shape
    }
    return bb;
}

Point                     Symbol::  getTermPosition ( Term* term ) const
{
    Point point = term->getPosition() ;//Utiliser la methode getPosition dans term
    return point ;
}

TermShape*                Symbol:: getTermShape    ( Term* term) const
{
    for ( vector<Shape*>::const_iterator it=shapes_.begin() ; it != shapes_.end() ; ++it)//Utiliser iterator pour recuperer chaque element dans le vecteur shapes_
    {
        TermShape* termshape = dynamic_cast<TermShape*> (*it) ;//Recuperer chaque element
        if(termshape != NULL)
        {
            if(termshape->getTerm() -> getName() == term -> getName())
            {
                return termshape ;
            }
        }

    }
    return NULL;
}

void                       Symbol::add             ( Shape* shape)
{
    shapes_.push_back(shape) ;
}

void                       Symbol::remove          ( Shape* shape)
{
    int n=0;
    for ( vector<Shape*>::const_iterator it=shapes_.begin() ; it != shapes_.end() ; ++it )//Utiliser iterator pour trouver le meme element shape et le mettre a NULL
    {
        if(*it==shape)
        {
            shapes_[n]=NULL;
        }
        n++;

    }

}

void                       Symbol::toXml           ( std::ostream& stream) const
{
    stream << indent++<< "<symbol>" <<endl ;
    size_t i;
    for(i=0; i<shapes_.size(); i++)
    {
        shapes_[i]->toXml(stream) ;//Affichier tous les elements dans le vecteur shapes_
    }

    stream << --indent<< "</symbol>"<<endl ;

}

Symbol*             Symbol::fromXml         ( Cell* cell, xmlTextReaderPtr reader)
{
    const xmlChar* symbolTag      = xmlTextReaderConstString( reader, (const xmlChar*)"symbol" );//Trouver le tag symbol
    const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );//Trouver nodename par reader
    Symbol* symbol = cell->getSymbol();

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
        if ((nodeName == symbolTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT))//Si on a le bon tag et que c'est la fin
        {
            break;
        }

        else
        {
            Shape::fromXml(symbol,reader) ;//Appeler la methode fromxml de shape
        }

    }
    return symbol;
}
}
