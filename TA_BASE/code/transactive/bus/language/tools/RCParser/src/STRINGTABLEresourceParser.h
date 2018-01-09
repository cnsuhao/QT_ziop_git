#pragma once
#include "IResourceParser.h"


class STRINGTABLEresourceParser : public IResourceParser
{
public:

    STRINGTABLEresourceParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IResourcePtr parse();

    static IResourceParserPtr create( const std::string& s )
    {
        return IResourceParserPtr( new STRINGTABLEresourceParser(s) );
    }

private:

    std::string m_string;
};
