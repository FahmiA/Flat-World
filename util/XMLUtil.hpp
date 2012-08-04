#ifndef XMLUTIL_H
#define XMLUTIL_H

#include <string>
using namespace std;

#include "tinyxml/tinyxml.h"

class XMLUtil
{
    public:
        string getString(TiXmlHandle &parent, const char* tag);
        int getInt(TiXmlHandle &parent, const char* tag);
        float getFloat(TiXmlHandle &parent, const char* tag);
};

#endif // XMLUTIL_H
