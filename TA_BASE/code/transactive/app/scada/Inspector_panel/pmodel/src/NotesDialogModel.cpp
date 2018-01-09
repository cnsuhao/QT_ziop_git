#include "InspectorpanelAppController.h"
#include "core\utilities\src\RunParams.h"
#include "notesdialogmodel.h"
#include "bus\scada\proxy_library\src\IDataNodeProxy.h"

namespace TA_Base_App
{
	NotesDialogModel::NotesDialogModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		:AbstractEquipmentPModel(appController,pmodelName)
	{
	}


	NotesDialogModel::~NotesDialogModel(void)
	{

	}

	bool	NotesDialogModel::setNotes(const std::string& notes)
	{
		std::string responseString;
		//bool result = 
		m_outputLogic->setNotes(notes, responseString);
		this->postStatusLine(responseString);
		return true;
	}

	std::string		NotesDialogModel::getNotes()
	{
		return m_displayLogic->getNotes();
	}

	bool	NotesDialogModel::isNotesPermitted()
	{
		return AccessControlHelper::getInstance()->IsActionPermitted(m_displayLogic->getDataNodeKey(), DATA_NODE_NOTES);
	}

	void	NotesDialogModel::onDataNodeProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataNode()");
		switch (updateType)
		{
		case TA_Base_Bus::NotesStateUpdate:
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