#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include  <string>
#include  <vector>
#include "XmlUtil.h"
#include <fstream>
#include  <libxml/xmlreader.h>
#include  <cstdlib>
#include  "Box.h"
#pragma once

namespace Netlist
{
class Symbol;
class Term;
class Cell;//forward declaration des classes
class Shape
{
public:
    Shape(Symbol*);
    virtual ~Shape();//DTOR de shape, en meme forme dans les classes fils, donc on ajoute virtual
    inline Symbol* getSymbol()const
    {
        return owner_;
    }
    virtual Box getBoundingBox()const=0;//Methode pour trouver bounding box
    static Shape* fromXml ( Symbol*, xmlTextReaderPtr );
    virtual void  toXml ( std::ostream&) const;

private:
    Symbol* owner_;
};


class LineShape:public Shape
{
public:
    LineShape(Symbol*,int x1,int y1,int x2,int y2);//CTOR pour LineShape
    ~LineShape();
    Box getBoundingBox()const;
    virtual void  toXml ( std::ostream&) const;
    static Shape* fromXml ( Symbol*, xmlTextReaderPtr );
    inline int getX1()
    {
        return x1_;
    }
    inline int getX2()
    {
        return x2_;
    }
    inline int getY1()
    {
        return y1_;
    }
    inline int getY2()
    {
        return y2_;
    }
private:
    // Symbol* owner_;//On n'a plus besoin pour la classe fils, deja declarer dans la classe mere
    int x1_,y1_,x2_,y2_;
};

class BoxShape:public Shape
{
public:
    //methodes
    BoxShape(Symbol*, const Box&);//CTOR pour shape box
    ~BoxShape();
    Box getBoundingBox() const;
    virtual void  toXml ( std::ostream&) const;
    static Shape* fromXml ( Symbol*, xmlTextReaderPtr );
private:
    //Symbol* owner_;
    Box box_;
};
/*
BoxShape::BoxShape(Symbol* s, const Box& b)
        :owner(s),box_(b)
        {owner_->add(this);}

//pour le destructeur
 BoxShape::~BoxShape(){
    owner_->remove(this);
 }
 BoxShape::getBoundingBox() const
 {return box_;}
*/

class EllipseShape:public Shape
{
public:
    EllipseShape(Symbol*,const Box&);//CTOR pour shape ellipse
    ~EllipseShape();
    Box getBoundingBox()const;
    virtual void  toXml ( std::ostream&) const;
    static Shape* fromXml ( Symbol*, xmlTextReaderPtr );
private:
    // Symbol* owner_;
    Box box_;
};

class ArcShape:public Shape
{
public:
    ArcShape(Symbol*,const Box&,int start, int span);//CTOR pour shape Arc
    ~ArcShape();
    Box getBoundingBox()const;
    virtual void  toXml ( std::ostream&) const;
    static Shape* fromXml ( Symbol*, xmlTextReaderPtr );
    inline int getStart()
    {
        return start_;
    }
    inline int getSpan()
    {
        return span_;
    }
private:
    //Symbol* owner_;
    Box box_;
    int start_,span_;
};

class TermShape:public Shape
{
public:
    enum NameAlign { TopLeft=1, TopRight=2, BottomLeft=3, BottomRight=4 };//Pour les donnees termshape, on creer un type pour appeler les donnees, comme on a fait pour direction dans term
    static std::string  toString    ( NameAlign );//Methode de conversion de type NameAlign a string pour afficher
    static NameAlign toNameAlign ( std::string );//Methode de conversion de string a NameAlign pour lecture
    TermShape(Symbol*,std::string name, int x1, int y1,NameAlign align);//CTOR pour shape TermShape
    ~TermShape();
    Box getBoundingBox()const;
    inline Term* getTerm() const
    {
        return term_;
    }
    inline int getX() const
    {
        return x_;
    }
    inline int getY() const
    {
        return y_;
    }
    inline NameAlign getAlign()const
    {
        return align_;
    }
    virtual void  toXml ( std::ostream&) const;
    static Shape* fromXml ( Symbol*, xmlTextReaderPtr );
private:
    Term* term_;
    int x_,y_;
    NameAlign align_;
};




}
#endif
