/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/scada/proxy_library/src/AgentSynchronisationInterface.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
///////////////////////////////////////////////////////////
//  AgentSynchronisationInterface.cpp
//  Implementation of the Class AgentSynchronisationInterface
//  Created on:      28-Jun-2004 03:51:38 PM
///////////////////////////////////////////////////////////

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif // defined (_MSC_VER)

#include "bus/scada/proxy_library/src/AgentSynchronisationInterface.h"
#include "bus/scada/proxy_library/src/DataPointProxyManager.h"
#include "bus/scada/proxy_library/src/DataNodeProxyManager.h"

#include "core/synchronisation/src/ThreadGuard.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"

using TA_Base_Core::DebugUtil;

namespace TA_Base_Bus
{

	/**
	 * Constructor
	 * @param agentName
	 * 
	 */
	AgentSynchronisationInterface::AgentSynchronisationInterface(std::string agentName, unsigned long locationKey)
    :
    m_agentName(agentName),
    m_terminate(false),
    m_locationKey(locationKey),
    m_synchronisationEnabled(false),
	m_isConnected(true) //TD15939
	{
	}


	AgentSynchronisationInterface::~AgentSynchronisationInterface()
	{
	}

	void AgentSynchronisationInterface::enableSynchronisation(bool enabled) 
	{
	   m_synchronisationEnabled = enabled;
	}

	
	void AgentSynchronisationInterface::add(unsigned long entityKey)
	{
        RequestItem item;

        item.requestType = Add;
        item.entityKey   = entityKey;

        queueRequest(item);
	}


	void AgentSynchronisationInterface::remove(unsigned long entityKey)
	{
        // We could check the queue for any pending adds and remove them
        // This might improve efficiency when queue is blocked and then 
        // agent starts working again


        //if there are alot of adds and removes in the queue, we shouldnt bother processing them
        //so we will clear the queue of all requests related to this point
        removeOldRequests(entityKey);
        RequestItem item;

        item.requestType = Remove;
        item.entityKey   = entityKey;

        queueRequest(item);
	}


	void AgentSynchronisationInterface::removeOldRequests(unsigned long entityKey)
	{
        TA_THREADGUARD(m_requestListLock);
        std::vector<RequestItem>::iterator it = m_requestList.begin();

        while(it != m_requestList.end())
        {
            if ( it->entityKey == entityKey )
            {
                it = m_requestList.erase(it);
            }
            else
            {
                it++;
            }
        }
	}

    
	AgentSynchronisationInterface::RequestItem AgentSynchronisationInterface::getNextRequest()
	{
	   RequestItem item;

	   TA_THREADGUARD(m_requestListLock);
	   if (m_requestList.size()<1)
	   {
		  item.requestType = EmptyQueue;
		  item.entityKey   = 0;
	   }
	   else
	   {
		  item = m_requestList.at(0);
		  m_requestList.erase(m_requestList.begin());
	   }

	   return item;
	}


	void AgentSynchronisationInterface::terminate()
	{
	   m_terminate = true;
	   m_requestListSemaphore.post();
	}


	void AgentSynchronisationInterface::queueRequest(RequestItem request)
	{
	   // Lock queue
	   TA_THREADGUARD(m_requestListLock);
	   m_requestList.push_back(request);

	   // Signal waiting thread
	   m_requestListSemaphore.post();
	}
}
