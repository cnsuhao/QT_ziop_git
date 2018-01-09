/**
* The source code in this file is the property of
* ST Engineering Electronics (LSG) and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanViewFactory.h $
* @author:  Hoa Le
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2016/11/21 13:03:08 $
* Last modified by:  $Author: hoa.le $
*
* This class is for creating factory of Plan Manager view objects.
*/

#ifndef PLANVIEWFACTORY_H
#define PLANVIEWFACTORY_H

#include "bus/generic_gui_view/src/TransActiveWinApp.h"

namespace TA_Base_App
{
	class PlanViewFactory : public TA_Base_Bus::IViewFactory
	{
	public:
		PlanViewFactory(TA_Base_Bus::ITransActiveApp* winApp);
		virtual ~PlanViewFactory(){};

		/** 
		* createView
		*
		* get the view based on specific name
		*
		* @param viewType
		* @param viewID
		* @return ITransActiveView *
		*/
		TA_Base_Bus::ITransActiveView * PlanViewFactory::createView(const char * viewType);

	private:
		TA_Base_Bus::ITransActiveApp* m_winApp;
	};
}
#endif // PLANVIEWFACTORY_H
