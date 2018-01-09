/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/data_management/archive_manager/src/RetrievingDataDlg.cpp $
  * @author:  Katherine Thomson
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Retrieves the data from the database for a manual archive.
  */

#if defined( WIN32 )
#pragma warning ( disable : 4503 ) 
#endif // defined( WIN32 )

#include <sstream>

#include "app/data_management/archive_manager/src/stdafx.h"
#include "app/data_management/archive_manager/src/RetrievingDataDlg.h"
#include "app/data_management/archive_manager/src/RetrieveCompleteDlg.h"
#include "core/exceptions/src/ArchiveException.h"
#include "core/exceptions/src/TransactiveException.h"
#include "core/utilities/src/DebugUtil.h"
#include "bus/generic_gui_view/src/TransactiveMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // RetrievingDataDlg dialog


    RetrievingDataDlg::RetrievingDataDlg(CWnd* pParent /*=NULL*/)
	    : AbstractDialog(dynamic_cast<TA_Base_Bus::ITransActiveWinApp*> (AfxGetApp()),RetrievingDataDlg::IDD, pParent)         
	{
        //TA_ASSERT( fromDate <= toDate, "The From date is before or the same as the To date." );

	    //{{AFX_DATA_INIT(RetrievingDataDlg)
	    m_range = _T("");
	    //}}AFX_DATA_INIT
    }

            
    
    RetrievingDataDlg::~RetrievingDataDlg()
    {
        //stop thread TODO
		m_realPModel->TerminateRetrievingThread();
    }


    void RetrievingDataDlg::DoDataExchange(CDataExchange* pDX)
    {
	    AbstractDialog::DoDataExchange(pDX);
	    //{{AFX_DATA_MAP(RetrievingDataDlg)
	    DDX_Control(pDX, IDC_DATE_STATIC, m_date);
	    DDX_Control(pDX, IDSTOP, m_stopButton);
	    DDX_Text(pDX, IDC_RANGE_STATIC, m_range);
	    //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(RetrievingDataDlg, AbstractDialog)
	    //{{AFX_MSG_MAP(RetrievingDataDlg)
	    ON_BN_CLICKED(IDSTOP, OnStop)
	    //}}AFX_MSG_MAP
	    ON_MESSAGE(WM_CLOSE_DLG, OnCloseDlg)
	    ON_MESSAGE(WM_COMPLETE_DLG, OnShowCompleteDlg)
        ON_MESSAGE(WM_DISABLE_DLG, OnDisableDlg)
        ON_WM_CLOSE()
    END_MESSAGE_MAP()
  

    /////////////////////////////////////////////////////////////////////////////
    // RetrievingDataDlg message handlers

    BOOL RetrievingDataDlg::OnInitDialog() 
    {
	    AbstractDialog::OnInitDialog();

        //  Give this dialog its parent's icons.

        SetIcon( GetParent()->GetIcon( true ), TRUE );			// Set big icon
        SetIcon( GetParent()->GetIcon( false ), FALSE );		// Set small icon

        //  Display an hour-glass cursor.

        HCURSOR hOldCursor = SetCursor( LoadCursor( NULL, IDC_APPSTARTING ) );

        // KT 8 Mar 04: PW #3161 Converted dates to local format for display.

        
	    m_range = m_realPModel->getRetrievingDateRangeString().c_str();
        UpdateData( FALSE );
	            
        // This is in a separate thread, so we don't block the UI.
        
        m_realPModel->startRetrievingThread();
    
        return TRUE;  // return TRUE unless you set the focus to a control
	                  // EXCEPTION: OCX Property Pages should return FALSE
    }


    void RetrievingDataDlg::OnStop() 
    {
		m_realPModel->stopRetrievingThread();
    }

    
    LRESULT RetrievingDataDlg::OnShowCompleteDlg(WPARAM wparam, LPARAM lparam)
    {
        RetrieveCompleteDlg dlg;
        if ( IDCANCEL == dlg.DoModal() )
        {
			m_realPModel->deleteRetrievedArchiveFiles();
            PostMessage( WM_CLOSE_DLG, 0, IDCANCEL );
        }
        else
        {
            PostMessage( WM_CLOSE_DLG, 0, IDOK );
        }
        return NULL;
    }


    LRESULT RetrievingDataDlg::OnCloseDlg(WPARAM, LPARAM lparam)
    {
		m_realPModel->setStateManagerIdle();
        AbstractDialog::EndDialog( lparam );
	    return NULL;	// This line must be included. Everything above is application specific
    }


    LRESULT RetrievingDataDlg::OnDisableDlg(WPARAM, LPARAM lparam)
    {
        // Disable the stop button
        m_stopButton.EnableWindow(FALSE);
    
        // Disable the close button
        CMenu* mnu = this->GetSystemMenu(FALSE);
        mnu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

        return NULL;
    }

        
    void RetrievingDataDlg::OnClose()
    {
        // User attempted to close the dialog. Behaviour should be identical to clicking 'Stop'.
        OnStop();
    }   
  

	///////////////////////////////////////////////////////////////////////////////////////////
	/// AbstractTransActiveView member functions

	std::string RetrievingDataDlg::getMyPModelName()
	{
		return WRITE_ARCHIVE_DLG_PMODEL;
	}

	void RetrievingDataDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<WriteArchivePModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel has been initialized successfully");
		m_realPModel->attachView(this, POST_MESSAGE_TO_RETRIEVINGDATADLG.c_str());	
		m_realPModel->attachView(this, SET_RETRIEVING_DATA_DATE_CTRL.c_str());	
		m_realPModel->attachView(this, SHOW_USER_MESSAGE_RETRIEVINGDATADLG.c_str());			
	}

	void RetrievingDataDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		std::string type(updateType);
		if( POST_MESSAGE_TO_RETRIEVINGDATADLG == type)
		{
			PostMessageEventArgs * postArgs = dynamic_cast<TA_Base_App::PostMessageEventArgs *>(args);
			PostMessage( postArgs->getMessageId(), 0, postArgs->getLParam() );
		}
		else if(SET_RETRIEVING_DATA_DATE_CTRL == type)
		{
			GenericEventArgs* gArgs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
			std::string dateStr;
			std::string * str1 = static_cast<std::string*> (gArgs->Param1);
			dateStr = *str1;
			m_date.SetWindowText(TAUnicodeString::CreateFromMultitByteString(dateStr.c_str()).str());
		}
		else if(SHOW_USER_MESSAGE_RETRIEVINGDATADLG == type)
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
	}
    
}
