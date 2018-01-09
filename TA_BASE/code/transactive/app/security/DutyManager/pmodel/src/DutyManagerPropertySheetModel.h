#ifndef DUTY_MANAGER_PROPERTY_SHEET_MODEL_H__
#define DUTY_MANAGER_PROPERTY_SHEET_MODEL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/DutyManagerPropertySheetModel.h $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This class works around a shortcoming in CMultiFontPropertySheet which causes 
  * pages to be resized to their original dimensions upon activation.
  *
  */
#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "app/security/DutyManager/pmodel/src/AppControllerUser.h"
namespace TA_Base_App
{
	class DutyManagerAppController;
    class DutyManagerPropertySheetModel : public TA_Base_Bus::AbstractPModel,public TA_Base_App::AppControllerUser
    {

    // Operations
    public:
	    DutyManagerPropertySheetModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName); 
	    virtual ~DutyManagerPropertySheetModel();
    };

} // namespace TA_Base_App

#endif // DUTY_MANAGER_PROPERTY_SHEET_MODEL_H__
