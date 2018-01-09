#pragma once
#include <string>
#include <vector>


namespace TA_Base_Bus
{
    struct TransActiveMessageConfig
    {
        TransActiveMessageConfig()
            : obsolete( false ),
              auditEvent( false ),
              id( 0 ),
              messageType( 0 ),
              helpID( 0 ),
              messageBoxStyle( 0 )
        {
        }

        bool obsolete;
        bool auditEvent;
        int id;
        std::string resID;
        int messageType;
        int helpID;
        int messageBoxStyle;
        std::string string;
        std::vector<std::string> parameters;
    };

    struct TransActiveMessageConfigParser
    {
        static void load( const std::string& fileName );
        static TransActiveMessageConfig* get( int id );
    };
}

#define loadMessageConfigFile(fileName) TA_Base_Bus::TransActiveMessageConfigParser::load(fileName)
#define getMessageConfig(id) TA_Base_Bus::TransActiveMessageConfigParser::get(id)
