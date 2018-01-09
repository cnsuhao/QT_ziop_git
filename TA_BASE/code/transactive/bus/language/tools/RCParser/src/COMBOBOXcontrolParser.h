#pragma once
#include "IControlParser.h"


class COMBOBOXcontrolParser : public IControlParser
{
public:

    COMBOBOXcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new COMBOBOXcontrolParser(s) );
    }

private:

    std::string m_string;
};
