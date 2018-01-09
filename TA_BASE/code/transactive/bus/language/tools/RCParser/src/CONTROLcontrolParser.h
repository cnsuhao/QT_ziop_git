#pragma once
#include "IControlParser.h"


class CONTROLcontrolParser : public IControlParser
{
public:

    CONTROLcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new CONTROLcontrolParser(s) );
    }

private:

    std::string m_string;
};
