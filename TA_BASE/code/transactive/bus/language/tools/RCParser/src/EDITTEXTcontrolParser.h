#pragma once
#include "IControlParser.h"


class EDITTEXTcontrolParser : public IControlParser
{
public:

    EDITTEXTcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new EDITTEXTcontrolParser(s) );
    }

private:

    std::string m_string;
};
