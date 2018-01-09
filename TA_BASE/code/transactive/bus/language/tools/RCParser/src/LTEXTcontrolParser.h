#pragma once
#include "IControlParser.h"


class LTEXTcontrolParser : public IControlParser
{
public:

    LTEXTcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new LTEXTcontrolParser(s) );
    }

private:

    std::string m_string;
};
