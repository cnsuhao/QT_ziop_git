/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemManager.cpp $
  * @author:  Justin Ebedes
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This is the main class for the System Manager.
  * It does not inherit from TransactiveWinApp, as
  * the System Manager must be able to run without
  * a Control Station. Instead it inherits directly
  * from CWinApp and re-implements some of the
  * TransactiveWinApp code.
  *
  */

#include "SystemManager.h"
#include "pmodel/src/SystemManagerAppController.h"

#include "SystemControllerDlg.h"
#include "TransActiveLoginDlg.h"
#include "SystemManagerDlg.h"

#include "pmodel/src/SystemManagerConsts.h"

#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/generic_gui_view/src/TransActiveWinApp.h"


#include "core/exceptions/src/GenericGUIException.h"
#include "core/exceptions/src/ProcessAlreadyRunningException.h"
#include "core/exceptions/src/CannotUseSysControllerException.h"
#include "core/exceptions/src/UserCancelException.h"

#include "core/threads/src/Thread.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/message/src/AuditMessageSender.h"
#include "core/message/src/NameValuePair.h"
#include "core/message/types/ApplicationStatusAudit_MessageTypes.h"


#include "ace/ACE.h"


using TA_Base_Core::GenericGUIException;
using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using TA_Base_Bus::TransActiveWinApp;

namespace TA_Base_App
{

    BEGIN_MESSAGE_MAP(SystemManager, TransActiveWinApp)
        //{{AFX_MSG_MAP(SystemManager)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG
        ON_COMMAND(ID_HELP, CWinApp::OnHelp)
        ON_COMMAND(ID_HELP_INDEX, CWinApp::OnHelpIndex)
    END_MESSAGE_MAP()


    SystemManager::SystemManager()
        : TransActiveWinApp(TA_Base_Bus::MVT_DIALOG, false)
    {
    }

    /////////////////////////////////////////////////////////////////////////////
    // The one and only SystemManager object

    SystemManager theApp;

    /////////////////////////////////////////////////////////////////////////////
    // SystemManager initialization

    TA_Base_Bus::IApplicationController* SystemManager::createApplicationController()
    {
        TA_Base_Bus::IApplicationController* pController = new TA_Base_App::SystemManagerAppController(this, "System Manager");
        return pController;
    }

    CWnd* SystemManager::createMainView(UINT& dlgID)
    {
        TALoadLang(APPLICATION_ID_SystemManager);

        //Construct the dialog
        SystemManagerDlg* dlg = new SystemManagerDlg(this);
        dlgID = SystemManagerDlg::IDD;
        return dlg;
    }

    void SystemManager::invoke(const char* methodName, const TA_Base_Bus::NameValueParams& params)
    {
        if (strcmp(methodName, APP_METHOD_LOGIN) == 0)
        {
            doLogin();
        }

        if (strcmp(methodName, APP_METHOD_LOCATE_SYSTEM_CONTROLLER) == 0)
        {
            locateSystemController();
        }
    }

    void SystemManager::doLogin()
    {
        TransActiveLoginDlg dlg(this);

        if (dlg.DoModal() == IDCANCEL)
        {
            TA_THROW(TA_Base_Core::UserCancelException("User cancel login"));
        }
    }

    void SystemManager::locateSystemController()
    {
        SystemControllerDlg dlg(this);

        if (dlg.DoModal() == IDCANCEL)
        {
            TA_THROW(TA_Base_Core::UserCancelException("User cancel systemController selection"));
        }
    }

    void SystemManager::requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent)
    {
        int typeCode = uiEvent->getTypeCode();
        TA_Base_Bus::UINotificationDataContextPtr dataContext = uiEvent->getMyDataContext();

        if (dataContext->getParamEncode() == TA_Base_Bus::PARAM_ENCODE_STR_1)
        {
            int msgID = dataContext->getTransActiveMessageId();
            std::string processName = dataContext->getParamValue(0);

            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << processName;
            userMsg.showMsgBox(msgID);

        }

    }

} // namespace TA_Base_App
