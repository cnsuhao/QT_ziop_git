/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/DutyManagerDlg.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This class represents the main Rihgts Manager dialog window.
  *
  */

#include "StdAfx.h"
#include "app/security/DutyManager/src/DutyManager.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerDlgModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
    const int DUTY_MGR_MINIMUM_WIDTH(600);
    const int DUTY_MGR_MINIMUM_HEIGHT(400);
	const int DUTY_MGR_INITIALIZE_TIMER = 1;
}

using TA_Base_Bus::DataCache;
using TA_Base_Bus::SessionCache;
using TA_Base_Bus::SessionInfoEx;
using TA_Base_Bus::SessionKey;
using TA_Base_Bus::SessionId;
using TA_Base_Bus::SessionUpdate;
using TA_Base_Bus::ProfileKey;
using TA_Base_Bus::ProfileKeyList;
//using TA_Base_Bus::TransActiveDialog;

using TA_Base_Core::DebugUtil;

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// DutyManagerDlg dialog

bool DutyManagerDlg::m_isMartixDataInited = false;
extern bool g_populated;


DutyManagerDlg::DutyManagerDlg(TA_Base_Bus::ITransActiveWinApp * app, CWnd* pParent /*=NULL*/):
		TA_Base_Bus::AbstractTransActiveDialog(app, DutyManagerDlg::IDD, pParent),
		//m_bridge(NULL),
		//m_dutyPropertySheet(NULL),
		m_regionDutyPropertyPage(NULL),
		m_subsystemDutyPropertyPage(NULL),
		m_otherSesSubDutyPropertyPage(NULL),
		m_shownStatus(true)		
{
    FUNCTION_ENTRY("DutyManagerDlg(TA_Base_Bus::IGUIAccess& genericGUICallback)");

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_DUTYMANAGER_ICON);

    TA_Base_Bus::ViewResizeProperty properties;
    properties.canMaximise = true;
    properties.maxWidth = -1;
    properties.maxHeight = -1;
    properties.minWidth = DUTY_MGR_MINIMUM_WIDTH;
    properties.minHeight = DUTY_MGR_MINIMUM_HEIGHT;
	m_resizeViewHelper->setViewResizeProperty( properties );


	//m_showOtherSession = m_realPModel->isShowOthereSessioin();
	//std::string showOtherSessionStr = TA_Base_Core::RunParams::getInstance().get(RPARAM_SHOWOTHERSES).c_str();
	//LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,"DutyManagerDlg::DutyManagerDlg(), showOtherSessionStr = %s",showOtherSessionStr.c_str());
	//if ( showOtherSessionStr.compare("OPEN") == 0 )
	//{
	//	m_showOtherSession = true;
	//}

    FUNCTION_EXIT;
}


DutyManagerDlg::~DutyManagerDlg()
{
    FUNCTION_ENTRY("~DutyManagerDlg()");

	//TA_Base_Core::MessageSubscriptionManager::getInstance().unsubscribeToMessages(this);
	m_subsystemDutyPropertyPage->DestroyWindow();
	delete m_subsystemDutyPropertyPage;
	m_subsystemDutyPropertyPage = NULL;

	m_regionDutyPropertyPage->DestroyWindow();
	delete m_regionDutyPropertyPage;
	m_regionDutyPropertyPage = NULL;

/*
		m_dutyPropertySheet.DestroyWindow();
		delete m_dutyPropertySheet;
		m_dutyPropertySheet = NULL;
		*/
	
	FUNCTION_EXIT;
}


