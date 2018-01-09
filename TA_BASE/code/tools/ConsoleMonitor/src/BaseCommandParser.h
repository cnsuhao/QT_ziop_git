#ifndef _COMMAND_MONITOR_H__
#define _COMMAND_MONITOR_H__

#include <vector>
#include "core/corba/src/CorbaUtil.h"
#include "core/process_management/idl/src/IManagedProcessCorbaDef.h"
#include "core/corba/idl/src/IChannelLocatorCorbaDef.h"

#include "MonitorCommonDataType.h"

namespace TA_Base_Core
{

    class BaseCommandParser
    {
    public:
        BaseCommandParser();
	    virtual ~BaseCommandParser(void);
        virtual void postInit() {};
        virtual std::string analyzeCommandInfo(CommandStateInfo & cmdInfo);
        virtual void displayUsage();
        virtual std::string getStateDesc();

        void parseCommand(const std::vector<std::string> & vtCommandArgs);


        const char * getModuleName();
        bool initCorbaEntity(const std::string & entityName, const std::string & endPoint);

        bool getCatchInput()
        {
            return m_isCatchInput;
        }

        void connectInteractiveObj(const std::vector<std::string> & vtCommandArgs);
        void dumpCorbaReference();

    protected:
        void setRunParam(const char * paramName, const char * paramValue);
        virtual bool processCommand(const std::vector<std::string> & vtCommandArgs);

    private:
        CORBA::Object_ptr resolveManangedProcess(const char * entityName);
        void processInteractiveCommand(const std::vector<std::string> & vtCommandArgs);
    
    protected:
        std::string m_processedAgentEntityName;
        std::string m_processedAgentEndPoint;
        std::string m_moduleName;
        TA_Base_Core::IManagedProcessCorbaDef_var m_managedProcessCorba;
        bool m_isCatchInput; //catch input internally

        TA_Base_Core::ISCSInteractive_var m_currIscsInteractiveObj;

    };
}

#endif