#pragma once
#include "IControlParser.h"


class RTEXTcontrolParser : public IControlParser
{
public:

    RTEXTcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new RTEXTcontrolParser(s) );
    }

private:

    std::string m_string;
};
