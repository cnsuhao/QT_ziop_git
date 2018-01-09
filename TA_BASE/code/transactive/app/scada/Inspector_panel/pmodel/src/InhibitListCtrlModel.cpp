#include "inhibitlistctrlmodel.h"
#include "bus\scada\proxy_library\src\IDataPointProxy.h"
#include "bus\scada\proxy_library\src\IDataNodeProxy.h"
#include "core\exceptions\src\ScadaProxyException.h"
#include "core\message\src\NameValuePair.h"
#include "core\message\types\InspectorPanelAudit_MessageTypes.h"
#include "inspectorpanelmodeleventargs.h"



namespace TA_Base_App
{
	InhibitListCtrlItem::InhibitListCtrlItem()
	:isWritable(false),
	isCtrlInhibit(false),
	isScanInhibit(false),
	isAlarmInhibit(false),
	isOverrideInhibit(false),
	isMmsInhibit(false)
	{
	}

	InhibitListCtrlModel::InhibitListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: AbstractEquipmentPModel(appController, pmodelName)
	{

	}

	InhibitListCtrlModel::~InhibitListCtrlModel()
	{

	}


	InhibitItem InhibitListCtrlModel::getInhibitListCtrlItem(unsigned int iPoint)
	{
		return m_displayLogic->getInhibitDisplayItem(iPoint);
	}


	std::vector<InhibitItem>	InhibitListCtrlModel::getAllInhibitDisplayItem()
	{
		return m_displayLogic->getAllInhibitDisplayItem();
	}


	/*bool	InhibitListCtrlModel::isChildScanInhibited(unsigned int iPoint)
	{
		return getChildDataPoint(iPoint)->isScanInhibited();
	}

	bool	InhibitListCtrlModel::isChildMmsAlarmInhibited(unsigned int iPoint)
	{
		return getChildDataPoint(iPoint)->isMmsAlarmInhibited();
	}

	bool	InhibitListCtrlModel::isChildAlarmInhibited(unsigned int iPoint)
	{
		return getChildDataPoint(iPoint)->isAlarmInhibited();
	}

	void	InhibitListCtrlModel::setInputInhibitStatus(unsigned int iPoint,EInputInhibitState inhibit, const std::string& sessionId)
	{
		getChildDataPoint(iPoint)->setInputInhibitStatus(inhibit,sessionId);
	}

	void	InhibitListCtrlModel::setOutputInhibitStatus(unsigned int iPoint,EOutputInhibitState inhibit,const std::string& sessionId)
	{
		getChildDataPoint(iPoint)->setOutputInhibitStatus(inhibit,sessionId);
	}*/

	bool	InhibitListCtrlModel::isWritableDp(unsigned int iPoint)
	{
		return m_displayLogic->isWritableDp(iPoint);
	}

	std::string			InhibitListCtrlModel::getDataPointDescription(unsigned int iPoint)
	{
		return m_displayLogic->getDataPointDescription(iPoint);
	}

	bool	InhibitListCtrlModel::isScanInhibitPermit(unsigned int iPoint)
	{
		return m_displayLogic->isActionPermitted(iPoint, DATA_POINT_SCAN_INHIBIT);
	}


	bool	InhibitListCtrlModel::isAlarmInhibitPermit(unsigned int iPoint)
	{
		return m_displayLogic->isActionPermitted(iPoint, DATA_POINT_ALARM_INHIBIT);
	}


	bool	InhibitListCtrlModel::isMmsAlarmInhibitPermit(unsigned int iPoint)
	{
		return m_displayLogic->isActionPermitted(iPoint, DATA_POINT_MMS_INHIBIT);
	}


	bool	InhibitListCtrlModel::isControlInhibitPermit(unsigned int iPoint)
	{
		return m_displayLogic->isActionPermitted(iPoint, DATA_POINT_CONTROL_INHIBIT);
	}

	bool	InhibitListCtrlModel::isOkToSetScanInhibit(unsigned int iPoint, bool bRequestScanInhibit)
	{
		return m_displayLogic->isOkToSetScanInhibit(iPoint,bRequestScanInhibit);
	}

	bool	InhibitListCtrlModel::isOkToSetAlarmInhibit(unsigned int iPoint, bool bRequestAlarmInhibit)
	{
		return m_displayLogic->isOkToSetAlarmInhibit(iPoint,bRequestAlarmInhibit);
	}

