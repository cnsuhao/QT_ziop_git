#include "InspectorpanelAppController.h"
#include "core\utilities\src\RunParams.h"
#include "directcontroldialogmodel.h"
#include "DataPointControlHelper.h"
#include "bus\scada\proxy_library\src\IDataPointProxy.h"
#include "core\exceptions\src\ScadaProxyException.h"
#include "bus\scada\proxy_library\src\IDataNodeProxy.h"
#include "CommonExceptionsCorbaDef.h"

namespace TA_Base_App
{
	DirectControlDialogModel::DirectControlDialogModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		:AbstractEquipmentPModel(appController,pmodelName),
		m_iPoint(0xFFFFFFFF),
		m_controlValue(""),
		m_actionAllowed(false),
		m_isConfigValid(false)
	{
	}


	DirectControlDialogModel::~DirectControlDialogModel(void)
	{

	}



	void		DirectControlDialogModel::init()
	{
		m_isConfigValid = false;
		
		m_iPoint = m_displayLogic->getDirectControlPoint();
		
		if(0xFFFFFFFF == m_iPoint)
		{
			return;
		}

		if(!m_displayLogic->isWritableDp(m_iPoint))
		{
			return;
		}

		std::string value = m_displayLogic->getDirectControlValue();
		if(value.size() == 0)
		{	
			return;
		}
		m_controlValue = value;
		m_isConfigValid = true;
	}


	std::string		DirectControlDialogModel::getControlDescription()
	{
		std::string rtnString;
		if(m_isConfigValid)
		{
			rtnString = m_displayLogic->getDataPointDescription(m_iPoint);
		}
		return rtnString;
	}


	std::string		DirectControlDialogModel::getControlValue()
	{
		return m_controlValue;
	}


	bool	DirectControlDialogModel::isActionAllowd()
	{
		return m_actionAllowed;
	}


	std::string		DirectControlDialogModel::getEquipmentLabel()
	{
		std::string equipmentLabel;
		equipmentLabel = m_displayLogic->getDataNodeAssetName();
		equipmentLabel += "\r\n";
		equipmentLabel += m_displayLogic->getDataNodeLabel();
		return equipmentLabel;
	}



	bool		DirectControlDialogModel::isConfigValid()
	{
		return m_isConfigValid;
	}


	void		DirectControlDialogModel::onInitStatusUpdate (EquipmentInitStatus status)
	{
		if(status == INIT_STEP5_COMPLETE)
		{
			this->init();
			this->notifyViews(PMODEL_NOTIFY_REFRESH_ALL_DATA,NULL);
		}
	}

	void		DirectControlDialogModel::onPertmissionUpdate()
	{
		if(!this->isConfigValid())
		{
			m_actionAllowed = false;
			return;
		}

		try
		{
			m_actionAllowed = m_displayLogic->isActionPermitted(m_iPoint,DATA_POINT_WRITE);
		}
		catch(...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo,"[16173] Exception occurred");
			m_actionAllowed = false;
		}
		this->notifyViews(PMODEL_NOTIFY_PERTMISSION,NULL);
	}

	void DirectControlDialogModel::directControl() 
	{
		if(!m_displayLogic->getDpAllowedToWrite(m_iPoint))
		{
			std::stringstream message;
			message << m_displayLogic->getDataPointDescription(m_iPoint) << ": Control Inhibited.";
			this->postStatusLine(message.str());

			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", message.str().c_str() );
			return;
		}
		else
		{
			std::stringstream message;
			message << m_displayLogic->getDataPointDescription(m_iPoint) << ": Updating value to \"" << m_controlValue << "\".";
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", message.str().c_str() );
			this->postStatusLine(message.str());
			
			std::string responseString;
			m_outputLogic->setValue(m_iPoint,m_controlValue,responseString);

			this->postStatusLine(responseString);
		}
	}
};