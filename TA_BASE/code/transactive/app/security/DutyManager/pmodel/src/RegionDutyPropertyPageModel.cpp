/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/RegionDutyPropertyPageModel.cpp $
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
#include "app/security/DutyManager/pmodel/src/RegionDutyPropertyPageModel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerAppController.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



using namespace TA_Base_App;
using namespace TA_Base_Bus;

/////////////////////////////////////////////////////////////////////////////
// RegionDutyPropertyPageModel property page

RegionDutyPropertyPageModel::RegionDutyPropertyPageModel(IGUIApplicationController* appController, const char * pmodelName) : 
   TA_Base_Bus::AbstractPModel(appController,  pmodelName),
	TA_Base_App::AppControllerUser(appController)
   {
	   FUNCTION_ENTRY("RegionDutyPropertyPageModel()");

	   FUNCTION_EXIT;
   }


RegionDutyPropertyPageModel::~RegionDutyPropertyPageModel()
{
    FUNCTION_ENTRY("~RegionDutyPropertyPageModel()");
	//DutyDataStore::cleanUp();
    FUNCTION_EXIT;
}

