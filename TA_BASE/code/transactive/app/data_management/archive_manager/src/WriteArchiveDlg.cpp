/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/data_management/archive_manager/src/WriteArchiveDlg.cpp $
  * @author:  Alan Brims
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Dialog box to list the files that will be written to archive media,
  * and prompt the operator to begin the write process.
  *
  */

#if defined( WIN32 )
#pragma warning ( disable : 4786 4250 4503 ) 
#endif // defined( WIN32 )


#include <map>
#include <sstream>

#include "app/data_management/archive_manager/src/stdafx.h"
#include "app/data_management/archive_manager/src/WriteArchiveDlg.h"
#include "app/data_management/archive_manager/src/BurnMediaDlg.h"
#include "core/exceptions/src/ArchiveException.h"
#include "core/message/types/ArchiveAudit_MessageTypes.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"
#include "bus/generic_gui_view/src/TransactiveMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
    //
    // Enum specifying the columns in the archive list.
    //

    enum EListColumns
    {
        DATE_COL    = 0,
        NAME_COL    = 1,
        SIZE_COL    = 2
    };
}

namespace TA_Base_App
{
    
    /////////////////////////////////////////////////////////////////////////////
    // WriteArchiveDlg dialog
    
    
    WriteArchiveDlg::WriteArchiveDlg( bool isManualArchive, CWnd* pParent /*=NULL*/)
        : AbstractDialog(dynamic_cast<TA_Base_Bus::ITransActiveWinApp*> (AfxGetApp()),WriteArchiveDlg::IDD, pParent),
		m_fileListColNameWidth(0)
    {        
        //{{AFX_DATA_INIT(WriteArchiveDlg)
	    //}}AFX_DATA_INIT
		m_isManualArchive = isManualArchive;
    }
     

