#pragma once
#include "Parameter.h"


struct CommandLineHelper
{
public:

    CommandLineHelper( int argc, char* argv[] );
    const ParameterPtrList& get_parameter_list() { return m_parameter_list; }

private:

    void build_application_id_map();
    void parse_command_line(int argc, char* argv[]);
    void display_usage();
    std::string search_file_by_extension_in_current_directory( const std::string& extension = ".rc" );
    std::string get_application_id_by_rc( const std::string& rc );
    std::string get_rc_h_by_rc( const std::string& rc );
    std::string get_language_by_rc( const std::string& rc );

private:

    ParameterPtrList m_parameter_list;
    std::map<std::string, std::string> m_application_id_map;
};
