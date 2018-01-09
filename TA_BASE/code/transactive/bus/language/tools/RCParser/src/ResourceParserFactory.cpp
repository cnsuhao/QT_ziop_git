#include "StdAfx.h"
#include "ResourceParserFactory.h"
#include "DIALOGresourceParser.h"
#include "DIALOGEXresourceParser.h"
#include "MENUresourceParser.h"
#include "STRINGTABLEresourceParser.h"
#include "TOOLBARresourceParser.h"
#include "RegexFactory.h"


ResourceParserFactory::ResourceParserFactory( const std::string& s )
    : m_string( s )
{
    m_create_map[ "DIALOG" ]        = &DIALOGresourceParser::create;
    m_create_map[ "DIALOGEX" ]      = &DIALOGEXresourceParser::create;
    m_create_map[ "MENU" ]          = &MENUresourceParser::create;
    m_create_map[ "STRINGTABLE" ]   = &STRINGTABLEresourceParser::create;
    m_create_map[ "TOOLBAR" ]       = &TOOLBARresourceParser::create;
}


IResourceParserPtrList ResourceParserFactory::create_resource_parsers()
{
    IResourceParserPtrList resource_parsers;

    #define resource_key_words "DIALOG|DIALOGEX|MENU|STRINGTABLE|TOOLBAR"

    const char* resource_regex_str =
        "(?x)"
        "^"
        "(\\w+ \\s+)?"                      // $1 id
        "(" resource_key_words ") \\s+"     // $2 type
        ".*?"
        "^BEGIN$"
        ".*?"
        "^END$"
        "";

    const boost::regex& resource_regex = RegexFactory::instance().create_regex( resource_regex_str );
    boost::sregex_iterator it( m_string.begin(), m_string.end(), resource_regex );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        const std::string& resource_type    = it->str(2);
        const std::string& resource_string  = it->str();

        IResourceParserPtr parser = create_resource_parser( resource_type, resource_string );

        if ( parser )
        {
            resource_parsers.push_back( parser );
        }
    }

    return resource_parsers;
}


IResourceParserPtr ResourceParserFactory::create_resource_parser( const std::string& resource_type, const std::string& resource_string )
{
    return m_create_map[ resource_type ]( resource_string );
}


IResourcePtrList ResourceParserFactory::parse()
{
    IResourcePtrList resources;
    IResourceParserPtrList resource_parsers = create_resource_parsers();

    for ( size_t i = 0; i < resource_parsers.size(); ++i )
    {
        IResourcePtr resource = resource_parsers[i]->parse();

        if ( resource )
        {
            resources.push_back( resource );
        }
    }

    return resources;
}
