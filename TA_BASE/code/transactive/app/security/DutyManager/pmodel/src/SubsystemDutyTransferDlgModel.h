#ifndef SUBSYSTEM_DUTY_TRANSFER_DLG_MODEL_H__
#define SUBSYSTEM_DUTY_TRANSFER_DLG_MODEL_H__
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/SubsystemDutyTransferDlg.h $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This dialog enables the operator to select from a list of transferable subsystems
  * and request duties for these subsystems.
  *
  */
#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerBridge.h"
#include "app/security/DutyManager/pmodel/src/AppControllerUser.h"

namespace TA_Base_App
{
	class DutyManagerAppController;
    class SubsystemDutyTransferDlgModel : public TA_Base_Bus::AbstractPModel,public TA_Base_App::AppControllerUser
    {
    // Operations
    public:
	    SubsystemDutyTransferDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
        virtual ~SubsystemDutyTransferDlgModel();
    };

} // namespace TA_Base_App

#endif // SUBSYSTEM_DUTY_TRANSFER_DLG_MODEL_H__
