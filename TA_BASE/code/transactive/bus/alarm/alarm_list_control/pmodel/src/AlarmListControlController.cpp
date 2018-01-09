/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/AlarmListControlController.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  */

#include "AlarmListControlController.h"    
#include "AlarmListModel.h"
#include "PreConflictAckDlgModel.h"
#include "AlarmListModelConsts.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/UserSettingsException.h"

using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
 
 
namespace TA_Base_Bus
{

    AlarmListControlController::AlarmListControlController(ITransActiveApp * pApp, const char * appName)
       : AbstractLibraryAppController(pApp, appName)
        
    {
    }

    AlarmListControlController::~AlarmListControlController()
    {

    }


    void AlarmListControlController::allocAppResource()
    {
		 
    }

    void AlarmListControlController::cleanup()
    {

	 
	}
   
 
	void AlarmListControlController::prepareAppControllerRun()
	{
	 
	}

    TA_Base_Bus::ITransActivePModel * AlarmListControlController::createPModel( const std::string & pModelName )
    {
        ITransActivePModel * pModel = NULL;
        if (pModelName == PMODEL_ALARM_LIST_CONTROL)
        {
            pModel = new AlarmListModel(this, pModelName.c_str());
        }
        else if (pModelName == PMODEL_PRE_CONFLICT_DLG)
        {
			pModel = new PreConflictAckDlgModel(this, pModelName.c_str());
        }       
        else
        {
            std::string errorDesc = "presentation model name is not valid: " + pModelName;
            TA_ASSERT(FALSE, errorDesc.c_str());
        } 
        return pModel;
    }


} // namespace TA_Base_App