void DutyManagerDlg::init()
{
	LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"[CL-21875] begin DutyManagerDlg::init");

	TA_ASSERT(this->m_hWnd != NULL, "the windows is not created yet!");
	// TD11122: Store the original title text.
    CString title;
    GetWindowText( title );	
	TAUtfString utfString(title);
	std::string strTitle = utfString.toMultiByteStdString( );
	m_realPModel->setTitle(strTitle);
	LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"DutyManagerDlg setTitle:%s",strTitle.c_str());

	m_showOtherSession = m_realPModel->isShowOthereSessioin();

	m_regionDutyPropertyPage	= new RegionDutyPropertyPage(m_pWinApp,0);
	m_subsystemDutyPropertyPage = new SubsystemDutyPropertyPage(m_pWinApp,0);
	if (m_showOtherSession)
	{
		m_otherSesSubDutyPropertyPage = new OtherSesSubDutyPropertyPage(m_pWinApp,0);
	}
	//m_regionDutyPropertyPage->Create(IDD_REGIONS_DLG);
	//m_subsystemDutyPropertyPage->Create(IDD_SUBSYSTEMS_DLG);
	// Initialise the tabs.
    m_dutyPropertySheet.AddPage(&(*m_regionDutyPropertyPage));
    m_dutyPropertySheet.AddPage(&(*m_subsystemDutyPropertyPage));
	//m_dutyPropertySheet = new DutyManagerPropertySheet;

	if (m_showOtherSession)
	{
		m_dutyPropertySheet.AddPage(&(*m_otherSesSubDutyPropertyPage));
	}
	LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"[CL-21875] in DutyManagerDlg::init, after AddPage OtherSesSubDutyPropertyPage");

	m_dutyPropertySheet.Create(this, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN);
	
    // Finally, position all the controls.
    //TD17082++
    positionControls();
    // positionControls();
	//SetTimer(DUTY_MGR_INITIALIZE_TIMER,0, NULL);	
	m_realPModel->loadData();
    //++TD17082
	LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugDebug,"[CL-21875] end DutyManagerDlg::init, after Settimer");
}

void DutyManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	AbstractTransActiveDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(DutyManagerDlg)
	//}}AFX_DATA_MAP
}




/////////////////////////////////////////////////////////////////////////////
// DutyManagerDlg message handlers

