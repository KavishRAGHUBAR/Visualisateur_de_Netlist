#include  <libxml/xmlreader.h>
#include  <string>
#include  <vector>
#include  "Box.h"
#include  "Symbol.h"
#include  "Shape.h"
#include "XmlUtil.h"
#include <fstream>
#include  <cstdlib>
#include  "Term.h"
#include  "Cell.h"


using namespace std;

namespace Netlist
{
Shape::Shape(Symbol* s):owner_(s)
{
    owner_->add(this);//CTOR pour la classe mere de shape, ajouter dans owner_
}

Shape::~Shape()
{
    owner_->remove(this);//DTOR pour la classe mere de shape
}

Box Shape::getBoundingBox()const
{
    return Box();//Shape en forme Box, donc on peut directement la recuperer avec le CTOR de Box
}

void  Shape::toXml ( ostream& stream) const {}

Shape* Shape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
{
    // Factory-like method.
    const xmlChar* boxTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
    const xmlChar* ellipseTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
    const xmlChar* arcTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
    const xmlChar* lineTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
    const xmlChar* termTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
    const xmlChar* nodeName
        = xmlTextReaderConstLocalName( reader );

    Shape* shape = NULL;
    if (boxTag == nodeName)
        shape = BoxShape::fromXml( owner, reader );
    if (ellipseTag == nodeName)
        shape = EllipseShape::fromXml( owner, reader );
    if (arcTag == nodeName)
        shape = ArcShape::fromXml( owner, reader );
    if (lineTag == nodeName)
        shape = LineShape::fromXml( owner, reader );
    if (termTag == nodeName)
        shape = TermShape::fromXml( owner, reader );

    if (shape == NULL)
        cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
             << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;

    return shape;
}






LineShape::LineShape(Symbol* owner,int x1,int y1,int x2,int y2):Shape(owner),x1_(x1),y1_(y1),x2_(x2),y2_(y2)
{
    //Rien a ecrire, deja declaré dans la classe meme
}

LineShape::~LineShape()
{
    //Rien a ecrire, deja declarer dans la classe meme
}

Box LineShape::getBoundingBox()const
{
    return Box(std::min(x1_,x2_),std::min(y1_,y2_),std::max(x1_,x2_),std::max(y1_,y2_));//En forme box, donc on cree un box en ordonnee x1 comme le min entre x1_et x2_, le meme principe pour y1, x2 comme le maximale de x1 et x2, le meme principe pour y2
}


void  LineShape::toXml ( std::ostream& stream) const
{
    stream << indent   << "<line x1=\""<<x1_<<"\" "<<"y1=\""<<y1_<<"\" "<<"x2=\""<<x2_<<"\" "<<"y2=\""<<y2_<<"\">"<<endl;//Affichier en forme sur le sujet
}

Shape* LineShape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
{
    string LineX1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );//Lire le contenu apres x1
    string LineY1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );//Lire le contenu apres y1
    string LineX2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );//Lire le contenu apres x2
    string LineY2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );//Lire le contenu apres y2

    if(not LineX1.empty() and not LineX2.empty() and not LineY1.empty() and not LineY2.empty())
    {
        LineShape* L=new LineShape(owner,atoi(LineX1.c_str()),atoi(LineY1.c_str()),atoi(LineX2.c_str()),atoi(LineY2.c_str()));//Si c'est un nouveau LineShape, on va creer cela en appelant le CTOR LineShape
        return L;
    }
    return NULL;

}


BoxShape::BoxShape(Symbol* owner, const Box& b):Shape(owner),box_(b)
{

}

BoxShape::~BoxShape()
{

}

Box BoxShape::getBoundingBox() const
{
    return box_;
}

void  BoxShape::toXml ( std::ostream& stream) const
{
    stream << indent   << "<line x1=\""<<box_.getX1()<<"\" "<<"y1=\""<<box_.getY1()<<"\" "<<"x2=\""<<box_.getX2()<<"\" "<<"y2=\""<<box_.getY2()<<"\">"<<endl;
}

Shape* BoxShape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
{
    string BoxX1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );//Lire le contenu apres x1
    string BoxY1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );//Lire le contenu apres y1
    string BoxX2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );//Lire le contenu apres x2
    string BoxY2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );//Lire le contenu apres y2

    if(not BoxX1.empty() and not BoxX2.empty() and not BoxY1.empty() and not BoxY2.empty())
    {
        Box B=Box(atoi(BoxX1.c_str()),atoi(BoxY1.c_str()),atoi(BoxX2.c_str()),atoi(BoxY2.c_str()));
        //Si c'est un nouveau BoxShape, on va la creer en appelant CTOR Box,
        //parce que pour le CTOR de BoxShape, on a besoin d'un parametre en type box
        BoxShape* Bs=new BoxShape(owner,B);//Creer avec CTOR de BoxShape
        return Bs;
    }
    return NULL;

}


EllipseShape::EllipseShape(Symbol* owner,const Box& box):Shape(owner),box_(box)
{

}

