#pragma once
#include "IResourceParser.h"


class STRINGresourceParser : public IResourceParser
{
public:

    STRINGresourceParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IResourcePtr parse();

    static IResourceParserPtr create( const std::string& s )
    {
        return IResourceParserPtr( new STRINGresourceParser(s) );
    }

private:

    std::string m_string;
};
