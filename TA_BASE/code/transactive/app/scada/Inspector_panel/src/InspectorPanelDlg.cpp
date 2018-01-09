/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/InspectorPanelDlg.cpp $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class provides the main dialog used to display the data point information.  
  * It has a tab control with 4 associated dialogs (1 per tab).   
  */

#include "app/scada/Inspector_Panel/src/stdafx.h"

#include <sstream>
#include "ace/OS.h"

#include "app/scada/Inspector_Panel/src/InspectorPanel.h"
#include "app/scada/Inspector_Panel/src/resource.h"
#include "app/scada/Inspector_Panel/src/InspectorPanelDlg.h"


#include "bus/application_types/src/apptypes.h"

#include "bus/generic_gui_pmod/src/AppLauncher.h"

#include "bus/user_settings/src/SettingsMgr.h"


#include "core/data_access_interface/src/ResourceAccessFactory.h"
#include "core/data_access_interface/src/IResource.h"

#include "core/exceptions/src/ApplicationException.h"


#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using TA_Base_Core::RunParams;
using TA_Base_Core::DebugUtil;
using TA_Base_Bus::SettingsMgr;
using TA_Base_Bus::AbstractTransActiveDialog;
using namespace TA_Base_App;

namespace TA_Base_App
{
	static const std::string  INFORMATION_PAGE         = "INFORMATION";
	static const std::string  CONTROL_PAGE             = "CONTROL";
    static const std::string  LIMITS_PAGE			   = "LIMITS";
	static const std::string  NOTES_PAGE               = "NOTES";
	static const std::string  TAG_PAGE                 = "TAG";
	static const std::string  INHIBIT_PAGE			   = "INHIBIT";
	static const std::string  OVERRIDE_PAGE			   = "OVERRIDE";
    static const std::string  METER_PAGE			   = "METER";
	static const std::string  DIRECT_CONTROL_PAGE      = "DIRECTCONTROL";

	/////////////////////////////////////////////////////////////////////////////
	// CInspectorPanelDlg dialog

	//
	// Constructor
	//
	CInspectorPanelDlg::CInspectorPanelDlg(TA_Base_Bus::ITransActiveWinApp * app)
	: TA_Base_Bus::AbstractTransActiveDialog(app, CInspectorPanelDlg::IDD, NULL), 
      m_resourceId(0), 
	  //m_equipment (NULL), 
      //m_rightsMgr (NULL), 
      m_heightDifference (0), 
	  m_isInitialTab (false),
      m_isInDirectControl(false),
	  m_realPModel(NULL),
	  m_directControlDlg(app)
	{
		FUNCTION_ENTRY ("CInspectorPanelDlg()");
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

		m_previousTab = NoTab;
		m_requestedTab = ControlTab;
		m_requestedPage = CONTROL_PAGE;
	
		FUNCTION_EXIT;
	}

	//
	// Destructor
	//
	CInspectorPanelDlg::~CInspectorPanelDlg()
	{
		FUNCTION_ENTRY ("~CInspectorPanelDlg()");
		exit(0);
		FUNCTION_EXIT;
	}

	//
	// DoDataExchange
	//
	void CInspectorPanelDlg::DoDataExchange(CDataExchange* pDX)
	{
		FUNCTION_ENTRY ("DoDataExchange()");

		AbstractTransActiveDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CInspectorPanelDlg)
		DDX_Control(pDX, IDC_STATUS, m_statusBox);
		DDX_Control(pDX, IDC_MAINTAB, m_MainTab);
		//}}AFX_DATA_MAP

