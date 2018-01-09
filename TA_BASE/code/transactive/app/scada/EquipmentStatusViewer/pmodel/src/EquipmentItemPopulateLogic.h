#ifndef EQUIPMENT_ITEM_POPULATE_LOGIC_H
#define EQUIPMENT_ITEM_POPULATE_LOGIC_H

#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentItemEventArg.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/DnUpdateProcessor.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentStatusListItem.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
#include "bus/scada/proxy_library/src/ScadaProxyFactory.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/utilities/src/RunParams.h"

namespace TA_Base_App
{
	class DataNodeAgentProxy;

	class EquipmentItemPopulateLogic : public DnUpdateProcessCallBack,
									   public TA_Base_Core::RunParamUser, 
									   public TA_Base_Bus::AbstractStateChangeSubject<TA_Base_Bus::IStateChangeObserver>							
	{
	public:
		EquipmentItemPopulateLogic(TA_Base_Core::IEntityData* pEntityData);
		virtual ~EquipmentItemPopulateLogic();
		void processorCallBack(boost::shared_ptr<DnUpdateRequest> newRequest);
		void onRunParamChange(const std::string& name, const std::string& value);
		void notifyDnUpdate(std::vector<unsigned long> & dnKeys);

		/**
		 *	PopulateLocations()
		 *
		 *	Populates the combo box with all available locations
		 *
		 */
		bool populateLocations(std::map<unsigned long, std::string>& locs);

		/**
		 *	PopulateEquipmentStatusList()
		 *
		 *	Populates the equipment status list with equipment/datapoint information corresponding
		 *	to the selected report.
		 *
		 *	@param	locationKey	location key of populating equipment status
		 *
		 */
		void populateEquipmentStatusList(unsigned long locationKey);

		bool isAllowPrintOrExport();
	
		unsigned long getCurrentLoginLocation();
		/**
		 *	CleanUpResource()
		 *
		 *	call before process exit, only call one time
		 *
		 */
		void cleanUpResource();

		/**
		 *	pocessItemUpdate()
		 *
		 *	call before process exit, only call one time
		 *
		 *	@param	entityKey	item entitykey which need update
		 */
		void pocessItemUpdate(unsigned long entityKey, unsigned long colFlags, bool isFiltered, 
			const int* icons = NULL, const std::string* texts = NULL);
	
		const FilterFlagsAndStrings& getFilters();

		void setSelectReportKey(std::string& reportKey);

		std::string getSelectReportKey();

		void clearCurrentFiltersForActive();

		void unloadFiltersFromDB();

		void combineFilters(std::string& aReportKey);

		bool isColUpdate(int col, unsigned long colUpdateFlags);

		bool getEquipmentItemInfo(unsigned long entityKey, EquipmentItemInfo& itemInfo);
		
		void* getSortDataPtr();

		static int compareItems(unsigned long key1, unsigned long key2, Sort_Parameters* sortPara);

		std::string getDefaultExportPath();

		std::string getCurSessionLocDisplayName();

		void logAuditMessage(std::string &aReportName, std::string &aLocationName, EDSV_AUDIT_MSG eMsgType);
	private:
		bool checkPermission();
		void processSessionIdChange();
		void clearCurrentFilters();
		/**
		 *	LoadFiltersFromDB()
		 *
		 *	Loads filters from the database into the collection
		 *
		 *	@param	aReportKey	Primary key of the report for which to read filters
		 *	@return				Flag to signify success (true) or failure (false)
		 *
		 */
        bool LoadFiltersFromDB(std::string& aReportKey);
	private:
		bool											m_isAllowedToPrintOrExport;
		bool											m_isAllowedToConfigure;
		bool											m_isAllowedToViewAllReports;
		bool											m_hasCleanUp;
		unsigned long									m_curLoginLocation;
		unsigned long									m_selectedLocationKey;
		DnUpdateProcessor*								m_DnUpdateProcessor;
		TA_Base_Core::IEntityData*						m_pEntityData;
		TA_Base_Bus::AuthenticationLibrary*				m_authenticationLibrary;
		TA_Base_Core::ProfileAccessFactory&				m_profileAccessFactory;
		TA_Base_Bus::ScadaProxyFactory&					m_scadaProxyFactory;
		std::string										m_previousSessionId;
		std::string										m_selectedReportKey;
		TA_Base_Core::ReEntrantThreadLockable		    m_DataNodeAgentProxyLock;
		std::map< std::string, DataNodeAgentProxy* >	m_DatanodeAgentProxy;
		std::map<unsigned long, boost::shared_ptr<TA_Base_Core::DataNodeEntityData> > m_dnEntityDataPtrMap;
		std::map< unsigned long, std::vector<std::string> >	m_dnAgentsMap;
		EquipmentStatusListItemMap						m_reportItems;
		TA_Base_Core::ReEntrantThreadLockable		    m_reportItemsLock;
		FilterFlagsAndStrings							m_currentFilters;
		FilterFlagsAndStrings							m_storedFilters;
		static const std::string						PARENT_ENTITY_NAME;
	};
}

#endif