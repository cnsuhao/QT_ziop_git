#ifndef SELECT_SES_DUTY_DLG_MODEL_H__
#define SELECT_SES_DUTY_DLG_MODEL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectSesSubDutyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SelectSesSubDutyDlg dialog

#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerBridge.h"
#include "app/security/DutyManager/pmodel/src/AppControllerUser.h"

namespace TA_Base_App
{
	class DutyManagerAppController;
	class SelectSesSubDutyDlgModel :  public TA_Base_Bus::AbstractPModel,public TA_Base_App::AppControllerUser
	{
		// Construction
	public:
		SelectSesSubDutyDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~SelectSesSubDutyDlgModel();

	};
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // SELECT_SES_DUTY_DLG_MODEL_H__