		FUNCTION_EXIT;
	}

	std::string		CInspectorPanelDlg::getMyPModelName(void)
	{
		return PMODEL_INSPECTOR_PANEL_DLG;
	}

	void CInspectorPanelDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<InspectorPanelDlgModel*> (m_pModel);
		m_realPModel->attachView(this);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
	}

		
	void CInspectorPanelDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		//TA_Base_Bus::EventArgs* newEventArgs = const_cast<EventArgs*>(args);
		EquipmentEventArgs *realArgs = dynamic_cast<EquipmentEventArgs*> (args);
		TA_ASSERT(realArgs != NULL,"Invalid args");

		if (_stricmp(updateType, DOMAIN_DATA_CHANGE_INIT_STATUS) == 0)
		{
			if(realArgs->getEquipmentInitStatus() == INIT_STEP2_CREATE_DATANODE_PROXY)
			{
				this->updateView();
			}
			else if(realArgs->getEquipmentInitStatus() == INIT_STEP3_LOAD_DATAPOINTS)
			{
				this->updateMainDialog();
				std::string screenName = m_realPModel->getRequestScreen();
				if(screenName.size() > 0 )
				{
					this->setPage(screenName);
				}
			}

			//std::string *statusString = new std::string(this->getInitStatusString(realArgs->getEquipmentInitStatus()));
			TAUnicodeString *statusString = new TAUnicodeString();
			*statusString = this->getInitStatusString(realArgs->getEquipmentInitStatus());
			AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, (WPARAM)statusString, 0);	
		}
	}

	//
	// setPage
	//
	void CInspectorPanelDlg::setPage(std::string pageName)
	{
		FUNCTION_ENTRY ("setPage()");

		m_requestedPage = pageName;
		m_isInDirectControl = false;

		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "*** Requested Page = %s", m_requestedPage.c_str() );

		// For direct control page
        if (pageName==DIRECT_CONTROL_PAGE)
		{
            // Resize direct control dialog to the same as the mainTab
            CRect mainTabDimensions;
	        m_MainTab.GetWindowRect(mainTabDimensions);
	        this->ScreenToClient(mainTabDimensions);
            m_directControlDlg.MoveWindow(mainTabDimensions);

            m_MainTab.ShowWindow(SW_HIDE);
            m_directControlDlg.ShowWindow(SW_SHOW);

            m_isInDirectControl = true;

			FUNCTION_EXIT;
            return;
		}

        // Normal tabbed window 
        m_directControlDlg.ShowWindow(SW_HIDE);
        m_MainTab.ShowWindow(SW_SHOW);
		
		ETabNumbers newTab = m_previousTab;

		// Find to the required page
        if (pageName==INFORMATION_PAGE)
		{
			newTab = InformationTab;			
		}
		else if (pageName==CONTROL_PAGE)
		{
			newTab = ControlTab;
		}
		else if (pageName==LIMITS_PAGE)
		{
			newTab = LimitsTab;
		}
		else if (pageName==TAG_PAGE)
		{
			newTab = TagTab;
		}
		else if (pageName==NOTES_PAGE)
		{
			newTab = NotesTab;
		}
		else if (pageName==INHIBIT_PAGE)
		{
			newTab = InhibitTab;
		}
		else if (pageName==OVERRIDE_PAGE)
		{
			newTab = OverrideTab;
		}
		else if (pageName==METER_PAGE)
		{
			newTab = MeterTab;
		}

		if (m_realPModel != NULL)
		{
			// if all data points belongs to the equipment have been properly configured
			if ( true == m_realPModel->haveAllDataPointsConfigured() )
			{
				// if different tab selected
				if (newTab != m_previousTab)
				{
					// only proceed if previous tab indicates it has no unsaved data
					// and OK to change to different tab
					if ( ( false == m_MainTab.isUnsavedChangeOnTab ( m_previousTab ) ) || 
						 ( true == m_MainTab.isOkToChangeTab ( m_previousTab ) ) )
					{
						// change main tab from previous tab to the selected tab
						m_MainTab.SetCurSel(newTab);

						// save selected tab for further reference
						m_previousTab = newTab;					
					}
					m_requestedTab = m_previousTab;
				}
			}
			else
			{
				m_requestedTab = newTab;
			}
		}

		FUNCTION_EXIT;
	}

	
	//
	// updateMainDialog
	//
	void CInspectorPanelDlg::updateMainDialog()
	{
		
		
		setTitleInMainThread(m_realPModel->getTitle());

		// For normal tabbed window
        if (!m_isInDirectControl)
        {
            setEquipmentLabelInMainThread(m_realPModel->getLabel().c_str());
        }

		// Check resizing
		//resizeIfRequired();
	}

	//
	// onRunParamChange
	//
	

	
	/////////////////////////////////////////////////////////////////////////////
	// CInspectorPanelDlg message handlers

	BEGIN_MESSAGE_MAP(CInspectorPanelDlg, TA_Base_Bus::AbstractTransActiveDialog)
	//{{AFX_MSG_MAP(CInspectorPanelDlg)
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
		ON_NOTIFY(NM_CLICK, IDC_MAINTAB, OnClickMaintab)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
//		ON_MESSAGE (LOAD_DATAPOINTS_MESSAGE, OnLoadDataPoints)
        ON_MESSAGE (UPDATE_LABEL_IN_MAIN_THREAD_MESSAGE, OnUpdateLabelInMainThread)
