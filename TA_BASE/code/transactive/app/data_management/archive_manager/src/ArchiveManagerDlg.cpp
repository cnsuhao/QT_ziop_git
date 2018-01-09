/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/data_management/archive_manager/src/ArchiveManagerDlg.cpp $
* @author:  Alan Brims
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2015/10/09 15:42:35 $
* Last modified by:  $Author: CM $
*
* Dialog for the operator to select the archive operation to perform.
* Inherits from TransActiveDialog.
*/

#if defined( WIN32 )
#pragma warning ( disable : 4503 ) 
#endif // defined( WIN32 )

#include "app/data_management/archive_manager/src/stdafx.h"
#include "app/data_management/archive_manager/src/ArchiveManagerDlg.h"
#include "app/data_management/archive_manager/src/RestoreArchiveDlg.h"
#include "app/data_management/archive_manager/src/WriteArchiveDlg.h"
#include "app/data_management/archive_manager/src/DateRangeDlg.h"
#include "app/data_management/archive_manager/src/RetrievingDataDlg.h"
#include "app/data_management/archive_manager/src/DeleteArchiveDlg.h"
#include "app/data_management/archive_manager/src/ArchiveGuiObserver.h"
#include "bus/generic_gui_view/src/TransactiveMessage.h"
#include "core/exceptions/src/ArchiveException.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace 
{
	UINT getIconType( TA_ArchiveManager::EMsgLevel messageLevel )
	{
		switch( messageLevel )
		{
		case TA_ArchiveManager::INFO:
			return MB_ICONINFORMATION;
		case TA_ArchiveManager::WARN:
			return MB_ICONWARNING;
		case TA_ArchiveManager::ERR:
		default:
			return MB_ICONERROR;
		}
	}
}


namespace TA_Base_App
{

	/////////////////////////////////////////////////////////////////////////////
	// CArchiveManagerDlg dialog

	CArchiveManagerDlg::CArchiveManagerDlg(TA_Base_Bus::ITransActiveWinApp * app)
		: TA_Base_Bus::AbstractTransActiveDialog(app, CArchiveManagerDlg::IDD, NULL),
		m_realPModel(NULL)
	{
		// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

		m_menuMain.CreateMenu();

		// If necessary this code can go in OnInitDialog before the call to TransActiveDialog::OnInitDialog()
		TA_Base_Bus::ViewResizeProperty properties;
		properties.canMaximise = false;
		properties.maxHeight = -1;
		properties.maxWidth = -1;
		properties.minWidth = -1;
		properties.minHeight = -1;
		m_resizeViewHelper->setViewResizeProperty(properties);
		IGuiObserver::setInstance(new ArchiveGuiObserver());
	}

	CArchiveManagerDlg::~CArchiveManagerDlg()
	{
		IGuiObserver::freeInstance();
	}

