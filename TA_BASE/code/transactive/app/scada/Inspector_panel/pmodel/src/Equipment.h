#ifndef EQUIPMENT_H
#define EQUIPMENT_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/GZL6_TIP/TA_BASE/transactive/app/scada/inspector_panel/src/Equipment.h $
  * @author:  Andy Parker
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2013/06/26 15:43:09 $
  *
  * Last modified by:  $Author: jinmin.huang $
  * 
  * This class provides access to a piece of equipment and its associated data points.
  *
  */
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __WIN32__
#pragma warning(disable:4786)
#endif

#include <vector>
#include <deque>
#include <list>
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/threads/src/Thread.h"
#include "core/synchronisation/src/Semaphore.h"
#include "bus/scada/proxy_library/src/ScadaProxyFactory.h"
#include "bus/scada/proxy_library/src/DataPointProxySmartPtr.h"
#include "bus/scada/proxy_library/src/DataNodeProxySmartPtr.h"
#include "bus/scada/proxy_library/src/IEntityUpdateEventProcessor.h"
#include "bus\generic_gui_pmod\src\PModelInterface.h"
#include "AccessControlHelper.h"
#include "equipmentmessage.h"

namespace TA_Base_Bus
{
	class ScadaProxyFactory;
}

using namespace TA_Base_Bus;

namespace TA_Base_App
{
	//Equipment init step
	enum EquipmentInitStatus
	{
		INIT_STEP_UNKNOWN,
		INIT_STEP0_BEGIN,
		INIT_STEP1_LOAD_DATANODE,
		INIT_STEP1_LOAD_DATANODE_ERROR,
		INIT_STEP2_CREATE_DATANODE_PROXY,
		INIT_STEP2_CREATE_DATANODE_PROXY_ERROR,
		INIT_STEP3_LOAD_DATAPOINTS,
		INIT_STEP3_LOAD_DATAPOINTS_ERROR,
		INIT_STEP4_CREATE_DATAPOINT_PROXY,
		INIT_STEP4_CREATE_DATAPOINT_PROXY_ERROR,
		INIT_STEP5_COMPLETE
	};

	class EquipmentEventArgs : public TA_Base_Bus::EventArgs
	{
	public:
		EquipmentEventArgs(unsigned long entitykey, TA_Base_Bus::ScadaEntityUpdateType type);
		EquipmentEventArgs(EquipmentInitStatus initStatus);
		~EquipmentEventArgs();
		static const	char*	getInitStatusString(EquipmentInitStatus status); 
	public:
		unsigned long						getEntityKey();
		TA_Base_Bus::ScadaEntityUpdateType	getUpdateType();
		EquipmentInitStatus					getEquipmentInitStatus();
	private:
		unsigned long						m_entityKey;
		ScadaEntityUpdateType				m_updateType;
		EquipmentInitStatus					m_equipmentInitStatus;
	};

