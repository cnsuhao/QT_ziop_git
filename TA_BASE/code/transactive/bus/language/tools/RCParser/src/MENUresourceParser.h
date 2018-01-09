#pragma once
#include "IResourceParser.h"


class MENUresourceParser : public IResourceParser
{
public:

    MENUresourceParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IResourcePtr parse();

    static IResourceParserPtr create( const std::string& s )
    {
        return IResourceParserPtr( new MENUresourceParser(s) );
    }

private:

    std::string m_string;
};
