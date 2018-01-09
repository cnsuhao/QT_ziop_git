/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/DataNodeAgentProxy.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  */
// DataNodeAgentProxy.h: interface for the DataNodeAgentProxy class.
//
//////////////////////////////////////////////////////////////////////

#ifndef EDSV_DATANODE_AGENT_PROXY_H
#define EDSV_DATANODE_AGENT_PROXY_H

#include <set>
#include <string>
#include <sys/timeb.h>
#include "core/naming/src/NamedObject.h"
#include "bus/scada/common_library/IDL/src/IDataNodeAgentCorbaDef.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentItemPopulateLogic.h"

namespace TA_Base_App
{
	class EquipmentItemPopulateLogic;

    class DataNodeAgentProxy : public TA_Base_Core::Thread      
    {
    public:
        typedef TA_Base_Core::NamedObject< TA_Base_Bus::IDataNodeAgentCorbaDef,
                                           TA_Base_Bus::IDataNodeAgentCorbaDef_ptr,
                                           TA_Base_Bus::IDataNodeAgentCorbaDef_var >   DataNodeAgentInterface;

		DataNodeAgentProxy(std::string & agentName, EquipmentItemPopulateLogic* populateLogic);

	    virtual ~DataNodeAgentProxy();

        virtual void run();
        
        virtual void terminate();

		void pausePoll();

		void resumePoll();

		void setToBadNotConnectedQuality();
                
    private:
		volatile bool							m_pollSwitchOn;
        volatile bool							m_toBeTerminated;
		DataNodeAgentInterface*                 m_agentDnInterface;
		unsigned long							m_observerId;
		std::set<unsigned long>					m_storeOITPDn;
		EquipmentItemPopulateLogic*				m_populateLogic;
    };
}
#endif // DATANODE_AGENT_PROXY_H
