#include <sstream>
#include <assert.h>

#include "core/synchronisation/src/ThreadGuard.h"

#include "CommandParserFactory.h"
#include "NSAModuleCommandParser.h"
#include "BaseCommandParser.h"

namespace TA_Base_Core
{
    CommandParserFactory * CommandParserFactory::m_theClassInstance = NULL;
    TA_Base_Core::NonReEntrantThreadLockable CommandParserFactory::m_singletonLock;

    CommandParserFactory::CommandParserFactory(void)
    {
        init();
    }

    CommandParserFactory::~CommandParserFactory(void)
    {
        uninit();
    }

    CommandParserFactory & CommandParserFactory::getInstance()
    {
        TA_Base_Core::ThreadGuard guard(m_singletonLock);
        if ( m_theClassInstance == NULL )
        {
            m_theClassInstance = new CommandParserFactory();
        }
        return *m_theClassInstance;
    }


    void CommandParserFactory::uninit()
    {
        std::map<std::string, BaseCommandParser *>::iterator it;
        for(it = m_commandParserMap.begin(); it != m_commandParserMap.end(); it++)
        {
            BaseCommandParser * parser = it->second;
            assert(parser != NULL);
            delete parser;
        }
        m_commandParserMap.clear();
    }

    void CommandParserFactory::init()
    {
        BaseCommandParser * parser = new BaseCommandParser();
        m_commandParserMap[parser->getModuleName()] = parser;

        parser = new NSAModuleCommandParser();
        m_commandParserMap[parser->getModuleName()] = parser;

        m_currentCommandParser = parser;

    }

    std::string CommandParserFactory::listAllModules()
    {
        std::ostringstream ostr;
        ostr << " all modules:  ";
        std::map<std::string, BaseCommandParser *>::iterator it;
        for(it = m_commandParserMap.begin(); it != m_commandParserMap.end(); it++)
        {
            std::string name = it->first;
            ostr << name.c_str() << " ";
        }

        return ostr.str();
    }

    bool CommandParserFactory::isCorrectModuleName(const std::string & moduleName)
    {
        return (m_commandParserMap.find(moduleName) != m_commandParserMap.end());
    }

    void CommandParserFactory::processModuleCommand(const std::vector<std::string> & vtCommandArg)
    {
        if( vtCommandArg.size() == 1 )
        {
            std::string moduleList = listAllModules();
            std::cout << " " << moduleList.c_str() << std::endl;
        }
        else if( vtCommandArg.size() == 2 && _stricmp( "current", vtCommandArg[1].c_str() ) == 0 )
        {
            std::cout << m_currentCommandParser->getModuleName() << std::endl;
        }
        else if( vtCommandArg.size() == 4 ) 
        {
            if( switchModule(vtCommandArg[1], vtCommandArg[2], vtCommandArg[3] ) )
            {
                std::cout << " switch module successfully!"  << std::endl;        
            }
        }
        else
        {
            std::cout <<" invalid module command" << std::endl; 
        }
    }


    bool CommandParserFactory::switchModule(const std::string & moduleName, const std::string & entityName, const std::string & endPoint )
    {
        if (! isCorrectModuleName(moduleName) )
        {
            std::cout <<  "invalid module name: " << moduleName.c_str() << std::endl;
            return false;
        }
        BaseCommandParser * parser = getCommandParser(moduleName);
        assert( parser!=NULL );

        if ( parser->initCorbaEntity(entityName, endPoint) )
        {
            m_currentCommandParser = parser;
            std::cout << " " << parser->getStateDesc().c_str() << std::endl;
            return true;
        }
        return false;
    }

}