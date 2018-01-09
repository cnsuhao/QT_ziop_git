/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/scada/common_library/src/MmsScadaUtility.h $
  * @author:  Andrew Del Carlo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * This class provides utility functions for logging Audit Events.
  *
  */


#if !defined(AFX_MmsScadaUtility_H__C39208D3_EB6A_460A_AFA4_F659A6180B35__INCLUDED_)
#define AFX_MmsScadaUtility_H__C39208D3_EB6A_460A_AFA4_F659A6180B35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning (disable : 4503)
#endif // _MSC_VER > 1000

#include <string>
#include <map>
#include <vector>
#include "bus/scada/common_library/src/ScadaUtilities.h"
#include "bus/scada/proxy_library/src/IEntityUpdateEventProcessor.h"
#include "bus/scada/proxy_library/src/DataPointProxySmartPtr.h"
#include "bus/scada/proxy_library/src/DataNodeProxySmartPtr.h"
#include "core/data_access_interface/entity_access/src/DataPointEntityData.h"
#include "core/data_access_interface/entity_access/src/DataNodeEntityData.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"


namespace TA_Base_Core
{
    class ConfigUpdateDetails;
}

namespace TA_Base_Bus
{
	class RightsLibrary;
	class IEntity;
    class IEntityDataList;
    class ScadaProxyFactory;

	class MmsScadaUtility : public virtual IEntityUpdateEventProcessor
	{
	public:
	
		MmsScadaUtility();
		MmsScadaUtility(std::string agentName, 
						const std::map<unsigned long, TA_Base_Bus::IEntity*>* entityMap,
						bool useProxies = true);
		virtual ~MmsScadaUtility();

        //TD16871 
        //Mintao++
        /**
         * processOnlineUpdateOfMaintenanceData
         *
         * When there is a configuration update of the type and key matching
         * one registered by this class, this method will be invoked
         * to process the update accordingly.
         */
        void processOnlineUpdateOfMaintenanceData();
        
        void registerLocalEntities(const std::string& agentName, const std::map<unsigned long, TA_Base_Bus::IEntity*>* entityMap);

		/**
		* isDataPointMmsInhibited
		* @return false if datapoint, or subsystem that datapoint is in
		* is MMS Inhibited, or Global MMS inhibit is applied - else return true
		*
		* @throw TA_Base_Core::ScadaCommonException if can not read inhibit state for
		*		 global, datapoint or subsystem
		*/

		bool isDataPointMmsInhibited(unsigned long dataPointKey);
        bool isDataPointMmsInhibited(unsigned long dataPointKey, TA_Base_Core::DataPointEntityDataPtr dataPointEntityAccess);
		bool isDataPointMmsInhibited(const std::string& dataPointName);
		bool isMmsGlobalInhibited();
		bool isMmsSubsystemInhibited(unsigned long subsystemkey);
		/**
        * getMaintenanceDataObject
        *
        * @return MaintenanceDataObject for all datapoints that:
        * 1) are in the speciified location(0, ignore location) and
        * 2) are configured with the MMS_REPORT_TYPE = "CPT" or "VAL"           
        * @param locationKey: the pkey of the location to retrieve the data points(0, ignore location)
        *
        * @throw TA_Base_Core::DatabaseException 
        * @throw TA_Base_Core::DataException
        */
		std::vector<ScadaUtilities::MaintenanceData*> getMaintenanceDataObject(unsigned long locationKey);
           
		/**
        * getMaintenanceDataValue
        *
        * @return :
        * 1) update dataObjects with latest value       
		* @param dataObjects: input and output, data points which need to retrieve latest value.
        * 
        */
		void getMaintenanceDataValue(std::vector<ScadaUtilities::MaintenanceData*>& dataObjects );

	/**
	  * processEntityUpdateEvent
	  * 
	  * process the entity update event
	  * 
	  * @return virtual void 
	  * @param : unsigned long entityKey
	  * @param : ScadaEntityUpdateType updateType
	  * 
	  * @exception <exceptions> Optional
	  */
		virtual void processEntityUpdateEvent(unsigned long entityKey, ScadaEntityUpdateType updateType);

	protected:
		void freeSubSystemNodes();
		void releaseMMSEntityMap();
		bool isMmsDpInhibited(unsigned long dataPointKey, const std::string& agentName);
		void getMmsMaintDataValue (ScadaUtilities::MaintenanceData& data);
		ScadaUtilities::MaintenanceData* getMmmMaintData(TA_Base_Core::IEntityData* entityData);
	private:

        // Mintao++(TD13650)
        void initScadaProxyFactory();
        // Mintao++(TD13650)
	
		std::string m_localAgentName;
		const std::map<unsigned long, TA_Base_Bus::IEntity*> *m_localEntityMap;
		bool m_useProxies;
        bool m_globalInhibitDpExist;
        bool m_globalInhibitProxyCreated;
		DataPointProxySmartPtr*				  m_globalInhibitProxy;
        TA_Base_Core::ReEntrantThreadLockable m_globalInhibitLock;
		TA_Base_Core::ReEntrantThreadLockable m_subsystemNodesLock;
		std::map<unsigned long, DataNodeProxySmartPtr*> m_subsystemNodes;

        //this map cache the datapointentitydata which MMS_REPORT_TYPE in (CPT, VAL)
		std::map<unsigned long, TA_Base_Core::IEntityDataList> m_mmsEntityMap;
		//this map cache the DataNodeEntityData of datapoints's parents which in m_mmsEntityMap
		std::map<unsigned long, boost::shared_ptr<TA_Base_Core::DataNodeEntityData> > m_mmsDNEntityMap;
		//store all the locationKeys of m_mmsEntityMap used
		std::vector<unsigned long> m_locationKeys;
		//used for both m_locationKeys and m_mmsEntityMap
		TA_Base_Core::ReEntrantThreadLockable m_mmsDataLock;

	    //cache the scada proxy factory
		TA_Base_Bus::ScadaProxyFactory*			m_proxyFactory;				// Mintao++(TD13650)
		
		//used for filter EntityData which MMSReportType IN ('CPT','VAL')
		const static std::string MMSDataCustomToken;
	};
}
#endif // !defined(AFX_MmsScadaUtility_H__C39208D3_EB6A_460A_AFA4_F659A6180B35__INCLUDED_)