    void WriteArchiveDlg::DoDataExchange(CDataExchange* pDX)
    {
        AbstractDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(WriteArchiveDlg)
		DDX_Control(pDX, IDC_STATIC_INSERT_CDDVD, m_insertCDDVD);
	    DDX_Control(pDX, IDC_NUM_CDS_STATIC, m_numCds);
	    DDX_Control(pDX, IDOK, m_okButton);
        DDX_Control(pDX, IDC_FILE_LIST, m_fileList);
	    //}}AFX_DATA_MAP
    }
    
    
    BEGIN_MESSAGE_MAP(WriteArchiveDlg, AbstractDialog)
        //{{AFX_MSG_MAP(WriteArchiveDlg)
	    ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILE_LIST, OnItemChangedFileList)
	    ON_WM_CLOSE()
	    //}}AFX_MSG_MAP
    END_MESSAGE_MAP()
    
    /////////////////////////////////////////////////////////////////////////////
    // WriteArchiveDlg message handlers
    
    BOOL WriteArchiveDlg::OnInitDialog() 
    {
        AbstractDialog::OnInitDialog();
        	            
        // Give this dialog its parent's icons.

        SetIcon( GetParent()->GetIcon( true ), TRUE );			// Set big icon
        SetIcon( GetParent()->GetIcon( false ), FALSE );		// Set small icon

        // Display an hour-glass cursor.

        HCURSOR hOldCursor = SetCursor( LoadCursor( NULL, IDC_APPSTARTING ) );
        
        // Prepare the CListCtrl columns and fill it up.

        CRect rc;
        m_fileList.GetWindowRect( rc );
        CDC* pDC = m_fileList.GetDC();
        int dateWidth = pDC->GetTextExtent( _T(" 88/88/8888 ")).cx;     //  max date width - for the most common date format.

		int sizeMbWidth = 0;
		if( TA_Base_Core::RunParams::getInstance().isSet( WriteArchiveHelper::CD_ARCHIVE.c_str()) )
		{
			sizeMbWidth = pDC->GetTextExtent( _T(" 888.88 MB ")).cx;    //  max MB size (for a CD)
		}
		else
		{
			sizeMbWidth = pDC->GetTextExtent( _T(" 8888.88 MB ")).cx;    //  max MB size (for a DVD)
		}
        sizeMbWidth+= GetSystemMetrics( SM_CXVSCROLL);
        
		m_fileListColNameWidth = rc.Width()-dateWidth-sizeMbWidth-4;
        m_fileList.InsertColumn( DATE_COL,  TALoadString(IDS_DATE_COL_STR).str(), LVCFMT_CENTER, dateWidth, 0);
        m_fileList.InsertColumn( NAME_COL,  TALoadString(IDS_FILENAME_COL_STR).str(), LVCFMT_LEFT, m_fileListColNameWidth, 1);
        m_fileList.InsertColumn( SIZE_COL,  TALoadString(IDS_SIZE_COL_STR).str(), LVCFMT_RIGHT, sizeMbWidth, 2);

		m_realPModel->setWhetherManual(m_isManualArchive);

        populateList();

        // Make sure there are files in the list.  If not, abort.
        // This is not done in populateList() because at some stage we
        // may want to use populateList() to perform a refresh, and we may
        // not want the dialog to close if the list is empty.

        if ( 0 == m_fileList.GetItemCount() )
        {
            // The error message will have been displayed in populateList().

            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
                "Archive list is empty. Closing Insert Archive Media dialog." );
            AbstractDialog::EndDialog( IDABORT );
        }

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }


    void WriteArchiveDlg::populateList()
    {
		WriteArchiveHelper::FileDetailsVector fileDetails;
		bool success = m_realPModel->getFilesDeatilsVector(fileDetails);
		if(success)
		{
			//display cd or dvd details
			int nCDsStr = m_realPModel->getCdsMapSize();
			CString numCds;
			TAUnicodeString insertCDDVD;
			if( !TA_Base_Core::RunParams::getInstance().isSet( WriteArchiveHelper::CD_ARCHIVE.c_str()) )
			{
				numCds.Format(_T("%d"),nCDsStr);
				numCds += TALoadString( IDS_NUM_DVDS_MSG).str();
				insertCDDVD =  TALoadString( IDS_INSERT_DVD_MSG);
			}
			else
			{
				numCds.Format(_T("%d"),nCDsStr);
				numCds += TALoadString( IDS_NUM_CDS_MSG).str();
				insertCDDVD =  TALoadString( IDS_INSERT_CD_MSG);
			}
			m_numCds.SetWindowText(numCds);
			m_insertCDDVD.SetWindowText( insertCDDVD.str() );


			// Display the information in the list ctrl.
			// Put the file size in the item data, so we can use it when figuring out how much is going on each DVD.
			int item = 0;
			TAUnicodeString strMb;
			CDC *pDC = GetDC();
			UINT uColNameMaxWidth = 0;
			for( WriteArchiveHelper::FileDetailsVector::iterator it = fileDetails.begin(); it != fileDetails.end(); ++it )
			{
				// KT 8 Mar 04: PW #3161 Convert date to local format.

				item = m_fileList.InsertItem( m_fileList.GetItemCount(), 
					TAUnicodeString::CreateFromMultitByteString(TA_ArchiveManager::convertTimeTToStringDate( it->date ).c_str()).str() );
				m_fileList.SetItemText( item, NAME_COL, TAUnicodeString::CreateFromMultitByteString(it->name.c_str()).str() );
				CSize csColName = pDC->GetTextExtent(TAUnicodeString::CreateFromMultitByteString(it->name.c_str()).str());
				uColNameMaxWidth = uColNameMaxWidth < csColName.cx ? csColName.cx : uColNameMaxWidth;
				strMb.str().Format(_T("%0.2f MB"), it->sizeInMB );
				m_fileList.SetItemText( item, SIZE_COL, strMb.str() );
				//m_fileList.SetItemData( item, (unsigned long)( it->sizeInMB + 1 ) );
				m_fileList.SetItemData( item, item);
			}

			if(uColNameMaxWidth > m_fileListColNameWidth)
			{
				UINT uIncrement = uColNameMaxWidth - m_fileListColNameWidth;

				CRect rcFileList;
				m_fileList.GetWindowRect(&rcFileList);
				CRect rcDlg;
				GetWindowRect(&rcDlg);
				SetWindowPos(NULL, 0, 0, rcDlg.Width()+uIncrement, rcDlg.Height(), SWP_NOZORDER | SWP_NOMOVE);
				m_fileList.SetWindowPos(NULL, 0, 0, rcFileList.Width()+uIncrement, rcFileList.Height(), SWP_NOZORDER | SWP_NOMOVE);
				m_fileList.SetColumnWidth(1, uColNameMaxWidth);
			}


			DWORD dwFileListStyle = m_fileList.GetStyle();
			if(dwFileListStyle & WS_VSCROLL)
			{
				CRect rcFileList;
				m_fileList.GetWindowRect(&rcFileList);
				CRect rcDlg;
				GetWindowRect(&rcDlg);

				SetWindowPos(NULL, 0, 0, rcDlg.Width()+GetSystemMetrics(SM_CXVSCROLL), rcDlg.Height(), SWP_NOZORDER | SWP_NOMOVE);
				m_fileList.SetWindowPos(NULL, 0, 0, rcFileList.Width()+GetSystemMetrics(SM_CXVSCROLL), rcFileList.Height(), SWP_NOZORDER | SWP_NOMOVE);
			}

			// KT FOR TESTING:showCdBreakUp();
		}
	}


    void WriteArchiveDlg::OnItemChangedFileList(NMHDR* pNMHDR, LRESULT* pResult) 
    {
	    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

        // Don't allow items to be selected (highlighted).

        m_fileList.SetItemState(pNMListView->iItem, ~LVIS_SELECTED, LVIS_SELECTED);
	    
	    *pResult = 0;
    }


    void WriteArchiveDlg::OnOK() 
    {
		if(m_realPModel->isCDsMapEmpty())
		{
			AbstractDialog::OnOK();
		}
        // Otherwise, proceed to writing.

        try
        {
            // This will start burning as soon as the dialog is open.

			bool m_bCdBurn = false;
			if( TA_Base_Core::RunParams::getInstance().isSet( WriteArchiveHelper::CD_ARCHIVE.c_str()) )
			{
				m_bCdBurn = true;
			}
			std::string volumeLabel;
			std::string cdLabel;
			WriteArchiveHelper::FileDetailsVector files;
			m_realPModel->getBurnFileDetails(volumeLabel,cdLabel,files);
            BurnMediaDlg dlg( volumeLabel,cdLabel,files,m_bCdBurn);
			if( IDOK != dlg.DoModal() )
			{
				// If we aborted due to an error, don't change anything.
				return;
			}

			try
			{
				TA_ArchiveManager::sendAuditMessage( TA_Base_Core::ArchiveAudit::ArchiveCopied, "", m_isManualArchive ); 
			}
			catch( TA_Base_Core::ArchiveException& e )
			{
				LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
				TA_Base_Bus::TransActiveMessage userMsg;
				TAUnicodeString messageForUser = TAUnicodeString::CreateFromMultitByteString(e.what());
				TA_ArchiveManager::EMsgLevel messageLevel = TA_ArchiveManager::WARN;
				userMsg << messageForUser.c_str();
				UINT selectedButton = userMsg.showMsgBox(IDS_UW_050018);
				
				return;
			}
            
        }
        catch( TA_Base_Core::ArchiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );  
            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << e.what();
            UINT selectedButton = userMsg.showMsgBox(IDS_UE_050130);
        }
        catch( TA_Base_Core::TransactiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "TransactiveException", e.what() );            
            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << e.what();
            UINT selectedButton = userMsg.showMsgBox(IDS_UE_050130);
        }

        // Now we need to delete the files from the first entry in the map,
        // because the files have been burnt.  These should
        // be deleted from the file list also.

        try
        {
			m_realPModel->deleteFirstCdMapEntry();
        }
        catch( TA_Base_Core::ArchiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );  
            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << e.what();
            UINT selectedButton = userMsg.showMsgBox(IDS_UE_050130);
        }


        // Do we have more files to write?

        int count =m_realPModel->getCdMapsSize();
        if ( 0 >= count )
        {
            TA_Base_Bus::TransActiveMessage userMsg;
            UINT selectedButton = userMsg.showMsgBox(IDS_UI_050054);

            m_realPModel->cleanUpArchive();	    
            AbstractDialog::OnOK();
        }
        else
        {
            CString numCds;
			if( !TA_Base_Core::RunParams::getInstance().isSet( WriteArchiveHelper::CD_ARCHIVE.c_str()) )
			{
				numCds.Format(_T("%d"),count);
				numCds +=  TALoadString(IDS_NUM_DVDS_MSG).str();
				m_insertCDDVD.SetWindowText(TALoadString(IDS_INSERT_DVD_MSG).str());
			}
			else
			{
				numCds.Format(_T("%d"),count);
				numCds +=  TALoadString(IDS_NUM_CDS_MSG).str();
				m_insertCDDVD.SetWindowText(TALoadString(IDS_INSERT_CD_MSG).str());
			}
            m_numCds.SetWindowText( numCds);
        }
    }


    void WriteArchiveDlg::OnCancel() 
    {
		try
        {
            TA_ArchiveManager::sendAuditMessage( TA_Base_Core::ArchiveAudit::CopyArchiveCancelled );
        }
        catch( TA_Base_Core::ArchiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
        }
        m_realPModel->cleanUpArchive();	
	    AbstractDialog::OnCancel();
    }


    void WriteArchiveDlg::OnClose() 
    {
        m_realPModel->cleanUpArchive();	
	    AbstractDialog::OnClose();
    }

	void WriteArchiveDlg::showCdBreakUp()
    {
        // KT 5 Mar 04: THIS IS A METHOD FOR TESTING WRITEARCHIVEHELPER::SPLITINTOCDS().
        // IT IS NOT USED IN THE ACTUAL APPLICATION.

        TAUnicodeString message;
		message.str() +=  _T("This is the break up of files into CDs:");
       /* for ( WriteArchiveHelper::CdsMap::iterator it = m_cdsMap.begin(); it != m_cdsMap.end(); ++it )
        {
            message << "\n" << it->first;
            message << "\n" << it->second.first;
            for ( WriteArchiveHelper::FileDetailsVector::iterator fit = it->second.second.begin(); fit != it->second.second.end(); ++fit )
            {
                message << "\n    " << (*fit).nameWithPath;
            }
        }*/ /////Not used

        // AL 17/03/05: Haven't changed this message box to use Transactive Message as it's not used
        // used in the actual application.
        MessageBox( message.str(), _T("CD Details"), MB_OK | MB_ICONINFORMATION );
    }

	///////////////////////////////////////////////////////////////////////////////////////////
	/// AbstractTransActiveView member functions

	std::string WriteArchiveDlg::getMyPModelName()
	{
		return WRITE_ARCHIVE_DLG_PMODEL;
	}

	void WriteArchiveDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<WriteArchivePModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel has been initialized successfully");
		m_realPModel->attachView(this, SHOW_USER_MESSAGE_NOTIFY.c_str());	
		m_realPModel->attachView(this, DELETE_WRITE_LIST_CTRL.c_str());
	}

	void WriteArchiveDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
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
		else if (DELETE_WRITE_LIST_CTRL == type)
		{
			m_fileList.DeleteItem( 0 );
		}
	}

} // TA_Base_App
