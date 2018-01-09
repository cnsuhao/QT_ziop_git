/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/SubsystemDutyTransferDlg.cpp $
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
#include "app/security/DutyManager/pmodel/src/SubsystemDutyTransferDlgModel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerAppController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



using namespace TA_Base_App;
using namespace TA_Base_Bus;

/////////////////////////////////////////////////////////////////////////////
// SubsystemDutyTransferDlg dialog

namespace TA_Base_App
{

	SubsystemDutyTransferDlgModel::SubsystemDutyTransferDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
	 : TA_Base_Bus::AbstractPModel(appController, pmodelName),
	 TA_Base_App::AppControllerUser(appController)
	{
		FUNCTION_ENTRY("SubsystemDutyTransferDlgModel()");

		FUNCTION_EXIT;
	}


	SubsystemDutyTransferDlgModel::~SubsystemDutyTransferDlgModel()
	{
		//m_transferableSubsystems.clear();
		//DutyDataStore::cleanUp();
	}
}

