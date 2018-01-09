#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentItemPopulateLogic.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/DataNodeAgentProxy.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/AuditMessageUtility.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/DataNodeListItem.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/DataPointListItem.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/DataNodeSummaryListItem.h"
#include "bus/scada/common_library/src/ScadaUtilities.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "core/data_access_interface/src/IEquipReport.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataException.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/Session.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/src/EquipReportAccessFactory.h"
namespace TA_Base_App
{
	const std::string EquipmentItemPopulateLogic::PARENT_ENTITY_NAME = "Parent Entity";

	EquipmentItemPopulateLogic::EquipmentItemPopulateLogic(TA_Base_Core::IEntityData* pEntityData)
		:
	m_isAllowedToConfigure(false),
	m_isAllowedToPrintOrExport(false),
	m_isAllowedToViewAllReports(false),
	m_hasCleanUp(false),
	m_curLoginLocation(ULONG_MAX),
	m_selectedLocationKey ( -1 ),
	m_DnUpdateProcessor(NULL),
	m_pEntityData(pEntityData),
	m_authenticationLibrary(NULL),
	m_profileAccessFactory ( TA_Base_Core::ProfileAccessFactory::getInstance() ),
	m_scadaProxyFactory(TA_Base_Bus::ScadaProxyFactory::getInstance()),
	m_previousSessionId(""),
	m_selectedReportKey("")
	{
		m_authenticationLibrary = new TA_Base_Bus::AuthenticationLibrary(true);
		m_dnEntityDataPtrMap.clear();
		m_dnAgentsMap.clear();
		m_DnUpdateProcessor = new DnUpdateProcessor(*this);
		m_DnUpdateProcessor->start();
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_SESSIONID);
		checkPermission();
	}

	EquipmentItemPopulateLogic::~EquipmentItemPopulateLogic()
	{
		cleanUpResource();
	}

	void EquipmentItemPopulateLogic::processorCallBack(boost::shared_ptr<DnUpdateRequest> newRequest)
	{
		unsigned long entitykey = newRequest->getEntityKey();
		Node_Item nodeItem = newRequest->getNodeItem();

		{
			TA_Base_Core::ThreadGuard guard(m_reportItemsLock);
			if( m_reportItems.find(entitykey) != m_reportItems.end() )
			{
				return;
			}
		}

		EquipmentStatusListItemSharedPtr pItem;

		if (DATAPOINT == nodeItem)
		{
			pItem.reset( new DataPointListItem( entitykey, m_scadaProxyFactory, this ));
			TA_Base_Core::ThreadGuard guard(m_reportItemsLock);
			m_reportItems[entitykey] = pItem;
		}
		else if ( DATANODE == nodeItem)
		{
			pItem.reset( new DataNodeListItem( entitykey, m_scadaProxyFactory, this ));
			TA_Base_Core::ThreadGuard guard(m_reportItemsLock);
			m_reportItems[entitykey] = pItem;
		}
		else if (DATANODE_SUMMARY == nodeItem)
		{
			//a. load DataNodeEntityData
			boost::shared_ptr<TA_Base_Core::DataNodeEntityData> dnEntityDataPtr;
			std::map<unsigned long, boost::shared_ptr<TA_Base_Core::DataNodeEntityData> >::iterator iterDn;
			iterDn = m_dnEntityDataPtrMap.find( entitykey );
			if(iterDn != m_dnEntityDataPtrMap.end())
			{
				dnEntityDataPtr = iterDn->second;
			}
			else
			{
				TA_Base_Core::IEntityData* pEntityData = TA_Base_Core::EntityAccessFactory::getInstance().getEntity(entitykey);
				TA_Base_Core::DataNodeEntityData* pDnEntityData = dynamic_cast<TA_Base_Core::DataNodeEntityData*>(pEntityData);					
				TA_ASSERT( pDnEntityData != NULL, "Load DataNode EntityData failed!");
				dnEntityDataPtr = boost::shared_ptr<TA_Base_Core::DataNodeEntityData>(pDnEntityData);
				m_dnEntityDataPtrMap[entitykey] = dnEntityDataPtr;
			}

			bool isEquipmentDN = false;

			//b. get DataNode's child datapoints and handle them	
			TA_Base_Core::SharedIEntityDataList childDps = dnEntityDataPtr->getChildDataPoints();
			TA_Base_Core::SharedIEntityDataList::iterator iterDp;
			for(iterDp = childDps.begin(); iterDp != childDps.end(); ++iterDp)
			{
				boost::shared_ptr<TA_Base_Core::DataPointEntityData> dpEntityData;
				dpEntityData = boost::dynamic_pointer_cast<TA_Base_Core::DataPointEntityData>(*iterDp);

				if(dpEntityData.get() == NULL) 
					continue;

				unsigned long parentKey = dpEntityData->getParent();

				if(parentKey == dnEntityDataPtr->getKey())
				{
					isEquipmentDN = true;

					TA_Base_Core::ThreadGuard guard(m_reportItemsLock);

					if( m_reportItems.find(dpEntityData->getKey()) == m_reportItems.end() )
					{
						pItem.reset( new DataPointListItem(dpEntityData, dnEntityDataPtr, m_scadaProxyFactory, this));
						m_reportItems[ dpEntityData->getKey()] = pItem;
					}
				}
			}
			//c. add DataNodeItem
			if (isEquipmentDN)
			{
				pItem.reset( new DataNodeSummaryListItem(dnEntityDataPtr, m_scadaProxyFactory, this) );
				TA_Base_Core::ThreadGuard guard(m_reportItemsLock);
				m_reportItems[entitykey] = pItem;
			}
		}
		else
		{
			TA_ASSERT( false, "Invalid item node type (not Datanode, Datapoint nor DataNodeSummary).");
		}
	}

	void EquipmentItemPopulateLogic::onRunParamChange(const std::string& name, const std::string& value)
	{
		std::string sessionID = value;

		if (name.compare(RPARAM_SESSIONID) == 0 && sessionID != m_previousSessionId)
		{
			m_previousSessionId = sessionID;
			processSessionIdChange();
		}
	}

	void EquipmentItemPopulateLogic::notifyDnUpdate(std::vector<unsigned long> & dnKeys)
	{
		std::vector<unsigned long>::iterator iter;
		for (iter = dnKeys.begin(); iter != dnKeys.end(); ++iter)
		{
			boost::shared_ptr<DnUpdateRequest> request(new DnUpdateRequest(*iter, DATANODE_SUMMARY));
			m_DnUpdateProcessor->insertUnique(request);
		}
	}

	bool EquipmentItemPopulateLogic::populateLocations(std::map<unsigned long, std::string>& locs)
	{
		FUNCTION_ENTRY("PopulateLocations");
		
		bool isSuccess = false;
		using TA_Base_Core::ILocation;
		using TA_Base_Core::DataException;
		using TA_Base_Core::DatabaseException;

		std::vector<ILocation*> aLocations;	
		std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
		unsigned long locationKey = ULONG_MAX;
		int stepPosition = 0;
		bool isCentralProfile = false;
		try
		{
			TA_Base_Bus::SessionInfo sessionInfo = m_authenticationLibrary->getSessionInfo(sessionId, sessionId);
			stepPosition = 1;
			locationKey = sessionInfo.LocationId;
			m_curLoginLocation = locationKey;
			if(locationKey != 0)
			{
				std::vector<unsigned long > profileKeys = m_authenticationLibrary->getProfiles(sessionId);
				std::vector<unsigned long>::iterator profileIt;
				for (profileIt = profileKeys.begin(); profileIt != profileKeys.end(); profileIt++)
				{
					TA_Base_Core::IProfile* profile = m_profileAccessFactory.getProfile(*profileIt);

					if (!profile->requiresLocationToBeSelected())
					{
						isCentralProfile = true;
						break;
					}

					delete profile;
				}
			}

			stepPosition = 2;
			if (locationKey == 0 || isCentralProfile )																	
			{
				locs[0] = std::string("ALL");
				aLocations = TA_Base_Core::LocationAccessFactory::getInstance().getAllLocations(false);
			}
			else
			{
				ILocation* currentloc = TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(locationKey);
				aLocations.push_back(currentloc);
			}

			stepPosition = 3;
			for (std::vector<ILocation*>::iterator iter = aLocations.begin(); iter != aLocations.end(); ++iter)
			{
				try
				{
					// OCC need get rid of TDS location
					std::string locName = (*iter)->getName();
					bool isTDSLocAndInOCC = (locName == "TDS" && (isCentralProfile || locationKey == 0));
					unsigned long locKey = (*iter)->getKey();
					if ( locKey > 0  && !isTDSLocAndInOCC)
					{
						locs[locKey] = (*iter)->getDisplayName();
					}
				}
				catch (...)
				{
					
				}

				delete *iter;
				*iter = NULL;
			}
		}
		catch (...)
		{
			isSuccess = false;
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
				"PopulationLocation catch exception, position:%d", stepPosition);
		}

		FUNCTION_EXIT;
		return isSuccess;
	}
	
	void EquipmentItemPopulateLogic::populateEquipmentStatusList(unsigned long locationKey)
	{
		if(m_selectedLocationKey == locationKey) return;
		
		//1. stop DataNodeAgent from polling
		{
			TA_Base_Core::ThreadGuard guard(m_DataNodeAgentProxyLock);
			std::map< std::string, DataNodeAgentProxy* >::iterator itDnAgent;
			for(itDnAgent = m_DatanodeAgentProxy.begin(); itDnAgent != m_DatanodeAgentProxy.end(); ++itDnAgent)
			{
				(itDnAgent->second)->pausePoll();
			}
		}

		//2. terminate DnUpdateProcessor
		m_DnUpdateProcessor->terminateAndWait();

		//3. clear resource
		{
			TA_Base_Core::ThreadGuard guard(m_reportItemsLock);
			m_reportItems.clear();
		}
		
		//4. tell UI clean list
		notifyObservers(EDSV::DOMAIN_DATA_CHANGE_REPOPULATE_ITEM.c_str());

		//4. reset and restart DnUpdateProcessor
		m_DnUpdateProcessor->clearQueue();
		m_DnUpdateProcessor->start();

		//5. recalculate DataNodeAgentProxy according to new locationKey
		try
		{
			std::vector<std::string> AgentNames;
			std::map<unsigned long, std::vector<std::string> >::iterator itAgents = m_dnAgentsMap.find(locationKey);
			if (itAgents != m_dnAgentsMap.end())
			{
				AgentNames.insert(AgentNames.end(), (itAgents->second).begin(), (itAgents->second).end());
			}
			else
			{
				AgentNames = TA_Base_Core::EntityAccessFactory::getInstance().getAgentNamesOfTypeAtLocation("DataNode",locationKey);
				m_dnAgentsMap[locationKey] = AgentNames;
			}
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "get agent count:%d at location:%d", AgentNames.size(), locationKey);

			std::vector<std::string>::iterator itAgentName;
			std::map< std::string, DataNodeAgentProxy* >::iterator itDnAgentProxy;

			TA_Base_Core::ThreadGuard guard(m_DataNodeAgentProxyLock);
			for(itAgentName = AgentNames.begin(); itAgentName != AgentNames.end(); ++itAgentName)
			{
				if (*itAgentName != PARENT_ENTITY_NAME)
				{
					itDnAgentProxy = m_DatanodeAgentProxy.find(*itAgentName);
					if(itDnAgentProxy != m_DatanodeAgentProxy.end())
					{
						(itDnAgentProxy->second)->setToBadNotConnectedQuality();
						(itDnAgentProxy->second)->resumePoll();
					}
					else
					{
						DataNodeAgentProxy* pDnAgentProxy = new DataNodeAgentProxy(*itAgentName, this);
						m_DatanodeAgentProxy[*itAgentName] = pDnAgentProxy;
						pDnAgentProxy->start();
					}
				}
				else
				{
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo,
						"agent name is 'Parent Entity'");
				}
			}

			m_selectedLocationKey = locationKey;
		}
		catch (...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "populateEquipmentStatusList exception, location key:%d", locationKey);
		}
	}
	
	void EquipmentItemPopulateLogic::processSessionIdChange()
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Handle sessionId changed message");
		
		checkPermission();

		{
			TA_Base_Core::ThreadGuard guard(m_DataNodeAgentProxyLock);
			std::map< std::string, DataNodeAgentProxy* >::iterator itDnAgent;
			for(itDnAgent = m_DatanodeAgentProxy.begin(); itDnAgent != m_DatanodeAgentProxy.end(); ++itDnAgent)
			{
				(itDnAgent->second)->pausePoll();
			}
		}

		m_DnUpdateProcessor->terminateAndWait();
		
		{
			TA_Base_Core::ThreadGuard guard(m_reportItemsLock);
			m_reportItems.clear();
		}
		
		m_selectedLocationKey = -1;

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Business logic prepare to notify session changed.");
		notifyObservers(EDSV::DOMAIN_DATA_CHANGE_SESSION_UPDATE.c_str());	
	}

	unsigned long EquipmentItemPopulateLogic::getCurrentLoginLocation()
	{
		return m_curLoginLocation;
	}

	bool EquipmentItemPopulateLogic::checkPermission()
	{
		bool isChanged = false;

		if (m_pEntityData != NULL)
		{
			unsigned long entityKey = m_pEntityData->getKey();
			unsigned long locationKey = m_pEntityData->getLocation();
			unsigned long physicalSubsystemKey = m_pEntityData->getPhysicalSubsystem();
			unsigned long subsystemKey = m_pEntityData->getSubsystem();

			bool preIsAllowedToConfigure = m_isAllowedToConfigure;
			bool preIsAllowedToPrintOrExport = m_isAllowedToPrintOrExport;
			bool preIsAllowedToViewAllReports = m_isAllowedToViewAllReports;

			TA_Base_Bus::ScadaUtilities& scadaUtilities = TA_Base_Bus::ScadaUtilities::getInstance();
			if(physicalSubsystemKey <=0 )
			{
				subsystemKey=physicalSubsystemKey;
			}

			try
			{
				std::string currentSession = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
				m_isAllowedToConfigure = scadaUtilities.isActionAllowedWithLocSub( (unsigned int) TA_Base_Bus::aca_REPORT_CONFIGURATION_ESV,
					entityKey, locationKey, subsystemKey, currentSession);

				m_isAllowedToPrintOrExport = scadaUtilities.isActionAllowedWithLocSub ( (unsigned int) TA_Base_Bus::aca_PRINT_EXPORT_ESV,
					entityKey, locationKey, subsystemKey, currentSession);

				m_isAllowedToViewAllReports = scadaUtilities.isActionAllowedWithLocSub ( (unsigned int) TA_Base_Bus::aca_VIEW_ALL_REPORTS_ESV,
					entityKey, locationKey, subsystemKey, currentSession);
			}
			catch(...)
			{
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "checkPermission error");
			}

			isChanged = ( preIsAllowedToConfigure != m_isAllowedToConfigure || 
				preIsAllowedToPrintOrExport != m_isAllowedToPrintOrExport ||
				preIsAllowedToViewAllReports != m_isAllowedToViewAllReports );
		}
		
		return isChanged;
	}

	void EquipmentItemPopulateLogic::cleanUpResource()
	{
		try
		{
			if ( !m_hasCleanUp )
			{
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Begin to cleanup Resource before exit");

				m_hasCleanUp = true;

				TA_Base_Core::RunParams::getInstance().deregisterRunParamUser(this);

				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "deregisterRunParamUser");

				TA_Base_Bus::ScadaUtilities::removeInstance();

				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "ScadaUtilities::removeInstance()");

				std::map< std::string, DataNodeAgentProxy* >::iterator itDnAgent;

				{
					TA_Base_Core::ThreadGuard guard(m_DataNodeAgentProxyLock);
					for(itDnAgent = m_DatanodeAgentProxy.begin(); itDnAgent != m_DatanodeAgentProxy.end(); ++itDnAgent)
					{
						(itDnAgent->second)->terminate();
					}
					for(itDnAgent = m_DatanodeAgentProxy.begin(); itDnAgent != m_DatanodeAgentProxy.end(); ++itDnAgent)
					{
						delete itDnAgent->second;
						itDnAgent->second = NULL;
					}
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "clean DatanodeAgentProxy thread");
					m_DatanodeAgentProxy.clear();
				}

				if(m_DnUpdateProcessor != NULL)
				{
					delete m_DnUpdateProcessor;
					m_DnUpdateProcessor = NULL;
				}
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "clean DnUpdateProcessor thread");

				{
					TA_Base_Core::ThreadGuard guard(m_reportItemsLock);
					m_reportItems.clear();
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "clean all Report items");
				}
				
				TA_Base_Bus::ScadaProxyFactory::removeInstance();

				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "ScadaProxyFactory::removeInstance()");

				if (m_authenticationLibrary != NULL)
				{
					delete m_authenticationLibrary;
					m_authenticationLibrary = NULL;
				}

				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "finish cleanup Resource before exit");
			}
		}
		catch ( ... )
		{

		}
	}

	void EquipmentItemPopulateLogic::pocessItemUpdate(unsigned long entityKey, unsigned long colFlags, bool isFiltered, const int* icons, const std::string* texts)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "dazhi-key:%u,flags:%u,filter:%d", entityKey, colFlags, isFiltered);
		EquipmentItemUpdateEvent itemUpdateInfo(entityKey, colFlags, isFiltered);
		notifyObservers(EDSV::DOMAIN_DATA_CHANGE_ITEM_UPDATE.c_str(), &itemUpdateInfo);
	}

	bool EquipmentItemPopulateLogic::isAllowPrintOrExport()
	{
		return m_isAllowedToPrintOrExport;
	}

	const FilterFlagsAndStrings& EquipmentItemPopulateLogic::getFilters()
	{
		return m_currentFilters;
	}
	
	void EquipmentItemPopulateLogic::setSelectReportKey(std::string& reportKey)
	{
		m_selectedReportKey = reportKey;
	}

	std::string EquipmentItemPopulateLogic::getSelectReportKey()
	{
		return m_selectedReportKey;
	}

	void EquipmentItemPopulateLogic::clearCurrentFilters()
	{
		m_currentFilters.clear();

		for (int col = 0; col < ESL_FILTER_OFFSET_MAX; col++)
		{
			m_currentFilters.push_back( FilterElement());
		}
	}

	void EquipmentItemPopulateLogic::unloadFiltersFromDB()
	{
		m_storedFilters.clear();
	}
	
	void EquipmentItemPopulateLogic::combineFilters(std::string& aReportKey)
	{
		clearCurrentFilters();
		unloadFiltersFromDB();
		if (!LoadFiltersFromDB(aReportKey)) return;
		m_currentFilters = m_storedFilters;
		unloadFiltersFromDB();
	}

	void EquipmentItemPopulateLogic::clearCurrentFiltersForActive()
	{
		clearCurrentFilters();

		//  Set "Equipment Only" report type
		//  m_currentFilters[REPORT_TYPE_FILTER_OFFSET] = FilterElement( true, REPORT_TYPE_EQUIPMENT);

		//  Set "Equipment and DataPoint" report type
		m_currentFilters[REPORT_TYPE_FILTER_OFFSET] = FilterElement( true, REPORT_TYPE_ALL);

		//  Set Status filters
		//  First set the common filters for both the Active All & MMS reports
		m_currentFilters[STATUS_FILTER_OFFSET] = FilterElement( true, "");

		std::string toUpperStr = FilterString[MMS_FILTERSTRING];
		std::transform(toUpperStr.begin(), toUpperStr.end(), toUpperStr.begin(), std::toupper);
		m_currentFilters[STATUS_MMS_INHIBIT_FILTER_OFFSET] = FilterElement( true, toUpperStr);

		//  Now set the specific filters for just the Active All report
		if( ACTIVE_PROFILE_ALL_REPORT  == m_selectedReportKey )
		{
			toUpperStr = FilterString[SCAN_FILTERSTRING];
			std::transform(toUpperStr.begin(), toUpperStr.end(), toUpperStr.begin(), std::toupper);
			m_currentFilters[STATUS_SCAN_INHIBIT_FILTER_OFFSET] =   FilterElement( true, toUpperStr);

			toUpperStr = FilterString[ALARM_FILTERSTRING];
			std::transform(toUpperStr.begin(), toUpperStr.end(), toUpperStr.begin(), std::toupper);
			m_currentFilters[STATUS_ALARM_INHIBIT_FILTER_OFFSET] =  FilterElement( true, toUpperStr);

			toUpperStr = FilterString[CONTROL_FILTERSTRING];
			std::transform(toUpperStr.begin(), toUpperStr.end(), toUpperStr.begin(), std::toupper);
			m_currentFilters[STATUS_CONTROL_INHIBIT_FILTER_OFFSET]= FilterElement( true, toUpperStr);

			toUpperStr = FilterString[TAGGED_FILTERSTRING];
			std::transform(toUpperStr.begin(), toUpperStr.end(), toUpperStr.begin(), std::toupper);
			m_currentFilters[STATUS_TAGGED_FILTER_OFFSET] =         FilterElement( true, toUpperStr);

			toUpperStr = FilterString[TAGGED_PTW_FILTERSTRING];
			std::transform(toUpperStr.begin(), toUpperStr.end(), toUpperStr.begin(), std::toupper);
			m_currentFilters[STATUS_TAGGED_PTW_FILTER_OFFSET] =     FilterElement( true, toUpperStr);

			toUpperStr = FilterString[OVERRIDDEN_FILTERSTRING];
			std::transform(toUpperStr.begin(), toUpperStr.end(), toUpperStr.begin(), std::toupper);
			m_currentFilters[STATUS_OVERRIDDEN_FILTER_OFFSET] =     FilterElement( true, toUpperStr);
		}
	}

	bool EquipmentItemPopulateLogic::LoadFiltersFromDB(std::string& aReportKey)
	{
		using TA_Base_Core::IEquipReport;

		IEquipReport *aReport = NULL;

		try
		{
			aReport = TA_Base_Core::EquipReportAccessFactory::getInstance().getEquipReportByKey(aReportKey);
		}
		catch (...)
		{
			// something went wrong
			return false;
		}

		m_storedFilters.push_back( FilterElement( aReport->getReportTypeFilterFlag(), aReport->getReportTypeFilter()) );
		m_storedFilters.push_back( FilterElement( aReport->getAssetFilterFlag(), aReport->getAssetFilter()) );
		m_storedFilters.push_back( FilterElement( aReport->getDescFilterFlag(), aReport->getDescFilter()) );
		m_storedFilters.push_back( FilterElement( aReport->getDPNameFilterFlag(), aReport->getDPNameFilter()) );
		m_storedFilters.push_back( FilterElement( aReport->getDPTypeFilterFlag(), aReport->getDPTypeFilter()) );
		m_storedFilters.push_back( FilterElement( aReport->getDPValueFilterFlag(), aReport->getDPValueFilter()) );
		m_storedFilters.push_back( FilterElement( aReport->getStatusFilterFlag(), "") );
		m_storedFilters.push_back( FilterElement( aReport->getScanInhibitFlag(), "") );
		m_storedFilters.push_back( FilterElement( aReport->getAlarmInhibitFlag(), "") );
		m_storedFilters.push_back( FilterElement( aReport->getControlInhibitFlag(), "") );
		m_storedFilters.push_back( FilterElement( aReport->getMmsInhibitFlag(), "") );
		m_storedFilters.push_back( FilterElement( aReport->getTaggedFlag(), "") );
		m_storedFilters.push_back( FilterElement( aReport->getTaggedPTWFlag(), "") );
		m_storedFilters.push_back( FilterElement( aReport->getOverriddenFlag(), "") );
		m_storedFilters.push_back( FilterElement( aReport->getStatusChangeByFilterFlag(), aReport->getStatusChangeByFilter()) );
		m_storedFilters.push_back( FilterElement( aReport->getTimestampFilterFlag(), aReport->getTimestampFilter()) );

		delete aReport;

		return true;
	}

	bool EquipmentItemPopulateLogic::isColUpdate(int col, unsigned long colUpdateFlags)
	{
		EslColumns column = static_cast<EslColumns>(col);
		bool isUpdate = false;

		if (column >= ESL_COLUMN_MIN && column < ESL_COLUMN_MAX)
		{
			unsigned long updateFlags = colUpdateFlags;
			updateFlags &= (1 << column);
			if (updateFlags > 0)
			{
				isUpdate = true;
			}
		}

		return isUpdate;
	}	

	bool EquipmentItemPopulateLogic::getEquipmentItemInfo(unsigned long entityKey, EquipmentItemInfo& itemInfo)
	{
		bool isFound = false;

		TA_Base_Core::ThreadGuard guard(m_reportItemsLock);

		EquipmentStatusListItemMap::iterator itItem = m_reportItems.find(entityKey);
		if( itItem != m_reportItems.end() )
		{
			EquipmentStatusListItemSharedPtr item = itItem->second;
			for (int iCol = ESL_COLUMN_MIN; iCol < ESL_COLUMN_MAX; ++iCol)
			{
				itemInfo.m_icon[iCol] = item->getColumnIcon((EslColumns)iCol);
				itemInfo.m_text[iCol] = item->getColumnText((EslColumns)iCol);
			}
			isFound = true;
		}

		return isFound;
	}

	void* EquipmentItemPopulateLogic::getSortDataPtr()
	{
		return static_cast<void*>(&m_reportItems);
	}

	int EquipmentItemPopulateLogic::compareItems(unsigned long key1, unsigned long key2, Sort_Parameters* sortPara)
	{
		//  Compare two lines (rows) in the list control and return the relative position
		//  of item 1 to item 2
		//  ie. return -1 if item 1 should precede item 2, and +1 if it should go after item 2
		//
		int cmpResult = 0;
		bool bAsc     = sortPara->ascending;
		int  col	  = sortPara->sortColumn;
		// we should protect this map here
		EquipmentStatusListItemMap* pReportItems = static_cast<EquipmentStatusListItemMap*>(sortPara->realData);
		if (pReportItems != NULL)
		{
			//TA_Base_Core::ThreadGuard guard(m_reportItemsLock);

			EquipmentStatusListItemMap::const_iterator it1 = pReportItems->find(key1);
			EquipmentStatusListItemMap::const_iterator it2 = pReportItems->find(key2);
			if ( (pReportItems->end() == it1) || (pReportItems->end() == it2) )
			{
				cmpResult = 0;
			}

			cmpResult = it1->second->compare( *it2->second, (EslColumns)col);
			if (!bAsc)
			{
				cmpResult = -cmpResult;
			}
		}
		else
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "compare item convert ItemMap failed");
			cmpResult = 0;
		}
		
		return cmpResult;
	}

	std::string EquipmentItemPopulateLogic::getDefaultExportPath()
	{
		return TA_Base_Core::RunParams::getInstance().get(RPARAM_EXPORTDIR.c_str());
	}

	std::string EquipmentItemPopulateLogic::getCurSessionLocDisplayName()
	{
		std::string locDisplayName;
		TA_Base_Core::Session session(TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID));
		TA_Base_Core::ILocation* currentloc = NULL;
		try
		{
			currentloc= TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey( session.getLocationKey());
			locDisplayName = currentloc->getDisplayName();
		}
		catch (...)
		{
		}

		if(currentloc != NULL)
		{
			delete currentloc;
		}
		return locDisplayName;
	}

	void EquipmentItemPopulateLogic::logAuditMessage(std::string &aReportName, std::string &aLocationName, EDSV_AUDIT_MSG eMsgType)
	{
		CAuditMessageUtility auditLogWriter;
		switch(eMsgType)
		{
		case EAM_PRINT_REPORT:
			auditLogWriter.LogAuditMessage(aReportName, aLocationName, TA_Base_Core::ESVAudit::PrintReportLog);
			break;
		case EAM_CREATE_REPORT:
			auditLogWriter.LogAuditMessage(aReportName, aLocationName, TA_Base_Core::ESVAudit::CreateReportLog);
			break;
		case EAM_DEL_REPORT:
			auditLogWriter.LogAuditMessage(aReportName, aLocationName, TA_Base_Core::ESVAudit::DeleteReportLog);
			break;
		case EAM_MOD_REPORT:
			auditLogWriter.LogAuditMessage(aReportName, aLocationName, TA_Base_Core::ESVAudit::ModifyReportLog);
			break;
		default:
			break;
		}
	}
}

