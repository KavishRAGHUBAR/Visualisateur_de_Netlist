#ifndef NETLIST_NET_H
#define NETLIST_NET_H
#include <vector>
#include <string>

#include  "Term.h"
#include  "Instance.h"
#include  "Point.h"
#include "Indentation.h"
#pragma once


//La classe Net
namespace Netlist
{
class Cell;
class Node;//forward declaration des classes necessaires dans la classe net
class Line;

class Net
{
private :
    Cell* owner_;
    std::string name_; //pour l'utilisateur
    unsigned int id_; //Eviter des collisions de nom
    Term::Type type_; //External si connecte a au moins un Term External
    std::vector<Node*> nodes_;
    std::vector<Line*> lines_;
public:
    Net( Cell*, const std::string&, Term::Type );
    ~Net();
    std::string getName() const;
    Cell* getCell() const;
    unsigned int getId() const;
    Term::Type getType();
    const std::vector<Node*>& getNodes() const;
    size_t getFreeNodeId(); // premier index libre dans le tableau
    void add(Node*);
    bool remove(Node*);

    inline const std::vector<Line*>& getLines () const
    {
        return lines_;
    }
    void add ( Line* );

    bool remove ( Line* );


    Node* getNode(int id)const;


    void  toXml ( std::ostream&)const;
    static Net* fromXml ( Cell*,xmlTextReaderPtr );
};//fin de la classe Net


}
#endif
