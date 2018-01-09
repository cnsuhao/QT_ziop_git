#include "StdAfx.h"
#include "Utility.h"


std::string Utility::get_string_from_file( const std::string& file_path )
{
    std::ifstream is( file_path.c_str() );

    if ( is )
    {
        return std::string( std::istreambuf_iterator<char>(is), ( std::istreambuf_iterator<char>() ) );
    }
    else
    {
        return "";
    }
}


void Utility::remove_comments( std::string& s )
{
    std::stringstream strm;
    size_t n = s.size();
    char c = 0;
    int state = 0;

    for ( size_t i = 0; i < n; ++i )
    {
        c = s[i];

        if (state == 0 && c == '/')             // ex. [/]
        {
            state = 1;
        }
        else if (state == 1 && c == '*')        // ex. [/*]
        {
            state = 2;
        }
        else if (state == 1 && c == '/')        // ex. [//]
        {
            state = 4;
        }
        else if (state == 1)                    // ex. [<secure/_stdio.h> or 5/3]
        {
            strm << '/';
            state = 0;
        }
        else if (state == 2 && c == '*')        // ex. [/*he*]
        {
            state = 3;
        }
        else if (state == 2)                    // ex. [/*heh]
        {    state = 2;
        }
        else if (state == 3 && c == '/')        // ex. [/*heh*/]
        {
            state = 0;
        }
        else if (state == 3)                    // ex. [/*heh*e]
        {
            state = 2;
        }
        else if (state == 4 && c == '\\')       // ex. [//hehe\]
        {
            state = 9;
        }
        else if (state == 9 && c == '\\')       // ex. [//hehe\\\\\]
        {
            state = 9;
        }
        else if (state == 9)                    // ex. [//hehe\<enter> or //hehe\a]
        {
            state = 4;
        }
        else if (state == 4 && c == '\n')       // ex. [//hehe<enter>]
        {
            state = 0;
        }
        else if (state == 0 && c == '\'')       // ex. [']
        {
            state = 5;
        }
        else if (state == 5 && c == '\\')       // ex. ['\]
        {
            state = 6;
        }
        else if (state == 6)                    // ex. ['\n or '\' or '\t etc.]
        {
            state = 5;
        }
        else if (state == 5 && c == '\'')       // ex. ['\n' or '\'' or '\t' ect.]
        {
            state = 0;
        }
        else if (state == 0 && c == '\"')       // ex. ["]
        {
            state = 7;
        }
        else if (state == 7 && c == '\\')       // ex. ["\]
        {
            state = 8;
        }
        else if (state == 8)                    // ex. ["\n or "\" or "\t ect.]
        {
            state = 7;
        }
        else if (state == 7 && c == '\"')       // ex. ["\n" or "\"" or "\t" ect.]
        {
            state = 0;
        }

        if ( (state == 0 && c != '/') || state == 5 || state == 6 || state == 7 || state == 8 )
        {
            strm << c;
        }
    }

    s = strm.str();
}
