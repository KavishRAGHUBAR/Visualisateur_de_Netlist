#include  "Node.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"
#include  "Cell.h"
#include "Indentation.h"



using namespace std;

namespace Netlist
{

Term::Term(Cell* cell, const std::string& name,Direction direction)//CTOR de la classe Term s'il est external
    : owner_    (cell)//owner est cell
    , name_     (name)
    , direction_(direction)
    , type_     (External)
    , net_      (NULL)
    , node_     (this)//Si on n'a pas de valeur initiale pour node_
{
    cell ->add(this);//Ajoute dans cell
}

Term::Term(Instance* instance,const Term* modelTerm)//CTOR de la classe Term s'il est internal
    : owner_    (instance)//owner est instance
    , name_     (modelTerm->getName())
    , direction_(modelTerm->getDirection())
    , type_     (Internal)
    , net_      (NULL)
    , node_     (this)
{
    instance ->add(this);//Ajoute dans instance
}

Term::~Term()//DTOR de la classe Term
{
    if(getCell()) getCell()->remove(this);//Supprimer la classe Term external s'il est dans cell
    if(getInstance()) getInstance()->remove(this);//Suppression de la classe Term internal s'il est dans instance
}

bool Term::isExternal() const//Chercher s'il est external
{
    if (type_==2) return true;
    else return false;
}
bool Term::isInternal() const//Chercher s'il est internal
{
    if (type_==1) return true;
    else return false;
}
const string& Term::getName() const
{
    return name_;
}

//GETTER
Node* Term::getNode()
{
    return &(node_);
}
Net* Term::getNet() const
{
    return net_;
}

Term::Type Term::getType() const
{
    return type_;
}
void Term::setDirection (Direction direction)
{
    direction_=direction;
}
Cell* Term::getCell() const
{
    return (type_==External) ?static_cast <Cell*>(owner_) //en fonction du type, on fait un static_cast en Cell* ou Instance*
           :NULL;
}

Instance* Term::getInstance() const
{
    return (type_==Internal) ?static_cast <Instance*>(owner_)
           :NULL;
}

Cell* Term::getOwnerCell() const
{
    if(isExternal())
    {
        return static_cast <Cell*>(owner_);
    }
    else if(isInternal())
    {
        return static_cast <Instance*>(owner_)->getCell();
    }

    return NULL;
}

Term::Direction Term::getDirection()const
{
    return direction_;
}
Point Term::getPosition() const
{
    return node_.getPosition();
}

//SETTER
void Term::setNet(Net* net)
{
//if (net){
//
//    if(net -> getCell()!=getOwnerCell()){
//        cerr << "Error" << endl;
//        return;
//    }
//    net_ = net;
//    net_->add(&node_);
//}
    if(net==NULL)//Si on va vider la case net correspondante
    {

        vector <Node *> N = net_->getNodes();//On cherche le vecteur N
        N.insert(N.begin() + node_.getId(), NULL);//Laisse la case correspondante vide
        net_=NULL;
    }
    else//Si on va ajouter une nouvelle valeur de net
    {
        net_=net;
        net_->add(&node_);//Mettre la valeur par la fonction add
    }

}

void Term::setNet(std::string name)
{
    setNet((getOwnerCell()->getNet(name)));



    /*     Cell* c=getOwnerCell();//Trouver la owner cell
     *     net_=c->getNet(name);//Trouver le nom de net dans le owner cell
     *     if(net_!=NULL)//v�rification si net_ est vide
     *     {
     *         net_->add(&node_);
     *     }
     */
}
void  Term::setPosition  ( const Point& p)
{
    getNode()->setPosition(p);
    //node_.setPostion(p);
}
void  Term::setPosition  ( int x, int y )
{
    getNode()->setPosition(x,y);
}

std::string  Term::toString    ( Term::Type t)//Convertir le type du term du nombre en string
{
    if (t==1) return "Internal";
    else if(t==2) return "External";
    else return NULL;
}
string  Term::toString(Direction d)//Convertir la direction du term du nombre en string
{
    if (d==1) return "In";
    else if (d==2) return "Out";
    else if (d==3) return "Inout";
    else if (d==4) return "Tristate";
    else if (d==5) return "Transcv";
    else if (d==6) return "Unknown";
    else return NULL;
}
Term::Direction Term::toDirection (string str)//Convertir le type du term de string en type de direction
{
    if (str=="In") return In;
    else if (str=="Out") return Out;
    else if (str=="Inout") return Inout;
    else if (str=="Tristate") return Tristate;
    else if (str=="Transcv") return Transcv;
    else if (str=="Unknown") return Unknown;
    else exit(1);
}

Term::Type    Term::toType      ( string str)
{
    if(str=="External") return External;
    else if (str=="Internal") return Internal;
    else exit(1);
}


void  Term::toXml ( ostream& stream ) const
{

    stream << indent   <<"<term name=\"" << name_ << "\" "<< "direction=\""<<toString(direction_) << "\"/>" << endl;

}

Term* Term::fromXml ( Cell* C, xmlTextReaderPtr reader)
{
    string termName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );//Appeler reader pour trouver la valeur apres name
    string termDirection = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"direction" ) );//Appeler reader pour trouver la valeur apres direction
    string termX = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x" ) );//Appeler reader pour trouver la valeur apres x
    string termY = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y" ) );//Appeler reader pour trouver la valeur apres y

    if (not termName.empty() and not termDirection.empty() and not termX.empty() and not termY.empty() )
    {
        Term *T=new Term(C,termName,Term::toDirection(termDirection));//Creer un nouveau term par la fonction de CTOR de TERM
        T->setPosition(atoi(termX.c_str()),atoi(termY.c_str()));//Changer la position de term

        return T;
    }
    return NULL;

}

}
