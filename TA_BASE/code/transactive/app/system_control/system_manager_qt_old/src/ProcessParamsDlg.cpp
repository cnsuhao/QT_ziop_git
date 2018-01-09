/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ProcessParamsDlg.cpp $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * ProcessParamsDlg is associated with the dialog box allowing operators 
  * to set the processes' runtime parameters. In the current version, it  
  * only allows the debug level of processes to be changed.
  *
  */

#pragma warning(disable:4786)

#include "StdAfx.h"
#include "SystemManager.h"
#include "ProcessParamsDlg.h"
#include "../pmodel/src/ProcessParamsDlgModel.h"
#include "../pmodel/src/SystemManagerConsts.h"

#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/message/types/OperatorAudit_MessageTypes.h"
#include "core/message/src/AuditMessageSender.h"
#include "core/exceptions/src/SystemControllerException.h"

namespace TA_Base_App
{

    ProcessParamsDlg::ProcessParamsDlg(TA_Base_Bus::ITransActiveWinApp * app, 
		                               const std::vector<std::string> processes,
                                         CWnd* pParent)
        : TA_Base_Bus::AbstractDialog(app, ProcessParamsDlg::IDD, pParent)
        , m_processes(processes)
    { 

        //{{AFX_DATA_INIT(ProcessParamsDlg)
            // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }

    void ProcessParamsDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(ProcessParamsDlg)
        DDX_Control(pDX, IDC_DEBUG_LEVEL_COMBO, m_debugLevel);
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(ProcessParamsDlg, CDialog)
        //{{AFX_MSG_MAP(ProcessParamsDlg)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()


    void ProcessParamsDlg::OnOK()
    {
        // Get Debug Level
        TAUnicodeString debugLevel;
        int selection = m_debugLevel.GetCurSel();
        TA_ASSERT(selection != CB_ERR, "Invalid selection in Debug Level combo box");
        m_debugLevel.GetLBText(selection, debugLevel.str());

		std::string stdStr = debugLevel.toMultiByteStdString();

		m_realPModel->changeProcessParams(stdStr.c_str());


        CDialog::OnOK();
    }


	void ProcessParamsDlg::setupPModel()
	{
        m_realPModel = dynamic_cast<ProcessParamsDlgModel*> (m_pModel);
        TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
        m_realPModel->setProcesses(m_processes);
	}

    void ProcessParamsDlg::initUIControl()
    {
        if(m_realPModel->m_processes.empty())
        {
            TA_Base_Bus::TransActiveMessage userMsg;
            UINT selectedButton = userMsg.showMsgBox(IDS_UE_020036);
            EndDialog(IDCANCEL);
            return;
        }

        // Load all the allowed debug levels
        for (int i = TA_Base_Core::DebugUtil::DebugFatal; i < TA_Base_Core::DebugUtil::getInstance().countDebugLevels() ; i++)
        {
			std::string stdDebgLevel = TA_Base_Core::DebugUtil::getInstance().getStringDebugLevel((TA_Base_Core::DebugUtil::EDebugLevel)i);
            TAUnicodeString uniStr = TAUnicodeString::CreateFromMultitByteString(stdDebgLevel.c_str());

            m_debugLevel.AddString(uniStr.c_str());
        }

        // Need to display the current debug level (if all selected processes
        // are set to the same debug level) or a default debug level.

        // Firstly, get the debug level of the first selected process.
		TA_Base_Core::DebugUtil::EDebugLevel debugLevel = m_realPModel->getCurrentDebugLevel();

        m_debugLevel.SetCurSel(debugLevel);
    }

    std::string ProcessParamsDlg::getMyPModelName()
    {
        return PMODEL_PROCESS_PARAMS_DLG;
    }   

} // namespace TA_Base_App

