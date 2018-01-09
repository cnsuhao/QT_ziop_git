#pragma once
#include <string>


struct CtlConfig
{
    CtlConfig()
        : visiblity( true ),
          actionID( "NULL" ),
          maximumLength( -1 ),
          enableRTrim( true )
    {
    }

    std::string id;
    bool visiblity;
    std::string caption;
    std::string actionID;
    std::string tooltip;
    std::string helpFile;
    int maximumLength;
    bool enableRTrim;
    std::string styleSheet;
};


class CtlConfigParser
{
public:

    static void load( const std::string& file );
    static void unload( const std::string& file = "" );
    static CtlConfig* get( const std::string& id );
};


#define loadCtlConfigFile(file)    CtlConfigParser::load(file)
#define unloadCtlConfigFile(file)  CtlConfigParser::unload(file)
#define getCtlConfig(id)           CtlConfigParser::get(id)
