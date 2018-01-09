/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/AlarmManagerAppController.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  */
#include "AlarmManagerConsts.h"
#include "AlarmManagerAppController.h"
#include "app/alarm/alarm_manager/pmodel/src/AlarmGUIDlgModel.h"
#include "app/alarm/alarm_manager/pmodel/src/AlarmFilterDlgPModel.h"
#include "app\alarm\alarm_manager\pmodel\src\AlarmFilterService.h"
#include "app\alarm\alarm_manager\pmodel\src\multipleSelDlgPModel.h"
#include "app\alarm\alarm_manager\pmodel\src\SuppressionRulesDlgPModel.h"
 
#include "bus/application_types/src/apptypes.h"

#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/data_access_interface/entity_access/src/ConsoleAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/UserSettingsException.h"
#include "core/exceptions/src/CannotUseSysControllerException.h"


using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using TA_Base_Bus::SettingsMgr;
using namespace TA_Base_Bus;

namespace TA_Base_App
{

    AlarmManagerAppController::AlarmManagerAppController(TA_Base_Bus::ITransActiveApp * pApp, const char * appName)
       : TA_Base_Bus::AbstractGUIAppController(pApp, appName)
    
    {
    }

    AlarmManagerAppController::~AlarmManagerAppController()
    {

    }



    unsigned long AlarmManagerAppController::getApplicationType()
    {
	    return ALARM_GUI_APPTYPE;
    }


    void AlarmManagerAppController::allocAppResource()
    {
		 
    }

    void AlarmManagerAppController::cleanup()
    {

		 
	}

	 
   
	void AlarmManagerAppController::prepareAppControllerRun()
	{
		//m_consoleName = checkConsoleName();
	}

    TA_Base_Bus::ITransActivePModel * AlarmManagerAppController::createPModel( const std::string & pModelName )
    {
        ITransActivePModel * pModel = NULL;       
        if (pModelName == PMODEL_AlarmGUIDlg)
        {
			pModel = new AlarmGUIDlgModel(this, pModelName.c_str());
        }        
		else if (pModelName == PMODEL_AlarmFilterDlg)
		{ 
			pModel = new AlarmFilterDlgPModel(this, pModelName.c_str());
		}
		else if (pModelName == PMODEL_MultiSelDlg)
		{
			pModel = new multipleSelDlgPModel(this, pModelName.c_str());
		}
		else if (pModelName == PMODEL_SuppressAlarmRule)
		{
			pModel = new SuppressionRulesDlgPModel(this, pModelName.c_str());
		}
		else
        {
            std::string errorDesc = "presentation model name is not valid: " + pModelName;
            TA_ASSERT(FALSE, errorDesc.c_str());
        }
        return pModel;
    }


} // namespace TA_Base_App
