

#include "app/security/DutyManager/pmodel/src/OtherSesSubDutyPropertyPageModel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerAppController.h"
//#include "core/data_access_interface/src/RightsAccessFactory.h" //TD15533
//#include "core/data_access_interface/src/SessionAccessFactory.h" //TD15533
//#include "core/data_access_interface/src/ProfileAccessFactory.h" //TD15533

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
namespace
{

}

using namespace TA_Base_App;
using namespace TA_Base_Bus;

OtherSesSubDutyPropertyPageModel::OtherSesSubDutyPropertyPageModel(IGUIApplicationController* appController, const char * pmodelName) : 
	TA_Base_Bus::AbstractPModel(appController,  pmodelName),
	TA_Base_App::AppControllerUser(appController)
	{
		FUNCTION_ENTRY("OtherSesSubDutyPropertyPageModel()");

		FUNCTION_EXIT;
	}

	OtherSesSubDutyPropertyPageModel::~OtherSesSubDutyPropertyPageModel()
	{
		FUNCTION_ENTRY("~RegionDutyPropertyPageModel()");
		//DutyDataStore::cleanUp();
		FUNCTION_EXIT;
	}



