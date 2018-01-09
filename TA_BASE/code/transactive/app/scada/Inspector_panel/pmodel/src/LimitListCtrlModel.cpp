#include "bus\scada\proxy_library\src\IDataPointProxy.h"
#include "core\exceptions\src\ScadaProxyException.h"
#include "CommonExceptionsCorbaDef.h"
#include "limitlistctrlmodel.h"
#include "inspectorpanelmodeleventargs.h"


namespace TA_Base_App
{

	LimitListCtrlModel::LimitListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: AbstractEquipmentPModel(appController, pmodelName)
	{
	}


	LimitListCtrlModel::~LimitListCtrlModel(void)
	{

	}


	std::vector<unsigned long>		LimitListCtrlModel::getLimitIndex()
	{
		return m_displayLogic->getLimitIndex();
	}


	std::string						LimitListCtrlModel::getLimitInfoString(unsigned int iPoint)
	{
		return m_displayLogic->getLimitInfoString(iPoint);
	}


	void		LimitListCtrlModel::onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		switch (updateType)
		{
		case ConfigStateUpdate:
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


	bool	LimitListCtrlModel::isValid(unsigned int iPoint)
	{
		return m_displayLogic->isLimitDp(iPoint);
	}


	std::string		LimitListCtrlModel::getChildDataPointDescription(UINT iPoint)
	{
		return	m_displayLogic->getDataPointDescription(iPoint);
	}

	bool	LimitListCtrlModel::isLimitsAccessPermitted(unsigned int iPoint)
	{
		//return AccessControlHelper::getInstance()->IsActionPermitted(getChildDataPoint(iPoint)->getEntityKey(),action);
		return m_displayLogic->isActionPermitted(iPoint, IP_LIMITS_TAB_ACCESS);
	}
}
