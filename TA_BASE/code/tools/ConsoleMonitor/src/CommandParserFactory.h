#ifndef COMMAND_PARSER_FACTORY_H__
#define COMMAND_PARSER_FACTORY_H__

#include <map>
#include "BaseCommandParser.h" 

namespace TA_Base_Core
{
    class CommandParserFactory
    {
    public:
        ~CommandParserFactory(void);
        void init();
        void uninit();

        BaseCommandParser * getCurrentCommandParser()
        {
            return m_currentCommandParser;
        }

        static CommandParserFactory & getInstance();
        
        void processModuleCommand(const std::vector<std::string> & vtCommandArg);

        
        BaseCommandParser * getCommandParser(const std::string & moduleName)
        {
            return m_commandParserMap[moduleName];
        }

        BaseCommandParser * setCurrentCommandParser(const std::string & moduleName)
        {
            m_currentCommandParser = m_commandParserMap[moduleName];
            return m_currentCommandParser;
        }



    private:
        CommandParserFactory(void);
        bool isCorrectModuleName(const std::string & moduleName);
        std::string listAllModules();

        bool switchModule(const std::string & moduleName, const std::string & entityName, const std::string & endPoint );


        static CommandParserFactory * m_theClassInstance;
        std::map<std::string, BaseCommandParser *> m_commandParserMap;
        BaseCommandParser* m_currentCommandParser;
        static TA_Base_Core::NonReEntrantThreadLockable m_singletonLock;  // Guard m_theClassInstance

    };

}
#endif