BEGIN_MESSAGE_MAP(DutyManagerDlg, AbstractTransActiveDialog)
	//{{AFX_MSG_MAP(DutyManagerDlg)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_HELP_DUTY_MGR,OnTransActiveHelp)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_TASETLANG, OnSetLanguage)
	ON_MESSAGE(WM_RGN_DUTY_UPDATE, OnRegionDutyUpdate)
	ON_MESSAGE(WM_SUB_DUTY_UPDATE, OnSubsystemDutyUpdate)
	ON_MESSAGE(WM_SESSEION_UPDATE, OnSessionUpdate)
	ON_MESSAGE(WM_POPULATE_DATA, OnPopulateData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL DutyManagerDlg::OnInitDialog()
//void DutyManagerDlg::initUIControl()
{
    FUNCTION_ENTRY("OnInitDialog()");

	AbstractTransActiveDialog::OnInitDialog();

	//TransActiveDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    // Initialise current session details.
    //TD17082 - must be performed after onInitdialog to improve launch performance
    //initialiseCurrentSession();
	CWaitCursor waitCursor;	 
	CStatic* statusCtrl = reinterpret_cast<CStatic*>(GetDlgItem(IDC_STATUS_STATIC));

	CString checkCStr	= TALoadString( STRING_ID_DUTY_MANAGER_0003 ).c_str();
	statusCtrl->SetWindowText(checkCStr);	

	CRect windowRect;
	GetWindowRect(windowRect);
	ScreenToClient(windowRect);
	
	CRect statusBarRect;
	statusCtrl->GetWindowRect(statusBarRect);
	ScreenToClient(statusBarRect);
	statusBarRect.left = windowRect.left + 5;
	statusBarRect.right = windowRect.right - 9;
	statusBarRect.bottom = windowRect.bottom - 5;
	statusBarRect.top = statusBarRect.bottom - 20;
	statusCtrl->MoveWindow(statusBarRect);

	statusCtrl->ShowWindow(SW_SHOW);
	waitCursor.Restore();	


	// +++++++++++++++++++++++++++++++++++++++++++init()
	//m_regionDutyPropertyPage =  new RegionDutyPropertyPage(m_pWinApp,0);
	//m_subsystemDutyPropertyPage = new SubsystemDutyPropertyPage(m_pWinApp,0);
	//if (m_showOtherSession)
	//{
	//	m_otherSesSubDutyPropertyPage = new OtherSesSubDutyPropertyPage(m_pWinApp,0);
	//}
	////m_regionDutyPropertyPage->Create(IDD_REGIONS_DLG);
	////m_subsystemDutyPropertyPage->Create(IDD_SUBSYSTEMS_DLG);
	//// Initialise the tabs.
	//m_dutyPropertySheet.AddPage(&(*m_regionDutyPropertyPage));
	//m_dutyPropertySheet.AddPage(&(*m_subsystemDutyPropertyPage));
	////m_dutyPropertySheet = new DutyManagerPropertySheet;

	//if (m_showOtherSession)
	//{
	//	m_dutyPropertySheet.AddPage(&(*m_otherSesSubDutyPropertyPage));
	//}
	//LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"[CL-21875] in DutyManagerDlg::init, after AddPage OtherSesSubDutyPropertyPage");

	//m_dutyPropertySheet.Create(this, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN);

	// +++++++++++++++++++++++++++++++++++++++++++init()

    FUNCTION_EXIT;	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void DutyManagerDlg::initUIControl()
{
	FUNCTION_ENTRY("initUIControl()");

	init();

	//onInitGenericGUICompleted();

	FUNCTION_EXIT;	
}


void DutyManagerDlg::onInitGenericGUICompleted()
{
	LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugDebug,"[CL-21875] begin DutyManagerGUI::onInitGenericGUICompleted");

	//TA_Base_Core::IEntityData* guiEntity = getTransActiveWinApp()->getApplicationController()->getEntity();
	//TA_Base_Core::DutyManagerEntityData* dmEntityData = dynamic_cast<TA_Base_Core::DutyManagerEntityData*>(guiEntity);
	//TA_ASSERT(guiEntity  != NULL, "the GUI entity should not be NULL! ");
	//// Store away Rihgts Manager parameters for future reference.
	//if (dmEntityData->useLocationLabel())
	//{
	//	TA_Base_Core::RunParams::getInstance().set(RPARAM_USELOCATIONLABEL, "enable"); 
	//}
	//if (!dmEntityData->getNormalDutyColour().empty())
	//{
	//	TA_Base_Core::RunParams::getInstance().set( RPARAM_NORMALDUTYCOLOUR, dmEntityData->getNormalDutyColour().c_str() );
	//}
	//if (!dmEntityData->getDelegatedDutyColour().empty())
	//{
	//	TA_Base_Core::RunParams::getInstance().set( RPARAM_DELEGATEDDUTYCOLOUR, dmEntityData->getDelegatedDutyColour().c_str() );
	//}
	//if (!dmEntityData->getDegradedDutyColour().empty())
	//{
	//	TA_Base_Core::RunParams::getInstance().set( RPARAM_DEGRADEDDUTYCOLOUR, dmEntityData->getDegradedDutyColour().c_str() );
	//}
	//if (!dmEntityData->getUnallocatedSubsystemColour().empty())
	//{
	//	TA_Base_Core::RunParams::getInstance().set( RPARAM_UNALLOCATEDSUBSYSTEMCOLOUR, dmEntityData->getUnallocatedSubsystemColour().c_str() );
	//}

	//CWnd *pWnd = getApplicationWnd();
	//pWnd->LockWindowUpdate();
	//DutyManagerDlg * pDlg = dynamic_cast<DutyManagerDlg * >(pWnd);
	//TA_ASSERT(pDlg != NULL, "the application window is NULL!");

	//LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugDebug,"[CL-21875] in DutyManagerGUI::onInitGenericGUICompleted, begin pDlg->init()");
	//pDlg->init();
	//LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugDebug,"[CL-21875] in DutyManagerGUI::onInitGenericGUICompleted, end pDlg->init()");

	//pDlg->UnlockWindowUpdate();
	//pDlg->UpdateWindow();
	//LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugDebug,"[CL-21875] end DutyManagerGUI::onInitGenericGUICompleted");
};

std::string DutyManagerDlg::getMyPModelName()
{
	 return PMODEL_DUTY_MANAGER_DLG;
}

void DutyManagerDlg::setupPModel()
{
	m_realPModel = dynamic_cast<DutyManagerDlgModel*> (m_pModel);

	//---------- add delete
	m_pModel->attachView(this, VIEW_DATA);
	m_pModel->attachView(this, VIEW_AND_PRESENTATION_DATA);
}

void DutyManagerDlg::OnTransActiveHelp()
{
	m_realPModel->OnTransActiveHelp();
	//TA_Base_Bus::HelpLauncher::getInstance().displayHelp(true);
	//m_realPModel->displayHelp(true);
}




void DutyManagerDlg::OnAppAbout()
{
    //TA_Base_Bus::HelpLauncher::getInstance().displayAboutBox();
	 m_realPModel->OnAppAbout(); 
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void DutyManagerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

        // TD11803: A call to SendMessage was made when the window did not exist, for now just make sure
        // it won't generate an assertion and log the fact so it can be debugged further.
        if ( ::IsWindow(m_hWnd) )
        {
            SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        }
        else
        {
            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Attempted to SendMessage when window does not exist." );
        }

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
		//TransActiveDialog::OnPaint();
		AbstractTransActiveDialog::OnPaint();
	}
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR DutyManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void DutyManagerDlg::OnSize(UINT nType, int cx, int cy) 
{
	//TransActiveDialog::OnSize(nType, cx, cy);
	CDialog::OnSize(nType, cx, cy);
	
    positionControls();
}


//TD17082
void DutyManagerDlg::positionControls()
{
	if (::IsWindow(m_dutyPropertySheet.GetSafeHwnd()))
    {
        CRect windowRect;
        GetWindowRect(windowRect);
        ScreenToClient(windowRect);

        CRect propSheetRect;
        m_dutyPropertySheet.GetWindowRect(propSheetRect);
        ScreenToClient(propSheetRect);
        propSheetRect.left = windowRect.left + 3;
        propSheetRect.top = windowRect.top + 35;
        propSheetRect.right = windowRect.right - 9;

        //TD17082 - check if status bar should be displayed or not
		if (m_shownStatus)
		{	
			CStatic* statusCtrl = reinterpret_cast<CStatic*>(GetDlgItem(IDC_STATUS_STATIC));
			CRect statusBarRect;
			statusCtrl->GetWindowRect(statusBarRect);
			ScreenToClient(statusBarRect);
			statusBarRect.left = windowRect.left + 5;
			statusBarRect.right = windowRect.right - 9;
			statusBarRect.bottom = windowRect.bottom - 5;
			statusBarRect.top = statusBarRect.bottom - 20;
			statusCtrl->MoveWindow(statusBarRect);
			propSheetRect.bottom = statusBarRect.top - 2;
		}
		else		
		{
			CStatic* statusCtrl = reinterpret_cast<CStatic*>(GetDlgItem(IDC_STATUS_STATIC));
			CRect statusBarRect;
			statusCtrl->GetWindowRect(statusBarRect);
			ScreenToClient(statusBarRect);

			statusBarRect.left = windowRect.left -10;
			statusBarRect.top = windowRect.top - 10;

			statusBarRect.right = statusBarRect.left + 5;
			statusBarRect.bottom = statusBarRect.top + 5;
			
			statusCtrl->MoveWindow(statusBarRect);

			propSheetRect.bottom = windowRect.bottom - 5;
			//propSheetRect.bottom = windowRect.bottom + 10;  // Not OK
			//propSheetRect.bottom = windowRect.bottom - 20;  // No effect
		}
        //++TD17082

        m_dutyPropertySheet.MoveWindow(propSheetRect);  // The prop sheet will size the pages
    }
}

void DutyManagerDlg::OnAppExit() 
{
	DestroyWindow();
}

void DutyManagerDlg::OnClose() 
{	
	DestroyWindow();	
}


SubsystemDutyPropertyPage& DutyManagerDlg::getSubsytemDutyPropertyPage()
{
	return *m_subsystemDutyPropertyPage;
}

OtherSesSubDutyPropertyPage& DutyManagerDlg::getOhterSesSubDutyPropertyPage()
{
	return *m_otherSesSubDutyPropertyPage;
}


void DutyManagerDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
{
	int typeCode = -1;
	if (NULL == args)
	{
		typeCode = -1;
	}
	else
	{
		typeCode = args->getTypeCode();
	}

	bool canDeleteArgs = false;

	if (_stricmp(updateType, PRESENTATION_DATA) == 0)
	{
	}
	else if (_stricmp(updateType, VIEW_AND_PRESENTATION_DATA) == 0)
	{
	}
	else // _stricmp(updateType, VIEW_DATA) == 0
	{
		switch(typeCode)
		{
		case DM_MESSAGE_BOX_INFO:
			break;
		case DM_MESSAGE_BOX_WARNING:
			break;
		case DM_MESSAGE_BOX_ERROR:
			break;
		case DM_UPDATE_DLG:
			break;
		case DM_MESSAGE_BOX_WARNING_AND_CLOSE:
			break;
		case DM_POPULATE_DATA:
			{
				PopulateDataEventArgs* pdEa = dynamic_cast<PopulateDataEventArgs*>(args);
				PostMessage(WM_POPULATE_DATA, pdEa->isPopulateStaticData(), pdEa->isPopulateDynamicData());
				canDeleteArgs = true;
			}			
			break;
		case DUTY_AGENT_RECOVER_UPDATE:
			{
				DutyManagerEventArgs* pdEa = dynamic_cast<DutyManagerEventArgs*>(args);
				PostMessage(WM_USER_RECEIVE_DUTY_AGENT_RECOVER_UPDATE, 0, pdEa->getLocationId());
				canDeleteArgs = true;
			}
			break;
		case DM_RBN_DUTY_CHANGED:
			{
				PostMessage(WM_RGN_DUTY_UPDATE, reinterpret_cast<WPARAM>(args), 0);
			}
			break;
		case DM_SUB_DUTY_CHANGED:
			{
				PostMessage(WM_SUB_DUTY_UPDATE, reinterpret_cast<WPARAM>(args), 0);
			}
			break;
		case DM_SESSION_CHANGED:
			{
				PostMessage(WM_SESSEION_UPDATE, reinterpret_cast<WPARAM>(args), 0);
			}
			break;
		default:
			LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"notifyUpdate no typecode:%d",typeCode);
			break;
		}
	}

	if ( (NULL != args) && (true == canDeleteArgs) )
	{
		delete args;
	}

}

