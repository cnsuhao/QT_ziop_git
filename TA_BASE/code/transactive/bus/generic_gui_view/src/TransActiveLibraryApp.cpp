/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/TransActiveLibraryApp.cpp $
* @author:  Karen Graham
* @version: $Revision: #4 $
*
* Last modification: $DateTime: 2012/12/03 14:09:46 $
* Last modified by:  $Author: limin.zhu $
*
* This inherits from CWinApp and implements all common functionality for
* TransActive applications.
*/

#include "bus/generic_gui_view/src/StdAfx.h"
#include "bus/generic_gui_view/src/TransActiveConsts.h"
#include "bus/generic_gui_pmod/src/PModelDataType.h"
#include "bus/generic_gui_view/src/TransActiveLibraryApp.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "bus/generic_gui_view/src/TransActiveViewManager.h"
//#include "bus/generic_gui_view/src/TransActiveFrame.h"
//#include "bus/generic_gui_view/src/TransActiveMDIFrame.h"
//#include "bus/generic_gui_view/src/TransActivePropertySheet.h"


#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/generic_gui_pmod/src/GenericGuiConstants.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include "core/exceptions/src/GenericGUIException.h"
#include "core/exceptions/src/ProcessAlreadyRunningException.h"
#include "core/exceptions/src/RightsException.h"
#include "core/exceptions/src/MessageResourceException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"
#include "core/threads/src/Thread.h"



#include "core/data_access_interface/entity_access/src/ConsoleAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"


#include "bus/generic_gui_view/resource.h" // TD14164 azenitha
#include <QtWidgets/QApplication>
//#include "core/threads/src/Thread.h"



//#include <new.h>


#include <winsock2.h>  // Used by getHostName

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

using TA_Base_Core::DebugUtil;
using TA_Base_Core::GenericGUIException;
using TA_Base_Core::ProcessAlreadyRunningException;
using TA_Base_Core::TransactiveException;
using TA_Base_Core::MessageResourceException;
using TA_Base_Core::RightsException;
using TA_Base_Core::RunParams;
using namespace TA_Base_Core;
/////////////////////////////////////////////////////////////////////////////
// CEventsGuiApp

namespace TA_Base_Bus
{
    //const TCHAR * OLE_INIT_FAILED_STR = _T("OLE initialization failed.  Make sure that the OLE libraries are the correct version.");

    /////////////////////////////////////////////////////////////////////////////



    // TransActiveLibraryApp construction

    TransActiveLibraryApp::TransActiveLibraryApp()
        : m_mainView(NULL)
        , m_appController(NULL)
        , m_pViewManager(NULL)
    {
        m_pViewManager = new TransActiveViewManager();
    }


    TransActiveLibraryApp::~TransActiveLibraryApp()
    {
        doStopApp();
    }


    void TransActiveLibraryApp::setStyleSheet(const std::string&sheetFileName)
    {
        //Do nothing, because it should be called on TransActiveWinApp
    }


    void TransActiveLibraryApp::switchLanguage(const std::string&languageShortcut)
    {
        //Just need to call switch language of forms.
        //loading of  translator  has been called on TransActiveWinApp
        m_pViewManager->switchLanguage();
    }

    /////////////////////////////////////////////////////////////////////////////
    // TransActiveLibraryApp initialization

