// plan_mft_step_message_dll.h : main header file for the plan_mft_step_message_dll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif


// Cplan_mft_step_message_dllApp
// See plan_mft_step_message_dll.cpp for the implementation of this class
//
namespace TA_Base_App
{
    class Cplan_mft_step_message_dllApp : public CWinApp
    {
    public:
        Cplan_mft_step_message_dllApp();

        // Overrides
    public:
        virtual BOOL InitInstance();

        DECLARE_MESSAGE_MAP()
    };
}

