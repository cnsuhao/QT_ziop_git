#pragma once
#include "IResourceParser.h"


class DIALOGresourceParser : public IResourceParser
{
public:

    DIALOGresourceParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IResourcePtr parse();

    static IResourceParserPtr create( const std::string& s )
    {
        return IResourceParserPtr( new DIALOGresourceParser(s) );
    }

private:

    std::string m_string;
};
