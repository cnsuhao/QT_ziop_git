#pragma once


struct Utility
{
    static std::string get_string_from_file( const std::string& file_path );
    static std::vector<std::string> get_file_list( const std::string& the_path ); // file or directory
    static void remove_comments( std::string& s );
};