    int TransActiveLibraryApp::startApp()
    {
        bool shouldStartMessagePump = false;

        try
        {
            doStartApp();
            shouldStartMessagePump = true;
        }
        catch (const GenericGUIException& ex)
        {
            handleGenericGUIException(ex);
        }
        catch (const ProcessAlreadyRunningException&)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "ProcessAlreadyRunningException", "Shutting down quietly as this application is already running and must only be running once.");
        }
        catch (const MessageResourceException& ex)
        {
            handleMessageResourceException(ex);
        }
        catch (const TransactiveException&)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "TransActiveException", "Because this has been caught at the top level we must now shutdown.");

            if (m_appController != NULL)
            {
                m_appController->notifyFatalErrorWhenLaunch();
            }

            TA_Base_Bus::TransActiveMessage userMsg;
            std::string appName = TA_Base_Core::RunParams::getInstance().get(RPARAM_APPNAME);
            userMsg << appName;
            int selectedButton = userMsg.showMsgBox(IDS_UE_010010);
        }
        catch (...)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown", "Because this has been caught at the top level we must now shutdown.");

            if (m_appController != NULL)
            {
                m_appController->notifyFatalErrorWhenLaunch();
            }

            TA_Base_Bus::TransActiveMessage userMsg;
            std::string appName = TA_Base_Core::RunParams::getInstance().get(RPARAM_APPNAME);
            userMsg << appName;
            int selectedButton = userMsg.showMsgBox(IDS_UE_010010);
        }

        if (shouldStartMessagePump == false)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }


    bool TransActiveLibraryApp::notify(QObject* receiver, QEvent* event)
    {
        try
        {
            return QApplication::instance()->notify(receiver, event);
        }
        catch (GenericGUIException& e)
        {
            handleGenericGUIException(e);
        }

        return false;
    }

    void TransActiveLibraryApp::handleGenericGUIException(const GenericGUIException& ex)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "GenericGUIException", "Now calculating which error it was so an appropriate message can be displayed to the user");
        int errorId;

        TA_Base_Bus::TransActiveMessage userMsg;

        if (ex.getFailType() != GenericGUIException::ACCESS_DENIED)
        {
            //userMsg << m_appController->getApplicationDisplayName();
        }

        switch (ex.getFailType())
        {
        case (GenericGUIException::ENTITY_CONFIGURATION_INVALID):
            errorId = IDS_UE_010011;
            break;

        case (GenericGUIException::ENTITY_DOES_NOT_EXIST):
        case (GenericGUIException::ENTITY_NOT_UNIQUE):
            errorId = IDS_UE_010013;
            break;

        case (GenericGUIException::COMMAND_LINE_INVALID):
            errorId = IDS_UE_010008;
            break;

        case (GenericGUIException::NO_DATABASE_CONNECTION):
            errorId = IDS_UE_010001;
            break;

        case (GenericGUIException::COMMUNICATION_ERROR_WITH_CONTROL_STATION):
            errorId = IDS_UE_010014;
            break;

        case (GenericGUIException::ACCESS_DENIED):
        {
            errorId = IDS_UE_010021;
            std::string userName;
            std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
            TA_Base_Bus::AuthenticationLibrary authLibrary;
            TA_Base_Bus::SessionInfo sessionInfo = authLibrary.getSessionInfo(sessionId, sessionId);
            std::auto_ptr<TA_Base_Core::IOperator> iOperator(TA_Base_Core::OperatorAccessFactory::getInstance().getOperator(sessionInfo.UserId, false));
            userName = iOperator->getName();
            std::string actionName = "start this application";
            userMsg << userName;
            userMsg << actionName;
            break;
        }

        case (GenericGUIException::UNKNOWN_ACCESS_RIGHTS):
        {
            errorId = IDS_UE_010019;
            break;
        }

        case (GenericGUIException::NO_ACTIVE_SESSION):   //TD1141
            errorId = IDS_UE_020050;
            break;

        case (GenericGUIException::INITIALISATION_ERRORS):
        default:
            errorId = IDS_UE_010012;
            std::string appName = TA_Base_Core::RunParams::getInstance().get(RPARAM_APPNAME);
            userMsg << appName;
            break;
        }

        userMsg.showMsgBox(errorId);
    }

    void TransActiveLibraryApp::handleMessageResourceException(const MessageResourceException& ex)
    {
        // NickJ: 07/12/04: Added catch for error coming from TransActiveMessage box
        // The problem was a message box that failed to display. Need to notify the user what is happening,
        // but WITHOUT using a TransActiveMessage

        // First, log the catch
        LOG_EXCEPTION_CATCH(SourceInfo, "MessageResourceException", "Caught exception at top level, so application will have to shutdown.");

        // Then the FATAL log
        std::string errorMessage;

        switch (ex.getFailType())
        {
        case (MessageResourceException::DLL_NOT_FOUND):
        {
            errorMessage = "The MessageResource DLL could not be found. Message ID: %d";
            break;
        };

        case (MessageResourceException::FORMATTING_ERROR):
        {
            errorMessage = "The message retrieved for ID %d contained invalid formatting details.";
            break;
        };

        case (MessageResourceException::LANGUAGE_ERROR):
        {
            errorMessage = "The message ID %d could not be found within the MessageResource for this systems language context.";
            break;
        };

        case (MessageResourceException::RESOURCE_NOT_FOUND_ERROR):
        {
            errorMessage = "The message ID %d could not be found within the MessageResource DLL.";
            break;
        };

        case (MessageResourceException::GENERAL_ERROR):
        {
            errorMessage = "An unspecified error occured while attempting to display the message with ID %d.";
            break;
        };

        default:
        {
            TA_ASSERT(false, "An invalid MessageResourceException type was found");
        }
        }

        errorMessage += " Error description reported from library follows:";
        LOG_GENERIC(SourceInfo, DebugUtil::DebugFatal, errorMessage, ex.getMessageID());
        LOGMORE(SourceInfo, ex.what());
        LOGMORE(SourceInfo, "As this exception was not caught within the application, GenericGUI must now exit.");

        //TODO
        // Finally, raise a message box informing the user of what is happening.

        // TD14164 azenitha++
//      TAUnicodeString errMsg;
        //errMsg.LoadString(IDS_ERROR_MESSAGE);

        /*::MessageBox(NULL,"An error occured while attempting to display an application message box.\nThis is an unrecoverable error. Check log file for further details.\nControl Station will now exit.",
        m_applicationName,MB_OK|MB_ICONSTOP);*/

        //TAUnicodeString tempUniStr = TAUnicodeString::CreateFromMultitByteString(m_appController->getApplicationDisplayName());

        //::MessageBox(NULL, errMsg.c_str(), tempUniStr.c_str(), MB_OK|MB_ICONSTOP);
        // TD14164 ++azenitha
    }

    int TransActiveLibraryApp::doStopApp()
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "doStopApp");

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveLibraryApp begin to delete main view");


        if (m_mainView != NULL)
        {
            delete m_mainView;
            m_mainView = NULL;
        }


        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveLibraryApp begin to delete application controller");

        if (m_appController != NULL)
        {
            m_appController->cleanup();
            delete m_appController;
            m_appController = NULL;
        }

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveLibraryApp begin to clean other resource");

        LOG(SourceInfo, DebugUtil::FunctionExit, "doStopApp");
        // return CWinApp::ExitInstance();
        return true;
    }


    BOOL TransActiveLibraryApp::displayMainView()
    {
        FUNCTION_ENTRY("TransActiveLibraryApp::displayMainView");

        ITransActiveView* m_mainView = createMainView();

        // Assign the dialog as the main window.
        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Created main window");


        //'m_mainView = dynamic_cast<ITransActiveView*> (mainWnd);
        TA_ASSERT(m_mainView != NULL, "Main view should be initialized successfully");

        // Start the dialog
        m_mainView->showView(false);

        LOG(SourceInfo, DebugUtil::FunctionExit, "TransActiveLibraryApp::displayMainView");

        FUNCTION_EXIT;
        return true;
    }

    void TransActiveLibraryApp::changeAppFocus(const TA_Base_Core::EFocusType focus)
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "changeAppFocus");

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveLibraryApp try to change focus");


        // If the application is null then we are trying to change the focus before it has
        // set up and initialised correctly. We will just return.
        if (! isMainViewInit())
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn, "changeAppFocus() was called on the application before it was initialised correctly. It will not change focus");
            LOG(SourceInfo, DebugUtil::FunctionExit, "changeAppFocus");
            return;
        }

        // if it is transactive window then we'll post a message to the window thread, otherwise,
        // we'll just do it from this thread
        bool isTransActiveWnd = isTransActiveMainView();

        if (isTransActiveWnd)
        {
            TA_GenericGui::EWindowPosition foc;

            switch (focus)
            {
            case (TA_Base_Core::Minimise):
                foc = TA_GenericGui::MINIMISE;
                break;

            case (TA_Base_Core::Maximise):
                foc = TA_GenericGui::MAXIMISE;
                break;

            case (TA_Base_Core::GetFocus):
                foc = TA_GenericGui::FOCUS;
                break;

            default:
                TA_ASSERT(false, "Invalid enumerated type passed for window focus change");
            }

            //m_pMainWnd->PostMessage(WM_SET_WINDOW_POSITION, NULL, foc);

            LOG(SourceInfo, DebugUtil::FunctionExit, "changeAppFocus");
            return;
        }

        switch (focus)
        {
        case (TA_Base_Core::Minimise):
            // If window is not already minimised then minimise it
            /*if(!m_pMainWnd->IsIconic())
            {
                m_pMainWnd->ShowWindow(SW_MINIMIZE);
            }*/
            break;

        case (TA_Base_Core::Maximise):
            //TODO
            //m_pMainWnd->ShowWindow(SW_MAXIMIZE);
            //m_pMainWnd->SetForegroundWindow();
            //// Set the window to be the top most so it is given focus and then move it back again.
            //m_pMainWnd->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING);
            //m_pMainWnd->SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING);
            break;

        case (TA_Base_Core::GetFocus):
            //if(m_pMainWnd->IsIconic())
            //{
            //  m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
            //}
            //m_pMainWnd->SetForegroundWindow();
            //// Set the window to be the top most so it is given focus and then move it back again.
            //m_pMainWnd->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING);
            //m_pMainWnd->SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING);
            break;

        default:
            TA_ASSERT(false, "Invalid enumerated type passed for window focus change");
        }

        LOG(SourceInfo, DebugUtil::FunctionExit, "changeAppFocus");
    }

    void TransActiveLibraryApp::moveMainView(const WindowPositionInfo & viewPositionInfo, bool isFirstCalculatePos)
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "moveMainView");

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveLibraryApp try to move main view");

        //Maochun Sun++
        //TD12779
        if (! isMainViewInit())
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn,
                "moveWindow() was called on the application before it was initialised correctly.");
            LOG(SourceInfo, DebugUtil::FunctionExit, "moveMainView");
            return;
        }

        //++Maochun Sun
        //TD12779

        // see if the application window inherits from one of our TransActive classes.  If it is, then
        // we can safely send the WM_SET_WINDOW_POSITION message to the window which will have the
        // appropriate handler for handing it.  Otherwise, if it is not a TransActive window,
        // then we'll have to reposition the window from this thread.
        bool isTransActiveWnd = isTransActiveMainView();

        // if its coordinates are changed, then move to the new coordinates, otherwise, centre the
        // application without changing the size of the application.
        if (! TA_EqualRECT(viewPositionInfo.appDim, TA_GenericGui::DEFAULT_RECT))
        {
            TA_Rect windowRect = viewPositionInfo.appDim;

            if (isTransActiveWnd)
            {
                // Have to use post message rather than send message 'cos we want it to fully repositioned itself
                // before giving it focus.
                //m_pMainWnd->SendMessage(WM_SET_WINDOW_POSITION, reinterpret_cast<WPARAM>(&windowRect), TA_GenericGui::REPOSITION);
            }
            else
            {
                //m_pMainWnd->MoveWindow(&windowRect);
                changeAppFocus(TA_Base_Core::GetFocus);
            }

            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugInfo, "Moved to: (%li,%li,%li,%li)", windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);
            LOG(SourceInfo, DebugUtil::FunctionExit, "moveMainView");
            return;
        }

        // not going to reposition it.  If it's the first time, then we'll centre it.  If it's the subsequent time
        // then we won't bother
        if (isTransActiveWnd)
        {
            // send the set window message to the main window thread.  Have to wait for the set
            // window position method to finish 'cos we want the new position
            //m_pMainWnd->SendMessage(WM_SET_WINDOW_POSITION, NULL, isFirstCalculatePos  ? TA_GenericGui::CENTRE  : TA_GenericGui::NO_REPOSITION);
        }
        else
        {
            if (isFirstCalculatePos)
            {
                //m_pMainWnd->CenterWindow(CWnd::GetDesktopWindow());
            }

            changeAppFocus(TA_Base_Core::GetFocus);
        }

        LOG(SourceInfo, DebugUtil::FunctionExit, "moveMainView");
    }

    bool TransActiveLibraryApp::isTransActiveMainView()
    {
        return m_mainView->isMainView();
    }

    void TransActiveLibraryApp::closeAllTransActiveMessages()
    {
        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveLibraryApp closeAllTransActiveMessages ");
        //TODO:
        // TransActiveMessage::closeAllMessages();
    }

    void TransActiveLibraryApp::displayAboutBox()
    {
        HelpLauncher::getInstance().displayAboutBox();
    }

    void TransActiveLibraryApp::initApplicationController()
    {
        m_appController = createApplicationController();

        if (m_appController == NULL)
        {
            LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "application controll failed to init, need to exit");
            return;
        }

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to call initAppController");

        //TAUnicodeString uniStr(m_lpCmdLine);
        //std::string stdCmdLine = uniStr.toMultiByteStdString();

        m_appController->initAppController(m_commandLine, false);
    }

    void TransActiveLibraryApp::doStartApp()
    {
        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to init application controller");
        initApplicationController();

        /*LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to display main view");
        displayMainView();*/

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to prepare for runing app");
        prepareAppRun();

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to run controller");
        m_appController->runController();
    }

    const std::string& TransActiveLibraryApp::getCommandLine()
    {
        return m_commandLine;
    }

    /*
    void  TransActiveLibraryApp::setCommandLine(int argc, char *argv[])
    {
       std::vector<std::string> args;
       std::string arg;
       m_commandline="";
       for (int i = 0; i < argc; i++)
       {
           arg=argv[i];
           m_commandline=m_commandline + " " + arg ;
       }
    }*/
} // End namespace TA_Base_Bus
