/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemManagerDlg.cpp $
  * @author:  Justin Ebedes
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * SystemManagerDlg is the class associated with the application's main window.
  * This class inherits from AdminObserver. The main window registers itself 
  * as an observer with the AdminManager.
  *
  */

#pragma warning(disable:4786)

#include <afxpriv.h>   //For AfxLoadString
#include <algorithm>
#include <string>

#include "StdAfx.h"
#include "SystemManager.h"
#include "../pmodel/src/SystemManagerDlgModel.h"
#include "SystemManagerDlg.h"
#include "../pmodel/src/SystemManagerConsts.h"
#include "../pmodel/src/AppServiceConsts.h"

#include "StopProcessDlg.h"
#include "ShutdownSystemControllerDlg.h"
#include "ProcessParamsDlg.h"
#include "ChangeOperationModeDlg.h"

#include "bus/common_subsystem_facade/src/SecuritySubsystemFacade.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/message/src/AuditMessageSender.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/SystemControllerException.h"

//#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
//#include "core/data_access_interface/src/ResourceAccessFactory.h"
//#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/data_access_interface/src/IResource.h"

using TA_Base_Core::RunParams;

namespace TA_Base_App
{
    SystemManagerDlg::SystemManagerDlg(TA_Base_Bus::ITransActiveWinApp * app)
		: TA_Base_Bus::AbstractTransActiveDialog(app, SystemManagerDlg::IDD, NULL)
		, m_realPModel(NULL)
    {
        //{{AFX_DATA_INIT(SystemManagerDlg)
        m_statusString = _T("");
	//}}AFX_DATA_INIT
        // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
        m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
        m_currentChildWindow     = NULL;
        m_sysControllerDialogBox = NULL;

		TA_Base_Bus::ViewResizeProperty properties;
		properties.canMaximise = true;
		properties.maxHeight = -1;
		properties.maxWidth = -1;
		properties.minHeight = 250;
		properties.minWidth = 500;
		m_resizeViewHelper->setViewResizeProperty(properties);
    }