//        ON_MESSAGE (PROCESS_RUNPARAM_IN_MAIN_THREAD_MESSAGE, OnProcessRunParamInMainThread)
        ON_MESSAGE (UPDATE_STATUS_WINDOW_MESSAGE, OnUpdateStatusWindow)
		ON_MESSAGE (RESIZE_WINDOW_MESSAGE,OnResizeWindow)
		ON_WM_DESTROY()
		ON_BN_CLICKED(ID_HELP_IP, OnHelpButton) //TD15124 azenitha
	END_MESSAGE_MAP()

	//TD15124 azenitha++
	void CInspectorPanelDlg::OnHelpButton() 
	{
		m_realPModel->displayHelp();
	}
	//TD15124 ++azenitha


	void CInspectorPanelDlg::init()
	{
        // Initialise the DirectControlDialog
		m_directControlDlg.Create(IDD_CONFIRM_DIRECT_CONTROL_DIALOG, this);
        m_directControlDlg.resetDialog();
		
		//m_realPModel->init();		
	}
	//
	// OnInitDialog
	//
	void CInspectorPanelDlg::initUIControl()
	{
		FUNCTION_ENTRY ("OnInitDialog()");

		CRect windowSize;
		GetWindowRect(&windowSize);

 		TA_Base_Bus::ViewResizeProperty properties;
 		properties.canMaximise = false;
 		properties.maxHeight = -1;
 		properties.maxWidth = -1;
 		properties.minHeight = windowSize.bottom - windowSize.top;
 		properties.minWidth = windowSize.right - windowSize.left;
		m_resizeViewHelper->setViewResizeProperty(properties);

		//AbstractTransActiveDialog::OnInitDialog();

		SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		this->init();

		FUNCTION_EXIT;
	}

	void CInspectorPanelDlg::updateView()
	{
		//this->updateMainDialog();
	}

	//
	// OnPaint
	//
	void CInspectorPanelDlg::OnPaint() 
	{
		//FUNCTION_ENTRY ("OnPaint()");

		if (IsIconic())
		{
			CPaintDC dc(this); // device context for painting

			SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

			// Center icon in client rectangle
			int cxIcon = GetSystemMetrics(SM_CXICON);
			int cyIcon = GetSystemMetrics(SM_CYICON);
			CRect rect;
			GetClientRect(&rect);
			int x = (rect.Width() - cxIcon + 1) / 2;
			int y = (rect.Height() - cyIcon + 1) / 2;

			// Draw the icon
			dc.DrawIcon(x, y, m_hIcon);
		}
		else
		{
			AbstractTransActiveDialog::OnPaint();
		}

		//FUNCTION_EXIT;
	}

	void CInspectorPanelDlg::OnDestroy()
    {
        FUNCTION_ENTRY( "OnDestroy" );

		/*if (m_equipment != NULL)
		{
			delete m_equipment;
			m_equipment = NULL;
		}
		*/

        CDialog::OnDestroy();       

        FUNCTION_EXIT;
   }
	//
	// OnQueryDragIcon
	//
	HCURSOR CInspectorPanelDlg::OnQueryDragIcon()
	{
		FUNCTION_ENTRY ("OnQueryDragIcon()");
		FUNCTION_EXIT;

		return (HCURSOR) m_hIcon;
	}

	//
	// OnCancel
	//
	void CInspectorPanelDlg::OnCancel() 
	{
		FUNCTION_ENTRY ("OnCancel()");

		/*if (m_equipment != NULL)
		{
			delete m_equipment;
			m_equipment = NULL;
		}*/

		//TD18095, jianghp, to fix the performance of showing manager application
		AbstractTransActiveDialog::DestroyWindow();

		FUNCTION_EXIT;
	}

	//
	// OnClickMaintab
	//
	void CInspectorPanelDlg::OnClickMaintab(NMHDR* pNMHDR, LRESULT* pResult) 
	{
		FUNCTION_ENTRY ("OnClickMaintab()");

		*pResult = 0;

		// Check to see if we are allowed to change tabs
		ETabNumbers iTab = (ETabNumbers)m_MainTab.GetCurSel();

		if (m_MainTab.isUnsavedChangeOnTab(m_previousTab))
		{
			if (!m_MainTab.isOkToChangeTab(m_previousTab))
			{
				// Don't change tabs
				m_MainTab.SetCurSel(m_previousTab);
				return;
			}
		}

		m_previousTab = iTab;

		FUNCTION_EXIT;
	}

	//
	// resizeIfRequired
	//
	void CInspectorPanelDlg::resizeIfRequired()
	{
		FUNCTION_ENTRY ("resizeIfRequired()");

		if (m_heightDifference == 0)
		{
			CRect windowRect;
			GetWindowRect(&windowRect);

			CRect tabRect;
			m_MainTab.GetWindowRect(&tabRect);

			m_heightDifference = windowRect.Height()-tabRect.Height();
		}
		
		int yOffset = m_MainTab.resize();

		if (yOffset != 0)
		{
			// Move status group box
			CWnd *window = GetDlgItem(IDC_STATUS_GROUP_BOX);
			CRect statusGrpBoxRect;
			window->GetWindowRect(&statusGrpBoxRect);
			ScreenToClient(&statusGrpBoxRect);
			statusGrpBoxRect.OffsetRect(0, yOffset);
			window->SetWindowPos(&wndNoTopMost, statusGrpBoxRect.left, statusGrpBoxRect.top, 
												0, 0, SWP_NOSIZE|SWP_NOZORDER);
		}
	
		if (yOffset != 0)
		{
			// Move status box
			CWnd *window = GetDlgItem(IDC_STATUS);
			CRect statusBoxRect;
			window->GetWindowRect(&statusBoxRect);
			ScreenToClient(&statusBoxRect);
			statusBoxRect.OffsetRect(0, yOffset);
			window->SetWindowPos(&wndNoTopMost, statusBoxRect.left, statusBoxRect.top, 
												0, 0, SWP_NOSIZE|SWP_NOZORDER);
			m_statusBox.Invalidate(); 	
		}

		if (yOffset != 0)
		{
			// Move cancel button
			CWnd *window = GetDlgItem(IDCANCEL);
			CRect cancelRect;
			window->GetWindowRect(&cancelRect);
			ScreenToClient(&cancelRect);
			cancelRect.OffsetRect(0, yOffset);
			window->SetWindowPos(&wndNoTopMost, cancelRect.left, cancelRect.top, 
												0, 0, SWP_NOSIZE|SWP_NOZORDER);
			GetDlgItem(IDCANCEL)->Invalidate(); 
		}

		if (yOffset != 0)
		{
			// Move help button
			CWnd *window = GetDlgItem(ID_HELP_IP); //TD15124 azenitha
			CRect helpRect;
			window->GetWindowRect(&helpRect);
			ScreenToClient(&helpRect);
			helpRect.OffsetRect(0, yOffset);
			window->SetWindowPos(&wndNoTopMost, helpRect.left, helpRect.top, 
												0, 0, SWP_NOSIZE|SWP_NOZORDER);
			GetDlgItem(ID_HELP_IP)->Invalidate(); //TD15124 azenitha
		}

		// Resize window
		CRect windowRect, tabRect;
		GetWindowRect(&windowRect);		
		m_MainTab.GetWindowRect(&tabRect);

		// Stay on top and make sure it ll resize eventually
		SetWindowPos(&wndTopMost, 0, 0, windowRect.Width(), tabRect.Height()+m_heightDifference, SWP_NOMOVE|SWP_SHOWWINDOW);		
		//Invalidate();
	
		FUNCTION_EXIT;
	}

    void CInspectorPanelDlg::setEquipmentLabelInMainThread(const std::string& label)
    {
        CWnd* window = GetDlgItem(IDC_EQUIPMENT_DESCRIPTION);
        TAUnicodeString* temporalString = new TAUnicodeString(CONVERT_FROM_STD(label));
        PostMessage(
            UPDATE_LABEL_IN_MAIN_THREAD_MESSAGE,
            reinterpret_cast<UINT>(window),
            reinterpret_cast<LONG>(temporalString));
    }

    void CInspectorPanelDlg::setTitleInMainThread(const std::string& label)
    {
		TAUnicodeString* pTitle = new TAUnicodeString(TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_APP_TITLE, CONVERT_FROM_STD(label)));
        PostMessage(
            UPDATE_LABEL_IN_MAIN_THREAD_MESSAGE,
            reinterpret_cast<UINT>(this),
            reinterpret_cast<LONG>(pTitle));
    }

    LRESULT CInspectorPanelDlg::OnUpdateLabelInMainThread(UINT wParam, LONG lParam)
    {
        TAUnicodeString* pTitle = reinterpret_cast<TAUnicodeString*>(lParam);
        CWnd* desiredLabel = reinterpret_cast<CWnd*>(wParam);

        if (pTitle)
        {
            if (desiredLabel)
            {
				TAUnicodeString& description = *pTitle;

                int pos = description.Find ( _T("&"), 0 );
                if ( -1 != pos )
                {
                    description.str().Insert ( pos, _T("&") );
                }

                // Wrap the text around if it will not fit into the label (TD 11255)
                int charsWide = 0;
                SIZE sizeString;
                CRect clientRect;
                desiredLabel->GetClientRect( &clientRect);
                CDC* pdc = desiredLabel->GetDC();

                if (GetTextExtentExPoint( pdc->GetSafeHdc(), 
                    description.str(), description.GetLength(),
                    clientRect.Width(), &charsWide, NULL, &sizeString))
                {
                    if ( description.GetLength() > charsWide)
                    {
                        //  The string is too big to fit into the desiredLabel.
                        //  Default text wrapping will work if there is a space...

                        pos = description.Find ( _T(" "), 0 );
                        if ( ( -1 == pos ) || (pos > charsWide) )
                        {
                            // ... but instead of a space, insert a linebreak to be sure.
                            description.str().Insert ( charsWide, _T("\n") );
                        }
                    }
                    desiredLabel->ReleaseDC( pdc);
                }

                desiredLabel->SetWindowText ( description.str() );
            }
            delete pTitle;
        }
        return 0;
    }




	void CInspectorPanelDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
    {
        // Find out the coordinates of the current window
        WINDOWPLACEMENT place;
        GetWindowPlacement(&place);

        int screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        int numScreens = GetSystemMetrics(SM_CMONITORS);

        // Determine the screen we are currently on.
        int currentScreen = place.rcNormalPosition.left / (screenWidth / numScreens);

        // If we have changed screen, then attempt to retrieve the available working area.

        if (currentScreen != m_currentScreen)
        {
            m_currentScreen = currentScreen;

            // Get the available working area from the control station
            try
            {
                m_workingArea = TA_Base_Bus::AppLauncher::getInstance().getRect(
                    TA_Base_Bus::TA_GenericGui::SCREEN_ALL, 
                    TA_Base_Bus::TA_GenericGui::AREA_MAINNAVIGATOR);
            }
            catch(TA_Base_Core::ApplicationException& ex)
            {
                // Log the catch, and set the workingArea rect to 0.
                std::stringstream errorMessage;
                errorMessage << "An exception was caught while attempting to retrieve the schematic area of the current"
                    << "screen. X coord = " << place.rcNormalPosition.left << ". Error Message:";
                LOG_EXCEPTION_CATCH(SourceInfo,"ApplicationException",errorMessage.str().c_str());
                LOGMORE(SourceInfo,ex.what());

                // Set the workingArea rect to 0
                m_workingArea.SetRectEmpty();
            }
        }

        // The resized window, represented by lpwndpos, cannot fall outside the schematic rectangle, so reset 
        // variables as necessary
        if (!m_workingArea.IsRectEmpty())
        {
            // Set the width
            if (lpwndpos->cx > m_workingArea.Width())
            {
                lpwndpos->cx = m_workingArea.Width();
            }

            // Set the height
            if (lpwndpos->cy > m_workingArea.Height())
            {
                lpwndpos->cy = m_workingArea.Height();
            }

            // Set the x-coordinate
            if (lpwndpos->x < m_workingArea.left)
            {
                lpwndpos->x = m_workingArea.left;
            }
            else if (lpwndpos->x + lpwndpos->cx > m_workingArea.right)
            {
                lpwndpos->x = m_workingArea.right - lpwndpos->cx;
            }

            // Set the y-coordinate
            if (lpwndpos->y < m_workingArea.top)
            {
                lpwndpos->y = m_workingArea.top;
            }
            else if (lpwndpos->y + lpwndpos->cy > m_workingArea.bottom)
            {
                lpwndpos->y = m_workingArea.bottom - lpwndpos->cy;
            }
        }

		//TD14564
		//set the window invalid and cause the window to be redrawn.
		//Invalidate();   
		ShowWindow(SW_SHOW);
    }


	// TD #11930
	// Handle the PostMessage message to update the status window text.
	// The status text should be in wParam, and the timestamp in lParam
    LRESULT CInspectorPanelDlg::OnUpdateStatusWindow(UINT wParam, LONG lParam)
    {
        TAUnicodeString* statusText = reinterpret_cast<TAUnicodeString*>(wParam);

		if(statusText)
        {
			if(statusText->GetLength() > 0)
			{
				CInspectorPanelDlg* dlg = dynamic_cast<CInspectorPanelDlg*>(AfxGetApp()->m_pMainWnd);

				// use the timestamp if it's set, otherwise use the current time
				time_t timeT = ( lParam > 0 ) ? lParam : time (NULL);
				struct tm localTm;
				ACE_OS::localtime_r( &timeT, &localTm );
				TAUnicodeString month;
				switch(localTm.tm_mon)
				{
					case 0 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0151);//_T("JAN");
							 break;
					case 1 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0152);//_T("FEB");
							 break;
					case 2 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0153);//_T("MAR");
							 break;
					case 3 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0154);//_T("APR");
							 break;
					case 4 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0155);//_T("MAY");
							 break;
					case 5 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0156);//_T("JUN");
							 break;
					case 6 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0157);//_T("JUL");
							 break;
					case 7 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0158);//_T("AUG");
							 break;
					case 8 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0159);//_T("SEP");
							 break;
					case 9 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0160);//_T("OCT");
							 break;
					case 10 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0161);//_T("NOV");
							 break;
					case 11 : month = TALoadString(STRING_ID_INSPECTOR_PANEL_0162);//_T("DEC");
							 break;
				}

				TAUnicodeString timeString;
				timeString.str().Format(_T("%.2d-%s-%.4d %.2d:%.2d:%.2d "),localTm.tm_mday, month.str(), localTm.tm_year+1900, localTm.tm_hour,localTm.tm_min, localTm.tm_sec);
				
				timeString += *statusText; 

				dlg->m_statusBox.SetWindowText(timeString.str());
			}

            delete statusText;
        }
	
		return 0;
	}

	

	LRESULT CInspectorPanelDlg::OnResizeWindow(UINT wParam, LONG lParam)
	{
		this->resizeIfRequired();
		return 0;
	}
	

	void CInspectorPanelDlg::OnClose() 
	{
		exit(0);
		AbstractTransActiveDialog::DestroyWindow();
	}

	void CInspectorPanelDlg::resetList()
	{
		m_MainTab.resetList();
	}


	CString	CInspectorPanelDlg::getInitStatusString(EquipmentInitStatus status)
	{
		switch(status)
		{
		case	INIT_STEP0_BEGIN:
			//return "Step0:Begin";
			return  TALoadString(STRING_ID_INSPECTOR_PANEL_0140).str();
		
		case	INIT_STEP1_LOAD_DATANODE:
			//return "Step1:Load DataNode Data";
			return TALoadString(STRING_ID_INSPECTOR_PANEL_0141).str();

		case	INIT_STEP1_LOAD_DATANODE_ERROR:
			//return "Step1:Load DataNode Data Error";
			return TALoadString(STRING_ID_INSPECTOR_PANEL_0142).str();

		case	INIT_STEP2_CREATE_DATANODE_PROXY:
			//return "Step2:Create DataNode Proxy";
			return TALoadString(STRING_ID_INSPECTOR_PANEL_0143).str();

		case	INIT_STEP2_CREATE_DATANODE_PROXY_ERROR:
			//return "Step2:Create DataNode Proxy Error";
			return TALoadString(STRING_ID_INSPECTOR_PANEL_0144).str();

		case	INIT_STEP3_LOAD_DATAPOINTS:
			//return "Step3: Load DataPoint Data";
			return TALoadString(STRING_ID_INSPECTOR_PANEL_0145).str();

		case	INIT_STEP3_LOAD_DATAPOINTS_ERROR:
			//return "Step3: Load DatPoint Data Error";
			return TALoadString(STRING_ID_INSPECTOR_PANEL_0146).str();

		case	INIT_STEP4_CREATE_DATAPOINT_PROXY:
			//return "Step4: Create DataPoint Proxy";
			return TALoadString(STRING_ID_INSPECTOR_PANEL_0147).str();

		case	INIT_STEP4_CREATE_DATAPOINT_PROXY_ERROR:
			//return "Step4: Create DataPoint Proxy Error";
			return TALoadString(STRING_ID_INSPECTOR_PANEL_0148).str();

		case	INIT_STEP5_COMPLETE:
			//return "Step5: Init Complete!";
			return TALoadString(STRING_ID_INSPECTOR_PANEL_0149).str();

		default:
			//return "UnknownStatus";
			return TALoadString(STRING_ID_INSPECTOR_PANEL_0150).str();
		}
	}
}





