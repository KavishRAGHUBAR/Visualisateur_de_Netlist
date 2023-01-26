#ifndef NETLIST_TERM_H
#define NETLIST_TERM_H

#include <iostream>
#include <vector>
#include <string>
#include  "Instance.h"
#include  "Point.h"
#include  "Node.h"
#include "XmlUtil.h"
#include <fstream>
#include  <libxml/xmlreader.h>
#include  <cstdlib>
#pragma once

namespace Netlist
{
//forward declaration pour le compilateur
class Cell;
class Net;
class Instance;
//La classe Term
class Term
{


public:
    enum Type {Internal =1, External =2};
    enum Direction {In=1,Out=2,Inout=3, Tristate=4, Transcv=5, Unknown=6};

    static std::string  toString    ( Term::Type );
    static std::string  toString    ( Direction );
    static Direction    toDirection ( std::string );
    static Type         toType      ( std::string );

private:
    void* owner_;
    std::string name_;
    Direction direction_;
    Type type_; //Internal : Instance et External : Cell
    Net* net_;
    NodeTerm node_; //objet node "entier"
//methodes
public:
    Term(Cell*, const std::string& name,Direction); //externe
    Term(Instance*,const Term* modelTerm); //interne
    ~Term();
    bool isExternal() const;
    bool isInternal() const;
    const std::string& getName() const;
    Node* getNode();
    Net* getNet() const;
    Cell* getCell() const;
    Cell* getOwnerCell() const;
    Instance* getInstance() const;
    Direction getDirection()const;
    Point getPosition() const;
    Type getType() const;
    void setNet(Net*);
    void setNet(std::string name);
    inline void  setDirection ( Direction );
    void  setPosition  ( const Point& );
    void  setPosition  ( int x, int y );
    void  toXml ( std::ostream&) const;
    static Term* fromXml ( Cell*, xmlTextReaderPtr );

//...
};//fin de la classe Term



}

//...
#endif // TERM_H
