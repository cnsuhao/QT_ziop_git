/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scheduling/scheduling_manager/plugin/model/src/SchedulingManagerGUI.cpp $
  * @author:  
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This inherits from AbstractGUIApplication and implements all methods specific to documents.
  */

#include "app/scheduling/scheduling_manager/plugin/model/src/SchedulingManagerGUI.h"
#include "app/scheduling/scheduling_manager/plugin/model/src/SchedulingModelConstants.h"
#include "app/scheduling/scheduling_manager/plugin/model/src/MainFrameModel.h"
#include "app/scheduling/scheduling_manager/plugin/model/src/JobDialogModel.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/application_types/src/apptypes.h"
#include "bus/user_settings/src/SettingsMgr.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/exceptions/src/UserSettingsException.h"

using namespace TA_Base_App::SchedulingManager;
using TA_Base_Core::RunParams;
using TA_Base_Core::DebugUtil;
using TA_Base_Bus::SettingsMgr;

namespace TA_Base_App
{

    //////////////////////////////////////////////////////////////////////
    // Construction/Destruction
    //////////////////////////////////////////////////////////////////////

	SchedulingManagerGUI::SchedulingManagerGUI(TA_Base_Bus::ITransActiveWinApp * pApp, const char * appName):
       AbstractGUIAppController(pApp, appName)
    {   
    }


    SchedulingManagerGUI::~SchedulingManagerGUI()
    {

    }

    unsigned long SchedulingManagerGUI::getApplicationType()
    {
	    return SCHEDULING_MANAGER;
    }

    void SchedulingManagerGUI::checkCommandLine()
    {  
    }


    void SchedulingManagerGUI::entityChanged(const std::vector<std::string>& changes)
    {
        // The Scheduling Manager does not use any entity parameters, so don't need to do anything
    }


    void SchedulingManagerGUI::serverIsDown()
    {
        // when server is down, we want to disable all access related actions.  
        RunParams::getInstance().set(RPARAM_SERVERSTATUS.c_str(), RVALUE_SERVER_DOWN.c_str());
    }


    void SchedulingManagerGUI::serverIsUp()
    {
        // Tell the app that it could allow access related actions again.  
        RunParams::getInstance().set(RPARAM_SERVERSTATUS.c_str(), RVALUE_SERVER_UP.c_str());
    }

    void SchedulingManagerGUI::prepareForClose()
    {
        try
        {
            saveAllUserSettings();
        }
        catch ( TA_Base_Core::UserSettingsException&)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "Unknown Exception", "Caught some unknown exception when saving user settings");
        }
    }

	TA_Base_Bus::ITransActivePModel * SchedulingManagerGUI::createPModel( const std::string & pModelName )
	{
		TA_Base_Bus::ITransActivePModel * pModel = NULL;
		if (PMODEL_MAIN_FRAME == pModelName )
		{
			pModel = new MainFrameModel(this, pModelName.c_str());
		}
		else if (PMODEL_JOB_DLG == pModelName)
		{
			pModel = new JobDialogModel(this, pModelName.c_str());
		}
		else
		{
			std::string errorDesc = "presentation model name is not valid: " + pModelName;
			TA_ASSERT(FALSE, errorDesc.c_str());
		}
		return pModel;
	}

	void SchedulingManagerGUI::cleanup()
	{

		cleanupSingletons();

		AbstractGUIAppController::cleanup();
	}

	void SchedulingManagerGUI::cleanupSingletons()
	{
	}

	void SchedulingManagerGUI::registerRunParamChange( void )
	{

	}

	void SchedulingManagerGUI::onRunParamChange( const std::string& name, const std::string& value )
	{

	}
};
