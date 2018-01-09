/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/DutyManagerPropertySheetModel.cpp $
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

#include "app/security/DutyManager/pmodel/src/DutyManagerPropertySheetModel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerAppController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


DutyManagerPropertySheetModel::DutyManagerPropertySheetModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
	 :TA_Base_Bus::AbstractPModel(appController, pmodelName),
	 TA_Base_App::AppControllerUser(appController)
{
	FUNCTION_ENTRY("DutyManagerPropertySheetModel()");

	FUNCTION_EXIT;
}

DutyManagerPropertySheetModel::~DutyManagerPropertySheetModel()
{

}


