#pragma once
#include "IControlParser.h"


class GROUPBOXcontrolParser : public IControlParser
{
public:

    GROUPBOXcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new GROUPBOXcontrolParser(s) );
    }

private:

    std::string m_string;
};
