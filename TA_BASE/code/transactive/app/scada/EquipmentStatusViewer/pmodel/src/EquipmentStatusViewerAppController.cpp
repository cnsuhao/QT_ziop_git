#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentStatusViewerAppController.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentStatusViewerDlgModel.h"
#include "bus/application_types/src/apptypes.h"

namespace TA_Base_App
{
	EquipmentStatusAppController::EquipmentStatusAppController( TA_Base_Bus::ITransActiveWinApp * pWinApp, const char * applicatoinName )
		: TA_Base_Bus::AbstractGUIAppController(pWinApp, applicatoinName)
	{

	}

	EquipmentStatusAppController::~EquipmentStatusAppController()
	{

	}

	unsigned long EquipmentStatusAppController::getApplicationType()
	{
		return EQUIP_STATUS_VIEWER_GUI_APPTYPE;
	}

	TA_Base_Bus::ITransActivePModel * EquipmentStatusAppController::createPModel(const std::string & pModelName)
	{
		TA_Base_Bus::ITransActivePModel* pModel = NULL;
		if (pModelName == EDSV::PMODEL_MAIN_DIALOG)
		{
			pModel = new EquipmentStatusViewerDlgModel(this, EDSV::PMODEL_MAIN_DIALOG.c_str());
		}
		return pModel;
	}
}
