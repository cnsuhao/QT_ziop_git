#pragma once
#include "IControlParser.h"


class CHECKBOXcontrolParser : public IControlParser
{
public:

    CHECKBOXcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new CHECKBOXcontrolParser(s) );
    }

private:

    std::string m_string;
};
