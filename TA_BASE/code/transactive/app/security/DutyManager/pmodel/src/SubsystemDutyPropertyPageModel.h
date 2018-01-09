#ifndef DUTY_MANAGER_SUBSYSTEM_DUTY_PROPERTY_PAGE_MODEL_H__
#define DUTY_MANAGER_SUBSYSTEM_DUTY_PROPERTY_PAGE_MODEL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/SubsystemDutyPropertyPageModel.h $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This class represents the "Subsystems" tab. It displays the active 
  * subsystem duty state for every physical subsystem in every region.
  *
  */
#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerBridge.h"
#include "app/security/DutyManager/pmodel/src/AppControllerUser.h"

namespace TA_Base_App
{
	class DutyManagerAppController;
    class SubsystemDutyPropertyPageModel : public TA_Base_Bus::AbstractPModel,public TA_Base_App::AppControllerUser
	{

    // Operations
    public:
	    SubsystemDutyPropertyPageModel(IGUIApplicationController* appController, const char * pmodelName);
	    virtual ~SubsystemDutyPropertyPageModel();


		TA_Base_Bus::RegionKeyList getLocations();

		bool isRegionAccessible(
			const TA_Base_Bus::RegionKey regionKey,
			const TA_Base_Bus::RegionKeyList regionKeys);


	 private:
			std::vector<TA_Base_App::PSSA> m_actionGroupsDB;
    };

} // namespace TA_Base_App

#endif //DUTY_MANAGER_SUBSYSTEM_DUTY_PROPERTY_PAGE_MODEL_H__
