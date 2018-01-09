#include "GenericDemoAppController.h"
#include "GenericDemoConsts.h"

#include "TestCommonDlgModel.h"
#include "GenericDemoDlgModel.h"

#include "bus\application_types\src\apptypes.h"

#include "core\utilities\src\DebugUtil.h"


namespace TA_Base_App
{

    const int GENERIC_DEMO_APP_TYPE = JOB_REQUEST_MANAGER;  //use DVR_GUI_APPTYPE 

	GenericDemoAppController::GenericDemoAppController(TA_Base_Bus::ITransActiveWinApp * pApp, const char * appName)
        : TA_Base_Bus::AbstractGUIAppController(pApp, appName)
	{
	}

	GenericDemoAppController::~GenericDemoAppController(void)
	{
	}


	unsigned long GenericDemoAppController::getApplicationType()
	{
		return GENERIC_DEMO_APP_TYPE;
	}

	void GenericDemoAppController::checkEntity( TA_Base_Core::IEntityData* guiEntity )
	{
		//do more work to check
		AbstractGUIAppController::checkEntity(guiEntity);
	}

	void GenericDemoAppController::checkCommandLine()
	{
		return;
	}

    TA_Base_Bus::ITransActivePModel * GenericDemoAppController::createPModel( const std::string & pModelName )
    {
        TA_Base_Bus::ITransActivePModel * pModel = NULL;
        if (pModelName == PMODEL_GENERIC_DEMO_DLG)
        {
            pModel = new GenericDemoDlgModel(this, pModelName.c_str());
        }
        else if (pModelName == PMODEL_TEST_COMMON_DLG)
        {
            pModel = new TestCommonDlgModel(this, pModelName.c_str());
        }
        else
        {
            std::string errorDesc = "presentation model name is not valid: " + pModelName;
            TA_ASSERT(FALSE, errorDesc.c_str());
        }
        return pModel;
    }


}