	class CEquipment :	public TA_Base_Bus::IEntityUpdateEventProcessor,
						public TA_Base_Core::Thread,
						public TA_Base_Bus::AbstractStateChangeSubject<TA_Base_Bus::IStateChangeObserver>,
						public IPermissionnNotification
	{
		public:
			CEquipment(std::string equipmentName);
			virtual ~CEquipment();
		public:
			void				startProcess();
			void				updateEquipmentName(const std::string& newEquipmentName);


			
			
			std::string 		getEquipmentName();
			TA_Base_Bus::DataNodeProxySmartPtr&					getDataNode();
			std::vector<TA_Base_Bus::DataPointProxySmartPtr*>&	getChildDataPointVector();
			TA_Base_Bus::DataPointProxySmartPtr&				getChildDataPoint(UINT iPoint);
			TA_Base_Bus::DataPointProxySmartPtr&				getChildDataPointFromEntityKey(unsigned long key);

			// wenching++ (TD13353)
			// Accessor for vector of indexes
			std::vector<unsigned long>&		getWriteableIndex();
			std::vector<unsigned long>&		getNotWriteableIndex();
			std::vector<unsigned long>&		getLimitIndex();
			std::vector<unsigned long>&		getMeterIndex();
			// ++wenching (TD13353)

			
			UINT				getChildDataPointIndexFromEntityKey(unsigned long key);
			unsigned int		getChildDataPointCount();

			bool				hasControlPoint();
			bool				hasInformationPoint();
			bool				hasMeteredPoint();
			bool				hasLimitedPoint();
			bool				haveAllDataPointsConfigured();
			bool				getPScadaFlag(); //Using for IEC104 Protocol
			int					getControlPointIndex(unsigned long key);
			bool				isEntityBlank();
			unsigned long		getChildKeyByName(std::string& entityName);
			unsigned long		getDirectControlKey();
		protected:
			//interface from Thread
			virtual void		run();
			virtual void		terminate();
			//interface from IEntityUpdateEventProcessor
			virtual void		processEntityUpdateEvent(unsigned long entityKey, TA_Base_Bus::ScadaEntityUpdateType updateType);
			//interface from IPermissionnNotification
			virtual void		processPermissionnChange();
		private:
			bool				initialize();
			void				clearEquipment();
			void				clearScadaProxy();
			void				initScadaProxy();
			bool				loadDataNodeData();
			bool 				createDataNodeProxy();
			bool				loadDataPointData();
			bool				createDataPointProxy();
			void				initMap();	//wenching++ (TD13353)
			void				initListsHelper(); //wenching++ (TD13353)
			void 				clearDataPointObjects();
			void				setupPermissions();
			bool				loadDataPoints();

			void				notifyInitStatusChange(EquipmentInitStatus initStatus);
			void				notifyEntityDataChange(unsigned long entitykey, ScadaEntityUpdateType type);
			void				notifyPermissionnChange();
		
	private:
			std::string								m_equipmentName;
			TA_Base_Bus::DataNodeProxySmartPtr		*m_dataNode;
			boost::shared_ptr<TA_Base_Core::DataNodeEntityData>		m_dnEntityData;
			unsigned long							m_dataNodeEntityKey;
			bool									m_isEntityBlank;
			typedef std::vector<boost::shared_ptr<TA_Base_Core::DataPointEntityData> > ChildDataPointData;
			typedef std::vector<TA_Base_Bus::DataPointProxySmartPtr*> ChildDataPoints;
			typedef std::map<unsigned long, unsigned long>	DatapointsKeyIndex; // find the datapoints index by entity key
			typedef DatapointsKeyIndex::iterator			DatapointsKeyIndexIt;
			typedef std::map<unsigned long, long>			ControlDataPointRelatedMap; // find the control data point and its related input data point
			typedef ControlDataPointRelatedMap::iterator	ControlDataPointRelatedMapIt; 
			
			ChildDataPointData		m_dataPointEntities;
			ChildDataPoints			m_dataPoints;
			DatapointsKeyIndex		m_datapointsKeyIndexMap;
			const unsigned int		m_maxDataPoints;
			volatile bool			m_hasControlPoint;
			volatile bool			m_hasInformationPoint;
			volatile bool			m_hasMeteredPoint;
			volatile bool			m_hasLimitedPoint;
			volatile bool			m_haveAllDataPointConfigured;

			std::vector<unsigned long>				m_writeableIpoint;		// Vector of indexes (information points)
			std::vector<unsigned long>				m_notWriteableIpoint;		// Vector of indexes (control points)
			std::vector<unsigned long>				m_meterIpoint;				// Vector of indexes (meter points)
			std::vector<unsigned long>				m_limitIpoint;				// Vector of indexes (limit points)
			ControlDataPointRelatedMap				m_datapointsRelatedMap; 

			TA_Base_Core::ReEntrantThreadLockable	m_indexVectorLock;

			TA_Base_Bus::ScadaProxyFactory*			m_proxyFactory;
			//jinmin++ for thread
			//typedef std::pair<unsigned long,TA_Base_Bus::ScadaEntityUpdateType> ScadaProxyMsgQueItem;
			typedef std::deque<EquipmentMessage>		EquipmentMessageQueue;
			EquipmentMessageQueue						m_equipmentMessageQue;
			TA_Base_Core::NonReEntrantThreadLockable	m_queueLock;
			TA_Base_Core::Semaphore						m_semaphore;
			volatile bool										m_isRunning;
	private:
		//std::list<TA_Base_Bus::IStateChangeObserver*> m_observers;
		//TA_Base_Core::NonReEntrantThreadLockable m_observerLock;

	};
}
#endif // #ifndef EQUIPMENT_H