void DutyManagerDlg::requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent) 
{

}

LRESULT DutyManagerDlg::OnPopulateData(WPARAM wParam, LPARAM lParam)
{
	bool bStaticData = (bool) wParam;
	bool bDynamicData = (bool) lParam;
	if( (true == bStaticData) || (true == bDynamicData) )
	{
		if ( true == bStaticData)
		{
			AfxGetApp()->DoWaitCursor(1);
			m_regionDutyPropertyPage->initialiseRegionDutyGridStatic();
			m_subsystemDutyPropertyPage->initialiseSubsystemDutyGridStatic();
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "building region page static data finished");

			//CString statusMsg;
			//statusMsg.LoadString(IDS_SUBSYSTEM_STATUS);
			//SendDlgItemMessage(IDC_STATUS_STATIC,WM_SETTEXT,0,(LPARAM)( (LPCSTR) statusMsg));

			//SendDlgItemMessage(IDC_STATUS_STATIC,WM_SETTEXT,0,  TALoadString(IDS_SUBSYSTEM_STATUS).GetString() );

			CString statusMsg;
			statusMsg.LoadString(IDS_SUBSYSTEM_STATUS);
			SendDlgItemMessage(IDC_STATUS_STATIC,WM_SETTEXT,0,(LPARAM)statusMsg.AllocSysString() );
			
		}

		if ( true == bDynamicData)
		{
			AfxGetApp()->DoWaitCursor(1);
			m_regionDutyPropertyPage->updateRegionDutyGrid();
			m_subsystemDutyPropertyPage->updateSubsystemDutyGrid();
			if ( m_showOtherSession && m_otherSesSubDutyPropertyPage != NULL )
			{
				m_otherSesSubDutyPropertyPage->initialiseOhterSessionDutyGrid();
			}

			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "building page dynamic data finished");

			SendDlgItemMessage(IDC_STATUS_STATIC,WM_SHOWWINDOW,SW_HIDE , 0);

			m_shownStatus = false;
			g_populated = true;
			m_isMartixDataInited = true;
		}

		this->positionControls();	

		//this->RedrawWindow();
		this->RedrawWindow(NULL,NULL,RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ALLCHILDREN);

		AfxGetApp()->DoWaitCursor(-1);

		m_regionDutyPropertyPage->redrawScrollBar();
	}
	else
	{
		std::string tempStr = TALoadString( STRING_ID_DUTY_MANAGER_0003 ).toMultiByteStdString();
		MessageBox::warning(  tempStr );
		ASSERT(::AfxGetMainWnd() != NULL);
		::AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}

	return 0;
}

