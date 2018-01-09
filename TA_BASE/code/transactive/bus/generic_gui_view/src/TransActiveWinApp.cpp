/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/TransActiveWinApp.cpp $
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
#include "bus/generic_gui_view/src/TransActiveWinApp.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "bus/generic_gui_view/src/TransActiveViewManager.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
#include "bus/generic_gui_view/resource.h" // TD14164 azenitha
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
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
//#include <winsock2.h>  // Used by getHostName
#include <QLibraryInfo>
#include <QLocale>
#include <QFile>
#include <QDir>
#include <QStyleFactory>

using TA_Base_Core::DebugUtil;
using TA_Base_Core::GenericGUIException;
using TA_Base_Core::ProcessAlreadyRunningException;
using TA_Base_Core::TransactiveException;
using TA_Base_Core::MessageResourceException;
using TA_Base_Core::RightsException;
using TA_Base_Core::RunParams;
using namespace TA_Base_Core;

namespace TA_Base_Bus
{
    TransActiveWinApp::TransActiveWinApp(int argc, char* argv[], MainViewType viewType, bool mustConnectControlStation)
        : QApplication(argc, argv),
          m_mainViewType(viewType),
          m_mainView(NULL),
          m_appController(NULL),
          m_mustConnectControlStation(mustConnectControlStation),
          m_pViewManager(NULL)
    {
        setQuitOnLastWindowClosed(true);
        RunParams::getInstance().parseCmdLine(argc, argv);
        setCommandLine(argc, argv);

        m_langPath = QString::fromStdString(TA_Base_Core::getRunParamValue("LanguagePath", (QDir::currentPath() + "/lang/").toStdString()));
        m_stylePath = QString::fromStdString(TA_Base_Core::getRunParamValue("StylePath", (QDir::currentPath() + "/style/").toStdString()));

        //regiester meta type
        m_pViewManager = new TransActiveViewManager();

        //set default qss
        setStyleSheet("default.qss");

        qRegisterMetaType<std::string>("std::string");
        qRegisterMetaType<TRA_WPARAM>("TRA_WPARAM");
        qRegisterMetaType<TRA_LPARAM>("TRA_LPARAM");
        connect(this, SIGNAL(changeLanguage(const std::string&)), this, SLOT(switchLanguageSlot(const std::string&)));
        connect(this, SIGNAL(changeStyleSheet(const std::string&)), this, SLOT(setStyleSheetSlot(const std::string&)));
    }

    TransActiveWinApp::~TransActiveWinApp()
    {
        RunParams::getInstance().deregisterRunParamUser(this);
        doStopApp();
    }

    void TransActiveWinApp::setStyleSheet(const std::string& sheetFileName)
    {
        QMetaObject::invokeMethod(this, "setStyleSheetSlot", Qt::AutoConnection, Q_ARG(const std::string, sheetFileName));
        //emit changeStyleSheet(sheetFileName);
    }

    void TransActiveWinApp::setStyleSheetSlot(const std::string sheetFileName)
    {
        auto filefullpath = boost::filesystem::path(m_stylePath.toStdString()) / sheetFileName;
        QFile file(filefullpath.string().c_str());
        file.open(QFile::ReadOnly);
        QString strStyle = QString::fromLatin1(file.readAll());

        if (file.exists())
        {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "setStyleSheet %s)", filefullpath.string().c_str());
            QApplication::setStyleSheet(strStyle);
        }

