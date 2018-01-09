// PlanMFTProcessor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "app/system_control/dll_host/src/IDllComponent.h"
#include "app/response_plans/plan_mft_processor/src/PlanMFTProcessor.h"
#include "app/response_plans/plan_mft_processor/src/PlanProcessor.h"

namespace TA_Base_App
{
//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// Cplan_mft_step_message_dllApp

BEGIN_MESSAGE_MAP(Cplan_mft_step_message_dllApp, CWinApp)
END_MESSAGE_MAP()


// Cplan_mft_step_message_dllApp construction

Cplan_mft_step_message_dllApp::Cplan_mft_step_message_dllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cplan_mft_step_message_dllApp object

Cplan_mft_step_message_dllApp theApp;


// Cplan_mft_step_message_dllApp initialization

BOOL Cplan_mft_step_message_dllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" __declspec(dllexport) TA_Base_App::IDllComponent* createDllComponent()
{
    return new TA_Base_App::PlanProcessor;
}
}
