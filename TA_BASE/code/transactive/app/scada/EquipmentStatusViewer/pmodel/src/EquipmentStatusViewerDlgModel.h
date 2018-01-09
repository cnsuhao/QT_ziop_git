#ifndef EQUIPMENT_STATUS_VIEWER_DLG_MODEL_H
#define EQUIPMENT_STATUS_VIEWER_DLG_MODEL_H

#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentItemPopulateLogic.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentItemEventArg.h"
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"


namespace TA_Base_App
{
	class EquipmentStatusViewerDlgModel : public TA_Base_Bus::AbstractPModel
	{
	public:
		EquipmentStatusViewerDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		~EquipmentStatusViewerDlgModel();
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);
		virtual void allocViewResource();
		virtual void cleanup();
		bool populateLocation(std::map<unsigned long, std::string>& locs);
		void populateEquipmentStatusList(unsigned long locationKey);
		bool isAllowPrintOrExport();
		unsigned long getCurrentLoginLocation();
		void setSelectReportKey(std::string& reportKey);
		std::string getSelectReportKey();
		void clearCurrentFiltersForActive();
		bool isColUpdate(int col, unsigned long colUpdateFlags);
		bool getEquipmentItemInfo(unsigned long entityKey, EquipmentItemInfo& itemInfo);
		void* getSortDataPtr();
		static int compareItems(unsigned long key1, unsigned long key2, Sort_Parameters* sortPara);
		std::string getDefaultExportPath();
		std::string getCurSessionLocDisplayName();
		void logAuditMessage(std::string &aReportName, std::string &aLocationName, EDSV_AUDIT_MSG eMsgType);
	protected:
		void releaseViewResource();

	private:
		EquipmentItemPopulateLogic* m_populatelogic;
		TA_Base_Core::ReEntrantThreadLockable	   m_listItemInfosLock;
		std::map<unsigned long, EquipmentItemInfo> m_listItemInfos;
	};
}

#endif