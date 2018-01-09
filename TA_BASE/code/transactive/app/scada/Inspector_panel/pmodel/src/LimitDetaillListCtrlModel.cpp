#include "bus\scada\proxy_library\src\IDataPointProxy.h"
#include "core\exceptions\src\ScadaProxyException.h"
#include "CommonExceptionsCorbaDef.h"
#include "limitdetailllistctrlmodel.h"
#include "bus\scada\common_library\src\DpValue.h"
#include "inspectorpanelmodeleventargs.h"
#include "ConvertFunction.h"

namespace TA_Base_App
{

	LimitDetaillListCtrlModel::LimitDetaillListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: AbstractEquipmentPModel(appController, pmodelName),
		m_iPoint(0xFFFFFFFF)

	{

	}


	LimitDetaillListCtrlModel::~LimitDetaillListCtrlModel(void)
	{

	}


	std::vector<LimitDetailItem>	LimitDetaillListCtrlModel::getLimitDetaillListCtrlItem(unsigned int iPoint)
	{
		m_iPoint = iPoint;
		return m_displayLogic->getAllLimitDetailDisplayItem(iPoint);
	}

	std::string		LimitDetaillListCtrlModel::getChildDataPointDescription(UINT iPoint)
	{
		return	m_displayLogic->getDataPointDescription(iPoint);
	}

	bool	LimitDetaillListCtrlModel::isWritableDp(unsigned int iPoint)
	{
		return m_displayLogic->isWritableDp(iPoint);
	}

	std::string	LimitDetaillListCtrlModel::getChildEntityName(unsigned int iPoint)
	{
		return m_displayLogic->getDataPointName(iPoint);
	}

	/*void LimitDetaillListCtrlModel::fillThresholdString(LimitDetailItem& item, unsigned int iPoint)
	{
		// Threshold Value
		std::ostringstream threshold;
		threshold.clear();
		threshold.precision(10);

		threshold << item.thresholdValue;

			// if eng unit is defined, attached eng unit to the value
		if ( getChildDataPoint(iPoint)->getType() == DPT_ANALOGUE && false == getChildDataPoint(iPoint)->getUnit().empty() )
		{
				threshold << " " << getChildDataPoint(iPoint)->getUnit();
		}


		std::string currentValue = threshold.str() ;
		std::string::size_type posSearch = currentValue.find("0C"); 
		if( posSearch != std::string::npos )
		{
			currentValue.erase( posSearch, 1);
			//						currentValue.insert(posSearch, "?);
		}

		// CL16786++
		if( item.typeString == "OH" )
		{
			currentValue.insert( 0, "> " );
		}
		else if(item.typeString == "OL" )
		{
			currentValue.insert( 0, "< " );
		}
		// ++CL16786
		item.thresholdString = currentValue;
	}*/

	std::string LimitDetaillListCtrlModel::alarmTypeToString(TA_Base_Bus::EDataPointAlarms alarmType)
	{
		return ConvertFunction::convertAlarmTypeToString(alarmType);
	}


	std::string		LimitDetaillListCtrlModel::getChildUnit(unsigned int iPoint)
	{
		return	m_displayLogic->getDataPointUnit(iPoint);
	}

	double	LimitDetaillListCtrlModel::getChildLimit(unsigned int iPoint,bool isLow)
	{
		return m_displayLogic->getDataPointLimit(iPoint,isLow);
	}



	bool		LimitDetaillListCtrlModel::setAlarmProperties (unsigned int iPoint, const AlarmProperty & newAlarmProperty )
	{
		std::string responseString;
		//bool result = 
		m_outputLogic->setAlarmProperties(iPoint,newAlarmProperty,responseString);
		this->postStatusLine(responseString);
		return true;
	}

	bool		LimitDetaillListCtrlModel::getAlarmProperty(unsigned int iPoint, const char* alarmType,AlarmProperty& config)
	{
		return m_displayLogic->findAlarmProperty(iPoint,alarmType,config);
	}


	bool		LimitDetaillListCtrlModel::isAlarmTypeMatchable(unsigned int iPoint, const std::string& alarmType)
	{
		AlarmProperty config;
		return m_displayLogic->findAlarmProperty(iPoint,alarmType,config);
	}

	void LimitDetaillListCtrlModel::onDataPointProxyUpdate(unsigned long key, ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataPoint()");
		switch(updateType)
		{
		case ValueStateUpdate:
		case ConfigStateUpdate:
			{
				unsigned int iPoint = m_displayLogic->getChildDataPointIndexFromEntityKey(key);
				if(iPoint == m_iPoint)
				{
					TA_Base_Bus::EventArgs* pEventArgs = new InspectorPanelModelEventArgs(iPoint);
					this->notifyViews(PMODEL_NOTIFY_UPDATE_DATA,pEventArgs);
				}
			}

		default:
			break;
		}

		FUNCTION_EXIT;
	}

}
