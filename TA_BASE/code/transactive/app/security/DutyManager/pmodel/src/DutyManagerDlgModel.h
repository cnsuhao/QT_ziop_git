#ifndef DUTY_MANAGER_DLG_MODEL_H__
#define DUTY_MANAGER_DLG_MODEL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/DutyManagerDlgModel.h $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This class represents the main Rights Manager dialog window.
  *
  */

#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "app/security/DutyManager/pmodel/src/AppControllerUser.h"
namespace TA_Base_App
{
	class DutyManagerAppController;
	//TD18095, replace inheritate from ISessionObserver, and IConfigDataObserver with aggregate two objects.
    class DutyManagerDlgModel : public TA_Base_Bus::AbstractPModel,public TA_Base_App::AppControllerUser
						// ,  public TA_Base_Core::SpecialisedMessageSubscriber<TA_Base_Core::CommsMessageCorbaDef>
    {
    // Operations
    public:
		DutyManagerDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
        virtual ~DutyManagerDlgModel();

	public:

		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);


		void loadData();
		void setTitle(std::string& title);

		void OnTransActiveHelp();
		void OnAppAbout();

		//std::string getLocationNameByLocationKey(const unsigned long locationKey);

	private:
		void registerUpdate();
		void unregisterUpdate();

    };

} // namespace TA_Base_App

#endif // DUTY_MANAGER_DLG_MODEL_H__