	bool	InhibitListCtrlModel::isOkToSetControlInhibit(unsigned int iPoint, bool bRequestControlInhibit)
	{
		return m_displayLogic->isOkToSetControlInhibit(iPoint,bRequestControlInhibit);
	}

	bool	InhibitListCtrlModel::isOkToSetMmsInhibit(unsigned int iPoint, bool bRequestMmsInhibit)
	{
		return m_displayLogic->isOkToSetMmsInhibit(iPoint,bRequestMmsInhibit);
	}


	bool	InhibitListCtrlModel::setScanInhibit(unsigned int iPoint, bool bRequestScanInhibit)
	{
		std::string responseString;
		//bool result = 
		m_outputLogic->setScanInhibit(iPoint,bRequestScanInhibit,responseString);
		//this->postStatusLine(responseString);
		//return result;
		return true;
	}


	bool	InhibitListCtrlModel::setAlarmInhibit(unsigned int iPoint, bool bRequestAlarmInhibit)
	{
		std::string responseString;
		//bool result = 
		m_outputLogic->setAlarmInhibit(iPoint,bRequestAlarmInhibit,responseString);
		this->postStatusLine(responseString);
		return true;
	}


	bool	InhibitListCtrlModel::setControlInhibit(unsigned int iPoint, bool bRequestControlInhibit)
	{
		std::string responseString;
		//bool result = 
		m_outputLogic->setControlInhibit(iPoint,bRequestControlInhibit,responseString);
		this->postStatusLine(responseString);
		return true;
	}


	bool	InhibitListCtrlModel::setMmsInhibit(unsigned int iPoint, bool bRequestMmsInhibit)
	{
		std::string responseString;
		//bool result = 
		m_outputLogic->setMmsInhibit(iPoint,bRequestMmsInhibit,responseString);
		this->postStatusLine(responseString);
		return true;
	}


	bool	InhibitListCtrlModel::isScanInhibitColumnEnable(unsigned int iPoint)
	{
		return m_displayLogic->isScanInhibitable(iPoint);
	}


	bool	InhibitListCtrlModel::isAlarmInhibitColumnEnable(unsigned int iPoint)
	{
		return m_displayLogic->isAlarmInhibitable(iPoint);
	}


	bool	InhibitListCtrlModel::isMmsInhibitColumnEnable(unsigned int iPoint)
	{
		return m_displayLogic->isMmsInhibitable(iPoint);
	}


	bool	InhibitListCtrlModel::isControlInhibitColumnEnable(unsigned int iPoint)
	{
		return m_displayLogic->isControlInhibitable(iPoint);
	}


	void		InhibitListCtrlModel::onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataPointDisplay()");

		switch (updateType)
		{
		case InhibitStateUpdate:
		case ValueStateUpdate:
			{
				unsigned int iPoint = m_displayLogic->getChildDataPointIndexFromEntityKey(key);
				TA_Base_Bus::EventArgs* pEventArgs = new InspectorPanelModelEventArgs(iPoint);
				this->notifyViews(PMODEL_NOTIFY_UPDATE_DATA,pEventArgs);
			}
			break;
		default:
			break;
		}

		FUNCTION_EXIT;
	}


	void		InhibitListCtrlModel::onDataNodePorxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		if (updateType == CombinedInhibitMMSUpdate)
		{
			this->notifyViews(PMODEL_NOTIFY_REFRESH_ALL_DATA,NULL);
		}
	}

	void InhibitListCtrlModel::submitAuditMessage(const std::string& inhibitType, const std::string& addRemove, 
		const std::string& dataPoint, unsigned long entityKey)
	{
		TA_Base_Core::DescriptionParameters descParams;

		TA_Base_Core::NameValuePair inhibitTypePair("InhibitType", inhibitType);
		TA_Base_Core::NameValuePair addRemovePair("AddedRemoved", addRemove);
		TA_Base_Core::NameValuePair dataPointPair("DataPoint", dataPoint);

		descParams.clear();
		descParams.push_back(&inhibitTypePair);
		descParams.push_back(&addRemovePair);
		descParams.push_back(&dataPointPair);
	}

};