    void SystemManagerDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(SystemManagerDlg)
		DDX_Control(pDX, IDC_MENU_LINE, m_menuLine);
        DDX_Control(pDX, IDC_MAIN_PROCESSLIST, m_processList);
        DDX_Text(pDX, IDC_STATUS, m_statusString);
	//}}AFX_DATA_MAP
    }

    BEGIN_MESSAGE_MAP(SystemManagerDlg, AbstractTransActiveDialog)
	    //{{AFX_MSG_MAP(SystemManagerDlg)
	    ON_BN_CLICKED(ID_FILE_LOGIN, OnLogin)
	    ON_BN_CLICKED(ID_FILE_LOGOUT, OnLogout)
	    ON_BN_CLICKED(ID_CONTROL_STARTPROCESS, OnStartProcess)
	    ON_BN_CLICKED(ID_CONTROL_SETPARAM, OnDebugLevel)
	    ON_BN_CLICKED(ID_CONTROL_CHANGEPROCESSOPERATIONMODE, OnChangeProcessMode)
	    ON_BN_CLICKED(ID_VIEW_OPTIONS, OnOptions)
	    ON_BN_CLICKED(ID_CONTROL_STOPPROCESS, OnStopProcess)
	    ON_BN_CLICKED(ID_CONTROL_SHUTDOWNSYSTEMCONTROLLER, OnShutdownMonitor)
	    ON_BN_CLICKED(ID_VIEW_REFRESH, OnRefresh)
	    ON_WM_CONTEXTMENU()
        ON_WM_SIZE()
        ON_WM_SIZING()
	    ON_BN_CLICKED(ID_HELP_ABOUT, OnAbout)
	    ON_COMMAND(ID_BUTTON_LOGIN, OnButtonLogin)
	    ON_COMMAND(ID_BUTTON_LOGOUT, OnButtonLogout)
	    ON_COMMAND(ID_BUTTON_START, OnButtonStartProcess)
	    ON_COMMAND(ID_BUTTON_STOP, OnButtonStopProcess)
        ON_COMMAND(ID_BUTTON_REFRESH, OnButtonRefresh)
		ON_NOTIFY(LVN_ITEMCHANGED, IDC_MAIN_PROCESSLIST, onItemchangedMainProcesslist)
		ON_COMMAND(ID_FILE_EXIT, OnClose)
		ON_WM_ACTIVATE()
        ON_MESSAGE( WM_SHUTDOWN_SYSTEM_CONTROLLER, onShutdownSystemController)
		ON_MESSAGE( WM_NEW_PROCESS_DATA, onNewProcessData)
	//}}AFX_MSG_MAP
        ON_MESSAGE(WM_TASETLANG, &SystemManagerDlg::OnSetLanguage)
    END_MESSAGE_MAP()

    // If you add a minimize button to your dialog, you will need the code below
    // to draw the icon.  For MFC applications using the document/view model,
    // this is automatically done for you by the framework.


    void SystemManagerDlg::OnLogin()
    {
		if (m_sysControllerDialogBox == NULL)
		{
			m_sysControllerDialogBox = new SystemControllerDlg(m_pWinApp, this);
		}
        m_currentChildWindow = m_sysControllerDialogBox;
		
        int result = m_sysControllerDialogBox->DoModal();
        m_currentChildWindow = NULL;

        if (result == IDCANCEL)
        {
            return; // Not connected!
        }

		std::string sessionId = m_realPModel->getSessionId();
		std::string operatorName = m_pWinApp->getApplicationController()->getOperatorName();
		if (operatorName.empty())
		{
			sessionId = "";
		}


		int errorId = 0;
		int res = m_realPModel->doLogin(errorId, sessionId, operatorName);

        if (res != ERR_AUTH_NO_ERROR)
        {
            TAUnicodeString action, error, actionName;
            action.LoadString(IDS_LOGIN);
            error.LoadString(errorId);
            actionName = action + error;
			//To Display the Port Number
			if(errorId == IDS_NO_SYSTEM_CONTROLLER)
			{
				std::string portNumber = RunParams::getInstance().get(RPARAM_PORTNUMBER);
				TAUnicodeString portNumberString = TAUnicodeString::CreateFromMultitByteString(portNumber.c_str());
				actionName += _T(" ") + portNumberString;
			}
            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << actionName;
            UINT selectedButton = userMsg.showMsgBox(IDS_UE_020046);
            return;
        }

		m_realPModel->sendAuditMessage(sessionId, TA_Base_Core::OperatorAudit::OperatorAdminLogIn);

 
        // Check the permissions for the currently logged in operator.
        checkPermissions();

        // Register with Admin Manager as an observer
        m_realPModel->registerUpdate();
        

        setLoggedInMode(true);

        CMenu* pMenu = GetMenu();
        pMenu->EnableMenuItem(ID_CONTROL_STARTPROCESS,MF_GRAYED);
        pMenu->EnableMenuItem(ID_CONTROL_STOPPROCESS,MF_GRAYED);
        pMenu->EnableMenuItem(ID_CONTROL_CHANGEPROCESSOPERATIONMODE,MF_GRAYED);
        pMenu->EnableMenuItem(ID_VIEW_OPTIONS, m_realPModel->m_isSetOptionsAllowed ? MF_ENABLED : MF_GRAYED);
        
        std::string hostName = m_realPModel->getSystemControllerHostName();
        TAUnicodeString uniHostName = TAUnicodeString::CreateFromMultitByteString(hostName.c_str());
        TAUnicodeString windowTitle = TAAfxFormatString1( STRING_ID_SYSTEM_MANAGER_0003, uniHostName.c_str() );

        this->SetWindowText(windowTitle.c_str());

		TAUnicodeString uniOperatorName = TAUnicodeString::CreateFromMultitByteString(operatorName.c_str());

        m_statusString = TAAfxFormatString2( STRING_ID_SYSTEM_MANAGER_0001, uniOperatorName.str(), uniHostName.str() ).c_str();

		UpdateData(FALSE);
    }

    void SystemManagerDlg::OnLogout()
    {
        TAUnicodeString actionName;
        actionName.LoadString(IDS_LOGOUT);
        TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << actionName;
        UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);
        if (selectedButton != IDYES)
        {
            return;
        }

        m_realPModel->m_userLoggedIn = false;
        logout();
        this->SetWindowText( TALoadString(STRING_ID_SYSTEM_MANAGER_0002).c_str() ); // "System Manager"
    }

    void SystemManagerDlg::setLoggedInMode(bool loggedIn)
    {
        m_realPModel->m_userLoggedIn = loggedIn;
    
        setupMenu(loggedIn);
        setupToolbar(loggedIn); 
    }

    void SystemManagerDlg::OnClose()
    {
        TAUnicodeString actionName;
        actionName.LoadString(IDS_EXIT);
        TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << actionName;
        UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);
        if (selectedButton != IDYES)
        {
            return;
        }
    
        logout();

		//TD18095, jianghp, to fix the performance of showing manager application
		AbstractTransActiveDialog::DestroyWindow();
    }

    void SystemManagerDlg::logout()
    {         
        // Log an event.

		std::string sessionId = m_pWinApp->getApplicationController()->getSessionId();
		m_realPModel->sendAuditMessage(sessionId, TA_Base_Core::OperatorAudit::OperatorAdminLogOut);
            
        // Unregister with the AdminManager as an observer
        m_realPModel->unregisterUpdate();
   
	    //Ask the Admin Manager to terminate session
        m_realPModel->logout();

        //Close the open child window
        if (m_currentChildWindow)
        {
            if (m_currentChildWindow->m_hWnd)
            {
                m_currentChildWindow->EndDialog(IDCANCEL);
            }
        }

        m_statusString = TALoadString( STRING_ID_SYSTEM_MANAGER_0000 ).c_str(); // "Logged out"

        UpdateData(FALSE);
        setLoggedInMode(false);
        m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_START,false);
        m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_STOP,false);
        m_processList.DeleteAllItems();
    }

    void SystemManagerDlg::OnStartProcess()
    {
        std::vector<std::string> states;
		states.push_back(AppServiceConsts::STOPPED);

        std::vector<std::string> nonRunningProcesses = getSelectedProcessesWithState(states);
        StartProcessDlg StartDlg(m_pWinApp, nonRunningProcesses,this);
        m_currentChildWindow = &StartDlg;
        StartDlg.DoModal();

        m_currentChildWindow = NULL;
    }

    void SystemManagerDlg::OnDebugLevel()
    {
        std::vector<std::string> states;
		states.push_back(AppServiceConsts::STOPPED);
        states.push_back(AppServiceConsts::RUNNING);
        states.push_back(AppServiceConsts::RUNNING_CONTROL);
        states.push_back(AppServiceConsts::RUNNING_MONITOR);
        states.push_back(AppServiceConsts::GOING_TO_CONTROL);
        states.push_back(AppServiceConsts::GOING_TO_MONITOR);
        // Ignore non-managed processes when selecting processes
        std::vector<std::string> runningProcesses = getSelectedProcessesWithState(states,false,true);
        ProcessParamsDlg ParamDlg(m_pWinApp, runningProcesses, this);
        m_currentChildWindow = &ParamDlg;
        ParamDlg.DoModal();

        m_currentChildWindow = NULL;
    }

    void SystemManagerDlg::OnChangeProcessMode()
    {
        std::vector<std::string> states;
        states.push_back(AppServiceConsts::RUNNING_CONTROL);
        states.push_back(AppServiceConsts::RUNNING_MONITOR);

        // When selecting processes ignore services and non-managed processes
        std::vector<std::string> runningProcesses = getSelectedProcessesWithState(states,true,true);
        ChangeOperationModeDlg changeModeDlg(m_pWinApp, runningProcesses,this);
        m_currentChildWindow = &changeModeDlg;
        changeModeDlg.DoModal();

        m_currentChildWindow = NULL;
    }

    void SystemManagerDlg::OnOptions()
    {
        OptionsDlg optionsDlg(m_pWinApp, this);
        m_currentChildWindow = &optionsDlg;
        optionsDlg.DoModal();
        m_currentChildWindow = NULL;
    }

    void SystemManagerDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
    {
		if (_stricmp(updateType, PMODEL_DATA_CHANGE_SESSION_ID) == 0)
		{
			checkPermissions();
		}
		if (_stricmp(updateType, DOMAIN_DATA_CHANGE_NEW_PROCESS_DATA) == 0)
		{
			getNewData();
		}

    }

	LRESULT SystemManagerDlg::onNewProcessData(WPARAM wparam, LPARAM lparam)
	{
        // Get new process list from the AdminManager and update display.
		try
		{
			m_processList.update(m_realPModel->getProcesses());
	 
			// Update the menu / tool bar icons to match the new process data.
	        onItemchangedMainProcesslist(NULL,NULL);
		}
        catch(TA_Base_Core::SystemControllerException&)
        {
			m_realPModel->unregisterUpdate();

            TA_Base_Bus::TransActiveMessage userMsg;
            UINT selectedButton = userMsg.showMsgBox(IDS_UE_020022);
			
            logout();
        }

		return 0;
	}

    void SystemManagerDlg::OnStopProcess()
    {
        // Can stop processes with Running, Not Running, or Startup state.
        std::vector<std::string> states;
        states.push_back(AppServiceConsts::RUNNING);
        states.push_back(AppServiceConsts::RUNNING_CONTROL);
        states.push_back(AppServiceConsts::RUNNING_MONITOR);
        states.push_back(AppServiceConsts::GOING_TO_CONTROL);
        states.push_back(AppServiceConsts::GOING_TO_MONITOR);
        states.push_back(AppServiceConsts::STARTUP);
        states.push_back(AppServiceConsts::NOT_RUNNING);
        states.push_back(AppServiceConsts::NOT_STARTED);

        std::vector<std::string> stoppableProcesses = getSelectedProcessesWithState(states);

        StopProcessDlg stopDlg(m_pWinApp, stoppableProcesses, this);
        m_currentChildWindow = &stopDlg;
        stopDlg.DoModal();
        m_currentChildWindow = NULL;
    }

    void SystemManagerDlg::OnShutdownMonitor()
    {
        ShutdownSystemControllerDlg shutdownDlg(m_pWinApp, m_realPModel->getSystemControllerHostName(), this);
        m_currentChildWindow = &shutdownDlg;
        shutdownDlg.DoModal();
        m_currentChildWindow = NULL;
    }

    void SystemManagerDlg::OnRefresh()
    {
        try
        {
            m_realPModel->refreshData();
        }
        catch(TA_Base_Core::SystemControllerException&)
        {
            TA_Base_Bus::TransActiveMessage userMsg;
            UINT selectedButton = userMsg.showMsgBox(IDS_UE_020022);

			logout();
        }
    }

    void SystemManagerDlg::OnContextMenu(CWnd* pWnd, CPoint point)
    {
        if ((pWnd == &m_processList) && (m_realPModel->m_userLoggedIn))
        {
            POSITION pos = m_processList.GetFirstSelectedItemPosition();

			if (pos == NULL)
			{
				// No processes selected, so don't want to do anything.
				return;
			}

            CMenu ContextMenu;
            ContextMenu.LoadMenu(IDR_CONTEXT_MENU);
            TATranslateMenu( IDR_CONTEXT_MENU, &ContextMenu );

            // Initially disable all menu items.
            CMenu* pContextMenu = ContextMenu.GetSubMenu(0);
            pContextMenu->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
            pContextMenu->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);
            pContextMenu->EnableMenuItem(2, MF_BYPOSITION | MF_GRAYED);   

            m_processList.freeze();
            std::string currStatus;

            while (pos != NULL)
            {
                int itemNum = m_processList.GetNextSelectedItem(pos);
                currStatus = m_processList.getStatus(itemNum);

                if( (currStatus.compare(AppServiceConsts::RUNNING_CONTROL) == 0) ||
                    (currStatus.compare(AppServiceConsts::RUNNING_MONITOR) == 0) ||
                    (currStatus.compare(AppServiceConsts::GOING_TO_CONTROL) == 0) ||
                    (currStatus.compare(AppServiceConsts::GOING_TO_MONITOR) == 0) ||
                    (currStatus.compare(AppServiceConsts::RUNNING) == 0) )
                {
                    ContextMenu.EnableMenuItem(IDM_STOP_PROCESS, m_realPModel->m_isStopProcessAllowed ? MF_ENABLED : MF_GRAYED);

					TAUnicodeString tmpUniStr = m_processList.GetItemText(itemNum,0);
                    std::string entityDescription = tmpUniStr.toMultiByteStdString();

                    bool isManagedProcess = m_realPModel->isManagedProcessFromDescription(entityDescription);

                    // Can only change process operation mode on processes that are managed and are not services
                    if( isManagedProcess )
                    {
                        ContextMenu.EnableMenuItem(IDM_PROCESS_PARAM, m_realPModel->m_isSetProcessParamAllowed ? MF_ENABLED : MF_GRAYED);
                    }
                }
                else if( (currStatus.compare(AppServiceConsts::STARTUP) == 0) ||
                         (currStatus.compare(AppServiceConsts::NOT_STARTED) == 0) ||
                         (currStatus.compare(AppServiceConsts::NOT_RUNNING) == 0) )
                {
                    ContextMenu.EnableMenuItem(IDM_STOP_PROCESS,
                        m_realPModel->m_isStopProcessAllowed ? MF_ENABLED : MF_GRAYED);
                }
                else if (currStatus.compare(AppServiceConsts::STOPPED) == 0)
                {
                    ContextMenu.EnableMenuItem(IDM_START_PROCESS,
                        m_realPModel->m_isStartProcessAllowed ? MF_ENABLED : MF_GRAYED);
					ContextMenu.EnableMenuItem(IDM_PROCESS_PARAM,
						m_realPModel->m_isSetProcessParamAllowed ? MF_ENABLED : MF_GRAYED);

                }
            }

            m_processList.unfreeze();

            int nCmd = (int) pContextMenu->TrackPopupMenu(
                TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
                point.x, point.y,
                this);

            // Process the appropriate context menu command.
            switch (nCmd)
            {
            case IDM_START_PROCESS:
                OnStartProcess();
                break;
            case IDM_STOP_PROCESS:
                OnStopProcess();
                break;
            case IDM_PROCESS_PARAM:
                OnDebugLevel();
                break;
            default:
                break;
            }

            m_currentChildWindow = NULL;
        }
        else
        {
            CDialog::OnContextMenu(pWnd, point);
        }
    }

    LRESULT SystemManagerDlg::onShutdownSystemController(WPARAM wparam, LPARAM lparam)
    {
        try
        {
            m_realPModel->shutdownSystemController();
        }
        catch(TA_Base_Core::SystemControllerException&)
        {
            // The System Controller will generally take longer than 20
            // seconds to shutdown - so do nothing if an exception was thrown.
        }

        logout();
        return 0;
    }

    void SystemManagerDlg::OnSize(UINT nType, int cx, int cy)
    {
        CDialog::OnSize(nType, cx, cy);

		// Resize the line under the menus.
	    if(m_menuLine.m_hWnd != NULL)
	    {
			CRect menuLineRect;
			m_menuLine.GetWindowRect(menuLineRect);
			ScreenToClient(menuLineRect);
			m_menuLine.MoveWindow(0, 0, cx, menuLineRect.Height(), true);
		}

        resizeChildWindows(cx, cy);
    }

    void SystemManagerDlg::resizeChildWindows(int width, int height)
    {
        RECT  ListWinRect;
        if (m_processList.m_hWnd)
        {
            m_processList.GetWindowRect(&ListWinRect);

            ListWinRect.top    = 70;
            ListWinRect.left   = 2;
            ListWinRect.bottom = max(ListWinRect.top + height - 93, ListWinRect.top);
            ListWinRect.right  = max(ListWinRect.left + width - 3, ListWinRect.left);
            m_processList.MoveWindow(&ListWinRect);
        }

        CWnd* pStatus = GetDlgItem(IDC_STATUS);
    
        if (pStatus)
        {
            if (pStatus->m_hWnd)
            {
                ListWinRect.top    = ListWinRect.bottom + 2;
                ListWinRect.bottom = ListWinRect.top + 20;
                pStatus->MoveWindow(&ListWinRect);
            }
        }
    }

    void SystemManagerDlg::OnSizing(UINT fwSide, LPRECT pRect)
    {
        pRect->bottom = max (pRect->bottom, pRect->top + 140);
        CDialog::OnSizing(fwSide, pRect);
    }

    void SystemManagerDlg::setupMenu(bool loggedIn)
    {
        CMenu* pMenu = GetMenu();
        if(loggedIn)
        {
            pMenu->EnableMenuItem(ID_FILE_LOGIN,MF_GRAYED);
            pMenu->EnableMenuItem(ID_FILE_LOGOUT,MF_ENABLED);
            pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		    pMenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);

            pMenu->EnableMenuItem(ID_CONTROL_SHUTDOWNSYSTEMCONTROLLER,
                m_realPModel->m_isShutdownAllowed ? MF_ENABLED : MF_GRAYED);        
            pMenu->EnableMenuItem(ID_CONTROL_SETPARAM,
                MF_GRAYED);
		    
            this->DrawMenuBar();
        }
        else
        {
            pMenu->EnableMenuItem(ID_FILE_LOGIN,MF_ENABLED);
            pMenu->EnableMenuItem(ID_FILE_LOGOUT,MF_GRAYED);
            pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED | MF_DISABLED);
		    pMenu->EnableMenuItem(2, MF_BYPOSITION | MF_GRAYED | MF_DISABLED);

            pMenu->EnableMenuItem(ID_CONTROL_SHUTDOWNSYSTEMCONTROLLER, MF_GRAYED);

		    this->DrawMenuBar();
        }

    }
    void SystemManagerDlg::setupToolbar(bool loggedIn)
    {
        // Login button should be enabled, if not logged in and vice versa
        m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_LOGIN,!loggedIn);
        m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_LOGOUT,loggedIn);
    
        m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_REFRESH,loggedIn);    
    }

    void SystemManagerDlg::initToolbar()
    {
        m_toolBar.GetToolBarCtrl().SetButtonSize(CSize(69,55));
        
        m_toolBar.SetButtonText(0,_T("Login"));
        m_toolBar.SetButtonText(1,_T("Logout"));
        m_toolBar.SetButtonText(2,_T("Start"));
        m_toolBar.SetButtonText(3,_T("Stop"));
        m_toolBar.SetButtonText(4,_T("Refresh"));
        m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_START,false);
        m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_STOP,false); 

        TATranslateToolBar(IDR_TOOLBAR1, &m_toolBar );
    }

    void SystemManagerDlg::OnAbout()
    {
        TA_Base_Bus::HelpLauncher::getInstance().displayAboutBox();  
    }

    void SystemManagerDlg::OnButtonLogin() 
    {
	    OnLogin();	
    }
    void SystemManagerDlg::OnButtonLogout() 
    {
	    OnLogout();	
    }
    void SystemManagerDlg::OnButtonStartProcess() 
    {
	    OnStartProcess();	
    }

    void SystemManagerDlg::OnButtonStopProcess() 
    {
	    OnStopProcess();	
    }

    void SystemManagerDlg::OnButtonRefresh() 
    {
	    OnRefresh();
    }


    void SystemManagerDlg::onItemchangedMainProcesslist(NMHDR* pNMHDR, LRESULT* pResult) 
    {
	    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
        CMenu* pMenu = GetMenu();

        // If the operator clicks inside the list client area, but does not select
        // a process, turn off all related buttons/menu items.
        m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_START, false);
        m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_STOP, false);
        pMenu->EnableMenuItem(ID_CONTROL_STARTPROCESS, MF_GRAYED);
        pMenu->EnableMenuItem(ID_CONTROL_STOPPROCESS, MF_GRAYED);
        pMenu->EnableMenuItem(ID_CONTROL_CHANGEPROCESSOPERATIONMODE, MF_GRAYED);
        pMenu->EnableMenuItem(ID_CONTROL_SETPARAM, MF_GRAYED);

        m_processList.freeze();
        std::string currStatus;        
        POSITION pos = m_processList.GetFirstSelectedItemPosition();

        while (pos != NULL)
        {
            int itemNum = m_processList.GetNextSelectedItem(pos);
            currStatus = m_processList.getStatus(itemNum);

            if ( (currStatus.compare(AppServiceConsts::RUNNING_CONTROL) == 0) ||
                 (currStatus.compare(AppServiceConsts::RUNNING_MONITOR) == 0) ||
                 (currStatus.compare(AppServiceConsts::GOING_TO_CONTROL) == 0) ||
                 (currStatus.compare(AppServiceConsts::GOING_TO_MONITOR) == 0) ||
                 (currStatus.compare(AppServiceConsts::RUNNING) == 0) )
            {
                m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_STOP, m_realPModel->m_isStopProcessAllowed);

                pMenu->EnableMenuItem(ID_CONTROL_STOPPROCESS, m_realPModel->m_isStopProcessAllowed ? MF_ENABLED : MF_GRAYED);

				TAUnicodeString tmpUniStr = m_processList.GetItemText(itemNum,0);
                std::string entityDescription = tmpUniStr.toMultiByteStdString();

                bool isManagedProcess = m_realPModel->isManagedProcessFromDescription(entityDescription);
                bool isServiceProcess = m_realPModel->isServiceProcessFromDescription(entityDescription);
				bool isAutoFailBack = m_realPModel->isAutoFailBackFromDescription(entityDescription);

                // Can only set params on managed processes
                if( isManagedProcess )
                {
                    pMenu->EnableMenuItem(ID_CONTROL_SETPARAM, m_realPModel->m_isSetProcessParamAllowed ? MF_ENABLED : MF_GRAYED);
                    
                    // Can only change process operation mode on processes that are managed and are not services
                    if (!(isServiceProcess||isAutoFailBack))
                    {
						// TD17104
						if ((currStatus.compare(AppServiceConsts::RUNNING_CONTROL) == 0) ||
							(currStatus.compare(AppServiceConsts::RUNNING_MONITOR) == 0) ||
							(currStatus.compare(AppServiceConsts::RUNNING) == 0) )
						{
							pMenu->EnableMenuItem(ID_CONTROL_CHANGEPROCESSOPERATIONMODE, m_realPModel->m_isSetOperatingModeAllowed ? MF_ENABLED : MF_GRAYED);
						}
                    }
                }
            }
            else if (currStatus.compare(AppServiceConsts::STARTUP) == 0)
            {
                m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_STOP, m_realPModel->m_isStopProcessAllowed);

                pMenu->EnableMenuItem(ID_CONTROL_STOPPROCESS, m_realPModel->m_isStopProcessAllowed ? MF_ENABLED : MF_GRAYED);
            }
            else if (currStatus.compare(AppServiceConsts::NOT_RUNNING) == 0)
            {
                m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_STOP, m_realPModel->m_isStopProcessAllowed);

                pMenu->EnableMenuItem(ID_CONTROL_STOPPROCESS, m_realPModel->m_isStopProcessAllowed ? MF_ENABLED : MF_GRAYED);            
            }
            else if (currStatus.compare(AppServiceConsts::NOT_STARTED) == 0)
            {
                m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_STOP, m_realPModel->m_isStopProcessAllowed);

                pMenu->EnableMenuItem(ID_CONTROL_STOPPROCESS, m_realPModel->m_isStopProcessAllowed ? MF_ENABLED : MF_GRAYED);            
            }
            else if (currStatus.compare(AppServiceConsts::STOPPED) == 0)
            {
                m_toolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_START, m_realPModel->m_isStartProcessAllowed);

                pMenu->EnableMenuItem(ID_CONTROL_STARTPROCESS, m_realPModel->m_isStartProcessAllowed ? MF_ENABLED : MF_GRAYED);
            }
        }

        m_processList.unfreeze();
	    
		if (pResult != 0)
		{
			*pResult = 0;
		}
    }


    std::vector<std::string> SystemManagerDlg::getSelectedProcesses()
    {
        std::vector<std::string> processes;
        POSITION pos = m_processList.GetFirstSelectedItemPosition();
        while (pos != NULL)
        {
            int itemNum = m_processList.GetNextSelectedItem(pos);
            processes.push_back(m_processList.getProcessName(itemNum));
        }
        return processes;
    }

    std::vector<std::string> SystemManagerDlg::getSelectedProcessesWithState(const std::vector<std::string>& states,
                                                                             bool ignoreServices /*=false*/,
                                                                             bool ignoreNonManaged /*=false*/)
    {
        std::vector<std::string> processes;

        // Iterate through each selected process
        POSITION pos = m_processList.GetFirstSelectedItemPosition();
        while (pos != NULL)
        {
            int itemNum = m_processList.GetNextSelectedItem(pos);
            std::string processState = m_processList.getStatus(itemNum);

			TAUnicodeString tmpUniStr = m_processList.GetItemText(itemNum,0);
            std::string entityDescription = tmpUniStr.toMultiByteStdString();
            
			bool isManagedProcess = m_realPModel->isManagedProcessFromDescription(entityDescription);
            bool isServiceProcess = m_realPModel->isServiceProcessFromDescription(entityDescription);
			bool isAutoFailBack = m_realPModel->isAutoFailBackFromDescription(entityDescription);
            if ((ignoreServices && isServiceProcess)||(ignoreServices && isAutoFailBack))
            {
                // Ignore. No need to check states
                continue;
            }
            else if (ignoreNonManaged && !isManagedProcess)
            {
                //Ignore. No need to check states
                continue;
            }
			
            // Now check if the state matches any of those passed in
            std::vector<std::string>::const_iterator statesToCheckFor = states.begin();
            while(statesToCheckFor != states.end())
            {
                if (processState == *statesToCheckFor)
                {
                    processes.push_back(m_processList.getProcessName(itemNum));
                    break;
                }
                ++statesToCheckFor;
            }
        }
        return processes;
    }

    void SystemManagerDlg::checkPermissions()
    {
		FUNCTION_ENTRY("checkPermissions");
        std::vector<UINT> vtErrorIdList;

        bool errorOccurred =  m_realPModel->checkPermissions(vtErrorIdList);

        // If something has gone wrong, let the user know and disable everything.
        if (errorOccurred)
        {
            TAUnicodeString totalErrorDesc;
            TAUnicodeString oneErrorDesc;

            //functionList.LoadString(IDS_ALL_FUNCTIONALITY);
            TA_Base_Bus::TransActiveMessage userMsg;
            for (std::vector<UINT>::iterator iter = vtErrorIdList.begin();
                iter != vtErrorIdList.end(); ++iter)
            {
                oneErrorDesc.LoadString(*iter);

                totalErrorDesc.str().Insert( totalErrorDesc.GetLength(), _T("\r\n"));
                totalErrorDesc.str().Insert(totalErrorDesc.GetLength(), oneErrorDesc.c_str());
            }
            userMsg << totalErrorDesc.str().GetBufferSetLength(totalErrorDesc.GetLength());
            UINT selectedButton = userMsg.showMsgBox(IDS_UI_020002);

        }


		// yanrong: privilege maybe changed, so tell main dialog to update display (e.g enable or not start/stop button).
		getNewData();

		FUNCTION_EXIT;
    }
	
    void SystemManagerDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
    {
	    AbstractTransActiveDialog::OnActivate(nState, pWndOther, bMinimized);
	    
        if( (bMinimized == FALSE) && 
            ( (nState == WA_ACTIVE) || (nState == WA_CLICKACTIVE) )) 
        {
            // This is when the window is restored (i.e no longer minimised)
            // Position precisely the child windows
            RECT ClientRect;
            GetClientRect(&ClientRect);
            resizeChildWindows(ClientRect.right, ClientRect.bottom);

            // Draw the Toolbar
            RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
        }
    }

	void SystemManagerDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<SystemManagerDlgModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");

        m_pModel->attachView(this, PMODEL_DATA_CHANGE_SESSION_ID);
        m_pModel->attachView(this, DOMAIN_DATA_CHANGE_NEW_PROCESS_DATA);

	}

	void SystemManagerDlg::initUIControl()
	{
		// Set the icon for this dialog.  The framework does this automatically
		// when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);         // Set big icon
		SetIcon(m_hIcon, FALSE);        // Set small icon

		m_toolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | 
			WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | 
			CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_toolBar.LoadToolBar(IDR_TOOLBAR1);

		// Position precisely the child windows
		RECT ClientRect;
		GetClientRect(&ClientRect);
		resizeChildWindows(ClientRect.right, ClientRect.bottom);
		initToolbar();

		// Draw the Toolbar
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

		// Initialise menu and toolbar items
		// there is no user logged in yet, so setup the menu accordingly
		setLoggedInMode(false);

        TAAppendLangMenu( IDR_MAINMENU, GetMenu() );
        TATranslateMenu( IDR_MAINMENU, GetMenu() );
        DrawMenuBar();
	}

    void SystemManagerDlg::getNewData()
    {
		PostMessage(WM_NEW_PROCESS_DATA, 0, 0);
	}

    std::string SystemManagerDlg::getMyPModelName()
    {
        return PMODEL_SYSTEM_MANAGER_DLG;
    }


    LRESULT SystemManagerDlg::OnSetLanguage(WPARAM wParam, LPARAM lParam)
    {
        TATranslateMenu( IDR_MAINMENU, GetMenu() );
        DrawMenuBar();
        TATranslateToolBar(IDR_TOOLBAR1, &m_toolBar );

        return AbstractTransActiveDialog::OnSetLanguage( wParam, lParam );
    }

} // namespace TA_Base_App
