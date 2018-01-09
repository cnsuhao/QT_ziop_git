/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemManagerDlg.h $
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

#ifndef SYSTEM_MANAGER_DLG_INCLUDED
#define SYSTEM_MANAGER_DLG_INCLUDED

//#include "ProcessListCtrl.h"
//#include "StartProcessDlg.h"
//#include "OptionsDlg.h"
//#include "SystemControllerDlg.h"
//#include "bus/security/rights_library/src/RightsLibrary.h"
#include "GeneratedFiles/ui_SystemManagerDlg.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
//#include "core/utilities/src/RunParams.h"
#include <memory>
#include <string>
#include <vector>


namespace TA_Base_App
{

    class SystemManagerDlgModel;

    class SystemManagerDlg : public TA_Base_Bus::AbstractTransActiveDialog
    {
        Q_OBJECT

    public:

        SystemManagerDlg( TA_Base_Bus::ITransActiveWinApp* app );
        ~SystemManagerDlg(){}
        
        //CStatic    m_menuLine;
        //ProcessListCtrl    m_processList;
        //CString    m_statusString;

        void getNewData();

    protected:
    
        //interface virtual method
        virtual std::string getMyPModelName();
        void setupPModel();

        virtual void initUIControl();

        /**
          * logout
          * 
          * Terminates the current System Manager session.
          */
        void logout();

        /**
          * setupMenu
          * 
          * Enables/disables menu items based on whether an operator is logged in or out.
          *
          * @param loggedIn  True if an operator is logged in, and false otherwise.
          */
        void setupMenu(bool loggedIn);

        /**
          * setupToolbar
          * 
          * Enables/Disables toolbar buttons based on whether an operator is logged in 
          * or out.
          *
          * @param loggedIn  True if an operator is logged in, and false otherwise.
          */
        void setupToolbar(bool loggedIn);

        /**
          * initToolbar
          * 
          * Sets the text for each toolbar button.
          *
          */
        void initToolbar();

        /**
          * setLoggedInMode
          * 
          * Sets the AdminClient mode (Logged in / Logged out) and enables or disables
          * the action buttons depending on that state.
          *
          * @param loggedIn  True if an operator is logged in, and false otherwise.
          */
        void setLoggedInMode(bool loggedIn);
    
        /**
          * resizeChildWindows
          * 
          * Positions and resizes the child windows (process list control, status bar)
          * according to the size of the main client.     
          *
          * @param width   The new width the client area.
          * @param height  The new height the client area.
          */
        void resizeChildWindows(int width, int height);

        /** 
          * onShutdownSystemController
          *
          * Message handler for the WM_SHUTDOWN_SYSTEM_CONTROLLER message sent
          * when a request to shutdown a System Controller is made.
          *
          */
        void onShutdownSystemController(WPARAM wparam, LPARAM lparam);

        /**
          * onNewProcessData
          *
          * Message handler for the WM_NEW_PROCESS_DATA message sent when there
          * is new process data.
          */
        void onNewProcessData(WPARAM wparam, LPARAM lparam);

        /**
          * getSelectedProcesses
          *
          * Returns the name of all processes that have been selected in the dialog
          * by the operator.
          */
        std::vector<std::string> getSelectedProcesses();

        /**
          * getSelectedProcessesWithState
          *
          * Returns the name of all processes that have been selected in the dialog
          * by the operator and have the specified state. It also checks the managed and
          * services criteria.
          *
          * @param state  The states of the processes to return.
          * @param ignoreServices This indicates if service process should be included or not.
          *                       True indicates services will be excluded. Default is to include them
          * @param ignoreNonManaged This indicates if non-managed process should be included or not.
          *                         True indicates non-managed processes will be excluded. Default
          *                         is to include them.
          */
        std::vector<std::string> getSelectedProcessesWithState(const std::vector<std::string>& state,
                                                               bool ignoreServices = false,
                                                               bool ignoreNonManaged = false);

        /**
          * checkPermissions
          *
          * Sets the m_is...Allowed boolean member variables by checking the available
          * rights from the rights library.
          */
        void checkPermissions();

private:

        HICON m_hIcon;

        void OnLogin();
        void OnLogout();
        void OnOK() {};
        void OnStartProcess();
        void OnDebugLevel();
        void OnChangeProcessMode();
        void OnOptions();
        void OnStopProcess();
        void OnShutdownMonitor();
        void OnRefresh();
        //void OnContextMenu(CWnd* pWnd, CPoint point);
        void OnSize(UINT nType, int cx, int cy);
        void OnSizing(UINT fwSide, LPRECT pRect);
        void OnAbout();
        void OnButtonLogin();
        void OnButtonLogout();
        void OnButtonStartProcess();
        void OnButtonStopProcess();
        void OnButtonRefresh();
        void onItemchangedMainProcesslist(NMHDR* pNMHDR, LRESULT* pResult);
        void OnClose();
        //void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

        virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);

        /**
          * A pointer to the child window created by this class.
          */
        //CDialog* m_currentChildWindow;

        /**
          * A pointer to the login dialog.
          */
        //SystemControllerDlg* m_sysControllerDialogBox;
    
        /**
          * The toolbar.
          */
        //CToolBar m_toolBar;

private:

        SystemManagerDlgModel* m_realPModel;
        Ui_SystemManagerDlg m_ui;
    };

}

#endif
