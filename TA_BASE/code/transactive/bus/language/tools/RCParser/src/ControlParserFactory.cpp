#include "StdAfx.h"
#include "ControlParserFactory.h"
#include "CHECKBOXcontrolParser.h"
#include "COMBOBOXcontrolParser.h"
#include "CONTROLcontrolParser.h"
#include "CTEXTcontrolParser.h"
#include "DEFPUSHBUTTONcontrolParser.h"
#include "EDITTEXTcontrolParser.h"
#include "GROUPBOXcontrolParser.h"
#include "ICONcontrolParser.h"
#include "LISTBOXcontrolParser.h"
#include "LTEXTcontrolParser.h"
#include "PUSHBOXcontrolParser.h"
#include "PUSHBUTTONcontrolParser.h"
#include "RADIOBUTTONcontrolParser.h"
#include "RTEXTcontrolParser.h"
#include "RegexFactory.h"


ControlParserFactory::ControlParserFactory( const std::string& s )
    : m_string( s )
{
    m_create_map[ "CHECKBOX" ]      = &CHECKBOXcontrolParser::create;
    m_create_map[ "COMBOBOX" ]      = &COMBOBOXcontrolParser::create;
    m_create_map[ "CONTROL" ]       = &CONTROLcontrolParser::create;
    m_create_map[ "CTEXT" ]         = &CTEXTcontrolParser::create;
    m_create_map[ "DEFPUSHBUTTON" ] = &DEFPUSHBUTTONcontrolParser::create;
    m_create_map[ "EDITTEXT" ]      = &EDITTEXTcontrolParser::create;
    m_create_map[ "GROUPBOX" ]      = &GROUPBOXcontrolParser::create;
    m_create_map[ "ICON" ]          = &ICONcontrolParser::create;
    m_create_map[ "LISTBOX" ]       = &LISTBOXcontrolParser::create;
    m_create_map[ "LTEXT" ]         = &LTEXTcontrolParser::create;
    m_create_map[ "PUSHBOX" ]       = &PUSHBOXcontrolParser::create;
    m_create_map[ "PUSHBUTTON" ]    = &PUSHBUTTONcontrolParser::create;
    m_create_map[ "RADIOBUTTON" ]   = &RADIOBUTTONcontrolParser::create;
    m_create_map[ "RTEXT" ]         = &RTEXTcontrolParser::create;
}


IControlParserPtrList ControlParserFactory::create_control_parsers()
{
    IControlParserPtrList control_parsers;

    #define control_key_words "CHECKBOX|COMBOBOX|CONTROL|CTEXT|DEFPUSHBUTTON|EDITTEXT|GROUPBOX|ICON|LISTBOX|LTEXT|PUSHBOX|PUSHBUTTON|RADIOBUTTON|RTEXT"

    const char* control_regex_str =
        "(?x)"
        "^ \\s+ (" control_key_words ") \\s+"
        "";

    const boost::regex& control_regex = RegexFactory::instance().create_regex( control_regex_str );
    boost::sregex_iterator it( m_string.begin(), m_string.end(), control_regex );
    boost::sregex_iterator end;
    std::vector< std::pair<std::string, size_t> > control_info_list; // type, position

    for ( ; it != end; ++it )
    {
        control_info_list.push_back( std::make_pair( it->str(1), it->position() ) );
    }

    control_info_list.push_back( std::make_pair( "", m_string.size() ) );

    for ( size_t i = 0; i < control_info_list.size() - 1; ++i )
    {
        const std::string& control_type = control_info_list[i].first;
        const std::string& control_string = m_string.substr( control_info_list[i].second, control_info_list[i + 1].second - control_info_list[i].second );

        IControlParserPtr parser = create_control_parser( control_type, control_string );

        if ( parser )
        {
            control_parsers.push_back( parser );
        }
    }

    return control_parsers;
}


IControlParserPtr ControlParserFactory::create_control_parser( const std::string& control_type, const std::string& control_string )
{
    return m_create_map[ control_type ]( control_string );
}


IControlPtrList ControlParserFactory::parse()
{
    IControlPtrList controls;
    IControlParserPtrList control_parsers = create_control_parsers();

    for ( size_t i = 0; i < control_parsers.size(); ++i )
    {
        IControlPtr control = control_parsers[i]->parse();

        if ( control )
        {
            controls.push_back( control );
        }
    }

    return controls;
}
