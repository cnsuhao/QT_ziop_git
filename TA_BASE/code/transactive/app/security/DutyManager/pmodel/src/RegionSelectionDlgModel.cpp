/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/RegionSelectionDlg.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This dialog enables the operator to select from a list of regions for which
  * they currently hold region duties.
  *
  */

#include "app/security/DutyManager/pmodel/src/RegionSelectionDlgModel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerAppController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



using namespace TA_Base_App;
using namespace TA_Base_Bus;

/////////////////////////////////////////////////////////////////////////////
// RegionSelectionDlg dialog


RegionSelectionDlgModel::RegionSelectionDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
: TA_Base_Bus::AbstractPModel(appController, pmodelName),
TA_Base_App::AppControllerUser(appController)
{
	FUNCTION_ENTRY("RegionSelectionDlgModel()");
	

	FUNCTION_EXIT;
}


RegionSelectionDlgModel::~RegionSelectionDlgModel()
{
	//m_transferableSubsystems.clear();
	//DutyDataStore::cleanUp();
}