LRESULT DutyManagerDlg::OnReceiveDutyAgentRecover(WPARAM wParam, LPARAM lParam)
{
	//std::stringstream messageInfo;

	TAUtfString messageInfo;	

	try
	{
		std::string locationName = m_realPModel->getLocationNameByLocationKey(lParam);
		TAUtfString locationNameUnicode = TAUtfString::CreateFromMultitByteString(locationName.c_str());

		messageInfo = TALoadString( STRING_ID_DUTY_MANAGER_0009 );
		messageInfo += locationNameUnicode;
		messageInfo += TALoadString( STRING_ID_DUTY_MANAGER_0010 );
		//TA_Base_Core::ILocation* locattion = TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(lParam);
		//messageInfo << "the DutyAgent at location "<< locattion->getName();
		//messageInfo << " is just recover from broken down. If you have some operation related to that location rights. Please close the Rights manager ane reopen it.";
	}
	catch(...)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "can't get location info for location key %d", lParam);
		//messageInfo << " One of the DutyAgent is just recover from broken down. If you have some operation related to the rights. Please close the Rihgts manager ane reopen it.";
		messageInfo = TALoadString( STRING_ID_DUTY_MANAGER_0011 );
	}

	MessageBox::info( messageInfo.toMultiByteStdString() );
	return 0;
}

