/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanManagerApp.h $
* @author:  Bart Golab
* @version: $Revision: #3 $
*
* Last modification: $DateTime: 2016/11/21 13:03:08 $
* Last modified by:  $Author: hoa.le $
*
* Application class for Plan Manager.
*/

#ifndef PLANMANAGERMANAGER_APP_H
#define PLANMANAGERMANAGER_APP_H

#include "bus/generic_gui_view/src/TransActiveWinApp.h"

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// PlanManagerApp

	class PlanManagerApp : public TA_Base_Bus::TransActiveWinApp
	{
		// Operations
	public:

		/**
		* PlanManagerApp
		*
		* Constructor for the PlanManagerApp object.
		*
		* @return
		*
		* @exception
		*/
		PlanManagerApp(int argc, char *argv[]);
		virtual	~PlanManagerApp()
		{
		};

		TA_Base_Bus::IApplicationController * createApplicationController();
		virtual TA_Base_Bus::ITransActiveView* createMainView();
		virtual void requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent){};

	private:
		// Copy constructor and operator= made private to prevent them being used.
		PlanManagerApp(const PlanManagerApp&);
		PlanManagerApp& operator=(const PlanManagerApp&);
	};

	/////////////////////////////////////////////////////////////////////////////
} // namespace TA_Base_App

#endif // PLANMANAGERMANAGER_APP_H
