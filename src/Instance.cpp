#include  <cstdlib>
#include  "Cell.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"
#include  "Point.h"
#include "Indentation.h"
#include  "Symbol.h"
#include  "Shape.h"

#include "XmlUtil.h"
#include <fstream>
#include  <libxml/xmlreader.h>
#include  <cstdlib>


using namespace std;

namespace Netlist
{

Instance:: Instance( Cell* owner, Cell* model, const std::string& name )//CTOR de Instance
    :owner_(owner)
    ,masterCell_(model)
    ,name_(name)
    ,terms_() //duplicata du modele
    ,position_()
{
//    for(Term* term:owner_->getTerms())
//        new Term(this,term);

    owner->add(this);//Ajouter l'instances dans Cell
    for ( vector<Term*>::const_iterator iterm=model->getTerms().begin() ; iterm != model->getTerms().end() ; ++iterm )
    {
        new Term(this,(*iterm));//Nouvel element Term
    }

}


Instance::~Instance()//DTOR de Instance
{
    //getCell()->remove(this);
    while(not terms_.empty())
        delete terms_[0];
}

//GETTER
const std::string&        Instance::getName       () const
{
    return name_;
}

Point                     Instance::getPosition   () const
{
    return position_;
}

const std::vector<Term*>& Instance::getTerms      () const
{
    return terms_;
}

Cell*   Instance::getCell()const
{
    return owner_;   //correspond a l'owner
}

Cell* Instance::getMasterCell() const
{
    return masterCell_;
}

Term* Instance::getTerm(const string& term) const
{
    for ( vector<Term*>::const_iterator iterm=terms_.begin() ; iterm != terms_.end() ; ++iterm )
    {
        if ((*iterm)->getName() == term)  return *iterm;//Iterateur pour trouver l'element du vecteur term dans la classe Instance
    }
    return NULL;
}

void Instance::add(Term* term)
{
    if (getTerm(term->getName()))
    {
        cerr << "[ERROR] Attemp to add duplicated terminal <" << term->getName() << ">." << endl;
        exit( 1 );
    }
    terms_.push_back( term );//Ajouter les nouvels elements du vecteur term dans la classe Instance
}

void Instance::remove(Term* term)
{
    for ( vector<Term*>::iterator iterm=terms_.begin() ; iterm != terms_.end() ; ++iterm )
    {
        if (*iterm == term) terms_.erase( iterm );//Supprimer l'element du vecteur term de la classe Instance
    }
}

bool Instance::connect(const string& name, Net* net)
{
    Term* term = getTerm( name );
    if (term == NULL) return false;

    term->setNet( net );
    return true;
}

//SETTER
void  Instance::setPosition   ( const Point& p)//Changer la methode de setPosition pour aussi changer la position de termshape
{
    //position_ = p;
    int x1_inst=p.getX();
    int y1_inst=p.getY();//Trouver les donnees x et y en forme point
    vector<Term*> terms=this->getTerms();//Creer terms et trouver this par instance
    size_t i;
    for(i=0; i<terms.size(); ++i) //boucle en longeur du vecteur terms
    {
        Shape* shapes=this->getMasterCell()->getSymbol()->getTermShape(terms[i]);//Creer shapes et trouver this par la methode getTermShape

        if(shapes != NULL) //ce qui signifie qu'il existe
        {
            TermShape* termshape=static_cast<TermShape*>(shapes);//conversion en utilisant static_cast
            int x1=termshape->getX();
            int y1=termshape->getY();//Trouver x1 et y1 dans la classe TermShape
            terms[i]->setPosition(x1+x1_inst,y1+y1_inst);//Et appeler la methode setPosition de Term pour changer la position de term
        }
    }
    position_=p;
}

void  Instance::setPosition   ( int x, int y )
{
    //position_ = Point(x,y);

    vector<Term*> terms=this->getTerms();//Le meme principe qu'on a fait en haut
    size_t i;
    for(i=0; i<terms.size(); ++i)
    {
        //std::cout << "FONCTION TEST" << std::endl;
        Shape* shapes=this->getMasterCell()->getSymbol()->getTermShape(terms[i]);

        if(shapes != NULL)
        {
            TermShape* termshape=static_cast<TermShape*>(shapes);
            int x1=termshape->getX();
            int y1=termshape->getY();
            //std::cout << "INS TERM X " << x1+x << " Y : " << y1 + y << std::endl;
            terms[i]->setPosition(x1+x,y1+y);
        }
    }
    position_=Point(x,y);

}

void  Instance::toXml ( ostream& stream ) const
{

    stream << indent   <<"<instance name=\"" << name_ << "\" "<< "mastercell=\""<<masterCell_->getName()<<"\" "<< "x=\""<<position_.getX()<<"\" " << "y=\""<<position_.getY() << "\"/>"<<endl;

}

Instance* Instance::fromXml ( Cell* C, xmlTextReaderPtr reader)
{
    string instName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );//Appeler reader pour trouver la valeur apres name
    string instMast = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"mastercell" ) );//Appeler reader pour trouver la valeur apres mastercell
    string instX = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x" ) );//Appeler reader pour trouver la valeur apres x
    string instY = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y" ) );//Appeler reader pour trouver la valeur apres y

    if (not instName.empty() and not instMast.empty() and not instX.empty() and not instY.empty() )  //Si on retrouve tous les elements, on peut donc creer les instances
    {
        Instance *I=new Instance(C,C->find(instMast),instName.c_str());//Appeler la fonction de CTOR d'instance pour creer la nouvelle instance
        I->setPosition(atoi(instX.c_str()),atoi(instY.c_str()));//Appeler la fonction setPosition pour changer la valeur de position par la fonction atoi(convertir str en int)

        return I;
    }
    return NULL;

}

}
