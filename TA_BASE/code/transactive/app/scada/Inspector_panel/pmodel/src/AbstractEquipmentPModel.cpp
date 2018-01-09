#include "./AbstractEquipmentPModel.h"
#include "bus/scada/proxy_library/src/IDataPointProxy.h"
#include "bus\scada\proxy_library\src\IDataNodeProxy.h"
#include "core\exceptions\src\ScadaProxyException.h"

namespace TA_Base_App
{
	AbstractEquipmentPModel::AbstractEquipmentPModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
	:TA_Base_Bus::AbstractPModel(appController, pmodelName),
	m_pRealAppController(NULL),
	m_displayLogic(NULL),
	m_outputLogic(NULL)
	{
		m_pRealAppController = dynamic_cast<InspectorpanelAppController*> (appController);
		TA_ASSERT(m_pRealAppController != NULL, "Can't cast to InspectorpanelAppController");

		m_displayLogic	= m_pRealAppController->getDisplayLogic();
		m_outputLogic	= m_pRealAppController->getOutputLogic();

		m_displayLogic->registerObserver(this,DOMAIN_DATA_CHANGE_INIT_STATUS);
		m_displayLogic->registerObserver(this,DOMAIN_DATA_CHANGE_ENTITY_DATA);
		m_displayLogic->registerObserver(this,DOMAIN_DATA_CHANGE_PERTMITTION);
	}


	AbstractEquipmentPModel:: ~AbstractEquipmentPModel()
	{
		m_displayLogic->unregisterObserver(this);
		m_pRealAppController =  NULL;
		//m_pEquipment =	NULL;
	}

	void AbstractEquipmentPModel::initialize(DisplayLogic*	displayLogic, OutputLogic* outputLogic)
	{
		m_displayLogic = displayLogic;
		m_outputLogic  =  outputLogic;
	}

	
	void AbstractEquipmentPModel::cleanUp()
	{
		m_displayLogic =  NULL;
		m_outputLogic  =  NULL;
	}

	void AbstractEquipmentPModel::notifyUpdate(const char * updateType,TA_Base_Bus::EventArgs * args)
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

	void		AbstractEquipmentPModel::onInitStatusUpdate (EquipmentInitStatus status)
	{
		if(status == INIT_STEP5_COMPLETE)
		{
			this->notifyViews(PMODEL_NOTIFY_REFRESH_ALL_DATA,NULL);
		}
	}

	void		AbstractEquipmentPModel::onPertmissionUpdate()
	{
		this->notifyViews(PMODEL_NOTIFY_PERTMISSION,NULL);
	}

	void		AbstractEquipmentPModel::onDataNodeProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType)
	{

	}


	void		AbstractEquipmentPModel::onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType)
	{

	}

	unsigned long	AbstractEquipmentPModel::getDataNodeEntityKey()
	{
		return	m_displayLogic->getDataNodeKey();
	}

	std::string		AbstractEquipmentPModel::getChildDataPointDescription(UINT iPoint)
	{
		return	m_displayLogic->getDataPointDescription(iPoint);
	}


	EDataPointStatusColor AbstractEquipmentPModel::getDataPointStatusColor(unsigned int iPoint)
	{
		return m_displayLogic->getDataPointStatusColor(iPoint);
	}


	std::string AbstractEquipmentPModel::getDisplayValueWithUnit(unsigned int iPoint)
	{
		return m_displayLogic->getDisplayValueWithUnit(iPoint);
	}

	void	AbstractEquipmentPModel::postStatusLine(const std::string& statusLine)
	{
		this->m_pRealAppController->showStatusMessage(statusLine);
	}

	void	AbstractEquipmentPModel::postStatusLine(int codeId, const std::vector<std::string>	&parameters)
	{
		this->m_pRealAppController->showStatusMessage(codeId, parameters);
	}
}

