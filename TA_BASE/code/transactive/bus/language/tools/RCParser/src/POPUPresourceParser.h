#pragma once
#include "IResourceParser.h"


class POPUPresourceParser : public IResourceParser
{
public:

    POPUPresourceParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IResourcePtr parse();

    static IResourceParserPtr create( const std::string& s )
    {
        return IResourceParserPtr( new POPUPresourceParser(s) );
    }

private:

    std::string m_string;
};
