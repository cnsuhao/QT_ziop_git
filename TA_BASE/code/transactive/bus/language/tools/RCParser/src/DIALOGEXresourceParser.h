#pragma once
#include "IResourceParser.h"


class DIALOGEXresourceParser : public IResourceParser
{
public:

    DIALOGEXresourceParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IResourcePtr parse();

    static IResourceParserPtr create( const std::string& s )
    {
        return IResourceParserPtr( new DIALOGEXresourceParser(s) );
    }

private:

    std::string m_string;
};
