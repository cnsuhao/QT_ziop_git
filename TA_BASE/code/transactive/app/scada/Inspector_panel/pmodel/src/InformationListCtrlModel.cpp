#include "InformationListCtrlModel.h"
#include "bus/scada/proxy_library/src/DataPointProxySmartPtr.h"
#include "bus/scada/proxy_library/src/IDataPointProxy.h"
#include <atltime.h>
#include "core\exceptions\src\ScadaProxyException.h"
#include "inspectorpanelmodeleventargs.h"

namespace TA_Base_App
{

	InformationListCtrlModel::InformationListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: AbstractEquipmentPModel(appController, pmodelName)
	{
		//m_pEquipment->unregisterObserver(this, DOMAIN_DATA_CHANGE_PERTMITTION);
	}


	InformationListCtrlModel::~InformationListCtrlModel()
	{

	}


	void		InformationListCtrlModel::onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType type)
	{
		switch(type)
		{
			case TA_Base_Bus::ValueStateUpdate:
			case TA_Base_Bus::AlarmSummaryUpdate://limin++, TD20740
			case TA_Base_Bus::AckSummaryUpdate:  //limin++, TD20740
			case TA_Base_Bus::QualitySummaryUpdate: // for the case where only quality changes.
				{
					unsigned int iPoit = m_displayLogic->getChildDataPointIndexFromEntityKey(key);
					if(isValid(iPoit))
					{
						TA_Base_Bus::EventArgs* pEventArgs = new InspectorPanelModelEventArgs(iPoit);
						this->notifyViews(PMODEL_NOTIFY_UPDATE_DATA,pEventArgs);
					}
				}
				break;
			
			default:
				break;
		}
	}


	bool	InformationListCtrlModel::isValid(unsigned int iPoit)
	{
		return m_displayLogic->isNotWritableDp(iPoit);
	}


	InformationItem InformationListCtrlModel::getInformationItem(unsigned int iPoint)
	{
		return m_displayLogic->getInfomationDisplayItem(iPoint);
	}

	std::vector<InformationItem>	InformationListCtrlModel::getAllInfomationDisplayItem()
	{
		return m_displayLogic->getAllInfomationDisplayItem();
	}
}