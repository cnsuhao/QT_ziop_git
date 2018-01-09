/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/GZL6_TIP/TA_BASE/transactive/app/scada/inspector_panel/src/Equipment.cpp $
  * @author:  Andy Parker
  * @version: $Revision: #4 $
  *
  * Last modification: $DateTime: 2013/06/26 15:43:09 $
  * Last modified by:  $Author: jinmin.huang $
  * 
  * This class provides access to a piece of equipment and its associated data points.
  *
  */

#ifdef __WIN32__
#pragma warning(disable:4786)
#endif

#include <sstream>
#include <string>
#include <limits>

#include "app/scada/Inspector_Panel/src/stdafx.h"

//#include "app/scada/inspector_panel/src/InspectorPanelDlg.h"
//#include "app/scada/inspector_panel/src/InspectorPanel.h"
#include "Equipment.h"

#include "bus/scada/proxy_library/src/DataPointProxySmartPtr.h"
#include "bus/scada/proxy_library/src/IDataPointProxy.h"
#include "bus/scada/proxy_library/src/DataNodeProxySmartPtr.h"
#include "bus/scada/proxy_library/src/IDataNodeProxy.h"

#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/data_access_interface/entity_access/src/DataNodeEntityData.h"
#include "core/data_access_interface/entity_access/src/DataPointEntityData.h"
#include "core/exceptions/src/ScadaProxyException.h"
#include "core/exceptions/src/DataException.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/data_access_interface/entity_access/src/DataNodeAccessFactory.h"
#include "inspectorpanelconsts.h"
#include "AccessControlHelper.h"

namespace CORE = TA_Base_Core;
namespace TA_Base_App
{

	EquipmentEventArgs::EquipmentEventArgs(unsigned long entitykey, ScadaEntityUpdateType type)
		:m_entityKey(entitykey),
		 m_updateType(type),
		 m_equipmentInitStatus(INIT_STEP_UNKNOWN)

	{
	}


	EquipmentEventArgs::EquipmentEventArgs(EquipmentInitStatus initStatus)
		:m_equipmentInitStatus(initStatus)
	{
		m_entityKey = 0;
		m_updateType = TA_Base_Bus::NotApplicable;
	}
	EquipmentEventArgs::~EquipmentEventArgs()
	{
	}

	unsigned long	EquipmentEventArgs::getEntityKey()
	{
		return m_entityKey;
	}
	TA_Base_Bus::ScadaEntityUpdateType	EquipmentEventArgs::getUpdateType()
	{
		return m_updateType;
	}
	EquipmentInitStatus	EquipmentEventArgs::getEquipmentInitStatus()
	{
		return m_equipmentInitStatus;
	}

	const	char*	EquipmentEventArgs::getInitStatusString(EquipmentInitStatus status)
	{
		switch(status)
		{
		case	INIT_STEP0_BEGIN:
			return "Step0:Begin";
			case	INIT_STEP1_LOAD_DATANODE:
				return "Step1:Load DataNode Data";
			
			case	INIT_STEP1_LOAD_DATANODE_ERROR:
				return "Step1:Load DataNode Data Error";
			
			case	INIT_STEP2_CREATE_DATANODE_PROXY:
				return "Step2:Create DataNode Proxy";
			
			case	INIT_STEP2_CREATE_DATANODE_PROXY_ERROR:
				return "Step2:Create DataNode Proxy Error";
			
			case	INIT_STEP3_LOAD_DATAPOINTS:
				return "Step3: Load DataPoint Data";
			
			case	INIT_STEP3_LOAD_DATAPOINTS_ERROR:
				return "Step3: Load DatPoint Data Error";
			
			case	INIT_STEP4_CREATE_DATAPOINT_PROXY:
				return "Step4: Create DataPoint Proxy";
			
			case	INIT_STEP4_CREATE_DATAPOINT_PROXY_ERROR:
				return "Step4: Create DataPoint Proxy Error";
			
			case	INIT_STEP5_COMPLETE:
				return "Step5: Init Complete!";
			
			default:
				return "UnknownStatus";
		}
	}
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	CEquipment::CEquipment(std::string equipmentName) 
       :m_dnEntityData( (TA_Base_Core::DataNodeEntityData*)NULL ),
		m_maxDataPoints( 1000 ), 
		m_hasControlPoint (false ),
		m_hasInformationPoint (false ),
		m_hasMeteredPoint( false ),
		m_hasLimitedPoint( false ),
		m_haveAllDataPointConfigured ( false ),
		m_proxyFactory ( NULL ),
		m_isEntityBlank( true ),
		m_semaphore( 0 ),
		m_equipmentName( equipmentName ),
		m_isRunning(false),
		m_dataNode(NULL)
	{
		FUNCTION_ENTRY ("CEquipment()");

		this->initScadaProxy();
        
		FUNCTION_EXIT;
	}

