/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source : $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/scada/proxy_library/src/ScadaProxyFactory.cpp $
  * @author  A. Parker
  * @version $Revision: #1 $
  * Last modification : $DateTime: 2015/10/09 15:42:35 $
  * Last modified by : $Author: CM $
  *
  */

#if defined(_MSC_VER)
#pragma warning(disable : 4786 4146 4018 4250 4290)
#endif // defined _MSC_VER

#include "bus/scada/proxy_library/src/ScadaProxyFactory.h"
#include "bus/scada/proxy_library/src/ScadaProxyManager.h"
#include "bus/scada/proxy_library/src/DataPointProxyManager.h"
#include "bus/scada/proxy_library/src/DataPointProxySmartPtr.h"
#include "bus/scada/proxy_library/src/DataNodeProxyManager.h"
#include "bus/scada/proxy_library/src/DataNodeProxySmartPtr.h"
#include "bus/scada/proxy_library/src/IDataPointProxy.h"
#include "bus/scada/proxy_library/src/IDataNodeProxy.h"
#include "bus/scada/proxy_library/src/IEntityUpdateEventProcessor.h"

#include "core/data_access_interface/entity_access/src/DataPointEntityData.h"
#include "core/data_access_interface/entity_access/src/DataNodeEntityData.h"
#include "core/exceptions/src/ScadaProxyException.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"

namespace TA_Base_Bus
{

    ScadaProxyFactory*              ScadaProxyFactory::m_singleton = NULL;
    unsigned int                    ScadaProxyFactory::m_refCounter = 0;
    TA_Base_Core::ReEntrantThreadLockable ScadaProxyFactory::m_singletonLock;

    ScadaProxyFactory::ScadaProxyFactory()
		: m_scadaProxyManager (NULL)
    {
        int numberOfWorkers = 5;
        if ( true == TA_Base_Core::RunParams::getInstance().isSet ( RPARAM_SCADA_OBSWORKERS ) )
        {
            numberOfWorkers = atoi ( 
				TA_Base_Core::RunParams::getInstance().get( RPARAM_SCADA_OBSWORKERS ).c_str() );
        }

		if (numberOfWorkers < 1)
		{
			numberOfWorkers = 1;
		}

        // Create the ScadaProxyManager
        m_scadaProxyManager = new ScadaProxyManager( numberOfWorkers );
    }


    ScadaProxyFactory::~ScadaProxyFactory()
    {
        delete m_scadaProxyManager;
        m_scadaProxyManager = NULL;
    }
    
    /**
     * ScadaProxyFactory::getInstance()
     *
     * Returns a pointer to the single instance
     * of SingeltonQueue, creating if necessary.
     *
     * @return  a pointer to the ScadaProxyFactory
     *
     */
    ScadaProxyFactory& ScadaProxyFactory::getInstance()
    {
        TA_THREADGUARD( m_singletonLock );

        if ( m_singleton == NULL )
        {
            m_singleton = new ScadaProxyFactory();
        }

        // increment reference counter
        m_refCounter++;

        return *m_singleton;
    }   

    void ScadaProxyFactory::removeInstance()
    {
        TA_THREADGUARD( m_singletonLock );

        TA_ASSERT(m_refCounter > 0, "m_refCounter <= 0");

        // decrement reference counter
        --m_refCounter;

        // if not referenced anywhere
        if ( 0 == m_refCounter )
        {
            if ( m_singleton != NULL )
            {
                delete m_singleton;
                m_singleton = NULL;
            }
        }
    }


    void ScadaProxyFactory::registerLocalEntities(std::string agentName, const std::map<unsigned long,IEntity*>* entityMap)
    {
        m_scadaProxyManager->registerLocalEntities( agentName, entityMap );
    }
    void ScadaProxyFactory::registerEntityData(TA_Base_Core::IEntityDataPtr EntityData, const std::string& entityDataType)
    {
        m_scadaProxyManager->registerEntityData( EntityData, entityDataType );
    }
    

    void ScadaProxyFactory::createDataPointProxy(unsigned long entityKey, 
		                                         IEntityUpdateEventProcessor& observer,
												 DataPointProxySmartPtr & dataPointProxySmartPtr)
    {
        TA_Base_Core::DataPointEntityDataPtr nullData;
		m_scadaProxyManager->createDataPointProxy(entityKey, nullData, observer, dataPointProxySmartPtr);
    }

    void ScadaProxyFactory::createDataPointProxy(TA_Base_Core::DataPointEntityDataPtr entityData, 
		IEntityUpdateEventProcessor& observer,
		DataPointProxySmartPtr & dataPointProxySmartPtr)
    {
        TA_ASSERT(entityData.get() != NULL, "input DataPointEntityData is NULL");

        m_scadaProxyManager->createDataPointProxy(entityData->getKey(), entityData, observer, dataPointProxySmartPtr);
    }
    

    void ScadaProxyFactory::createDataNodeProxy( unsigned long entityKey,
                                                                  IEntityUpdateEventProcessor& observer,
																  DataNodeProxySmartPtr & dataNodeProxySmartPtr,
                                                                  bool activateSummaries )
    {
        TA_Base_Core::DataNodeEntityDataPtr nullData;
        m_scadaProxyManager->createDataNodeProxy(entityKey, nullData, observer, dataNodeProxySmartPtr);
    }

    void ScadaProxyFactory::createDataNodeProxy( TA_Base_Core::DataNodeEntityDataPtr entityData,
        IEntityUpdateEventProcessor& observer,
		DataNodeProxySmartPtr & dataNodeProxySmartPtr, 
        bool activateSummaries )
    {
        TA_ASSERT(entityData.get() != NULL, "input DataNodeEntityData is NULL");
        
        m_scadaProxyManager->createDataNodeProxy(entityData->getKey(), entityData, observer, dataNodeProxySmartPtr);
    }
    
    void ScadaProxyFactory::setProxiesToMonitorMode()
    {
        //do nothing
    }


    void ScadaProxyFactory::setProxiesToControlMode()
    {
        //do nothing
    }
};