#include "overridelistctrlmodel.h"
#include "bus\scada\proxy_library\src\IDataPointProxy.h"
#include "core\exceptions\src\ScadaProxyException.h"
#include "CommonExceptionsCorbaDef.h"
#include "inspectorpanelmodeleventargs.h"


namespace TA_Base_App
{
	OverrideListCtrlItem::OverrideListCtrlItem()
		:isOverride(false)
	{

	}



	OverrideListCtrlModel::OverrideListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: AbstractEquipmentPModel(appController, pmodelName)
	{

	}


	OverrideListCtrlModel::~OverrideListCtrlModel(void)
	{

	}

	OverrideItem OverrideListCtrlModel::getOverrideItem(unsigned int iPoint)
	{
		return m_displayLogic->getOverrideDisplayItem(iPoint);
	}

	std::vector<OverrideItem>	OverrideListCtrlModel::getAllOverrideItem()
	{
		return m_displayLogic->getAllOverrideDisplayItem();
	}

	bool	OverrideListCtrlModel::isOkToOverride(unsigned int iPoint)
	{
		return m_displayLogic->isOkToOverride(iPoint);
	}

	bool	OverrideListCtrlModel::isOkToRemoveOverride(unsigned int iPoint)
	{
		return m_displayLogic->isOkToRemoveOverride(iPoint);
	}


	bool	OverrideListCtrlModel::isOverrideColumnEnable(unsigned int iPoint)
	{
		return (isOkToOverride(iPoint) || isOkToRemoveOverride(iPoint));
	}

	bool	OverrideListCtrlModel::isChildOverrideEnabled(unsigned int iPoint)
	{
		return m_displayLogic->isOverrideEnabled(iPoint);
	}

	bool	OverrideListCtrlModel::setManuallyOverrideValue (unsigned int iPoint, std::string& overrideValue)
	{
		std::string responseString;
		//return 
		m_outputLogic->setManuallyOverrideValue(iPoint,overrideValue,responseString);
		return true;
	}


	bool	OverrideListCtrlModel::removeOverride (unsigned int iPoint)
	{
		std::string responseString;
		//return 
		m_outputLogic->removeOverride(iPoint,responseString);
		return true;
	}

	bool	OverrideListCtrlModel::isNotWritableDp(unsigned int iPoint)
	{
		return m_displayLogic->isNotWritableDp(iPoint);
	}

	void		OverrideListCtrlModel::onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		switch (updateType)
		{
		case ValueStateUpdate:
		case AlarmSummaryUpdate:
			{
				unsigned int iPoint = m_displayLogic->getChildDataPointIndexFromEntityKey(key);
				if(isValid(iPoint))
				{
					TA_Base_Bus::EventArgs* pEventArgs = new InspectorPanelModelEventArgs(iPoint);
					this->notifyViews(PMODEL_NOTIFY_UPDATE_DATA,pEventArgs);
				}
			}
			break;
		default:
			break;
		}
	}

	bool	OverrideListCtrlModel::isValid(unsigned int iPoint)
	{
		return m_displayLogic->isNotWritableDp(iPoint);
	}

	std::string OverrideListCtrlModel::getDataPointDescription(unsigned int iPoint)
	{
		return m_displayLogic->getDataPointDescription(iPoint);
	}

	bool OverrideListCtrlModel::isDpDiaplayable(unsigned int iPoint)
	{
		return m_displayLogic->getDataPointDisplayOrder(iPoint) > 0;
	}

	std::string	OverrideListCtrlModel::getEquipmentDescription()
	{
		std::string rtnString;

		rtnString	= m_displayLogic->getDataNodeAssetName();
		rtnString	+= "\r\n";
		rtnString	+= m_displayLogic->getDataNodeLabel();
		return rtnString;
	}


	EPModelCellType	OverrideListCtrlModel::getCellTypeFromPoint(unsigned int iPoint)
	{
		if (m_displayLogic->isBoolean(iPoint) || m_displayLogic->isDerived(iPoint))
		{
			return PMCT_SELECTION;
		}
		else if (m_displayLogic->isNumber(iPoint))
		{
			return PMCT_NUMBER;
		}
		else if (m_displayLogic->isText(iPoint))
		{
			return PMCT_TEXT;
		}
		return PMCT_NOTEDITABLE;
	}

	std::vector<std::string>	OverrideListCtrlModel::getCellValues(unsigned int iPoint)
	{
		return m_displayLogic->getAllValues(iPoint);
	}
}