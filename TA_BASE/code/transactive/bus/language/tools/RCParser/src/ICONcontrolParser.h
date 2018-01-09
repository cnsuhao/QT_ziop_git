#pragma once
#include "IControlParser.h"


class ICONcontrolParser : public IControlParser
{
public:

    ICONcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new ICONcontrolParser(s) );
    }

private:

    std::string m_string;
};
