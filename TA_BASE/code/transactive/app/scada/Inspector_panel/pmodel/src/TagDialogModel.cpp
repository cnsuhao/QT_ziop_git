#include "InspectorpanelAppController.h"
#include "core\utilities\src\RunParams.h"
#include "tagdialogmodel.h"
#include "bus\scada\proxy_library\src\IDataNodeProxy.h"

namespace TA_Base_App
{
	TagDialogModel::TagDialogModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		:AbstractEquipmentPModel(appController,pmodelName)
	{

	}


	TagDialogModel::~TagDialogModel(void)
	{

	}


	bool	TagDialogModel::setTagState(const ETagState tagState, const EOutputInhibitState outputInhibitState,const EInputInhibitState inputInhibitState,const std::string& ptwId,	const std::string& tagComment)
	{
		std::string responseString;
		//bool result = 
		m_outputLogic->setTagState(tagState,outputInhibitState,inputInhibitState,ptwId,tagComment,responseString);
		this->postStatusLine(responseString);
		//return result;
		return true;
	}

	bool	TagDialogModel::removeTagState()
	{
		std::string responseString;
		//bool result = 
		m_outputLogic->removeTagState(responseString);
		this->postStatusLine(responseString);
		return true;
	}


	TA_Base_Bus::ETagState		TagDialogModel::getDataNodeTaggingState()
	{
		return m_displayLogic->getDataNodeTaggingState();
	}

	TA_Base_Bus::InhibitState   TagDialogModel::getDataNodeTagInhibitState()
	{
		return m_displayLogic->getDataNodeTagInhibitState();
	}

	std::string					TagDialogModel::getDataNodePtwId()
	{
		return m_displayLogic->getDataNodePtwId();
	}


	std::string					TagDialogModel::getDataNodeTagComment()
	{
		return m_displayLogic->getDataNodeTagComment();
	}


	bool TagDialogModel::isProfileAccessAllowed(bool buttonChecked, InhibitType type)
	{
		return m_displayLogic->isInhibitEnable(buttonChecked,type);
	}


	bool	TagDialogModel::isActionPermitted(AccessControlActions action)
	{
		return AccessControlHelper::getInstance()->IsActionPermitted(m_displayLogic->getDataNodeKey(),action);
	}

	std::string	TagDialogModel::getDataNodeAssetName()
	{
		return m_displayLogic->getDataNodeAssetName();
	}


	std::string	TagDialogModel::getDataNodeDescription()
	{
		return m_displayLogic->getDataNodeDescription();
	}


	bool	TagDialogModel::hasInformationPoint()
	{
		return m_displayLogic->hasInformationPoint();
	}


	bool	TagDialogModel::hasControlPoint()
	{
		return m_displayLogic->hasControlPoint();
	}

	void	TagDialogModel::onDataNodeProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataNode()");
		switch (updateType)
		{
		case TA_Base_Bus::TagStateUpdate:
			{
				this->notifyViews(PMODEL_NOTIFY_REFRESH_ALL_DATA, NULL);
			}
			break;

		default:
			// Do nothing for these updates
			break;
		}
		
		FUNCTION_EXIT;
	}
};

