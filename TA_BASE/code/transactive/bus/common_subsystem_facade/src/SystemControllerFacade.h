#ifndef SYSTEM_CONTROLLER_FACADE_H__
#define SYSTEM_CONTROLLER_FACADE_H__

#include "ManagedProcessDataConverter.h"
#include "core/process_management/IDL/src/IProcessManagerCorbaDef.h"

namespace TA_Base_Bus
{
    class IProcessManagerAccessor
    {
    public:
        virtual ~IProcessManagerAccessor(void) {};
        virtual void terminating(TA_Base_Core::ProcessId p_processID, TA_Base_Core::EManagedProcessTerminateCode p_code) = 0;
        virtual void registerManagedProcess(TA_Base_Core::IManagedProcessCorbaDef_ptr, TA_Base_Core::ProcessId, CORBA::ULong, const char*) = 0;
        virtual void registerManagedApplication(TA_Base_Core::IManagedApplicationCorbaDef_ptr p_managedApp, TA_Base_Core::ProcessId p_processId) = 0;
        virtual TA_Base_Core::RunParamSeq* getParams(TA_Base_Core::ProcessId p_processId, const char* hostname) = 0;
        virtual void getParams(TA_Base_Core::ProcessId p_processId, const char* hostname, TA_Base_Core::RunParams::ParamVector& vtRunParamInfo) = 0;
        virtual void poll() = 0;
        virtual void statusChanged(TA_Base_Core::EProcessStatus status, TA_Base_Core::ProcessId p_processId) = 0;
        virtual void setProcessManagerCallTimeout(size_t timeout) = 0;
    };

    class ISystemControllerAdminAccessor
    {
    public:
        virtual ~ISystemControllerAdminAccessor(void) {};
        virtual TA_Base_Core::ProcessDataSeq* getProcessData() = 0;
        virtual void getProcessData(std::vector<ManagedProcessData>& processData) = 0;
        virtual void shutdown(const char* hostname) = 0;
        virtual void startProcess(const char* entityName) = 0;
        virtual void changeAgentOperationMode(const char* entityName, TA_Base_Core::EOperationMode operationMode) = 0;
        virtual void stopProcess(const char* entityName) = 0;
        virtual void setRunParam(const char* entityName, const TA_Base_Core::RunParam& param) = 0;
        virtual void setRunParam(const char* entityName, const TA_Base_Core::RunParams::ParamNameValue& param) = 0;
        virtual TA_Base_Core::AgentStatusEnum getAgentsStatus() = 0;
        virtual void setAdminCallTimeout(size_t timeout) = 0;
    };

    class ISystemControllerAccessor : public ISystemControllerAdminAccessor, public IProcessManagerAccessor
    {
    public:
        virtual bool isValidConfig() = 0;
        virtual void setEndPointInfo(const std::string& hostName, unsigned long portNumber = 0) = 0;
        virtual void resetEndPointInfo() = 0;
        virtual bool ping() = 0;
        virtual std::string getHostName() = 0;
    };

    class SystemControllerAccessor: public ISystemControllerAccessor
    {
    public:
        SystemControllerAccessor();
        virtual ~SystemControllerAccessor(void);
        // -----------------Begin Interface  ISystemControllerAdminAccessor Virtual Method ---------------------
        virtual TA_Base_Core::ProcessDataSeq* getProcessData();
        virtual void getProcessData(std::vector<ManagedProcessData>& processData);
        virtual void shutdown(const char* hostname);
        virtual void startProcess(const char* entityName);
        virtual void changeAgentOperationMode(const char* entityName, TA_Base_Core::EOperationMode operationMode);
        virtual void stopProcess(const char* entityName);
        virtual void setRunParam(const char* entityName, const TA_Base_Core::RunParam& param);
        virtual void setRunParam(const char* entityName, const TA_Base_Core::RunParams::ParamNameValue& param);
        virtual TA_Base_Core::AgentStatusEnum getAgentsStatus();
        virtual void setAdminCallTimeout(size_t timeout);
        // -----------------End Interface  ISystemControllerAdminAccessor Virtual Method ---------------------
        // -----------------Begin Interface  IProcessManagerAccessor Virtual Method ---------------------
        virtual void terminating(TA_Base_Core::ProcessId p_processID, TA_Base_Core::EManagedProcessTerminateCode p_code);
        virtual void registerManagedProcess(TA_Base_Core::IManagedProcessCorbaDef_ptr, TA_Base_Core::ProcessId, CORBA::ULong, const char*);
        virtual void registerManagedApplication(TA_Base_Core::IManagedApplicationCorbaDef_ptr p_managedApp, TA_Base_Core::ProcessId p_processId);
        virtual TA_Base_Core::RunParamSeq* getParams(TA_Base_Core::ProcessId p_processId, const char* hostname);
        virtual void getParams(TA_Base_Core::ProcessId p_processId, const char* hostname, TA_Base_Core::RunParams::ParamVector& vtRunParamInfo);
        virtual void poll();
        virtual void statusChanged(TA_Base_Core::EProcessStatus status, TA_Base_Core::ProcessId p_processId);
        virtual void setProcessManagerCallTimeout(size_t timeout);
        // -----------------End Interface  IProcessManagerAccessor Virtual Method ---------------------
        virtual bool isValidConfig();
        virtual void setEndPointInfo(const std::string& hostName, unsigned long portNumber = 0);
        virtual void resetEndPointInfo();
        virtual bool ping();
        virtual std::string getHostName() { return m_hostName; }
    private:
        void locateSystemControllerAdmin();
        void locateProcessManager();
        TA_Base_Core::ISystemControllerAdminCorbaDef_ptr getSystemControllerAdmin();
        TA_Base_Core::IProcessManagerCorbaDef_ptr getProcessManager();
    private:
        TA_Base_Core::ISystemControllerAdminCorbaDef_var m_systemControllerAdminCorbaDef;
        TA_Base_Core::IProcessManagerCorbaDef_var m_processManagerCorbaDef;
        std::string m_processMgrCorbalocStr;
        std::string m_systemControllerAdminCorbalocStr;
        std::string m_hostName;
        size_t m_adminCallTimeout;
        size_t m_processManagerCallTimeout;
    };
}

#endif
