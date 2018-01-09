/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/GZL6_TIP/TA_BASE/transactive/app/system_control/system_controller/console_manager/src/ProcessManager.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2012/06/12 13:35:44 $
  * Last modified by:  $Author: builder $
  *
  */
// ProcessManager.h
//
// Demonstration implementation of the ProcessManagerDmIf interface
//
// Author:      Bruce Fountain 08-Feb-2001
// Modified by: $Author: builder $
// Modified on: $DateTime: 2012/06/12 13:35:44 $
// Version:     $Revision: #1 $
//

#pragma once
#include "Singleton.h"
#include "IProcessManagerCorbaDef.h"
#include "IManagedApplicationCorbaDef.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/RunParamUser.h"
#include "core/corba/src/CorbaUtil.h"
#include "core/corba/src/ServantBase.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"

struct SystemControllerAdmin;

class ProcessManager
    : public virtual POA_TA_Base_Core::IProcessManagerCorbaDef,
      public virtual TA_Base_Core::ServantBase,
      public virtual TA_Base_Core::RunParamUser,
      public Singleton<ProcessManager>
{
    friend struct SystemControllerAdmin;

public:

    ProcessManager();

    std::string getRunningApps();
    void terminateApp(unsigned long id);
    void changeMode(bool isControl, unsigned long id);
    void changeGUI(TA_Base_Core::EFocusType focus, unsigned long id);
    void onRunParamChange(const std::string& paramName, const std::string& paramValue);
    bool setClientRunParam(int id, const std::string& paramName, const std::string& paramValue);
    void serverChange(bool isUp, unsigned long id);
    void terminateAndWaitForAll();

protected:

    void terminating(TA_Base_Core::ProcessId p_processID, TA_Base_Core::EManagedProcessTerminateCode p_code);
    void registerManagedProcess(TA_Base_Core::IManagedProcessCorbaDef_ptr p_managedProcess, TA_Base_Core::ProcessId p_processID, CORBA::ULong applicationType, const char* entity);
    void registerManagedApplication(TA_Base_Core::IManagedApplicationCorbaDef_ptr p_managedApp, TA_Base_Core::ProcessId p_processId);
    TA_Base_Core::RunParamSeq* getParams(TA_Base_Core::ProcessId id, const char* hostname);
    void poll();
    void statusChanged(TA_Base_Core::EProcessStatus status, TA_Base_Core::ProcessId p_processId);
    virtual TA_Base_Core::EOperationMode handleDoubleControlBaseOnCentralDb(const char*, CORBA::Boolean, CORBA::ULong) { return TA_Base_Core::NotApplicable; };

protected:

    struct Client
    {
        std::string entity;
        std::string type;
        unsigned long applicationType;
        TA_Base_Core::IManagedProcessCorbaDef_var managedProcess;
    };

    std::string getDescription(unsigned long id);
    Client& getClient(unsigned long id);
    TA_Base_Core::IManagedApplicationCorbaDef_var getApplication(unsigned id);
    void tryCatchAny(boost::function<void()> f, const std::string& msg = "");

protected:

    typedef std::map<TA_Base_Core::ProcessId, Client> ClientMap;
    typedef std::map<TA_Base_Core::ProcessId, TA_Base_Core::IManagedApplicationCorbaDef_var> AppMap;
    AppMap m_apps;
    ClientMap m_clients;
    TA_Base_Core::ReEntrantThreadLockable m_mapLock;
    TA_Base_Core::RunParams::ParamVector m_defaultParams;

    // For debug...
    static const char* toString(TA_Base_Core::EManagedProcessTerminateCode p_code);
};
