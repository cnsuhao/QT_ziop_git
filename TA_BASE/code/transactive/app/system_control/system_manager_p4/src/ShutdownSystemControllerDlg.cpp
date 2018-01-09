/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ShutdownSystemControllerDlg.cpp $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * ShutdownSystemControllerDlg is associated with the dialog box 
  * allowing operators to shutdown the System Controller currently
  * logged into.
  *
  */

#pragma warning(disable:4786)

#include <string>

#include "StdAfx.h"
#include "SystemManager.h"
#include "ShutdownSystemControllerDlg.h"
#include "../pmodel/src/ShutdownSystemControllerDlgModel.h"
#include "../pmodel/src/SystemManagerConsts.h"

#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include "core/message/src/MessagePublicationManager.h"
#include "core/message/types/OperatorAudit_MessageTypes.h"
#include "core/message/src/AuditMessageSender.h"


namespace TA_Base_App
{

    ShutdownSystemControllerDlg::ShutdownSystemControllerDlg(TA_Base_Bus::ITransActiveWinApp * app, const std::string & hostname, CWnd* pParent)
    : TA_Base_Bus::AbstractDialog(app, ShutdownSystemControllerDlg::IDD, pParent)
    , m_hostname(hostname)
    {
        //{{AFX_DATA_INIT(ShutdownSystemControllerDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void ShutdownSystemControllerDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(ShutdownSystemControllerDlg)
        DDX_Control(pDX, IDOK, m_shutdownButton);
        DDX_Control(pDX, IDC_HOSTNAME, m_hostnameLabel);
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(ShutdownSystemControllerDlg, CDialog)
    //{{AFX_MSG_MAP(ShutdownSystemControllerDlg)
    //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // ShutdownSystemControllerDlg message handlers

    void ShutdownSystemControllerDlg::OnOK()
    {
		m_realPModel->shutdownSystemController();


        // Post a message so the SystemManagerDlg shuts down the System Controller.
        CWnd* pWnd = GetParent();
        if (pWnd)
        {
            ::PostMessage(pWnd->m_hWnd, WM_SHUTDOWN_SYSTEM_CONTROLLER, 0, 0);
        }

        CDialog::OnOK();
    }

	void ShutdownSystemControllerDlg::setupPModel()
	{
        m_realPModel = dynamic_cast<ShutdownSystemControllerDlgModel*> (m_pModel);
        TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
        m_realPModel->setHostName(m_hostname);
	}

    void ShutdownSystemControllerDlg::initUIControl()
    {
		TAUnicodeString uniStr = TAUnicodeString::CreateFromMultitByteString(m_realPModel->m_hostname.c_str());
        m_hostnameLabel.SetWindowText(uniStr.c_str());
        m_shutdownButton.EnableWindow(TRUE);
    }

    std::string ShutdownSystemControllerDlg::getMyPModelName()
    {
        return PMODEL_SHUTDOWN_SYSTEM_CONTROLLER_DLG;
    }

} // namespace TA_Base_App

