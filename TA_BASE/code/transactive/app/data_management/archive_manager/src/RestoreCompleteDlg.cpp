/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/data_management/archive_manager/src/RestoreCompleteDlg.cpp $
  * @author:  Katherine Thomson
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Displayed if there are errors during the restoration process.
  */

#include "app/data_management/archive_manager/src/stdafx.h"
#include "app/data_management/archive_manager/src/RestoreCompleteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // RestoreCompleteDlg dialog


    RestoreCompleteDlg::RestoreCompleteDlg(CWnd* pParent /*=NULL*/)
	    : AbstractDialog(dynamic_cast<TA_Base_Bus::ITransActiveWinApp*> (AfxGetApp()),RestoreCompleteDlg::IDD, pParent)
    {
	    //{{AFX_DATA_INIT(RestoreCompleteDlg)
	    //}}AFX_DATA_INIT
    }


    void RestoreCompleteDlg::DoDataExchange(CDataExchange* pDX)
    {
	    AbstractDialog::DoDataExchange(pDX);
	    //{{AFX_DATA_MAP(RestoreCompleteDlg)
	    DDX_Control(pDX, IDC_ERROR_TREE, m_errorTree);
	    //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(RestoreCompleteDlg, AbstractDialog)
	    //{{AFX_MSG_MAP(RestoreCompleteDlg)
	    //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // RestoreCompleteDlg message handlers

    BOOL RestoreCompleteDlg::OnInitDialog() 
    {
	    AbstractDialog::OnInitDialog();
        
        //  Give this dialog its parent's icons.

        SetIcon( GetParent()->GetIcon( true ), TRUE );			// Set big icon
        SetIcon( GetParent()->GetIcon( false ), FALSE );		// Set small icon

        //  Display an hour-glass cursor.

        HCURSOR hOldCursor = SetCursor( LoadCursor( NULL, IDC_APPSTARTING ) );

        // If there are no errors to report, return yes, because we want to continue.

        if ( m_realPModel->getFileStatusMap().empty() )
        {
            AbstractDialog::EndDialog( IDOK );
        }

        insertItems( TA_ArchiveManager::EX_FAIL, _T("File(s) Containing Errors" ) );
        insertItems( TA_ArchiveManager::EX_OKWARN, _T( "File(s) Containing Warnings" ) );
        insertItems( TA_ArchiveManager::UNKNOWN, _T( "File(s) Containing An Unknown Status" ) );
         
        // If there are no errors to report, return yes, because we want to continue.
       
        if ( 0 == m_errorTree.GetCount() )
        {
            AbstractDialog::EndDialog( IDOK );
        }

        return TRUE;  // return TRUE unless you set the focus to a control
	          // EXCEPTION: OCX Property Pages should return FALSE
    }

 
    void RestoreCompleteDlg::insertItems( TA_ArchiveManager::EErrorStatus status, TCHAR* rootMsg )
    {
		TA_ArchiveManager::FileStatusMap fileStatusMap = m_realPModel->getFileStatusMap();
        std::pair< TA_ArchiveManager::FileStatusMap::const_iterator, 
            TA_ArchiveManager::FileStatusMap::const_iterator > range = 
            fileStatusMap.equal_range( status ); 
        
        if( ( range.first != range.second ) && ( fileStatusMap.end() != range.first ) )
        {
            // Insert a root item using the structure. We must
            // initialize a TVINSERTSTRUCT structure and pass its
            // address to the call. 

            TVINSERTSTRUCT tvInsert;
            tvInsert.hParent = NULL;
            tvInsert.hInsertAfter = NULL;
            tvInsert.item.mask = TVIF_TEXT;
			tvInsert.item.pszText = rootMsg;
            HTREEITEM hRoot = m_errorTree.InsertItem(&tvInsert);

            for ( ; range.first != range.second; ++range.first )
            {
                // Insert the files as subitems.

                HTREEITEM hItem = m_errorTree.InsertItem(TVIF_TEXT,
                   TAUnicodeString::CreateFromMultitByteString(range.first->second.c_str()).str(), 0, 0, 0, 0, 0, hRoot, NULL);
				
				m_errorTree.EnsureVisible(hItem);
 
				// TD16728 No HScroll in the ErrorTreeCtrl

				DWORD dwFileListStyle = m_errorTree.GetStyle();
				if(dwFileListStyle & WS_HSCROLL)
				{
					CRect rcErrorTree;
					m_errorTree.GetWindowRect(&rcErrorTree);
					CRect rcDlg;
					GetWindowRect(&rcDlg);

					CDC *pDC = GetDC();
					CSize csColName = pDC->GetTextExtent( range.first->second.c_str() );
					UINT uIncrement = csColName.cx + 50 - rcErrorTree.Width();
					
					SetWindowPos(NULL, 0, 0, rcDlg.Width()+uIncrement, rcDlg.Height(), SWP_NOZORDER | SWP_NOMOVE);
					m_errorTree.SetWindowPos(NULL, 0, 0, rcErrorTree.Width()+uIncrement, rcErrorTree.Height(), SWP_NOZORDER | SWP_NOMOVE);
				}

            }
        }
    }

	///////////////////////////////////////////////////////////////////////////////////////////
	/// AbstractTransActiveView member functions

	std::string RestoreCompleteDlg::getMyPModelName()
	{
		return RESTORE_ARCHIVE_DLG_PMODEL;
	}

	void RestoreCompleteDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<RestoreArchivePModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel has been initialized successfully");
	}

	void RestoreCompleteDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
	}

    
} // TA_Base_App