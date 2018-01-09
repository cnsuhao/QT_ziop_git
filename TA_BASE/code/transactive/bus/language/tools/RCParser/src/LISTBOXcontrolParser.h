#pragma once
#include "IControlParser.h"


class LISTBOXcontrolParser : public IControlParser
{
public:

    LISTBOXcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new LISTBOXcontrolParser(s) );
    }

private:

    std::string m_string;
};
