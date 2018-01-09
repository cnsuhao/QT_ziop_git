#if !defined(AFX_DUTYMANAGERAPP_H__3885936E_D54D_4FA8_B514_AF2286F39023__INCLUDED_)
#define AFX_DUTYMANAGERAPP_H__3885936E_D54D_4FA8_B514_AF2286F39023__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/DutyManagerApp.h $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This class provides the entry point into the application.
  * It instantiates DutyManagerGUI, but leaves it to the
  * GenericGUI to display.
  *
  */

#include "Resource.h"		// Main symbols
#include "bus/generic_gui_view/src/TransActiveWinApp.h"
#include "../pmodel/src/DutyManagerAppController.h"

namespace TA_Base_App
{
    class DutyManagerApp : public TA_Base_Bus::TransActiveWinApp
    {

    public:
        DutyManagerApp();
        ~DutyManagerApp();


		TA_Base_Bus::IApplicationController * createApplicationController();

		CWnd* createMainView(UINT & dlgID);

		void invoke(const char * methodName, const TA_Base_Bus::NameValueParams & params);

		virtual void requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent);


	private:
		DutyManagerAppController * getDutyManagerController();


    // Implementation
    protected:
        //{{AFX_MSG(DutyManagerApp)
	        // NOTE - the ClassWizard will add and remove member functions here.
	        //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

	private:
		//DutyManagerAppController * getDutyManagerController();

    };

} // namespace TA_Base_App

#endif // !defined(AFX_DUTYMANAGERAPP_H__3885936E_D54D_4FA8_B514_AF2286F39023__INCLUDED_)
