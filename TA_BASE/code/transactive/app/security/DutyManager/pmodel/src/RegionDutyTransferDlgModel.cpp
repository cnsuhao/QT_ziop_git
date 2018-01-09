/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/RegionDutyTransferDlg.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This dialog enables the operator to select from a list of transferable regions
  * and take duties for these regions.
  *
  */

#include "app/security/DutyManager/pmodel/src/RegionDutyTransferDlgModel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerAppController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;
using namespace TA_Base_Bus;

/////////////////////////////////////////////////////////////////////////////
// RegionDutyTransferDlg dialog




RegionDutyTransferDlgModel::RegionDutyTransferDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
	: TA_Base_Bus::AbstractPModel(appController, pmodelName),
	TA_Base_App::AppControllerUser(appController)
{
	FUNCTION_ENTRY("RegionDutyTransferDlgModel()");
	

	FUNCTION_EXIT;
}


RegionDutyTransferDlgModel::~RegionDutyTransferDlgModel()
{

}

void RegionDutyTransferDlgModel::getCurrentSessionTransferableRegionList(TransferableRegionList& trList)
{
	//SessionInfo sessionInfo= getSessionInfo( getCurrentSessionId() );
	//TransferableRegionList transferableRegions(m_realPModel->getTransferableRegions(sessionInfo));
	//std::sort(transferableRegions.begin(), transferableRegions.end(), TransferableRegionComparator());
	

	// Retrieve from the Duty Agent the list of transferable regions for the current session.
	// Sort by region key so that they will appear on the dialog in the order configured in the database.

	SessionInfo sessionInfo = getCurrentSessionInfo();
	trList = getTransferableRegions(sessionInfo);
	std::sort(trList.begin(), trList.end(), TransferableRegionComparator());

}

