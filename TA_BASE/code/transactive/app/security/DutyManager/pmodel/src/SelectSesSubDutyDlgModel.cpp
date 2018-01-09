// SelectSesSubDutyDlg.cpp : implementation file
//

#include "app/security/DutyManager/pmodel/src/SelectSesSubDutyDlgModel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerAppController.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{

}

using namespace TA_Base_App;
using namespace TA_Base_Bus;
/////////////////////////////////////////////////////////////////////////////
// SelectSesSubDutyDlg dialog


SelectSesSubDutyDlgModel::SelectSesSubDutyDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
: TA_Base_Bus::AbstractPModel(appController, pmodelName),
TA_Base_App::AppControllerUser(appController)
{
	FUNCTION_ENTRY("RegionDutyPropertyPageModel()");

	FUNCTION_EXIT;
}


SelectSesSubDutyDlgModel::~SelectSesSubDutyDlgModel()
{
	//m_transferableSubsystems.clear();
	//DutyDataStore::cleanUp();
}