	void CArchiveManagerDlg::DoDataExchange(CDataExchange* pDX)
	{
		AbstractTransActiveDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CArchiveManagerDlg)
		DDX_Control(pDX, IDC_BUTTON_SCHEDULE, m_automaticButton);	
		DDX_Control(pDX, IDC_BUTTON_RESTORE, m_restoreButton);
		DDX_Control(pDX, IDC_BUTTON_MANUAL, m_manualButton);
		DDX_Control(pDX, IDC_BUTTON_DISPLAY, m_deleteButton);
		//}}AFX_DATA_MAP
	}

	BEGIN_MESSAGE_MAP(CArchiveManagerDlg, AbstractTransActiveDialog)
		//{{AFX_MSG_MAP(CArchiveManagerDlg)
		ON_BN_CLICKED(IDC_BUTTON_SCHEDULE, OnButtonSchedule)
		ON_BN_CLICKED(IDC_BUTTON_MANUAL, OnButtonManual)
		ON_BN_CLICKED(IDC_BUTTON_RESTORE, OnButtonRestore)
		ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
		ON_WM_CLOSE()
		//}}AFX_MSG_MAP
		ON_MESSAGE(WM_SERVER_STATUS_CHANGE, OnServerStatusChange)
		ON_MESSAGE(WM_SESSIONID_CHANGE, OnSessionIdChange)
		ON_MESSAGE(WM_TASETLANG, OnSetLanguage)
	END_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////////////////
	// CArchiveManagerDlg message handlers

	BOOL CArchiveManagerDlg::OnInitDialog()
	{
		AbstractTransActiveDialog::OnInitDialog();

		//	Set the icon for this dialog.  The framework does this automatically
		//	when the application's main window is not a dialog

		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		// liborm 1/07/04 - fix for TD defect #3505
		// application icon draws over text in the title bar, so refresh the
		// title text
		TCHAR titleText[256];
		this->GetWindowText( titleText, 256 );
		this->SetWindowText( titleText );

		enableButtons();

		m_realPModel->init();

		return TRUE;  // return TRUE  unless you set the focus to a control
	}


	void CArchiveManagerDlg::OnOK() 
	{
		//  Our OK button is displayed as Close
		AbstractTransActiveDialog::OnClose();
	}


	void CArchiveManagerDlg::OnButtonSchedule() 
	{
		bool canWrite = m_realPModel->canWriteScheduledArchives();
		if(canWrite)
		{		       
			// Open the dialog for writing scheduled archives to removable media.
			WriteArchiveDlg dlg(false);
			int status = dlg.DoModal();
			// Process the returned status.
			if ( IDCANCEL == status )
			{
				m_realPModel->sendAuditMsgForCancel(EArchiveOperation::SCHEDULE);
				// If the copy process fails at any stage, the return value will be IDABORT,
				// an audit message will be sent and a message will be displayed to the user, 
				// so there will not be anything left to do here.
			}
		}
		m_realPModel->displayErrorMsg(EArchiveOperation::SCHEDULE);
	}


	void CArchiveManagerDlg::OnButtonManual() 
	{
		bool canCreateManual = m_realPModel->canCreateManualArchive();
		if(canCreateManual)
		{
			// Get the date range.
			DateRangeDlg rangeDlg;
			rangeDlg.setDvdVolume(m_realPModel->getManualDVDVolume());
			// ++yangguang TD16713
			int status = rangeDlg.DoModal();
			if ( IDOK == status )
			{
				// Get the dates that were selected in the DateRangeDlg.
				//time_t fromDate( rangeDlg.getSelectedFromDate() );
				//time_t toDate( rangeDlg.getSelectedToDate() );
				// Get the data for that date range.	        
				RetrievingDataDlg retDlg;//(fromDate, toDate );
				status = retDlg.DoModal();
				if ( IDOK == status )
				{
					// The toDate may have changed if the retrieval process was stopped partway through,
					// so get the toDate from the RetrievingDataDlg.	                
					//retDlg.getToDate();
					// Write the data to CD.
					WriteArchiveDlg writeDlg(true);//( fromDate, toDate );
					status = writeDlg.DoModal();
				}
			}
			// liborm 1/07/2004 - fix for TD defect #3511
			// dialogue is closed and control returns to Archive Manager
			else if ( IDCANCEL == status )
			{
				m_realPModel->sendAuditMsgForCancel(EArchiveOperation::MANUAL);
			}

			// Process the returned status.

			if ( IDOK == status )
			{
				/*
				// Send an audit message.

				try
				{
				// True => isManualArchive
				TA_ArchiveManager::sendAuditMessage( TA_Base_Core::ArchiveAudit::ArchiveCopied, "", true ); 
				}
				catch( TA_Base_Core::ArchiveException& e )
				{
				LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
				messageForUser = e.what();
				messageLevel = TA_ArchiveManager::WARN;
				return;
				}
				*/
			}
			else if ( IDCANCEL == status )
			{
				// If the user cancelled at any stage, send an audit message.
				m_realPModel->sendAuditMsgForCancel(EArchiveOperation::MANUAL);
			}

			// If the copy process fails at any stage, the return value will be IDABORT,
			// an audit message will be sent and a message will be displayed to the user, 
			// so there will not be anything left to do here.    
		}
		m_realPModel->displayErrorMsg(EArchiveOperation::MANUAL);		
	}


	void CArchiveManagerDlg::OnButtonRestore() 
	{		
		bool canRestore = m_realPModel->canRestoreArchives();			
		if(canRestore)
		{
			// Open the dialog for restoring archives from removable media.
			RestoreArchiveDlg dlg;
			int status = dlg.DoModal();
			if ( IDOK == status )
			{
				m_realPModel->sendRestoreAuditMsgForOK();
			}
			else if ( IDCANCEL == status )
			{
				m_realPModel->sendAuditMsgForCancel(EArchiveOperation::RESTORE);
			}
			// If the restoration process fails at any stage, the return value will be IDABORT,
			// an audit message will be sent and a message will be displayed to the user, 
			// so there will not be anything left to do here.
		}
		m_realPModel->displayErrorMsg(EArchiveOperation::RESTORE);
	}


	void CArchiveManagerDlg::OnButtonDisplay() 
	{
		// KT 11 Mar 04: PW #3133. This method has to be changed because the 
		// DeleteArchiveDlg now does both the deletion of the data and the
		// modification of the deletion due date, so the message sending
		// requirements are different.

		// liborm - 26 Oct 04 - Fix for TD #3719
		// Check that the user has permission.
		try
		{
			if(m_realPModel->canDeleteArchive())
			{
				DeleteArchiveDlg dlg;
				dlg.DoModal();
			}
		}
		catch( TA_Base_Core::ArchiveException& e )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << e.what();
			UINT selectedButton = userMsg.showMsgBox(IDS_UE_050129);
		}
	}

	LRESULT CArchiveManagerDlg::OnServerStatusChange(WPARAM, LPARAM)
	{
		enableButtons();
		if ( !(m_realPModel->isServerUp()) )
		{
			TA_Base_Bus::TransActiveMessage userMsg;
			UINT selectedButton = userMsg.showMsgBox(IDS_UW_050003);
		}
		return NULL;	// This line must be included. Everything above is application specific
	}



	LRESULT CArchiveManagerDlg::OnSessionIdChange(WPARAM, LPARAM)
	{
		enableButtons();
		return NULL;	// This line must be included. Everything above is application specific
	}


	void CArchiveManagerDlg::enableButtons()
	{
		m_realPModel->checkButtonEnabled();
		m_automaticButton.EnableWindow(m_realPModel->isAutomaticButtonEnabled());
		m_manualButton.EnableWindow( m_realPModel->isManualButtonEnabled() );
		m_restoreButton.EnableWindow( m_realPModel->isRestoreButtonEnabled() );
		m_deleteButton.EnableWindow( m_realPModel->isDeleteButtonEnabled() );
	}
	void CArchiveManagerDlg::OnClose() 
	{
		// TODO: Add your message handler code here and/or call default
		//TD18095, jianghp, to fix the performance of showing manager application
		AbstractTransActiveDialog::DestroyWindow();
	}

	////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	LRESULT CArchiveManagerDlg::OnSetLanguage(WPARAM wParam, LPARAM lParam)
	{	
		TATranslateMenu(IDR_MENU1 , &m_menuMain );
		return  AbstractTransActiveDialog::OnSetLanguage( wParam, lParam );
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	/// AbstractTransActiveDialog member functions

	void CArchiveManagerDlg::initUIControl()
	{		
		TAAppendLangMenu( IDR_MENU1, &m_menuMain );
		TATranslateMenu( IDR_MENU1, &m_menuMain );
		SetMenu(&m_menuMain); 
		DrawMenuBar();
	}

	std::string CArchiveManagerDlg::getMyPModelName()
	{
		return ARCHIVE_MANAGER_DLG_MODEL;
	}

	void CArchiveManagerDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<ArchiveManagerDlgPModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel has been initialized successfully");
		//m_realPModel->attachView(this, GET_RESOURCE_STRING_VALUE.c_str());	
		m_realPModel->attachView(this, SHOW_USER_MESSAGE_NOTIFY.c_str());	
		m_realPModel->attachView(this, POST_MESSAGE_INVOKE.c_str());	
	}

	void CArchiveManagerDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		std::string type(updateType);
		if(SHOW_USER_MESSAGE_NOTIFY == type)
		{
			TransactiveUserMsgEventArgs * msgArgs = dynamic_cast<TA_Base_App::TransactiveUserMsgEventArgs *>(args);
			TA_Base_Bus::TransActiveMessage userMsg;
			if(msgArgs->getArgs1Defined())
				userMsg<<msgArgs->getArgs1().c_str();
			if(msgArgs->getArgs2Defined())
				userMsg<<msgArgs->getArgs2().c_str();
			msgArgs->setReturnValue(userMsg.showMsgBox(msgArgs->getUID()));
		}
		else if (POST_MESSAGE_INVOKE == type)
		{
			PostMessageEventArgs * gargs = dynamic_cast<TA_Base_App::PostMessageEventArgs *>(args); 
			PostMessage( gargs->getMessageId(), 0, gargs->getLParam());
		}
	}

} // TA_Base_App


