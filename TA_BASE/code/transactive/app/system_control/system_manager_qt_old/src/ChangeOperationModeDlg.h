/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ChangeOperationModeDlg.h $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * ChangeOperationModeDlg is the class associated with the
  * Change Operation Mode dialog.
  *
  */

#if !defined(AFX_CHANGEOPERATIONMODEDLG_H__6CBF071A_BD8C_4AA9_8FB0_FEB16ADD9D11__INCLUDED_)
#define AFX_CHANGEOPERATIONMODEDLG_H__6CBF071A_BD8C_4AA9_8FB0_FEB16ADD9D11__INCLUDED_

#include "Resource.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"

#include <string>
#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace TA_Base_App
{
    class ChangeOperationModeDlgModel;

    class ChangeOperationModeDlg : public TA_Base_Bus::AbstractDialog
    {

    public:
    
        /** 
          * Constructor
          *
          * @param processes  The entity names of the processes that we will be
          *                   changing the operation mode of.
          * @param parent     The parent window.
          *
          */
        ChangeOperationModeDlg(TA_Base_Bus::ITransActiveWinApp * app, const std::vector<std::string>& processes, 
                                CWnd* parent = NULL);

        // Dialog Data
	    //{{AFX_DATA(ChangeOperationModeDlg)
	enum { IDD = IDD_CHANGEOPERATIONMODE_DIALOG };
	CListBox	m_processList;
	//}}AFX_DATA


        // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(ChangeOperationModeDlg)
	    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL

	protected:
        virtual std::string getMyPModelName();
        void setupPModel();

        void initUIControl();

    private:
    

	    // Generated message map functions
	    //{{AFX_MSG(ChangeOperationModeDlg)
	    virtual void OnOK();

        afx_msg void OnRadioControl();
	    afx_msg void OnRadioMonitor();
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
    private:
        ChangeOperationModeDlgModel * m_realPModel;
		std::vector<std::string> m_processes;
    };

} // namespace TA_Base_App

#endif // !defined(AFX_CHANGEOPERATIONMODEDLG_H__6CBF071A_BD8C_4AA9_8FB0_FEB16ADD9D11__INCLUDED_)