LRESULT DutyManagerDlg::OnRegionDutyUpdate(WPARAM wParam, LPARAM lParam)
{
	AfxGetApp()->DoWaitCursor(1);
	RegionDutyChangedEventArgs* pDutyArgs = reinterpret_cast<RegionDutyChangedEventArgs*>(wParam);
	m_regionDutyPropertyPage->OnRegionDutyUpdate(pDutyArgs->m_dutyUpdate);
	delete pDutyArgs;
	AfxGetApp()->DoWaitCursor(-1);

	return 0;
}

LRESULT DutyManagerDlg::OnSubsystemDutyUpdate(WPARAM wParam, LPARAM lParam)
{
	AfxGetApp()->DoWaitCursor(1);
	SubsystemDutyChangedEventArgs* pDutyArgs = reinterpret_cast<SubsystemDutyChangedEventArgs*>(wParam);
	//m_regionDutyPropertyPage->OnDutyUpdate(pDutyArgs->m_dutyUpdate);
	m_subsystemDutyPropertyPage->OnSubsystemDutyUpdate(pDutyArgs->m_dutyUpdate);
	if ( m_otherSesSubDutyPropertyPage != NULL )
	{
		m_otherSesSubDutyPropertyPage->OnSubsystemDutyUpdate(pDutyArgs->m_dutyUpdate);
	}

	delete pDutyArgs;
	AfxGetApp()->DoWaitCursor(-1);

	return 0;
}

LRESULT DutyManagerDlg::OnSessionUpdate(WPARAM wParam, LPARAM lParam)
{
	AfxGetApp()->DoWaitCursor(1);
	SessionChangedEventArgs* pSessionArgs = reinterpret_cast<SessionChangedEventArgs*>(wParam);
	m_regionDutyPropertyPage->OnSessionUpdate(pSessionArgs->m_sessionUpdate);
	m_subsystemDutyPropertyPage->OnSessionUpdate(pSessionArgs->m_sessionUpdate);
	if ( m_otherSesSubDutyPropertyPage != NULL )
	{
		m_otherSesSubDutyPropertyPage->OnSessionUpdate(pSessionArgs->m_sessionUpdate);
	}

	delete pSessionArgs;
	AfxGetApp()->DoWaitCursor(-1);

	return 0;
}

LRESULT DutyManagerDlg::OnSetLanguage(WPARAM wParam, LPARAM lParam)
{
	//TATranslateMenu( IDR_MAINMENU, GetMenu() );
	//DrawMenuBar();
	//TATranslateToolBar(IDR_TOOLBAR1, &m_toolBar );

	return AbstractTransActiveDialog::OnSetLanguage( wParam, lParam );
}

