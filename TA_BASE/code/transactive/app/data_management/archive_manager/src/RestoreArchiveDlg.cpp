/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/data_management/archive_manager/src/RestoreArchiveDlg.cpp $
  * @author:  Alan Brims
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Dialog for operator to select the archive files to be restored.
  *
  */

#include <sstream>
#include <ctype.h>

#include "app/data_management/archive_manager/src/stdafx.h"
#include "app/data_management/archive_manager/src/RestoreArchiveDlg.h"
#include "app/data_management/archive_manager/src/RestoreCompleteDlg.h"
#include "core/database/src/SimpleDb.h"
#include "core/exceptions/src/ArchiveException.h"
#include "core/exceptions/src/TransactiveException.h"
#include "core/message/types/ArchiveAudit_MessageTypes.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"
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
        TICK_COL    = 0,
        DATE_COL    = 1,
        STATUS_COL  = 2
    };
}

namespace TA_Base_App
{
    
    /////////////////////////////////////////////////////////////////////////////
    // RestoreArchiveDlg dialog
    
    
    RestoreArchiveDlg::RestoreArchiveDlg(CWnd* pParent /*=NULL*/)
        : AbstractDialog(dynamic_cast<TA_Base_Bus::ITransActiveWinApp*> (AfxGetApp()),RestoreArchiveDlg::IDD, pParent),
		m_isListPopulated( false ),
		m_returnValue( IDOK )
    {
        //{{AFX_DATA_INIT(RestoreArchiveDlg)
	    //}}AFX_DATA_INIT
    }
    

    RestoreArchiveDlg::~RestoreArchiveDlg()
    {
       
    }

    
    void RestoreArchiveDlg::DoDataExchange(CDataExchange* pDX)
    {
        AbstractDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(RestoreArchiveDlg)
	DDX_Control(pDX, IDC_DRIVE_COMBO, m_selectedDriveCombo);
	    DDX_Control(pDX, IDOK, m_closeButton);
	    DDX_Control(pDX, ID_BUTTON_RESTORE, m_restoreButton);
	    DDX_Control(pDX, IDCANCEL, m_cancelButton);
	    DDX_Control(pDX, IDC_BUTTON_SELECT_ALL, m_selectAllButton);
        DDX_Control(pDX, IDC_ARCHIVE_LIST, m_archiveList);
	//}}AFX_DATA_MAP
    }
    
    
    BEGIN_MESSAGE_MAP(RestoreArchiveDlg, AbstractDialog)
    //{{AFX_MSG_MAP(RestoreArchiveDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(ID_BUTTON_RESTORE, OnRestore)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ARCHIVE_LIST, OnItemChangedArchiveList)
	ON_BN_CLICKED(IDOK, OnClose)
	ON_CBN_SELCHANGE(IDC_DRIVE_COMBO, OnSelchangeDriveCombo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COMPLETE_DLG, OnShowCompleteDlg)
    END_MESSAGE_MAP()
      

    /////////////////////////////////////////////////////////////////////////////
    // RestoreArchiveDlg message handlers
    
