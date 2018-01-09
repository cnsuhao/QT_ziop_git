#pragma once
#include "IResourceParser.h"


class TOOLBARresourceParser : public IResourceParser
{
public:

    TOOLBARresourceParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IResourcePtr parse();

    static IResourceParserPtr create( const std::string& s )
    {
        return IResourceParserPtr( new TOOLBARresourceParser(s) );
    }

private:

    std::string m_string;
};
