#include "app/scada/EquipmentStatusViewer/pmodel/src/CustomDefines.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentStatusViewerDlgModel.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentItemEventArg.h"

namespace TA_Base_App
{
	EquipmentStatusViewerDlgModel::EquipmentStatusViewerDlgModel( TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName )
		:
	TA_Base_Bus::AbstractPModel(appController, pmodelName),
	m_populatelogic(NULL)
	{
		m_populatelogic = new EquipmentItemPopulateLogic(appController->getEntity());
		m_populatelogic->registerObserver(this, EDSV::DOMAIN_DATA_CHANGE_ITEM_UPDATE.c_str());
		m_populatelogic->registerObserver(this, EDSV::DOMAIN_DATA_CHANGE_REPOPULATE_ITEM.c_str());
		m_populatelogic->registerObserver(this, EDSV::DOMAIN_DATA_CHANGE_SESSION_UPDATE.c_str());
	}

	EquipmentStatusViewerDlgModel::~EquipmentStatusViewerDlgModel()
	{
		if (m_populatelogic != NULL)
		{
			delete m_populatelogic;
			m_populatelogic = NULL;
		}
	}

	void EquipmentStatusViewerDlgModel::notifyUpdate( const char * updateType, TA_Base_Bus::EventArgs * args/*=NULL*/ )
	{
		notifyViews(updateType, args);
	}

	void EquipmentStatusViewerDlgModel::allocViewResource()
	{

	}
	
	void EquipmentStatusViewerDlgModel::cleanup()
	{
		m_populatelogic->cleanUpResource();
	}

	void EquipmentStatusViewerDlgModel::releaseViewResource()
	{

	}

	bool EquipmentStatusViewerDlgModel::populateLocation( std::map<unsigned long, std::string>& locs )
	{
		bool isSuccess = m_populatelogic->populateLocations(locs);
		return isSuccess;
	}

	void EquipmentStatusViewerDlgModel::populateEquipmentStatusList(unsigned long locationKey)
	{
		m_populatelogic->populateEquipmentStatusList(locationKey);
	}

	bool EquipmentStatusViewerDlgModel::isAllowPrintOrExport()
	{
		return m_populatelogic->isAllowPrintOrExport();
	}

	unsigned long EquipmentStatusViewerDlgModel::getCurrentLoginLocation()
	{
		return m_populatelogic->getCurrentLoginLocation();
	}

	void EquipmentStatusViewerDlgModel::setSelectReportKey(std::string& reportKey)
	{
		m_populatelogic->setSelectReportKey(reportKey);
	}

	std::string EquipmentStatusViewerDlgModel::getSelectReportKey()
	{
		return m_populatelogic->getSelectReportKey();
	}

	void EquipmentStatusViewerDlgModel::clearCurrentFiltersForActive()
	{
		m_populatelogic->clearCurrentFiltersForActive();
	}

	bool EquipmentStatusViewerDlgModel::isColUpdate(int col, unsigned long colUpdateFlags)
	{
		return m_populatelogic->isColUpdate(col, colUpdateFlags);
	}

	bool EquipmentStatusViewerDlgModel::getEquipmentItemInfo(unsigned long entityKey, EquipmentItemInfo& itemInfo)
	{
		return m_populatelogic->getEquipmentItemInfo(entityKey, itemInfo);
	}

	int EquipmentStatusViewerDlgModel::compareItems(unsigned long key1, unsigned long key2, Sort_Parameters* sortPara)
	{
		return EquipmentItemPopulateLogic::compareItems(key1, key2, sortPara);
	}

	std::string EquipmentStatusViewerDlgModel::getDefaultExportPath()
	{
		return m_populatelogic->getDefaultExportPath();
	}

	std::string EquipmentStatusViewerDlgModel::getCurSessionLocDisplayName()
	{
		return m_populatelogic->getCurSessionLocDisplayName();
	}

	void* EquipmentStatusViewerDlgModel::getSortDataPtr()
	{
		return m_populatelogic->getSortDataPtr();
	}

	void EquipmentStatusViewerDlgModel::logAuditMessage(std::string &aReportName, std::string &aLocationName, EDSV_AUDIT_MSG eMsgType)
	{
		m_populatelogic->logAuditMessage(aReportName, aLocationName, eMsgType);
	}
}