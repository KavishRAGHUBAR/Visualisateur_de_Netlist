#ifndef  NETLIST_INDENTATION_H
#define  NETLIST_INDENTATION_H

#include <string>
#include <iostream>
#pragma once

namespace Netlist
{


class Indentation
{
public:
    Indentation ();
    Indentation&  operator++  ();
    Indentation   operator++  (int);
    Indentation&  operator--  ();
    Indentation   operator--  (int);
private:
    size_t        _tabulationSize;
    std::string   _indentation;

    friend std::ostream& operator<< ( std::ostream&, const Indentation& );
};


extern Indentation  indent;


}  // Netlist namespace.

#endif //  NETLIST_INDENTATION_H
