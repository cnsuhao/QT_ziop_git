/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source : $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/scada/proxy_library/src/ScadaProxyManager.cpp $
  * @author  A. Parker
  * @version $Revision: #1 $
  * Last modification : $DateTime: 2015/10/09 15:42:35 $
  * Last modified by : $Author: CM $
  *
  *
  *
  */

#if defined(_MSC_VER)
#pragma warning(disable : 4786 4146 4018 4250 4290)
#endif // defined _MSC_VER

#include "bus/scada/proxy_library/src/ScadaProxyManager.h"
#include "bus/scada/proxy_library/src/DataPointProxyManager.h"
#include "bus/scada/proxy_library/src/DataPointProxySmartPtr.h"
#include "bus/scada/proxy_library/src/DataNodeProxyManager.h"
#include "bus/scada/proxy_library/src/DataNodeProxySmartPtr.h"
#include "bus/scada/proxy_library/src/IDataPointProxy.h"
#include "bus/scada/proxy_library/src/IDataNodeProxy.h"
#include "bus/scada/proxy_library/src/IEntityUpdateEventProcessor.h"

#include "core/exceptions/src/ScadaProxyException.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"

namespace TA_Base_Bus
{
    ScadaProxyManager::ScadaProxyManager(int numWorkers)
    :
    m_observerUpdateQueueProcessor(NULL),
    m_dataPointProxyManager(NULL),
    m_dataNodeProxyManager(NULL)	
    {
		int numberOfWorkers = numWorkers;
        
		if ( true == TA_Base_Core::RunParams::getInstance().isSet ( RPARAM_SCADA_OBSWORKERS ) )
        {
            numberOfWorkers = atoi ( 
				TA_Base_Core::RunParams::getInstance().get( RPARAM_SCADA_OBSWORKERS ).c_str() );
        }

		if (numberOfWorkers < 1)
		{
			numberOfWorkers = 5;
		}

        // Create proxy observer manager instance
        m_observerUpdateQueueProcessor = new ObserverUpdateQueueProcessor(numberOfWorkers);

        // Create the data point managers
        m_dataPointProxyManager = new DataPointProxyManager( m_dataPointSyncManager, *m_observerUpdateQueueProcessor);
		m_dataPointSyncManager.setCallback( *m_dataPointProxyManager );

        // Create the data node managers
        m_dataNodeProxyManager = new DataNodeProxyManager( m_dataNodeSyncManager, *m_observerUpdateQueueProcessor);
		m_dataNodeSyncManager.setCallback( *m_dataNodeProxyManager );

		m_dataPointSyncManager.startSynchronisation();
		m_dataNodeSyncManager.startSynchronisation();
    }


    ScadaProxyManager::~ScadaProxyManager()
    {
		m_dataPointSyncManager.stopSynchronisation();
		m_dataNodeSyncManager.stopSynchronisation();

		m_dataPointSyncManager.removeAllAgentInterfaces();
		m_dataNodeSyncManager.removeAllAgentInterfaces();

		delete m_observerUpdateQueueProcessor;
		m_observerUpdateQueueProcessor =NULL;

        delete m_dataPointProxyManager;
        m_dataPointProxyManager = NULL;
        
		delete m_dataNodeProxyManager;
        m_dataNodeProxyManager = NULL;
    }

    void ScadaProxyManager::registerLocalEntities(std::string agentName, const std::map<unsigned long,IEntity*>* entityMap)
    {
        // Set local entities in synchronisation manager so that
        // it can optimally connect to the real data points and nodes.
        m_dataPointSyncManager.registerLocalEntities( agentName, entityMap );
        m_dataNodeSyncManager.registerLocalEntities( agentName, entityMap );
    }
    
    void ScadaProxyManager::registerEntityData(TA_Base_Core::IEntityDataPtr EntityData, const std::string& entityDataType)
    {
        if ( TA_Base_Core::DataPointEntityData::getStaticType() == entityDataType)
        {
            m_dataPointProxyManager->registerDataPointEntityData( EntityData );
        }
        else if ( TA_Base_Core::DataNodeEntityData::getStaticType() == entityDataType)
        {
            m_dataNodeProxyManager->registerDataNodeEntityData( EntityData );
        }
    }

    void ScadaProxyManager::createDataPointProxy(unsigned long entityKey, 
        TA_Base_Core::DataPointEntityDataPtr entityData, 
        IEntityUpdateEventProcessor& observer,
		DataPointProxySmartPtr & dataPointProxySmartPtr)
    {
		m_dataPointProxyManager->getProxySmartPointer(entityKey, entityData, observer, dataPointProxySmartPtr);
    }



    void ScadaProxyManager::createDataNodeProxy( unsigned long entityKey,
        TA_Base_Core::DataNodeEntityDataPtr entityData, 
        IEntityUpdateEventProcessor& observer,
		DataNodeProxySmartPtr & dataNodeProxySmartPtr,
        bool activateSummaries )
    {
		m_dataNodeProxyManager->getProxySmartPointer(entityKey, entityData, dataNodeProxySmartPtr, observer);
    }
};