EllipseShape::~EllipseShape()
{

}

Box EllipseShape::getBoundingBox()const
{
    return box_;
}

void  EllipseShape::toXml ( std::ostream& stream) const
{
    stream << indent   << "<line x1=\""<<box_.getX1()<<"\" "<<"y1=\""<<box_.getY1()<<"\" "<<"x2=\""<<box_.getX2()<<"\" "<<"y2=\""<<box_.getY2()<<"\" "<<endl;

}

Shape* EllipseShape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
{
    string ElliX1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );//Lire le contenu apres x1
    string ElliY1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );//Lire le contenu apres y1
    string ElliX2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );//Lire le contenu apres x2
    string ElliY2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );//Lire le contenu apres y2

    if(not ElliX1.empty() and not ElliX2.empty() and not ElliY1.empty() and not ElliY2.empty())
    {
        Box E=Box(atoi(ElliX1.c_str()),atoi(ElliY1.c_str()),atoi(ElliX2.c_str()),atoi(ElliY2.c_str()));
        //Si c'est un nouveau BoxShape, on va creer cela en appelant le CTOR Box, parce que pour CTOR de EllipseShape, il a besoin d'un parametre de type box
        EllipseShape* Es=new EllipseShape(owner,E);
        return Es;
    }
    return NULL;

}



ArcShape::ArcShape(Symbol* owner,const Box& b,int start, int span):Shape(owner),box_(b),start_(start),span_(span)
{

}

ArcShape::~ArcShape()
{

}

Box ArcShape::getBoundingBox()const
{
    return box_;
}

void  ArcShape::toXml ( std::ostream& stream) const
{
    stream << indent   << "<line x1=\""<<box_.getX1()<<"\" "<<"y1=\""<<box_.getY1()<<"\" "<<"x2=\""<<box_.getX2()<<"\" "<<"y2=\""<<box_.getY2()<<"\" "<<"start=\""<<start_<<"\" "<<"span=\""<<span_<<"\" " <<endl;

}


Shape* ArcShape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
{
    string ArcX1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );//Lire le contenu apres x1
    string ArcY1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );//Lire le contenu apres y1
    string ArcX2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );//Lire le contenu apres x2
    string ArcY2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );//Lire le contenu apres y2
    string Arcstart = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"start" ) );//Lire le contenu apres start
    string Arcspan = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"span" ) );//Lire le contenu apres span

    if(not ArcX1.empty() and not ArcX2.empty() and not ArcY1.empty() and not ArcY2.empty() and not Arcstart.empty() and not Arcspan.empty())
    {
        Box A=Box(atoi(ArcX1.c_str()),atoi(ArcY1.c_str()),atoi(ArcX2.c_str()),atoi(ArcY2.c_str()));
        //Si c'est un nouveau BoxShape, on va creer cela en appelant le CTOR Box, parceque pour le CTOR de ArcShape, il a besoin d'un parametre du type box
        ArcShape* As=new ArcShape(owner,A,atoi(Arcstart.c_str()),atoi(Arcspan.c_str()));
        return As;
    }
    return NULL;

}


TermShape::TermShape(Symbol* owner,string name, int x1, int y1,NameAlign align):Shape(owner),term_(NULL),x_(x1),y_(y1),align_(align)
{
    term_=owner->getCell()->getTerm(name);//CTOR de TermShape est different par rapport aux autres, on retrouve les bons terms mais pas creer
}

TermShape::~TermShape()
{

}

Box TermShape::getBoundingBox()const
{
    return Box(x_,y_,x_+10,y_-10);
}

void  TermShape::toXml ( std::ostream& stream) const
{
    stream << indent   << "<name=\""<<term_->getName()<<"\" "<<"x1=\""<<x_<<"\" "<<"y1=\""<<y_<<"\" "<<"align=\""<<toString(align_)<<"\"> "<<endl;

}

Shape* TermShape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
{
    string TermX1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
    string TermY1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
    string Termname= xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
    string Termalign = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"align" ) );

    if(not TermX1.empty() and not TermY1.empty() and not Termname.empty() and not Termalign.empty())
    {
        TermShape* Ts=new TermShape(owner,Termname,atoi(TermX1.c_str()),atoi(TermY1.c_str()),toNameAlign(Termalign));//Directement appeler le CTOR de TermShape pour creer TermShape
        return Ts;
    }
    return NULL;

}

std::string TermShape::toString    ( NameAlign n)//Methode de conversion
{
    if (n==1) return "top_left";
    else if (n==2) return "top_right";
    else if (n==3) return "bottom_left";
    else if (n==4) return "bottom_right";
    else exit(1);
}
TermShape::NameAlign TermShape::toNameAlign ( string str)//Methode de conversion
{
    if (str=="top_left") return TopLeft;
    else if (str=="top_right") return TopRight;
    else if (str=="bottom_left") return BottomLeft;
    else if (str=="bottom_right") return BottomRight;
    else exit(1);
}

}
