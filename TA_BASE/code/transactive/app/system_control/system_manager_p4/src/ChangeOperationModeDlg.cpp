/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ChangeOperationModeDlg.cpp $
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

#pragma warning(disable:4786)

#include "StdAfx.h"
#include "ChangeOperationModeDlg.h"
#include "../pmodel/src/ChangeOperationModeDlgModel.h"
#include "../pmodel/src/SystemManagerConsts.h"

#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include "core/utilities/src/DebugUtil.h"

namespace TA_Base_App
{

    ChangeOperationModeDlg::ChangeOperationModeDlg(TA_Base_Bus::ITransActiveWinApp * app, const std::vector<std::string>& processes, 
                                                    CWnd* parent)
        : TA_Base_Bus::AbstractDialog(app, ChangeOperationModeDlg::IDD, parent)
		, m_processes(processes)
    {
        //{{AFX_DATA_INIT(CStartProcessDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT

    }

    void ChangeOperationModeDlg::DoDataExchange(CDataExchange* pDX)
    {
	    CDialog::DoDataExchange(pDX);
	    //{{AFX_DATA_MAP(ChangeOperationModeDlg)
	DDX_Control(pDX, IDC_MODE_PROCESS_LIST, m_processList);
	//}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(ChangeOperationModeDlg, CDialog)
	    //{{AFX_MSG_MAP(ChangeOperationModeDlg)
	    ON_BN_CLICKED(IDC_CONTROL, OnRadioControl)
	    ON_BN_CLICKED(IDC_MONITOR, OnRadioMonitor)
	    //}}AFX_MSG_MAP
    END_MESSAGE_MAP()


    void ChangeOperationModeDlg::OnOK() 
    {
		m_realPModel->changeOperationMode();

	    
	    CDialog::OnOK();
    }

    void ChangeOperationModeDlg::OnRadioControl() 
    {
        m_realPModel->m_operationMode = TA_Base_Core::Control;
    }

    void ChangeOperationModeDlg::OnRadioMonitor() 
    {
	    m_realPModel->m_operationMode = TA_Base_Core::Monitor;
    }

	void ChangeOperationModeDlg::setupPModel()
	{
        m_realPModel = dynamic_cast<ChangeOperationModeDlgModel*> (m_pModel);
        TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
		m_realPModel->setProcesses(m_processes);
	}

    void ChangeOperationModeDlg::initUIControl()
    {
        // Set the label containing the processes.
        std::string processList;
        for(std::vector<std::string>::iterator iter = m_realPModel->m_processes.begin(); 
            iter != m_realPModel->m_processes.end(); iter++)
        {
			TAUnicodeString uniStr = TAUnicodeString::CreateFromMultitByteString((*iter).c_str());
            m_processList.AddString(uniStr.c_str());
        }

        CButton RadioButton;
        if (RadioButton.Attach(GetDlgItem(IDC_CONTROL)->m_hWnd))
        {
            RadioButton.SetCheck(1);
            RadioButton.Detach();
            OnRadioControl();
        }
    }

    std::string ChangeOperationModeDlg::getMyPModelName()
    {
         return PMODEL_CHANGE_OPERATIOIN_MODE_DLG;
    }


} // namespace TA_Base_App