	CEquipment::~CEquipment()
	{
		FUNCTION_ENTRY ("~CEquipment()");
		
		this->clearEquipment();

        LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo,"Equipment [0x%u] is destroyed", this);
        FUNCTION_EXIT;
	}

	void	CEquipment::initScadaProxy()
	{
		if(NULL == m_proxyFactory)
		{
			m_proxyFactory = &(TA_Base_Bus::ScadaProxyFactory::getInstance());
			TA_ASSERT ( ( 0 != m_proxyFactory ), "SCADA Proxy Factory not created");
			m_proxyFactory->setProxiesToControlMode();
		}
	}


	void	CEquipment::clearScadaProxy()
	{
		if(m_proxyFactory)
		{
			m_proxyFactory->setProxiesToMonitorMode();
			TA_Base_Bus::ScadaProxyFactory::removeInstance();
			m_proxyFactory = NULL;
		}
	}

	void	CEquipment::clearEquipment()
	{
		this->terminateAndWait();
		m_dnEntityData.reset();
		
		if(m_dataNode)
		{
			delete m_dataNode;
			m_dataNode = NULL;
		}

		m_haveAllDataPointConfigured = false;
		m_hasControlPoint = false;
		m_hasInformationPoint = false;
		m_hasMeteredPoint = false;
		m_hasLimitedPoint = false;
		m_isEntityBlank = true;
		m_datapointsKeyIndexMap.clear();
		m_writeableIpoint.clear();
		m_notWriteableIpoint.clear();
		m_meterIpoint.clear();
		m_limitIpoint.clear(); 

		m_datapointsRelatedMap.clear();
		m_datapointsKeyIndexMap.clear();

		clearDataPointObjects();
		clearScadaProxy();
	}

	void CEquipment::updateEquipmentName(const std::string& newEquipmentName)
	{
		if(newEquipmentName.size() > 0 && m_equipmentName != newEquipmentName)
		{
			this->clearEquipment();
			
			//clear m_semaphore
			while(m_semaphore.tryWait());

			{
				TA_THREADGUARD(m_queueLock);
				m_equipmentMessageQue.clear();
			}

			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo,"Change  equipment name from [%s] to [%s] !", m_equipmentName.c_str(), newEquipmentName.c_str());
			m_equipmentName = newEquipmentName;

			this->startProcess();
		}
	}

	void	CEquipment::startProcess()
	{
		m_isRunning = true;
		start();
	}

	void	CEquipment::notifyInitStatusChange(EquipmentInitStatus initStatus)
	{
		EquipmentEventArgs args( initStatus);
		this->notifyObservers(DOMAIN_DATA_CHANGE_INIT_STATUS, &args);
	}


	void	CEquipment::notifyEntityDataChange(unsigned long entitykey, ScadaEntityUpdateType type)
	{
		EquipmentEventArgs args( entitykey, type);
		this->notifyObservers(DOMAIN_DATA_CHANGE_ENTITY_DATA, &args);
	}

	void	CEquipment::notifyPermissionnChange()
	{
		this->notifyObservers(DOMAIN_DATA_CHANGE_PERTMITTION,NULL);
	}

	std::string CEquipment::getEquipmentName()
	{
		return m_equipmentName;
	}

	bool CEquipment::initialize()
	{

		//std::string statusString="开始加载设备";
		this->notifyInitStatusChange(INIT_STEP1_LOAD_DATANODE);
		if(!this->loadDataNodeData())
		{
			//statusString = "加载设备失败";
			this->notifyInitStatusChange(INIT_STEP1_LOAD_DATANODE_ERROR);
			return false;
		}

		//step2
		this->notifyInitStatusChange(INIT_STEP2_CREATE_DATANODE_PROXY);
		if(!this->createDataNodeProxy())
		{
			//statusString = "创建设备代理失败";
			this->notifyInitStatusChange(INIT_STEP2_CREATE_DATANODE_PROXY_ERROR);
			return false;
		}

		//step3
		this->notifyInitStatusChange(INIT_STEP3_LOAD_DATAPOINTS);
		if(!this->loadDataPointData())
		{
			//statusString = "加载数据点失败";
			this->notifyInitStatusChange(INIT_STEP3_LOAD_DATAPOINTS_ERROR);
			return false;
		}

		//step4
		this->notifyInitStatusChange(INIT_STEP4_CREATE_DATAPOINT_PROXY);
		if(this->createDataPointProxy())
		{
			this->setupPermissions();
			this->initMap();
			this->initListsHelper();
			m_haveAllDataPointConfigured = true;
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Equipment [0x%u] is init completed", this);
			//statusString = "创建数据点代理完成";
			this->notifyInitStatusChange(INIT_STEP5_COMPLETE);
		}
		else
		{	
			//statusString ="创建数据点代理完成失败";
			this->notifyInitStatusChange(INIT_STEP4_CREATE_DATAPOINT_PROXY);
			//AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, 0, 0);
			return false;
		}
		return true;
	}
	void CEquipment::run()
	{
		FUNCTION_ENTRY ("CEquipment::terminate");
		if( !m_isRunning )
		{
			//assure this thread can only be run once
			return;
		}
		if(!this->initialize())
		{
			FUNCTION_EXIT;
			m_isRunning = false;
			return;
		}
		
		try
		{
			EquipmentMessage nextItem;
			while(m_isRunning)
			{
				m_semaphore.wait();
				if( !m_isRunning )
				{
					return;
				}
				if(!m_equipmentMessageQue.empty())
				{
					{
						TA_THREADGUARD(m_queueLock);
						nextItem = m_equipmentMessageQue.front();
						m_equipmentMessageQue.pop_front();
					}

					if(nextItem.msgType == EntityUpdate)
					{
						this->notifyEntityDataChange(nextItem.entityKey, nextItem.updateType);
					}
					else if(nextItem.msgType == PermissionUpdate)
					{
						this->notifyPermissionnChange();
					}
				}
			}
		}
		catch(...)
		{
			m_isRunning = false;
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,"Failed to process scada proxy message queue,unknown exception");
		}
		FUNCTION_EXIT;
	}
		
	void CEquipment::terminate()
	{
		FUNCTION_ENTRY ("CEquipment::terminate");
		m_isRunning = false;
		m_semaphore.post();
		FUNCTION_EXIT
	}

	bool CEquipment::loadDataNodeData()
	{
		FUNCTION_ENTRY ("CEquipment::loadDataNode");
		bool rtn = false;
		try
		{
			TA_Base_Core::IEntityData* entityData = TA_Base_Core::EntityAccessFactory::getInstance().getEntity(m_equipmentName);
			TA_Base_Core::DataNodeEntityData* dnEntityData = dynamic_cast<TA_Base_Core::DataNodeEntityData*>(entityData);

			if(dnEntityData == NULL)
			{ 
				//should be DataPointEntity and load parent data node
				TA_Base_Core::DataPointEntityData* dpEntityData = dynamic_cast<TA_Base_Core::DataPointEntityData*>(entityData);
				unsigned long dnKey = dpEntityData->getParent();
				delete entityData;
				entityData = TA_Base_Core::EntityAccessFactory::getInstance().getEntity(dnKey);
				dnEntityData = dynamic_cast<TA_Base_Core::DataNodeEntityData*>(entityData);
				TA_ASSERT(NULL!=dnEntityData,"failed to cast entityData to dataNode data");
			}
			
			this->m_dnEntityData 		= boost::shared_ptr<TA_Base_Core::DataNodeEntityData>(dnEntityData);
			this->m_dataNodeEntityKey 	= dnEntityData->getKey(); 
			this->m_isEntityBlank 		= false;
			rtn = true;
		}
		catch(CORE::DataException & ex)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,"Failed to load equipment[%s], %s",m_equipmentName.c_str(), ex.what());
			rtn = false;
		}
		catch(...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,"Failed to load equipment[%s], Unknown exception",m_equipmentName.c_str());
			rtn = false;
		}
        FUNCTION_EXIT;
		return rtn;
	}

	bool CEquipment::createDataNodeProxy()
	{
		FUNCTION_ENTRY ("CEquipment::createDataNodeProxy");
		bool rtn = false;
		try
		{
			m_dataNode = new DataNodeProxySmartPtr();
			m_proxyFactory->createDataNodeProxy(m_dnEntityData, *this, (*m_dataNode));
			rtn = true;
		}
		catch(...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,"Failed to create datanode proxy");
			rtn = false;
		}
		FUNCTION_EXIT
		return rtn;
	}

	bool CEquipment::loadDataPointData()
	{
		FUNCTION_ENTRY ("CEquipment::loadDataPointData");
		bool rtn = false;
		try
		{
			std::vector<boost::shared_ptr<TA_Base_Core::IEntityData> > childEntities = this->m_dnEntityData->getChildDataPoints();
			this->m_dataPointEntities.reserve(childEntities.size());
			for(int index=0; index < childEntities.size(); index++)
			{
				m_dataPointEntities.push_back (boost::dynamic_pointer_cast<TA_Base_Core::DataPointEntityData>(childEntities[index] ) );
			}
			rtn = true;
		}
		catch(CORE::DataException & ex)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,"Failed to load DataPoints, %s", ex.what());
			m_dataPointEntities.clear();
			rtn = false;
		}
		catch(...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,"Failed to load DataPoints, Unknown exception");
			rtn = false;
		}
		FUNCTION_EXIT
		return rtn;
	}

	bool CEquipment::createDataPointProxy()
	{
		FUNCTION_ENTRY ("CEquipment::createDataPointProxy");
		bool rtn;
		try
		{
			unsigned int numberDataPoints = 0;
			m_dataPoints.reserve(m_dataPointEntities.size());
			for (int index=0; index < m_dataPointEntities.size() && numberDataPoints < m_maxDataPoints; ++index)
			{			
				TA_Base_Bus::DataPointProxySmartPtr *pDataPoint = new TA_Base_Bus::DataPointProxySmartPtr();
				m_proxyFactory->createDataPointProxy(m_dataPointEntities[index], *this, *pDataPoint);


				//for now, we already know datapoint's display order, so insert proxy into vector in order
                std::vector<TA_Base_Bus::DataPointProxySmartPtr*>::iterator itDp;
                for (itDp = m_dataPoints.begin(); itDp != m_dataPoints.end(); ++ itDp)
                {	
                    if ( (*pDataPoint)->getDisplayOrder() <= (*(*itDp))->getDisplayOrder())
                    {
                       itDp = m_dataPoints.insert(itDp, pDataPoint);
                        break;
                    }	
                }
                
                if (itDp == m_dataPoints.end())
                {
                    m_dataPoints.push_back(pDataPoint);
                }
				numberDataPoints++;		
			}
			rtn = true;
		}
		catch(TA_Base_Core::ScadaProxyException& e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, "Failed to create datapoint proxy, %s", e.what());
			rtn = false;
		}
		catch(...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, "Unknown excepiton");
			rtn = false;
		}
		FUNCTION_EXIT
		return rtn;
	}


	void	CEquipment::setupPermissions()
	{
		std::vector<AccessEntityInfo> entityKeys;	
		AccessEntityInfo entityInfo;

		// add the equipment data NODE first
		entityInfo.entityKey = (*m_dataNode)->getEntityKey();
		if (entityInfo.entityKey == 0L)	
		{
			return;
		}
		entityInfo.locationKey  = (*m_dataNode)->getLocationKey();
		entityInfo.subSystemKey = (*m_dataNode)->getPhysicalSubSystemKey();
		
		if (entityInfo.subSystemKey == 0)
		{
			entityInfo.subSystemKey = (*m_dataNode)->getSubSystemKey();
		}

		entityKeys.push_back(entityInfo);

		// add all the data POINTS for this equipment
		for (std::vector<DataPointProxySmartPtr*>::iterator iter = m_dataPoints.begin();iter != m_dataPoints.end(); iter++)
		{
			TA_Base_Bus::DataPointProxySmartPtr * pDataPointPtr = *iter;
			entityInfo.entityKey = (*pDataPointPtr)->getEntityKey();
			entityInfo.locationKey = (*pDataPointPtr)->getLocationKey();
			entityInfo.subSystemKey = (*pDataPointPtr)->getPhysicalSubSystemKey();
			if (entityInfo.subSystemKey == 0)
			{
				entityInfo.subSystemKey = (*pDataPointPtr)->getSubSystemKey();
			}
			entityKeys.push_back(entityInfo);
		}
		AccessControlHelper::getInstance()->registerObserver(this);
		AccessControlHelper::getInstance()->InitAccessControlMap(entityKeys);
	}

	//Using for IEC104 Protocol
	bool CEquipment::getPScadaFlag()
	{
		return false;
	}

	TA_Base_Bus::DataNodeProxySmartPtr& CEquipment::getDataNode()
	{
		FUNCTION_ENTRY ("getDataNode()");
		
//		TA_ASSERT (m_dataNode != NULL,"Invalid data node (getDataNode).");

		FUNCTION_EXIT;

		return *m_dataNode;
	}

	std::vector<TA_Base_Bus::DataPointProxySmartPtr*>& CEquipment::getChildDataPointVector()
	{
		FUNCTION_ENTRY ("getChildDataPointVector()");

        FUNCTION_EXIT;

		return m_dataPoints;
	}

	TA_Base_Bus::DataPointProxySmartPtr& CEquipment::getChildDataPoint(UINT iPoint)
	{
		FUNCTION_ENTRY ("getChildDataPoint()");

		TA_ASSERT (iPoint<m_dataPoints.size(),"Invalid data point (getChildDataPoint).");

        FUNCTION_EXIT;

		return (*m_dataPoints[iPoint]);
	}

	TA_Base_Bus::DataPointProxySmartPtr& CEquipment::getChildDataPointFromEntityKey(unsigned long key)
	{
		FUNCTION_ENTRY ("getChildDataPointFromEntityKey()");

        // Replace linear search
		DatapointsKeyIndexIt iter  = m_datapointsKeyIndexMap.find(key);
		if (iter != m_datapointsKeyIndexMap.end())
		{			
			return (*m_dataPoints[iter->second]);
		} 
	
		// If we got here then not found
		std::ostringstream message;
		message << "Error: DataPoint for entity key ";
		message << key;
		message << " not found";
		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
			"getChildDataPointFromEntityKey() - %s", message.str().c_str());

		FUNCTION_EXIT;

		// For some reason, calling the plain throw was not caught in the 
        // catch(...).  To be safe, we will use the ScadaProxyException for now.
        // Need to get a proper exception for this at some stage.  
		TA_THROW(TA_Base_Core::ScadaProxyException(message.str()));
	}


	UINT CEquipment::getChildDataPointIndexFromEntityKey(unsigned long key)
	{
		FUNCTION_ENTRY ("getChildDataPointIndexFromEntityKey()");
	
        // Replace linear search
		DatapointsKeyIndexIt iter = m_datapointsKeyIndexMap.find(key);
		if (iter != m_datapointsKeyIndexMap.end())
		{			
			return iter->second;
		} 

		// If we got here then not found
		std::ostringstream message;
		message << "Data point for entity key ";
		message << key;
		message << " not found";
		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
			"getChildDataPointIndexFromEntityKey() - %s", message.str().c_str());

		FUNCTION_EXIT;

		// Return an invalid index greater than the maximum
		return m_dataPoints.size();
	}

	unsigned int CEquipment::getChildDataPointCount()
	{
		FUNCTION_ENTRY ("getChildDataPointCount()");
        
        FUNCTION_EXIT;

		return m_dataPoints.size();
	}

	void CEquipment::processEntityUpdateEvent(unsigned long entityKey, TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("processEntityUpdateEvent()");

		if(updateType != TA_Base_Bus::ConfigAvailable)
		{
			EquipmentMessage msgItem;
			msgItem.msgType = EntityUpdate; 
			msgItem.entityKey = entityKey;
			msgItem.updateType = updateType;

			TA_THREADGUARD(m_queueLock);
			m_equipmentMessageQue.push_back(msgItem);
			m_semaphore.post();
		}
		FUNCTION_EXIT;
	}


	void CEquipment::processPermissionnChange()
	{
		EquipmentMessage msgItem;
		msgItem.msgType = PermissionUpdate; 

		TA_THREADGUARD(m_queueLock);
		m_equipmentMessageQue.push_back(msgItem);
		m_semaphore.post();
	}


	void CEquipment::clearDataPointObjects()
	{
		FUNCTION_ENTRY ("clearDataPointObjects()");
		
		std::vector<TA_Base_Bus::DataPointProxySmartPtr*>::iterator itDp;
		for (itDp = m_dataPoints.begin(); itDp != m_dataPoints.end(); ++ itDp)
		{
			TA_Base_Bus::DataPointProxySmartPtr * pDataPoint = *itDp;
			if (pDataPoint!=NULL)
			{
				delete pDataPoint;
			}
		}
		
        m_dataPoints.clear();

		m_hasControlPoint = false;
		m_hasInformationPoint = false;
		m_hasMeteredPoint = false;
		m_hasLimitedPoint = false;
		m_haveAllDataPointConfigured = false;

		FUNCTION_EXIT;
	}

	// Already set the flags when sorting
	bool CEquipment::hasControlPoint()
	{
		return m_hasControlPoint;
	}

	bool CEquipment::hasInformationPoint()
	{
		return m_hasInformationPoint;
	}

	bool CEquipment::hasMeteredPoint()
	{
		return m_hasMeteredPoint;
	}

	bool CEquipment::hasLimitedPoint()
	{
		return m_hasLimitedPoint;
	}

	bool CEquipment::haveAllDataPointsConfigured()
	{
		return m_haveAllDataPointConfigured;
	}

	// Create a map to replace linear search to find the datapoint index by entity key
	void CEquipment::initMap()
	{
        m_datapointsKeyIndexMap.clear();

		for (UINT iPoint=0;iPoint<m_dataPoints.size();iPoint++)
			m_datapointsKeyIndexMap.insert(DatapointsKeyIndex::value_type((*m_dataPoints[iPoint])->getEntityKey(), iPoint));
	}

	// Store different datapoints's index for list display according to their required config
	// Vectors will only be available after sorting (i.e. all config valid)
	void CEquipment::initListsHelper()
	{
		TA_THREADGUARD(m_indexVectorLock);

        m_writeableIpoint.clear();
		m_notWriteableIpoint.clear();
		m_meterIpoint.clear();
		m_limitIpoint.clear(); 
		
		for (unsigned long iPoint=0;iPoint<m_dataPoints.size();iPoint++)
		{
			if( (*m_dataPoints[iPoint])->getDisplayOrder() >= 0 )
			{
				if( false == (*m_dataPoints[iPoint])->isWriteable() )
				{
					// Information/Override lists/Inhibit Lists
					m_notWriteableIpoint.push_back(iPoint);

					// Meter List
                    if( (*m_dataPoints[iPoint])->getType() == TA_Base_Bus::DPT_METERED )
					{
						m_meterIpoint.push_back(iPoint);
                        m_hasMeteredPoint = true;
                    }

					// Limit List
					if ( ( false == (*m_dataPoints[iPoint])->isBoolean() ) &&		// if not a boolean dp
					     ( false == (*m_dataPoints[iPoint])->isText() ) &&		// if not a text dp
					     ( false == (*m_dataPoints[iPoint])->isDerived() ) )		// if not a derived dp
					{
						m_limitIpoint.push_back(iPoint);
						m_hasLimitedPoint = true;
					}

                    m_hasInformationPoint = true;
				}
				else
				{
					m_writeableIpoint.push_back(iPoint); // Control lists/ Inhibit Lists
                    m_hasControlPoint = true;

					//Set the map for control point related input data point and control point position in the m_datapoints
					//If control point have no related data point. then no need set in the map
// 					unsigned long nRelatedInputDataPointKey = (*m_dataPoints[iPoint])->getRelatedInputDataPoint();
// 					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
// 						"Get control data point %d related input data point %d", (*m_dataPoints[iPoint])->getEntityKey(), nRelatedInputDataPointKey);
// 
// 					if (nRelatedInputDataPointKey != 0)
// 					{
// 						m_datapointsRelatedMap.insert(ControlDataPointRelatedMap::value_type(nRelatedInputDataPointKey,iPoint));
// 					}
				}
			}
		}
	}

	// Accessors to get the index vectors

	std::vector<unsigned long>& CEquipment::getWriteableIndex()
	{
		FUNCTION_ENTRY ("getWriteableIndex()");
		FUNCTION_EXIT;

		return m_writeableIpoint;
	}

	std::vector<unsigned long>&	CEquipment::getNotWriteableIndex()
	{
		FUNCTION_ENTRY ("getNotWriteableIndex()");
		FUNCTION_EXIT;

		return m_notWriteableIpoint;
	}

	std::vector<unsigned long>& CEquipment::getLimitIndex()
	{
		FUNCTION_ENTRY ("getLimitIndex()");
		FUNCTION_EXIT;

		return m_limitIpoint;
	}

	std::vector<unsigned long>& CEquipment::getMeterIndex()
	{
		FUNCTION_ENTRY ("getMeterIndex()");
		FUNCTION_EXIT;

		return m_meterIpoint;
	}

	//Using Related Input data point to find relevant control data point
	int	CEquipment::getControlPointIndex(unsigned long key)
	{
		ControlDataPointRelatedMapIt iter  = m_datapointsRelatedMap.find(key);
		if (iter != m_datapointsRelatedMap.end())
		{			
			return (iter->second);
		} 
		else
			return -1;
	}
	bool CEquipment::isEntityBlank()
	{
		return m_isEntityBlank;
	}


	unsigned long		CEquipment::getChildKeyByName(std::string& entityName)
	{
		unsigned long rtnKey = 0;
		for(int index =0; index < m_dataPointEntities.size(); index++)
		{
			if(m_dataPointEntities[index]->getName() == entityName)
			{
				rtnKey =  m_dataPointEntities[index]->getKey();
				break;
			}
		}
		return rtnKey;
	}

	unsigned long		CEquipment::getDirectControlKey()
	{
		return getChildKeyByName(m_equipmentName);
	}
}


