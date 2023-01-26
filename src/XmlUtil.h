#ifndef NETLIST_XMLUTIL_H
#define NETLIST_XMLUTIL_H

#include <libxml/xmlreader.h>
#include <string>
#pragma once

namespace Netlist
{


std::string  xmlCharToString    ( xmlChar* );
bool         xmlGetIntAttribute ( xmlTextReaderPtr, const std::string& attribute, int& value );


}  // Netlist namespace.

#endif  // NETLIST_XMLUTIL_H
