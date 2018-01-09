#pragma once
#include "IControlParser.h"


class RADIOBUTTONcontrolParser : public IControlParser
{
public:

    RADIOBUTTONcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new RADIOBUTTONcontrolParser(s) );
    }

private:

    std::string m_string;
};