    BOOL RestoreArchiveDlg::OnInitDialog() 
    {
        AbstractDialog::OnInitDialog();
        
        //  Give this dialog its parent's icons.

        SetIcon( GetParent()->GetIcon( true ), TRUE );			// Set big icon
        SetIcon( GetParent()->GetIcon( false ), FALSE );		// Set small icon

        //  Display an hour-glass cursor.

        HCURSOR hOldCursor = SetCursor( LoadCursor( NULL, IDC_APPSTARTING ) );
        
        // Prepare the CListCtrl columns and fill it up.

        CDC * pDC = m_archiveList.GetDC();
        int dateWidth = pDC->GetTextExtent( _T(" 88/88/8888 ")).cx;                                     //  max date width - for the most common date format.
		int statusWidth = pDC->GetTextExtent( TAUnicodeString::CreateFromMultitByteString(RestoreArchiveHelper::RESTORABLE.c_str()).str()).cx + 15; //  max status width
        
        //  Put checkboxes down the left-hand side

        m_archiveList.SetExtendedStyle( LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT );

        LVCOLUMNW lvColumn;
        lvColumn.mask = LVCF_FMT | LVCF_IMAGE | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvColumn.fmt = LVCFMT_IMAGE; // LVCFMT_COL_HAS_IMAGES
        lvColumn.iImage = 1;
        lvColumn.iSubItem = TICK_COL;
        lvColumn.pszText = 0;
        lvColumn.cchTextMax = 0;
        lvColumn.cx = GetSystemMetrics( SM_CXICON ); 
        m_archiveList.InsertColumn( TICK_COL,  &lvColumn); 
        m_archiveList.InsertColumn( DATE_COL,  TALoadString(IDS_DATE_COL_STR).str(), LVCFMT_LEFT, dateWidth, DATE_COL);
        m_archiveList.InsertColumn( STATUS_COL,  TALoadString(IDS_STATUS_COL_STR).str(), LVCFMT_LEFT, statusWidth, STATUS_COL);

        // Populate selected drive combo box
		 std::vector<std::string> drives = m_realPModel->getCDDrives();
        for (std::vector<std::string>::iterator iter = drives.begin(); iter != drives.end(); iter++)
        {
			m_selectedDriveCombo.AddString(TAUnicodeString::CreateFromMultitByteString((*iter).c_str()).str());			
        }

        m_selectedDriveCombo.EnableWindow( (m_selectedDriveCombo.GetCount() > 1) );
        if (m_selectedDriveCombo.GetCount() > 0)
        {
            m_selectedDriveCombo.SetCurSel(0);
        }

        populateList();

        // Make sure there are files in the list.  If not, abort.
        // This is not done in populateList() because at some stage we
        // may want to use populateList() to perform a refresh, and we may
        // not want the dialog to close if the list is empty.

        if ( 0 == m_archiveList.GetItemCount() )
        {
            // populateList() will have displayed the error message already.

            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
                "Archive list is empty. Closing Restore Archive dialog." );
            AbstractDialog::EndDialog( IDABORT );
        }

