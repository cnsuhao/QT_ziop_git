#ifndef EQUIPMENT_STATUS_VIEWER_APP_CONTROLLER_H
#define EQUIPMENT_STATUS_VIEWER_APP_CONTROLLER_H

#include "bus/generic_gui_pmod/src/AbstractGUIAppController.h"
#include "bus/generic_gui_pmod/src/UserInteraction.h"

using namespace TA_Base_Bus;

namespace TA_Base_App
{
	class EquipmentStatusAppController : public TA_Base_Bus::AbstractGUIAppController
	{
	public:
		/**
		* Constructor
		*/
		EquipmentStatusAppController(TA_Base_Bus::ITransActiveWinApp * pWinApp, const char * applicatoinName);
		
		/**
          * Destructor
          */
		~EquipmentStatusAppController();
	

		//virtual void invoke(const char * methodName, const NameValueParams & params);

	protected:
		virtual ITransActivePModel * createPModel(const std::string & pModelName);
		virtual unsigned long getApplicationType();
		//virtual void sessionIdChanged();
	};
}

#endif