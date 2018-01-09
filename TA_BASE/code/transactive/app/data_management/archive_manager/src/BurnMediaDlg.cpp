/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/data_management/archive_manager/src/BurnMediaDlg.cpp $
  * @author:  Edward Wallis
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * Dialog box to handle the DVD Burn process.
  * This dialog is specific to DVD-R and should be replaced with
  * a similar dialog if an alternative media is required.
  *
  */

#if defined( WIN32 )
#pragma warning ( disable : 4503 ) 
#endif // defined( WIN32 )

#include <sstream>

#include "app/data_management/archive_manager/src/stdafx.h"
#include "app/data_management/archive_manager/src/BurnMediaDlg.h"
//#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "bus/generic_gui_view/src/TransactiveMessage.h"
#include "core/exceptions/src/ArchiveException.h"
//#include "core/message/types/ArchiveAudit_MessageTypes.h"
//#include "core/utilities/src/TAAssert.h"
//#include "core/utilities/src/DebugUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace TA_Base_App
{
    
    BurnMediaDlg::BurnMediaDlg( const std::string& volumeLabel, const std::string& cdLabel, 
        const WriteArchiveHelper::FileDetailsVector& files, bool bCdBurn, CWnd* pParent /*=NULL*/)
        : AbstractDialog(dynamic_cast<TA_Base_Bus::ITransActiveWinApp*> (AfxGetApp()),BurnMediaDlg::IDD, pParent),
          m_volumeLabel( volumeLabel ),
          m_cdLabel( cdLabel ),
          m_files( files ),
		  m_realPModel(NULL),
		  m_CDBurn(bCdBurn)
    {
        //{{AFX_DATA_INIT(BurnMediaDlg)
        m_csStage = _T("");
        m_csLog = _T("");
        //}}AFX_DATA_INIT
    }
        

    BurnMediaDlg::~BurnMediaDlg()
    {
		try
        {
            m_realPModel->terminateAndWait();
        }
        catch( TA_Base_Core::TransactiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "TransactiveException", e.what() );
        }
        catch( ... )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "Unknown Exception", "Unknown exception caught in destructor." );
        }
	}


    void BurnMediaDlg::DoDataExchange(CDataExchange* pDX)
    {
        AbstractDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(BurnMediaDlg)
	    DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	    DDX_Control(pDX, IDC_STAGE_STATIC, m_stageStatic);
	    DDX_Control(pDX, IDC_LOG_EDIT, m_editLog);
        DDX_Control(pDX, IDC_WRITE_PROGRESS, m_writeProgress);
        DDX_Text(pDX, IDC_STAGE_STATIC, m_csStage);
        DDX_Text(pDX, IDC_LOG_EDIT, m_csLog);
	    //}}AFX_DATA_MAP
    }
    
    
    BEGIN_MESSAGE_MAP(BurnMediaDlg, AbstractDialog)
        //{{AFX_MSG_MAP(BurnMediaDlg)
	    ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	    ON_MESSAGE(WM_CLOSE_DLG, OnCloseDlg)
    END_MESSAGE_MAP()
        
    /////////////////////////////////////////////////////////////////////////////
    // BurnMediaDlg message handlers
    
    BOOL BurnMediaDlg::OnInitDialog() 
    {
        AbstractDialog::OnInitDialog();
        
        //  Give this dialog its parent's icons.

        SetIcon( GetParent()->GetIcon( true ), TRUE );			// Set big icon
        SetIcon( GetParent()->GetIcon( false ), FALSE );		// Set small icon

        //  Display an hour-glass cursor.

        HCURSOR hOldCursor = SetCursor( LoadCursor( NULL, IDC_APPSTARTING ) );

        //  Nero reports write/burn progress in percent.

        m_writeProgress.SetRange( 0, 100 );

		m_realPModel->setDetails(m_volumeLabel,m_cdLabel,&m_files,m_CDBurn);
        
        // Start writing.

        m_realPModel->start();

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    
    void BurnMediaDlg::OnDestroy() 
    {
	    AbstractDialog::OnDestroy();
	    m_realPModel->cleanUpNero();	  
    }


    void BurnMediaDlg::OnCancel() 
    {
        // If we are in the middle of processing, we have to wait until 
        // it is convenient to cancel.

        if ( StateManager::PROCESSING == m_realPModel->getStateManagerState() )
        {
            TA_Base_Bus::TransActiveMessage userMsg;
            UINT selectedButton = userMsg.showMsgBox(IDS_UI_050055);
            m_realPModel->setStateManagerState(  StateManager::STOPPING );

            appendStringToLogDisplay( "Cancelling..." );

            // Don't let the operator press Cancel again.

            m_buttonCancel.EnableWindow( FALSE );
        }
        else
        {
            // We're not processing, so we can just cancel.

            AbstractDialog::OnCancel();
        }
    }

    
    LRESULT BurnMediaDlg::OnCloseDlg(WPARAM, LPARAM lparam)
    {
        // If the user has cancelled - don't go any further.

        if ( StateManager::STOPPING == m_realPModel->getStateManagerState() )
        {
            try
            {
                std::string operatorName = TA_ArchiveManager::getOperatorNameFromSessionId();
                std::string workstationName = TA_ArchiveManager::getWorkstationNameFromSessionId();

                TA_Base_Bus::TransActiveMessage userMsg;
                userMsg << operatorName << workstationName;
                UINT selectedButton = userMsg.showMsgBox(IDS_UI_050030);
            }
            catch( TA_Base_Core::ArchiveException& e )
            {
                LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
                
                TA_Base_Bus::TransActiveMessage userMsg;
                UINT selectedButton = userMsg.showMsgBox(IDS_UI_050060);
            }
        }

		m_realPModel->setStateManagerState( StateManager::IDLE );
        //m_stateManager.setState( StateManager::IDLE );
        AbstractDialog::EndDialog( lparam );
	    return NULL;	// This line must be included. Everything above is application specific
    }

	 void BurnMediaDlg::appendStringToLogDisplay( const std::string& sAdd )
    {
        //  Add the string to the active Windows Edit control if it is usable, otherwise
        //  just add it to the local string that will be DDX'd into the edit control.

        if ( ::IsWindow( m_editLog.m_hWnd) )
        {
            TAUnicodeString sBuffer;
			m_editLog.GetWindowText( sBuffer.str() );     // This method requires a CString.
            if (!sBuffer.IsEmpty())
            {
                sBuffer+= _T("\r\n");
            }
			sBuffer+= TAUnicodeString::CreateFromMultitByteString(sAdd.c_str());
            m_editLog.SetWindowText( sBuffer.str() );
            m_editLog.LineScroll( m_editLog.GetLineCount(),0 );
        }
        else
        {
            if (!m_csLog.IsEmpty() )
            {
                m_csLog+= "\r\n";
            }
            m_csLog+= TAUnicodeString::CreateFromMultitByteString(sAdd.c_str()).str();
        }
    }

	 void BurnMediaDlg::showCloseButton()
	 {
		 TAUnicodeString closeButtonStr;
		 closeButtonStr = TALoadString(IDS_CLOSE_BUTTON_CAPTION_STR);
		 m_buttonCancel.SetWindowText(closeButtonStr.str());
	 }

	 void BurnMediaDlg::enableCancelButton(bool enable)
	 {
		 m_buttonCancel.EnableWindow( enable );
	 }

	 void BurnMediaDlg::setBurnFinish(bool success)
	 {
		 TAUnicodeString closeButtonStr, stageString;
		 closeButtonStr= TALoadString(IDS_CLOSE_BUTTON_CAPTION_STR);
		 m_buttonCancel.SetWindowText( closeButtonStr.str());
		 m_buttonCancel.EnableWindow( TRUE );
		 if(success)
		 {
			 if(m_CDBurn)			 
				 stageString = TALoadString(IDS_CD_WRITE_SUCCESS_STR);			 
			 else
				 stageString = TALoadString(IDS_DVD_WRITE_SUCCESS_STR);
		 }
		 else
		 {
			 if(m_CDBurn)
				 stageString = TALoadString(IDS_CD_FAILED_STR);
			 else
				  stageString = TALoadString(IDS_DVD_FAILED_STR);				
		 }
		  m_stageStatic.SetWindowText( stageString.str()  );
	 }

	 bool BurnMediaDlg::showFileDialog(TAUnicodeString& selectedPath)
	 {
		  static TCHAR BASED_CODE szFilter[]( _T("Image Files (*.nrg)|*.nrg|All Files (*.*)|*.*||") ); 
		  CFileDialog dlgOpen(TRUE, NULL, _T("test.nrg"), OFN_OVERWRITEPROMPT, szFilter, this);
		  if (dlgOpen.DoModal() == IDOK)
		  {
			 selectedPath = dlgOpen.GetPathName();
			 return true;
		  }
		  return false;
	 }

	 void BurnMediaDlg::setProgress(DWORD dwProgressInPercent)
	 {
		 m_writeProgress.SetPos( dwProgressInPercent );
	 }
		///////////////////////////////////////////////////////////////////////////////////////////
	/// AbstractTransActiveView member functions

	std::string BurnMediaDlg::getMyPModelName()
	{
		return BURN_MEDIA_DLG_PMODEL;
	}

	void BurnMediaDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<BurnMediaPModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel has been initialized successfully");
		m_realPModel->attachView(this, SHOW_USER_MESSAGE_NOTIFY.c_str());	
		m_realPModel->attachView(this, POST_MESSAGE_INVOKE.c_str());	
		m_realPModel->attachView(this, APPEND_STRING_LOG_DISPLAY.c_str());	
		m_realPModel->attachView(this, SHOW_CLOSE_BUTTON_BURNMEDIA_DLG.c_str());	
		m_realPModel->attachView(this, DISABLECANCELBTN_BURNDLG.c_str());	
		m_realPModel->attachView(this, BURN_FINISHED_STATUS.c_str());	
		m_realPModel->attachView(this, SET_STAGE_STR.c_str());
		m_realPModel->attachView(this, OPEN_FILEDIALOG_IMAGE_SEL.c_str());	
		m_realPModel->attachView(this, SET_PROGRESS_PERCENT_BURN_DLG.c_str());	
	}

	void BurnMediaDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		std::string type(updateType);
		if(SHOW_USER_MESSAGE_NOTIFY == type)
		{
			TransactiveUserMsgEventArgs * msgArgs = dynamic_cast<TA_Base_App::TransactiveUserMsgEventArgs *>(args);
			TA_Base_Bus::TransActiveMessage userMsg;
			if(msgArgs->getArgs1Defined())
				userMsg<<TAUnicodeString::CreateFromMultitByteString(msgArgs->getArgs1().c_str());
			if(msgArgs->getArgs2Defined())
				userMsg<<TAUnicodeString::CreateFromMultitByteString(msgArgs->getArgs2().c_str());
			if(msgArgs->getArgs3Defined())
				userMsg<<TAUnicodeString::CreateFromMultitByteString(msgArgs->getArgs3().c_str());
			TAUnicodeString caption = TAUnicodeString::CreateFromMultitByteString(msgArgs->getCaption().c_str());
			UINT returnValue;
			if(caption.GetLength() != 0)
			{
				returnValue = userMsg.showMsgBox(msgArgs->getUID(),caption);
			}
			else
			{
				returnValue = userMsg.showMsgBox(msgArgs->getUID());
			}
			msgArgs->setReturnValue(returnValue);
		}
		else if (POST_MESSAGE_INVOKE == type)
		{
			PostMessageEventArgs * gargs = dynamic_cast<TA_Base_App::PostMessageEventArgs *>(args); 
			PostMessage( gargs->getMessageId(), 0, gargs->getLParam());
		}
		else if (APPEND_STRING_LOG_DISPLAY == type)
		{
			appendStringToLogDisplay(m_realPModel->getLogString());
		}
		else if(SHOW_CLOSE_BUTTON_BURNMEDIA_DLG == type)
		{
			showCloseButton();
		}
		else if(DISABLECANCELBTN_BURNDLG == type)
		{
			m_buttonCancel.EnableWindow( FALSE );
		}
		else if(BURN_FINISHED_STATUS == type)
		{
			GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args); 
			bool bSuccess = *((bool *) gargs->Param1);
			setBurnFinish(bSuccess);
		}
		else if(SET_STAGE_STR == type)
		{
			GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args); 
			std::string text = *((std::string *) gargs->Param1);
			m_stageStatic.SetWindowText(TAUnicodeString::CreateFromMultitByteString(text.c_str()).str());
		}
		else if(OPEN_FILEDIALOG_IMAGE_SEL == type)
		{		
			TAUnicodeString selectedPath;
			bool bRet = showFileDialog(selectedPath);
			std::string path = selectedPath.toMultiByteStdString();
			m_realPModel->m_bFileDialog = bRet;
			m_realPModel->m_selectImageFilePath = path;
		}
		else if(SET_PROGRESS_PERCENT_BURN_DLG == type)
		{
			GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args); 
			DWORD dwProgress = *((DWORD *) gargs->Param1);
			setProgress(dwProgress);
		}
	}
} // TA_Base_App
