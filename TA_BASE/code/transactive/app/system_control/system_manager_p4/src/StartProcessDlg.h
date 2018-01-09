/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/StartProcessDlg.h $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * StartProcessDlg is associated with the dialog box allowing 
  * operators to launch a process.
  *
  */

#if !defined(AFX_STARTPROCESSDLG_H__679184B8_2D48_11D5_B140_0050BAB0C7E8__INCLUDED_)
#define AFX_STARTPROCESSDLG_H__679184B8_2D48_11D5_B140_0050BAB0C7E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"

#include <string>
#include <vector>

namespace TA_Base_App
{
    class StartProcessDlgModel;

    class StartProcessDlg : public TA_Base_Bus::AbstractDialog
    {

    public:

        /**
          * Constructor
          *
          * @param processes  The entity names of the processes to start.
          * @param parent     The parent window.
          */
	    StartProcessDlg(TA_Base_Bus::ITransActiveWinApp * app, const std::vector<std::string>& processes, CWnd* parent = NULL);

        // Dialog Data
	    //{{AFX_DATA(StartProcessDlg)
	enum { IDD = IDD_STARTPROCESS_DIALOG };
	CListBox	m_processList;
	    CButton	m_okButton;
	//}}AFX_DATA

        // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(StartProcessDlg)
	    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL
	protected:
        virtual std::string getMyPModelName();
        void setupPModel();

private:

	    // Generated message map functions
	    //{{AFX_MSG(StartProcessDlg)
        void initUIControl();
        virtual void OnOK();

		//}}AFX_MSG
	    DECLARE_MESSAGE_MAP()

        StartProcessDlgModel * m_realPModel;
        std::vector<std::string> m_processes;
    };

} // namespace TA_Base_App

#endif // !defined(AFX_STARTPROCESSDLG_H__679184B8_2D48_11D5_B140_0050BAB0C7E8__INCLUDED_)
