#include "meterlistctrlmodel.h"
//#include "EquipmentProxy.h"
#include "bus\scada\proxy_library\src\IDataPointProxy.h"
#include "core\exceptions\src\ScadaProxyException.h"
#include "inspectorpanelmodeleventargs.h"

namespace TA_Base_App
{
	MeterListCtrlModel::MeterListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: AbstractEquipmentPModel(appController, pmodelName)
	{

	}


	MeterListCtrlModel:: ~MeterListCtrlModel(void)
	{

	}


	MeterItem	MeterListCtrlModel::getMeterListCtrlItem(unsigned int iPoint)
	{
		return m_displayLogic->getMeterItemDisplayItem(iPoint);
	}


	std::vector<MeterItem>		MeterListCtrlModel::getAllMeterDisplayItem()
	{
		return m_displayLogic->getAllMeterDisplayItem();
	}
	
	bool	MeterListCtrlModel::setMeteredCorrectionValue(unsigned int iPoint, double correction)
	{
		std::string responseString;
		//bool result =
		m_outputLogic->setMeteredCorrectionValue(iPoint,correction,responseString);
		return true;
	}


	double	MeterListCtrlModel::getChildValueAsDouble(unsigned int iPoint)
	{
		return m_displayLogic->getChildValueAsDouble(iPoint);
	}


	bool	MeterListCtrlModel::checkValueAdjustable(unsigned int iPoint, const std::string& newValue,double& correction,double& adjustedValue)
	{
		return m_displayLogic->checkValueAdjustable(iPoint,newValue,correction,adjustedValue);
	}

	std::string	MeterListCtrlModel::getChildDataPointDescription(unsigned int iPoint)
	{
		return m_displayLogic->getDataPointDescription(iPoint);
	}


	bool	MeterListCtrlModel::isMeterAdjustPermitted(unsigned int iPoint)
	{
		return m_displayLogic->isActionPermitted(iPoint,DATA_POINT_METER_ADJUST);
	}

	std::string	MeterListCtrlModel::getChildValueAsString(UINT iPoint)
	{
		return	m_displayLogic->getChildValueAsString(iPoint);
	}

	std::string		MeterListCtrlModel::getDataNodeAssetName()
	{
		return m_displayLogic->getDataNodeAssetName();
	}

	std::string		MeterListCtrlModel::getDataNodeDescription()
	{
		return m_displayLogic->getDataNodeDescription();
	}

	std::string		MeterListCtrlModel::getDataNodeLabel()
	{
		return m_displayLogic->getDataNodeLabel();
	}

	void		MeterListCtrlModel::onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		switch (updateType)
		{
		case ValueStateUpdate:
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

	bool	MeterListCtrlModel::isValid(unsigned int iPoint)
	{
		std::vector<unsigned long>& vectorIndex = m_displayLogic->getMeterIndex(); 
		bool isValid = false;
		for(unsigned int index =0; index < vectorIndex.size(); ++index)
		{
			if(vectorIndex.at(index) == iPoint)
			{
				isValid = true;
				break;
			}
		}
		return isValid;
	}
}

