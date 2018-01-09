#ifndef REGION_DUTY_TRANSFER_DLG_MODEL_H__
#define REGION_DUTY_TRANSFER_DLG_MODEL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/RegionDutyTransferDlg.h $
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
#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerBridge.h"
#include "app/security/DutyManager/pmodel/src/AppControllerUser.h"
namespace TA_Base_App
{
	class DutyManagerAppController;
	class RegionDutyTransferDlgModel :public TA_Base_Bus::AbstractPModel,public TA_Base_App::AppControllerUser
	{

		public:
			RegionDutyTransferDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
			virtual ~RegionDutyTransferDlgModel();

			void getCurrentSessionTransferableRegionList(TransferableRegionList& trList);
	};
}


#endif // REGION_DUTY_TRANSFER_DLG_MODEL_H__
