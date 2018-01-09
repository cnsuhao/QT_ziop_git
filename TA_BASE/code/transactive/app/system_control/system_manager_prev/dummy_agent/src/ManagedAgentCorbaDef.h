/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/system_manager_prev/dummy_agent/src/ManagedAgentCorbaDef.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
#if !defined(MANAGEDAGENTCORBADEF_H)
#define MANAGEDAGENTCORBADEF_H

#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include "core/process_management/IDL/src/IManagedAgentCorbaDef.h"
#include "core/corba/src/ServantBase.h"
#include "core/threads/src/Thread.h"

class ManagedAgentCorbaDef : public POA_TA_Base_Core::IManagedAgentCorbaDef,
                             public TA_Base_Core::ServantBase,
                             public TA_Base_Core::Thread
{

public:
    ManagedAgentCorbaDef();
    ~ManagedAgentCorbaDef();

    void setOperationMode(TA_Base_Core::EOperationMode mode);
    TA_Base_Core::EOperationMode getOperationMode();
    void notifyGroupOffline(const char* group);
    void notifyGroupOnline(const char* group);

    void run();
    void terminate();

    TA_Base_Core::IManagedAgentCorbaDef_ptr getManagedAgent()
    {
        return this->_this();
    }

    void invalidateNotifyHosts(const TA_Base_Core::IManagedAgentCorbaDef::NotifyHostSequence& notifyHosts)
	{
	}

private:
    TA_Base_Core::EOperationMode m_operationMode;
    bool m_isRunning;
};

#endif // MANAGEDAGENTCORBADEF_H
