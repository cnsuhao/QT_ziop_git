#ifndef DUTY_MANAGER_OTHER_SES_SUB_DUTY_PROPERTY_PAGE_MODEL_H__
#define DUTY_MANAGER_OTHER_SES_SUB_DUTY_PROPERTY_PAGE_MODEL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/RegionDutyPropertyPageModel.h $
* @author:  Bart Golab
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2013/09/03 14:11:12 $
* Last modified by:  $Author: CM $
*
* This class represents the "Regions" (Locations) tab. It displays the active 
* region duty state for logged in operators.
*
*/

#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerBridge.h"
#include "app/security/DutyManager/pmodel/src/AppControllerUser.h"

namespace TA_Base_App
{
	class DutyManagerAppController;
	class OtherSesSubDutyPropertyPageModel : public TA_Base_Bus::AbstractPModel,public TA_Base_App::AppControllerUser
	{
	public:
		OtherSesSubDutyPropertyPageModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~OtherSesSubDutyPropertyPageModel();
	};
}



#endif // DUTY_MANAGER_OTHER_SES_SUB_DUTY_PROPERTY_PAGE_MODEL_H__
