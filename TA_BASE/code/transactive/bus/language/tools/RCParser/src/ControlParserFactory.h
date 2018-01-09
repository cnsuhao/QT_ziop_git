#pragma once
#include "IControlParser.h"


class ControlParserFactory
{
public:

    ControlParserFactory( const std::string& s );

    IControlPtrList parse();

private:

    IControlParserPtrList create_control_parsers();
    IControlParserPtr create_control_parser( const std::string& control_type, const std::string& control_string );

private:

    std::string m_string;

    typedef IControlParserPtr ( *create_fun )( const std::string& );
    std::map<std::string, create_fun> m_create_map;
};
