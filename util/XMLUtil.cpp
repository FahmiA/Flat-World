#include "XMLUtil.hpp"

string XMLUtil::getString(TiXmlHandle &parent, const char* tag)
{
    TiXmlElement* element = parent.FirstChildElement(tag).ToElement();
    if(element)
        return element->GetText();
    else
        return "-1";
}

int XMLUtil::getInt(TiXmlHandle &parent, const char* tag)
{
    string text = getString(parent, tag);
    return atoi(text.c_str());
}

float XMLUtil::getFloat(TiXmlHandle &parent, const char* tag)
{
    string text = getString(parent, tag);
    return atof(text.c_str());
}
