#include "equipmentproxy.h"
#include "bus/scada/proxy_library/src/IDataPointProxy.h"
#include "bus\scada\proxy_library\src\IDataNodeProxy.h"
#include "core\exceptions\src\ScadaProxyException.h"
#include "AccessControlHelper.h"
//#include <afx.h>

namespace TA_Base_App
{
	EquipmentProxy::EquipmentProxy(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		:TA_Base_Bus::AbstractPModel(appController, pmodelName),
		//m_pEquipment(pEquipment),
		m_pRealAppController(NULL)
	{
		m_pRealAppController = dynamic_cast<InspectorpanelAppController*> (appController);
		TA_ASSERT(m_pRealAppController != NULL, "Can't cast to InspectorpanelAppController");

		m_pEquipment = m_pRealAppController->getEquipment();
		TA_ASSERT(m_pEquipment != NULL, "invalid m_pEquipment");

		m_pEquipment->registerObserver(this,DOMAIN_DATA_CHANGE_INIT_STATUS);
		m_pEquipment->registerObserver(this,DOMAIN_DATA_CHANGE_ENTITY_DATA);
		m_pEquipment->registerObserver(this,DOMAIN_DATA_CHANGE_PERTMITTION);
	}

	EquipmentProxy::~EquipmentProxy()
	{
		m_pEquipment->unregisterObserver(this);
		m_pRealAppController =  NULL;
		m_pEquipment =	NULL;
	}

	void EquipmentProxy::notifyUpdate(const char * updateType,TA_Base_Bus::EventArgs * args)
	{
		EquipmentEventArgs *realArgs = dynamic_cast<EquipmentEventArgs*> (args);
		if(_stricmp(updateType, DOMAIN_DATA_CHANGE_INIT_STATUS) == 0)
		{
			this->onInitStatusUpdate(realArgs->getEquipmentInitStatus());
		}
		else if(_stricmp(updateType, DOMAIN_DATA_CHANGE_ENTITY_DATA) == 0)
		{
			if(realArgs->getEntityKey() == getDataNodeEntityKey())
			{
				this->onDataNodeProxyUpdate(realArgs->getEntityKey(), realArgs->getUpdateType());
			}
			else
			{
				this->onDataPointProxyUpdate(realArgs->getEntityKey(), realArgs->getUpdateType());
			}
		}
		else if(_stricmp(updateType, DOMAIN_DATA_CHANGE_PERTMITTION) == 0)
		{
			this->onPertmissionUpdate();
		}
	}


	void		EquipmentProxy::onInitStatusUpdate (EquipmentInitStatus status)
	{
		if(status == INIT_STEP5_COMPLETE)
		{
			this->notifyViews(PMODEL_NOTIFY_REFRESH_ALL_DATA,NULL);
		}
	}

	void		EquipmentProxy::onPertmissionUpdate()
	{
		this->notifyViews(PMODEL_NOTIFY_PERTMISSION,NULL);
	}

	void	EquipmentProxy::setEquipmentPointer(CEquipment* pEquipment)
	{
		m_pEquipment = pEquipment;
	}


	bool	EquipmentProxy::isActionPermitted(unsigned int iPoint, AccessControlActions action)
	{
		return AccessControlHelper::getInstance()->IsActionPermitted(getChildDataPoint(iPoint)->getEntityKey(),action);
	}

	void	EquipmentProxy::postStatusLine(const std::string& statusLine)
	{
		this->m_pRealAppController->showStatusMessage(statusLine);
	}

	UINT	EquipmentProxy::getChildDataPointIndexFromEntityKey(unsigned long key)
	{
		return m_pEquipment->getChildDataPointIndexFromEntityKey(key);
	}


	unsigned int	EquipmentProxy::getChildDataPointCount()
	{
		return m_pEquipment->getChildDataPointCount();
	}

	bool	EquipmentProxy::hasControlPoint()
	{
		return m_pEquipment->hasControlPoint();
	}


	bool	EquipmentProxy::hasInformationPoint()
	{
		return m_pEquipment->hasInformationPoint();
	}


	bool	EquipmentProxy::hasMeteredPoint()
	{
		return m_pEquipment->hasMeteredPoint();
	}


	bool	EquipmentProxy::hasLimitedPoint()
	{
		return m_pEquipment->hasLimitedPoint();
	}


	bool	EquipmentProxy::isEntityBlank()
	{
		return m_pEquipment->isEntityBlank();
	}


	int		EquipmentProxy::getControlPointIndex(unsigned long key)
	{
		return m_pEquipment->getControlPointIndex(key);
	}


	unsigned long	EquipmentProxy::getChildKeyByName(std::string& entityName)
	{
		return m_pEquipment->getChildKeyByName(entityName);
	}


	std::vector<unsigned long>&	EquipmentProxy::getWriteableIndex()
	{
		return m_pEquipment->getWriteableIndex();
	}


	std::vector<unsigned long>&	EquipmentProxy::getNotWriteableIndex()
	{
		return m_pEquipment->getNotWriteableIndex();
	}


	std::vector<unsigned long>&	EquipmentProxy::getLimitIndex()
	{
		return m_pEquipment->getLimitIndex();
	}


	std::vector<unsigned long>&	EquipmentProxy::getMeterIndex()
	{
		return m_pEquipment->getMeterIndex();
	}


	bool	EquipmentProxy::haveAllDataPointsConfigured()
	{
		return m_pEquipment->haveAllDataPointsConfigured();
	}


	TA_Base_Bus::DataNodeProxySmartPtr&		EquipmentProxy::getDataNode()
	{
		return m_pEquipment->getDataNode();
	}


	std::vector<TA_Base_Bus::DataPointProxySmartPtr*>&	EquipmentProxy::getChildDataPointVector()
	{
		return m_pEquipment->getChildDataPointVector();
	}


	TA_Base_Bus::DataPointProxySmartPtr&	EquipmentProxy::getChildDataPoint(UINT iPoint)
	{
		return m_pEquipment->getChildDataPoint(iPoint);
	}


	TA_Base_Bus::DataPointProxySmartPtr&	EquipmentProxy::getChildDataPointFromEntityKey(unsigned long key)
	{
		return m_pEquipment->getChildDataPointFromEntityKey(key);
	}


	bool	EquipmentProxy::isChildInAlarmState(UINT iPoint)
	{
		bool rtn = false;
		rtn = getChildDataPoint(iPoint)->isInAlarmState();
		return rtn;
	}


	unsigned long	EquipmentProxy::getChildDataPointStatus(UINT iPoint)
	{
		return getChildDataPoint(iPoint)->getStatus();
	}

	std::string		EquipmentProxy::getChildDataPointDescription(UINT iPoint)
	{
		return	getChildDataPoint(iPoint)->getDescription();
	}

	std::string			EquipmentProxy::getChildEntityName(unsigned int iPoint)
	{
		return getChildDataPoint(iPoint)->getEntityName();
	}

	unsigned long		EquipmentProxy::getChildEntityKey(unsigned int iPoint)
	{
		return getChildDataPoint(iPoint)->getEntityKey();
	}


	std::string	EquipmentProxy::getChildUnit(UINT iPoint)
	{
		return	getChildDataPoint(iPoint)->getUnit();
	}


	std::string	EquipmentProxy::getChildValueAsString(UINT iPoint)
	{
		return	getChildDataPoint(iPoint)->getValueAsString();
	}


	std::string	EquipmentProxy::getChildAlarmStateShortString(UINT iPoint)
	{
		return	getChildDataPoint(iPoint)->getAlarmStateShortString();
	}

	bool	EquipmentProxy::isChildWritable(unsigned int iPoint)
	{
		bool rtn = false;
		rtn = getChildDataPoint(iPoint)->isWriteable();
		return rtn;
	}

	bool	EquipmentProxy::isChildControlInhibited(unsigned int iPoint)
	{
		bool rtn = false;
		rtn = getChildDataPoint(iPoint)->isControlInhibited();
		return rtn;
	}


	bool	EquipmentProxy::isChildBoolean(unsigned int iPoint)
	{
		bool rtn = false;
		rtn = getChildDataPoint(iPoint)->isBoolean();
		return rtn;
	}


	bool	EquipmentProxy::isChildDerived(unsigned int iPoint)
	{
		bool rtn = false;
		rtn = getChildDataPoint(iPoint)->isDerived();
		return rtn;
	}

	bool	EquipmentProxy::isChildNumber(unsigned int iPoint)
	{
		bool rtn = false;
		rtn = getChildDataPoint(iPoint)->isNumber();
		return rtn;
	}

	bool	EquipmentProxy::isChildText(unsigned int iPoint)
	{
		bool rtn = false;
		rtn = getChildDataPoint(iPoint)->isText();
		return rtn;
	}

	EDataPointControlState	EquipmentProxy::getChildDataPointControlState(unsigned int iPoint)
	{
		return getChildDataPoint(iPoint)->getControlState();
	}

	std::string			EquipmentProxy::getChildDataPointBooleanLabel(unsigned int iPoint, bool booleanValue)
	{
		return getChildDataPoint(iPoint)->getBooleanLabel(booleanValue);
	}


	DpvEnumLabelsMap	EquipmentProxy::getChildDataPointDerivedStatesAsMap(unsigned int iPoint)
	{
		return getChildDataPoint(iPoint)->getDerivedStatesAsMap();
	}


	std::string		EquipmentProxy::getDataNodeAssetName()
	{
		return m_pEquipment->getDataNode()->getAssetName();
	}

	std::string		EquipmentProxy::getDataNodeDescription()
	{
		return m_pEquipment->getDataNode()->getDescription();
	}

	unsigned long	EquipmentProxy::getDataNodeEntityKey()
	{
		return m_pEquipment->getDataNode()->getEntityKey();
	}

	std::string		EquipmentProxy::getDataNodeLabel()
	{
		return m_pEquipment->getDataNode()->getLabel();
	}

	int		EquipmentProxy::getChildDisplayOrder(unsigned int iPoint)
	{
		return getChildDataPoint(iPoint)->getDisplayOrder();
	}

	double	EquipmentProxy::getChildLimit(unsigned int iPoint, bool lowLimit)
	{
		return getChildDataPoint(iPoint)->getLimit(lowLimit);
	}

	std::string		EquipmentProxy::getCurrentValueAsString(DataPointProxySmartPtr& dataPoint)
	{
		std::string currentValue("");
		std::string unitString("");

		// DataPoint Value - may be override or field value
		std::string alarmState = "";
		try
		{
			alarmState = dataPoint->getAlarmStateShortString();
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
		}
		catch (...)
		{
		}

		if (alarmState != "")
		{
			currentValue = alarmState + " ";
		}
		else
		{
			currentValue = "";
		}

		try
		{
			// Change temperature unit format from "0C" to "¡ãC" using ALT+0176 (TD12415)
			unitString = dataPoint->getUnit();

// 			CString degreeString = unitString.c_str();
// 			if (degreeString.Find("?C", 0) >= 0)
// 			{
// 				degreeString.Replace(_T('?'), _T('?'));
// 			}
// 			unitString.assign(degreeString);

			currentValue +=  dataPoint->getValueAsString();
			currentValue += " ";
			currentValue += unitString;

		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
		}
		catch (...)
		{
		}	
		return currentValue;
	}


}
