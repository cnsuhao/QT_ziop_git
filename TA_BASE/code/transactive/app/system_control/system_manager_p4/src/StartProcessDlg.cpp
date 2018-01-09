/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/StartProcessDlg.cpp $
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

#pragma warning(disable:4786)

#include <string>

#include "StdAfx.h"
#include "StartProcessDlg.h"
#include "../pmodel/src/StartProcessDlgModel.h"
#include "../pmodel/src/SystemManagerConsts.h"

#include "bus/generic_gui_view/src/TransActiveMessage.h"

#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include "core/message/src/MessagePublicationManager.h"
#include "core/message/types/OperatorAudit_MessageTypes.h"
#include "core/message/src/AuditMessageSender.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/exceptions/src/SystemControllerException.h"

namespace TA_Base_App
{

    StartProcessDlg::StartProcessDlg(TA_Base_Bus::ITransActiveWinApp * app, const std::vector<std::string>& processes, CWnd* parent)
    : TA_Base_Bus::AbstractDialog(app, StartProcessDlg::IDD, parent)
    , m_processes(processes)
    {
        //{{AFX_DATA_INIT(StartProcessDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }

    void StartProcessDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(StartProcessDlg)
		DDX_Control(pDX, IDC_START_PROCESS_LIST, m_processList);
        DDX_Control(pDX, IDOK, m_okButton);
		//}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(StartProcessDlg, CDialog)
    //{{AFX_MSG_MAP(StartProcessDlg)
    //}}AFX_MSG_MAP
    END_MESSAGE_MAP()


    void StartProcessDlg::OnOK()
    {
		m_realPModel->startProcesses();

        CDialog::OnOK();
    }

	void StartProcessDlg::setupPModel()
	{
        m_realPModel = dynamic_cast<StartProcessDlgModel*> (m_pModel);
        TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
        m_realPModel->setProcesses(m_processes);
	}

    void StartProcessDlg::initUIControl()
    {
        // Set the label containing the processes to start.
        std::string processList;
        for(std::vector<std::string>::iterator iter = m_realPModel->m_processes.begin(); 
            iter != m_realPModel->m_processes.end(); iter++)
        {
			TAUnicodeString uniStr = TAUnicodeString::CreateFromMultitByteString((*iter).c_str());
            m_processList.AddString(uniStr.c_str());
        }

        m_okButton.EnableWindow();
    }

    std::string StartProcessDlg::getMyPModelName()
    {
        return PMODEL_START_PROCESS_DLG;
    }

} // namespace TA_Base_App