		// liborm - 27 Oct 2004 - fix for TD defect #3521
		// focus the first item of the archives list box
		// when the tab key is used to change focus to the list box
		m_archiveList.SetItemState( m_archiveList.GetTopIndex(), LVIS_FOCUSED, LVIS_FOCUSED );

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }


    void RestoreArchiveDlg::OnButtonSelectAll() 
    {
        LVITEM lvItem;
        ZeroMemory( &lvItem, sizeof( LVITEM));
	    for ( int i = 0; i < m_archiveList.GetItemCount(); i++ )
        {
            if ( m_archiveList.GetItemText( i, STATUS_COL ) == RestoreArchiveHelper::RESTORABLE.c_str() )
            {
                lvItem.iItem = i;
                lvItem.mask = LVIF_STATE;
                lvItem.state = INDEXTOSTATEIMAGEMASK( 2 );
                lvItem.stateMask = LVIS_STATEIMAGEMASK;
                m_archiveList.SetItem( &lvItem );
            }
        }
    }
  

    void RestoreArchiveDlg::OnItemChangedArchiveList(NMHDR* pNMHDR, LRESULT* pResult) 
    {
        // Don't do the updating until the list has been populated.

        if ( !m_isListPopulated )
        {
            *pResult = 0;
            return;
        }

        NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

        // Don't allow items to be selected (highlighted) except by the tick boxes.

        m_archiveList.SetItemState(pNMListView->iItem, ~LVIS_SELECTED, LVIS_SELECTED);

        // Only bother if the user clicked in the tick column.

        if ( TICK_COL == pNMListView->iSubItem )            
        {
            // Make sure the tick box stays hidden if the data is not restorable.

            if ( m_archiveList.GetItemText( pNMListView->iItem, STATUS_COL ) != RestoreArchiveHelper::RESTORABLE.c_str() )
            {
                m_archiveList.SetItemState( pNMListView->iItem, INDEXTOSTATEIMAGEMASK(0), LVIS_STATEIMAGEMASK );
            }

            // If any of the items are ticked, enable the Restore button.
            // Otherwise disable it.

            UINT state = 0;
            int itemCount = m_archiveList.GetItemCount();
			int item = 0;
	        for ( ; item < itemCount; item++ )
            {
                state = m_archiveList.GetItemState( item, LVIS_STATEIMAGEMASK );
                state >>= 13;
                if( 1 == state )
                {
                    break;  // At least one item is ticked.
                }
            }

            m_restoreButton.EnableWindow( item != itemCount );
        }

	    *pResult = 0;
    }

    
    LRESULT RestoreArchiveDlg::OnShowCompleteDlg(WPARAM, LPARAM)
    {
        RestoreCompleteDlg dlg;
        dlg.DoModal();
		m_realPModel->setStateManagerState(StateManager::IDLE);
		populateList();
		resetButtons();
		m_realPModel->terminateAndWait();
        return NULL;
    }


    void RestoreArchiveDlg::OnClose() 
    {
	    AbstractDialog::EndDialog( m_returnValue );
    }


    void RestoreArchiveDlg::OnCancel() 
    {
        // If we are in the middle of restoring, we have to wait for the
        // day that is currently being restored to be fully restored, before
        // we cancel.

		if ( StateManager::PROCESSING == m_realPModel->getStateManagerState())
        {
            TA_Base_Bus::TransActiveMessage userMsg;
            UINT selectedButton = userMsg.showMsgBox(IDS_UI_050052);

            try
            {
                TA_ArchiveManager::sendAuditMessage( TA_Base_Core::ArchiveAudit::RestoreArchiveCancelled );
            }
            catch( TA_Base_Core::ArchiveException& e )
            {
                LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
            }
			m_realPModel->setStateManagerState( StateManager::STOPPING );

            // Update all the items that still say RESTORABLE, to say CANCELLED.

	        for ( int item = 0; item < m_archiveList.GetItemCount(); item++ )
            {
				if( m_archiveList.GetItemText( item, STATUS_COL ) == TAUnicodeString::CreateFromMultitByteString(RestoreArchiveHelper::RESTORABLE.c_str()).str() )
                {
                    m_archiveList.SetItemText( item, STATUS_COL, TAUnicodeString::CreateFromMultitByteString(RestoreArchiveHelper::CANCELLED.c_str()).str());
                }
            }

            // Don't let the operator press Cancel again.

            m_cancelButton.EnableWindow( FALSE );
            m_returnValue = IDCANCEL;
            try
            {
                TA_ArchiveManager::sendAuditMessage( TA_Base_Core::ArchiveAudit::ArchiveRestored ); 
            }
            catch( TA_Base_Core::ArchiveException& e )
            {
                LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
				std::string messageForUser = e.what();
				TA_ArchiveManager::EMsgLevel messageLevel = TA_ArchiveManager::WARN;
                TA_Base_Bus::TransActiveMessage userMsg;
				userMsg << TAUnicodeString::CreateFromMultitByteString(messageForUser.c_str());
				UINT selectedButton = userMsg.showMsgBox(IDS_UW_050018);
                return;
            }
        }
        else
        {
            // We're not processing, so we can just cancel.

            AbstractDialog::OnCancel();
        }
    }


    void RestoreArchiveDlg::OnRestore() 
    {
        // Restoration is run in a separate thread, so we don't block the UI.

        try
        {
            TA_ArchiveManager::sendAuditMessage( TA_Base_Core::ArchiveAudit::RestoreArchiveRequest ); 
        }
        catch( TA_Base_Core::ArchiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
			std::string messageForUser = e.what();
			TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << TAUnicodeString::CreateFromMultitByteString(messageForUser.c_str());
			UINT selectedButton = userMsg.showMsgBox(IDS_UE_050128);           
            return;
        }
        m_realPModel->start();
    }


    void RestoreArchiveDlg::populateList()
    {
        // Get the dates and status' of the files that are on the DVD.
        // We don't need to know the filenames here, because we only show the dates, because
        // the user has to restore all the data for a particular date.

        CWaitCursor wait;

        try
        {
            // Get the currently selected drive letter.
            TAUnicodeString drive;
            int selection = m_selectedDriveCombo.GetCurSel();
            if (selection != CB_ERR)
            {
				m_selectedDriveCombo.GetLBText(selection, drive.str());
				m_fileDetails = m_realPModel->getAllFileDetails( drive.toMultiByteStdString() );
				if( (!m_fileDetails.empty()) && m_realPModel->hasInvalidArchiveFile())
				{
					return;
				}
            }
			else
			{
				LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
					"Restore Archive: No restore archive driver path is selected!");
			}
        }
        catch( TA_Base_Core::ArchiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
            TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::RestoreArchiveFailed, e.what() );
            
            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << e.what();
            UINT selectedButton = userMsg.showMsgBox(IDS_UE_050136);

            return;
        }
        if ( m_fileDetails.empty() )
        {
            // KT 10 Mar 04: PW #3136 - Changed '-' to '_' for operating system compatibility.

            TAUnicodeString error;
            error = TALoadString( UW050001_NO_VALID_FILENAMES );
            TA_ArchiveManager::sendAuditMessageIgnoreErrors( 
				TA_Base_Core::ArchiveAudit::RestoreArchiveFailed, error.toMultiByteStdString() ); 
            
            TA_Base_Bus::TransActiveMessage userMsg;
            UINT selectedButton = userMsg.showMsgBox(IDS_UW_050001);
			//td19031
            return;
        }

        // Display the dates and status' in the list ctrl.

        int item = 0;
        bool areSomeRestorable = false;
        m_archiveList.DeleteAllItems();
        for( RestoreArchiveHelper::FileDetailsVector::iterator it = m_fileDetails.begin(); it != m_fileDetails.end(); ++it )
        {
            item = m_archiveList.InsertItem( m_archiveList.GetItemCount(), NULL );
            m_archiveList.SetItem( item, TICK_COL, LVIF_IMAGE, 0, 1, 0, 0, 0);

            // KT 9 Mar 04: PW #3161 Converted to use time_t to remove dependence on date formatting.
			TAUnicodeString dateStr = TAUnicodeString::CreateFromMultitByteString(TA_ArchiveManager::convertTimeTToStringDate( it->date ).c_str());
			TAUnicodeString statusStr = TAUnicodeString::CreateFromMultitByteString(it->status.c_str());
            m_archiveList.SetItemText( item, DATE_COL,dateStr.str()  );
            m_archiveList.SetItemText( item, STATUS_COL, statusStr.str() );
			m_archiveList.SetItemData(item, item);
            if ( it->status != RestoreArchiveHelper::RESTORABLE )
            {
                m_archiveList.SetItemState( item, INDEXTOSTATEIMAGEMASK(0), LVIS_STATEIMAGEMASK );
            }
            else
            {
                m_archiveList.SetItemState( item, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK );
                areSomeRestorable = true;
            }
        }

        // Only enable the SelectAll button if there are some files that are restorable.

        if( areSomeRestorable )
        {
            m_selectAllButton.EnableWindow( TRUE );
        }

        m_isListPopulated = true;
    }


    void RestoreArchiveDlg::disableAllButCancelButton()
    {
        m_archiveList.EnableWindow( FALSE );
        m_selectAllButton.EnableWindow( FALSE );
        m_restoreButton.EnableWindow( FALSE );
        m_selectedDriveCombo.EnableWindow( FALSE );
        m_cancelButton.SetFocus();
        m_selectAllButton.SetButtonStyle( BS_PUSHBUTTON );
        m_restoreButton.SetButtonStyle( BS_PUSHBUTTON );
        m_cancelButton.SetButtonStyle( BS_DEFPUSHBUTTON );
    }


    void RestoreArchiveDlg::resetButtons()
    {
        m_archiveList.EnableWindow( TRUE );
        m_selectAllButton.EnableWindow( TRUE );

        // If any of the items are ticked, enable the Restore button.
        // Otherwise disable it.

        UINT state = 0;
        int itemCount = m_archiveList.GetItemCount();
		int item = 0;
	    for ( ; item < itemCount; item++ )
        {
            state = m_archiveList.GetItemState( item, LVIS_STATEIMAGEMASK );
            state >>= 13;
            if( 1 == state )
            {
                break;  // At least one item is ticked.
            }
        }

        if ( item == itemCount )
        {
            m_restoreButton.EnableWindow( FALSE );
            m_cancelButton.SetFocus();
        }
        else
        {
            m_restoreButton.EnableWindow( TRUE );
            m_cancelButton.SetFocus();
        }
		m_cancelButton.EnableWindow( TRUE );
    }


    void RestoreArchiveDlg::enableCloseButton()
    {
        m_closeButton.ShowWindow( TRUE );
        m_closeButton.SetFocus();
        m_cancelButton.ShowWindow( FALSE );
        m_restoreButton.ShowWindow( FALSE );
        m_selectAllButton.ShowWindow( FALSE );
        m_closeButton.EnableWindow( TRUE );
        m_closeButton.SetButtonStyle( BS_DEFPUSHBUTTON );
    }   

    void RestoreArchiveDlg::OnSelchangeDriveCombo() 
    {
        populateList();	 
    }

	void RestoreArchiveDlg::runThread()
	{
		// Make sure no exceptions escape from the run() method.
        try
        {
			m_realPModel->setStateManagerState( StateManager::PROCESSING );

            //  Display an hour-glass cursor.

            HCURSOR hOldCursor = SetCursor( LoadCursor( NULL, IDC_APPSTARTING));

            // Disable everything except Cancel and change the highlighting from
            // the Restore button to the Cancel button.

            disableAllButCancelButton();

            // Get all the ticked items (the ones to restore).

            RestoreArchiveHelper::DatesIndexMap restoredDatesMap;
            TAUnicodeString volumeLabel( _T("") );
            bool isComplete = getTickedItems( restoredDatesMap, volumeLabel );
            if( !isComplete )                                                   // The user cancelled.
            {        
                enableCloseButton();
				m_realPModel->setStateManagerState(StateManager::IDLE);
                return;
            }

            // If there are no files to restore, display a message box.

            if ( restoredDatesMap.empty() )
            {
                TA_Base_Bus::TransActiveMessage userMsg;
                UINT selectedButton = userMsg.showMsgBox(IDS_UI_050051);

                resetButtons();
				m_realPModel->setStateManagerState( StateManager::IDLE );
                return;
            }

            // Restore the archive files into the database.  This will be done
            // one complete day at a time.

            try
            {
				isComplete = restoreFiles( restoredDatesMap, volumeLabel );
                if ( !isComplete )                                              // The user cancelled.
                {
                    // Don't return here, because we should still process/clean up the log files so far.
                }
            }
            catch( TA_Base_Core::ArchiveException& e )
            {
                LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
                
                TA_Base_Bus::TransActiveMessage userMsg;
                userMsg << e.what();
                UINT selectedButton = userMsg.showMsgBox(IDS_UE_050135);

                // Don't return here, because we should still process/clean up the log files so far.
                if( m_returnValue != IDCANCEL )
                {
                    TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::RestoreArchiveFailed, e.what() ); 
                    m_returnValue = IDABORT;
                }
            }   

            // This next bit HAS to be done after restoreFiles has been called, so disable
            // the cancel button, so the operator can't cancel during this process.
            // (Even if they do somehow, a message box will be displayed, then the cancellation
            // will be ignored until this finishes).

            m_cancelButton.EnableWindow( FALSE );
        
            // This will delete all the log files if every import was successful.
            // It will display the Restore Complete dialog if there were errors/warnings during the restore.
            // Either way, this method will set the state to IDLE and enable the close button.

           processLogFiles();
        }
        catch( TA_Base_Core::TransactiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "TransactiveException", e.what() ); 
        }
        catch( ... )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "Unknown Exception", "Unknown exception caught in RestoreArchiveDlg::run()" ); 
        }
	}

	bool RestoreArchiveDlg::getTickedItems( RestoreArchiveHelper::DatesIndexMap& restoredDatesMap, TAUnicodeString& volumeLabel )
    {
        // Find the items that are ticked.
        UINT state = 0;
        int itemCount = m_archiveList.GetItemCount();
	    for ( int item = 0; item < itemCount; item++ )
        {
			if ( StateManager::STOPPING == m_realPModel->getStateManagerState() )
            {
                return false;
            }

            // Start setting up the volume label to be passed to updateInventory later on.

            if ( 0 == item )
            {
                volumeLabel.str() = m_archiveList.GetItemText( item, DATE_COL );
            }
            else if ( itemCount == item + 1 )
            {
                volumeLabel.str() += _T(" To ");
                volumeLabel.str() += m_archiveList.GetItemText( item, DATE_COL );
            }

            //  Determine the ticked items.
            
            state = m_archiveList.GetItemState( item, LVIS_STATEIMAGEMASK );
            state >>= 13;
            if( ( 1 == state ) && ( m_archiveList.GetItemText( item, STATUS_COL ) == RestoreArchiveHelper::RESTORABLE.c_str() ) )
            {
                //restoredDates.push_back( m_fileDetails[ item ].date );
				restoredDatesMap[item] = m_fileDetails[ item ].date;
            }
        }
        return true;
    }


	bool RestoreArchiveDlg::restoreFiles( const RestoreArchiveHelper::DatesIndexMap& restoredDatesMap
		, const TAUnicodeString& volumeLabel )
    {
        if( StateManager::STOPPING == m_realPModel->getStateManagerState() )
        {
            return false;
        }        

        // Restore the files.

        for( RestoreArchiveHelper::DatesIndexMap::const_iterator it = restoredDatesMap.begin(); it != restoredDatesMap.end(); ++it )
        {
            if( StateManager::STOPPING == m_realPModel->getStateManagerState() )
            {
                return false;
            }

            // Find the restored date in the list.  Note: Both the restoredDates vector 
            // and the list are in date order.
			m_archiveList.SetItemText( it->first, STATUS_COL, TAUnicodeString::CreateFromMultitByteString(RestoreArchiveHelper::IMPORTING.c_str()).str());
        
            try
            {
                // KT 27 Feb 04: PW #3133: This section has been changed so that users can remove
                // an individual days worth of restored data from the database.
                // KT 9 Mar 04: PW #3161 Converted to use time_t to remove dependence on date formatting.
				m_realPModel->restoreFiles(it->second,volumeLabel.toMultiByteStdString());
                m_archiveList.SetItemText( it->first, STATUS_COL, TAUnicodeString::CreateFromMultitByteString(RestoreArchiveHelper::COMPLETE.c_str()).str() );
            }
            catch( TA_Base_Core::ArchiveException& )
            {
                // Don't remove the AR_INVENTORY inventory table entry for this date, 
                // because we may have got partway through the restoration already.

                // This will be further dealt with outside this method.

                m_archiveList.SetItemText( it->first, STATUS_COL, TAUnicodeString::CreateFromMultitByteString(RestoreArchiveHelper::RESTORE_FAILED.c_str()).str());
                throw;
            }
        }
        return true;
    }    
	
	void RestoreArchiveDlg::processLogFiles()
	{
		bool bReturn = true;
		try
		{
			bReturn = m_realPModel->processLogFiles();
		}
		catch( TA_Base_Core::ArchiveException& e )
		{
			// The error message contains a list of the files that couldn't be deleted and the reasons.
			LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg<<e.what();
			userMsg.showMsgBox(IDS_UE_050138);
		}

		if(bReturn)
		{
			enableCloseButton();
		}
		else 
		{
			// They don't all contain successes, so show the dialog.
            // Do this by sending a message, so the windows thread handles the dialog.
            // If you don't do this, the thread shows the dialog and then aborts and
            // it debug asserts.
			PostMessage( WM_COMPLETE_DLG, 0, 0);	
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////
	/// AbstractTransActiveView member functions

	std::string RestoreArchiveDlg::getMyPModelName()
	{
		return RESTORE_ARCHIVE_DLG_PMODEL;
	}

	void RestoreArchiveDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<RestoreArchivePModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel has been initialized successfully");
		m_realPModel->attachView(this, THREAD_FUNCTION_INVOKE.c_str());	
		m_realPModel->attachView(this, SHOW_USER_MESSAGE_NOTIFY.c_str());	
	}

	void RestoreArchiveDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		std::string type(updateType);
		if( THREAD_FUNCTION_INVOKE == type)
		{
			runThread();
		}
		else if(SHOW_USER_MESSAGE_NOTIFY == type)
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

} // TA_Base_App