        //this->m_pViewManager->adjustSizeAllView();
    }

    void TransActiveWinApp::switchTranslator(QTranslator& translator, const QString& filename)
    {
        // remove the old translator
        qApp->removeTranslator(&translator);

        // load the new translator
        if (translator.load(filename))
        {
            qApp->installTranslator(&translator);
        }
    }

    void TransActiveWinApp::switchLanguage(const std::string& rLanguage)
    {
        //std::string * str = new std::string(rLanguage);
        //QString str = TAUtfString::CreateFromMultitByteString(rLanguage);
        bool r = QMetaObject::invokeMethod(this, "switchLanguageSlot", Qt::AutoConnection, Q_ARG(std::string, rLanguage));
        //emit changeLanguage(rLanguage);
    }

    void TransActiveWinApp::switchLanguageSlot(const std::string& rLanguage)
    {
        QString newLang = TAUtfString::createFromMultitByteString(rLanguage);

        if (m_currLang != newLang)
        {
            m_currLang = newLang;
            QLocale locale = QLocale(m_currLang);
            QLocale::setDefault(locale);

            QString entityName = TAUtfString::createFromMultitByteString(TA_Base_Core::RunParams::getInstance().get(RPARAM_ENTITYNAME));
            auto filePath = boost::filesystem::path(m_langPath.toStdString()) / str(boost::format("%s_%s.qm") % entityName.toStdString() % newLang.toStdString());
            QString filename = QString::fromStdString(filePath.string());

            if (QFile::exists(filename))
            {
                //change application translator
                switchTranslator(m_translator, filename);

                if (QFile::exists(filename))
                {
                    //change QT core translator
                    QString languageName = QLocale::languageToString(locale.language());
                    filename = m_langPath + "qt_" + languageName + ".qm";
                    switchTranslator(m_translatorQt, filename);
                }

                this->m_pViewManager->switchLanguage();
            }
        }
    }

    void TransActiveWinApp::onRunParamChange(const std::string& name, const std::string& value)
    {
        if (name == RPARAM_LANG)
        {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "Received RunParam (%s = %s)", name.c_str(), value.c_str());
            this->switchLanguage(value);
        }
        else if (name == RPARAM_STYLE)
        {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "Received RunParam (%s = %s)", name.c_str(), value.c_str());
            this->setStyleSheet(value);
        }
        else if (name == "StylePath")
        {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "Received RunParam (%s = %s)", name.c_str(), value.c_str());
            m_stylePath = QString::fromStdString(value);
        }
        else if (name == "LanguagePath")
        {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "Received RunParam (%s = %s)", name.c_str(), value.c_str());
            m_langPath = QString::fromStdString(value);
        }
    }

    /////////////////////////////////////////////////////////////////////////////
    // TransActiveWinApp initialization

    int TransActiveWinApp::startApp()
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

            std::string appName = TA_Base_Core::RunParams::getInstance().get(RPARAM_APPNAME);
            int selectedButton = showMsgBox(IDS_UE_010010, appName);
        }
        catch (...)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown", "Because this has been caught at the top level we must now shutdown.");

            if (m_appController != NULL)
            {
                m_appController->notifyFatalErrorWhenLaunch();
            }

            std::string appName = TA_Base_Core::RunParams::getInstance().get(RPARAM_APPNAME);
            int selectedButton = showMsgBox(IDS_UE_010010, appName);
        }

        if (shouldStartMessagePump)
        {
            return QApplication::exec();
        }

        return 1;
    }

    bool TransActiveWinApp::notify(QObject* receiver, QEvent* event)
    {
        try
        {
            return QApplication::notify(receiver, event);
        }
        catch (GenericGUIException& e)
        {
            handleGenericGUIException(e);
        }

        return false;
    }

    void TransActiveWinApp::handleGenericGUIException(const GenericGUIException& ex)
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
        case GenericGUIException::ENTITY_CONFIGURATION_INVALID:
            errorId = IDS_UE_010011;
            break;

        case GenericGUIException::ENTITY_DOES_NOT_EXIST:
        case GenericGUIException::ENTITY_NOT_UNIQUE:
            errorId = IDS_UE_010013;
            break;

        case GenericGUIException::COMMAND_LINE_INVALID:
            errorId = IDS_UE_010008;
            break;

        case GenericGUIException::NO_DATABASE_CONNECTION:
            errorId = IDS_UE_010001;
            break;

        case GenericGUIException::COMMUNICATION_ERROR_WITH_CONTROL_STATION:
            errorId = IDS_UE_010014;
            break;

        case GenericGUIException::ACCESS_DENIED:
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

        case GenericGUIException::UNKNOWN_ACCESS_RIGHTS:
        {
            errorId = IDS_UE_010019;
            break;
        }

        case GenericGUIException::NO_ACTIVE_SESSION: //TD1141
            errorId = IDS_UE_020050;
            break;

        case GenericGUIException::INITIALISATION_ERRORS:
        default:
            errorId = IDS_UE_010012;
            std::string appName = TA_Base_Core::RunParams::getInstance().get(RPARAM_APPNAME);
            userMsg << appName;
            break;
        }

        userMsg.showMsgBox(errorId);
    }

    void TransActiveWinApp::handleMessageResourceException(const MessageResourceException& ex)
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
        case MessageResourceException::DLL_NOT_FOUND:
            errorMessage = "The MessageResource DLL could not be found. Message ID: %d";
            break;

        case MessageResourceException::FORMATTING_ERROR:
            errorMessage = "The message retrieved for ID %d contained invalid formatting details.";
            break;

        case MessageResourceException::LANGUAGE_ERROR:
            errorMessage = "The message ID %d could not be found within the MessageResource for this systems language context.";
            break;

        case MessageResourceException::RESOURCE_NOT_FOUND_ERROR:
            errorMessage = "The message ID %d could not be found within the MessageResource DLL.";
            break;

        case MessageResourceException::GENERAL_ERROR:
            errorMessage = "An unspecified error occured while attempting to display the message with ID %d.";
            break;

        default:
            TA_ASSERT(false, "An invalid MessageResourceException type was found");
        }

        errorMessage += " Error description reported from library follows:";
        LOG_GENERIC(SourceInfo, DebugUtil::DebugFatal, errorMessage, ex.getMessageID());
        LOGMORE(SourceInfo, ex.what());
        LOGMORE(SourceInfo, "As this exception was not caught within the application, GenericGUI must now exit.");

        //TODO
        // Finally, raise a message box informing the user of what is happening.

        // TD14164 azenitha++
        // TAUnicodeString errMsg;
        //errMsg.LoadString(IDS_ERROR_MESSAGE);

        /*::MessageBox(NULL,"An error occured while attempting to display an application message box.\nThis is an unrecoverable error. Check log file for further details.\nControl Station will now exit.",
        m_applicationName,MB_OK|MB_ICONSTOP);*/

        //TAUnicodeString tempUniStr = TAUnicodeString::CreateFromMultitByteString(m_appController->getApplicationDisplayName());

        //::MessageBox(NULL, errMsg.c_str(), tempUniStr.c_str(), MB_OK|MB_ICONSTOP);
        // TD14164 ++azenitha
    }

    int TransActiveWinApp::doStopApp()
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "doStopApp");

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveWinApp begin to delete main view");

        if (m_mainViewType == MVT_DIALOG || m_mainViewType == MVT_PROPERTY_SHEET)
        {
            if (m_mainView != NULL)
            {
                delete m_mainView;
                m_mainView = NULL;
            }
        }

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveWinApp begin to delete application controller");

        if (m_appController != NULL)
        {
            m_appController->cleanup();
            delete m_appController;
            m_appController = NULL;
        }

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveWinApp begin to clean other resource");

        LOG(SourceInfo, DebugUtil::FunctionExit, "doStopApp");
        // return CWinApp::ExitInstance();
        return true;
    }

    //void TransActiveWinApp::WinHelp(DWORD dwData, UINT nCmd)
    //{
    //    if (HELP_CONTEXT == nCmd)
    //    {
    //        // display help for the specified topic
    //        HelpLauncher::getInstance().displayContextSpecificHelp(LOWORD(dwData));
    //    }
    //    else
    //    {
    //        // display the main help contents
    //        HelpLauncher::getInstance().displayHelp();
    //    }
    //}

    //void TransActiveWinApp::WinHelpInternal(DWORD dwData, UINT nCmd)
    //{
    //    if (HELP_CONTEXT == nCmd)
    //    {
    //        // display help for the specified topic
    //        HelpLauncher::getInstance().displayContextSpecificHelp(LOWORD(dwData));
    //    }
    //    else
    //    {
    //        // display the main help contents
    //        HelpLauncher::getInstance().displayHelp();
    //    }
    //}

    int TransActiveWinApp::outOfMemoryHandler(size_t size)
    {
        /*
        std::ostringstream oss;
        oss << "No more memory available.  Failed to allocated " << size << " bytes.";
        TA_THROW(std::bad_alloc(oss.str().c_str()));
        */
        // Shouldn't reach here but if it does, this should tell new to stop allocation attempts
        TA_THROW(std::bad_alloc());
        return 0;
    }

    TA_Base_Bus::ITransActiveView*  TransActiveWinApp::getMainView()
    {
        return this->getViewManager()->getMainView();
    }

    BOOL TransActiveWinApp::displayMainView()
    {
        FUNCTION_ENTRY("TransActiveWinApp::displayMainView");
        ITransActiveView* m_mainView = createMainView();

        // Assign the dialog as the main window.
        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Created main window");

        //'m_mainView = dynamic_cast<ITransActiveView*> (mainWnd);
        TA_ASSERT(m_mainView != NULL, "Main view should be initialized successfully");

        if (m_mainViewType == MVT_DIALOG)
        {
            // Start the dialog
            m_mainView->showView(false);
            LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Dialog begin to show on the screen");
        }
        else if (m_mainViewType == MVT_PROPERTY_SHEET)
        {
            // TODO:Start the property sheet
            //CPropertySheet* propertySheet = dynamic_cast<CPropertySheet*>(mainWnd);
            //TA_ASSERT(propertySheet != NULL, "m_mainView is not a CPropertySheet");

            //propertySheet->Create();
        }
        else if (m_mainViewType == MVT_SDI_FRAME || m_mainViewType == MVT_MDI_FRAME)
        {
            //TODO:LoadFrame is called by createMainView
            //CFrameWnd* frameWnd = dynamic_cast<CFrameWnd*>(mainWnd);
            //TA_ASSERT(frameWnd != NULL, "m_mainView is not a FrameWnd");

            //frameWnd->LoadFrame(dlgID); //dlgID is refer to IDResource of FrameWnd
        }
        else
        {
            TA_ASSERT(FALSE, "no this application type");
        }

        LOG(SourceInfo, DebugUtil::FunctionExit, "TransActiveWinApp::displayMainView");

        FUNCTION_EXIT;
        return true;
    }

    void TransActiveWinApp::changeAppFocus(const TA_Base_Core::EFocusType focus)
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "changeAppFocus");
        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveWinApp try to change focus");

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
            case TA_Base_Core::Minimise:
                foc = TA_GenericGui::MINIMISE;
                break;

            case TA_Base_Core::Maximise:
                foc = TA_GenericGui::MAXIMISE;
                break;

            case TA_Base_Core::GetFocus:
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
        case TA_Base_Core::Minimise:
            // If window is not already minimised then minimise it
            /*if(!m_pMainWnd->IsIconic())
            {
                m_pMainWnd->ShowWindow(SW_MINIMIZE);
            }*/
            break;

        case TA_Base_Core::Maximise:
            //TODO
            //m_pMainWnd->ShowWindow(SW_MAXIMIZE);
            //m_pMainWnd->SetForegroundWindow();
            //// Set the window to be the top most so it is given focus and then move it back again.
            //m_pMainWnd->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING);
            //m_pMainWnd->SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING);
            break;

        case TA_Base_Core::GetFocus:
            //if(m_pMainWnd->IsIconic())
            //{
            //    m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
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

    void TransActiveWinApp::moveMainView(const WindowPositionInfo& viewPositionInfo, bool isFirstCalculatePos)
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "moveMainView");

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveWinApp try to move main view");

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

    bool TransActiveWinApp::isTransActiveMainView()
    {
        return m_mainView->isMainView();
    }

    void TransActiveWinApp::closeAllTransActiveMessages()
    {
        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TransActiveWinApp closeAllTransActiveMessages ");
        //TODO:
        // TransActiveMessage::closeAllMessages();
    }

    void TransActiveWinApp::displayAboutBox()
    {
        HelpLauncher::getInstance().displayAboutBox();
    }

    void TransActiveWinApp::requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent)
    {
        TA_Base_Bus::UINotificationDataContextPtr dataContext = uiEvent->getMyDataContext();

        int msgID = dataContext->getTransActiveMessageId();
        int nResponse = 0;
        TA_Base_Bus::TransActiveMessage userMsg;

        for (size_t idx = 0; idx < dataContext->getParamSize(); idx++)
        {
            userMsg << dataContext->getParamValue(idx);
        }

        nResponse = userMsg.showMsgBox(msgID);
        uiEvent->setUIReplyType((UIReplyType)nResponse);
    }

    void TransActiveWinApp::initApplicationController()
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

        m_appController->initAppController(m_commandline, m_mustConnectControlStation);
    }

    void TransActiveWinApp::doStartApp()
    {
        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to init application controller");
        initApplicationController();

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to display main view");
        displayMainView();

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to prepare for runing app");
        prepareAppRun();

        RunParams::getInstance().registerRunParamUser(this, RPARAM_LANG);
        RunParams::getInstance().registerRunParamUser(this, RPARAM_STYLE);

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to run controller");
        m_appController->runController();
    }

    const std::string& TransActiveWinApp::getCommandLine()
    {
        return m_commandline;
    }

    void TransActiveWinApp::setCommandLine(int argc, char* argv[])
    {
        for (int i = 0; i < argc; i++)
        {
            m_commandline.append(argv[i]).append(" ");
        }
    }
} // End namespace TA_Base_Bus
