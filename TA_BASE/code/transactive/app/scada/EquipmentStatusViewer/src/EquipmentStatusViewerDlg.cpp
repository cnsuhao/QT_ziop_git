/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/EquipmentStatusViewerDlg.cpp $
 * @author:  Rod Rolirad
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/09/03 14:11:12 $
 * Last modified by:  $Author: CM $
 * 
 * Dialog class of the main dialog responsible for displaying the equipment status list,
 * location, report names, toolbar, status bar etc
 *
 */

#include "app/scada/EquipmentStatusViewer/src/stdafx.h"
#include "app/scada/EquipmentStatusViewer/src/EquipmentStatusViewer.h"
#include "app/scada/EquipmentStatusViewer/src/EquipmentStatusViewerDlg.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/CustomDefines.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentItemEventArg.h"
#include "app/scada/EquipmentStatusViewer/src/FileUtility.h"
#include "app/scada/EquipmentStatusViewer/src/ReportCommentDialog.h"
#include "app/scada/EquipmentStatusViewer/src/resource.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/mfc_extensions/src/print_list_ctrl/ListCtrlPrint.h"
#include "bus/mfc_extensions/src/file_dialog_no_folder_change/FileDialogArialDisableFolders.h"
#include <afxpriv.h>


namespace TA_Base_App
{
 	CEquipmentStatusViewerDlg::CEquipmentStatusViewerDlg(TA_Base_Bus::ITransActiveWinApp * pWinApp)
		:
		TA_Base_Bus::AbstractTransActiveDialog(pWinApp, CEquipmentStatusViewerDlg::IDD),
		m_realPModel(NULL),
		m_needSortListCtrl(false),
		m_EquipmentStatusList(pWinApp)
	{
		TA_Base_Bus::ViewResizeProperty properties;
		properties.canMaximise	= true;
		properties.maxHeight	= -1;
		properties.maxWidth		= -1;
		properties.minWidth		= 670;
		properties.minHeight	= 480;
		m_resizeViewHelper->setViewResizeProperty(properties);
	}

	CEquipmentStatusViewerDlg::~CEquipmentStatusViewerDlg()
	{ 
		
	}

	void CEquipmentStatusViewerDlg::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CEquipmentStatusViewerDlg)
	    DDX_Control(pDX, IDC_STOP, m_StopButton);
		DDX_Control(pDX, IDC_LOCATION_COMBO, m_Location);
		DDX_Control(pDX, IDC_EQUIPMENT_LIST, m_EquipmentStatusList);
		DDX_Control(pDX, IDC_REPORT_COMBO, m_ReportName);
	    //}}AFX_DATA_MAP
	}

	BEGIN_MESSAGE_MAP(CEquipmentStatusViewerDlg, CDialog)
		//{{AFX_MSG_MAP(CEquipmentStatusViewerDlg)

		ON_COMMAND(ID_VIEW_STATUSBAR, OnViewStatusbar)
		ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
		ON_COMMAND(ID_FILE_EXPORT, OnExport)
		ON_COMMAND(ID_FILE_PRINT, OnPrint)
		ON_COMMAND(ID_TOOLBAR_EXPORT, OnExport)
		ON_COMMAND(ID_TOOLBAR_PRINT1, OnPrint)
		ON_COMMAND(IDEXIT, OnExit)
		ON_COMMAND(ID_HELP_EQUIPMENTSTATUSVIEWERHELP, OnHelpEquipmentstatusviewerhelp)
		ON_COMMAND(ID_HELP_ABOUTEQUIPMENTSTATUSVIEWER, OnHelpAboutequipmentstatusviewer)
		ON_COMMAND(ID_TOOLBAR_HELP1, OnToolbarHelp)
		ON_NOTIFY(LVN_COLUMNCLICK, IDC_EQUIPMENT_LIST, OnColumnClickEquipmentList)
		ON_CBN_SELCHANGE(IDC_LOCATION_COMBO, OnSelchangeLocationCombo)
		ON_MESSAGE(WM_LIST_ITEM_UPDATE, OnListItemUpdate)
		ON_MESSAGE(WM_SESSION_CHANGE, OnSessionIdChange) 
		ON_MESSAGE(WM_SORT_LIST_CTRL, OnSortListCtrl)
		ON_MESSAGE(WM_REPOPULATE_ITEM, OnRepopulateItem)
		ON_MESSAGE(WM_TASETLANG, OnSetLanguage)
		ON_WM_SHOWWINDOW()
		ON_WM_CLOSE()
		ON_WM_SIZE()
	    //}}AFX_MSG_MAP
		ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
		ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	END_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////////////////
	// CEquipmentStatusViewerDlg message handlers

	static UINT indicators[] =
	{
		ID_PANE_0,          // status line indicator
	    ID_PANE_1,
		ID_PANE_2
	};

	BOOL CEquipmentStatusViewerDlg::OnInitDialog()
	{
		AbstractTransActiveDialog::OnInitDialog();

		HICON appIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
		SetIcon(appIcon, TRUE);
		HICON buttonIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_STOP), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
		if (buttonIcon)
		{
			m_StopButton.SetIcon(buttonIcon);
		}

		/*CreateStatusBar();
		CreateToolbar();

		CalculateToolBarStatusBarHeights();
		EnableToolTips(TRUE);

		m_WndToolBar.ShowWindow(SW_SHOW);
		m_WndStatusBar.ShowWindow(SW_SHOW);*/

		return TRUE; 
	}

	BOOL CEquipmentStatusViewerDlg::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
	{			
		ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

		// to be thorough we will need to handle UNICODE versions of the message also !!
		TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
		TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
		TCHAR szFullText[512];
		TAUnicodeString strTipText;
		UINT nID = pNMHDR->idFrom;

		if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
			pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
		{
			// idFrom is actually the HWND of the tool 
			nID = ::GetDlgCtrlID((HWND)nID);
		}

		if ((nID == ID_TOOLBAR_EXPORT) || (nID == ID_TOOLBAR_PRINT) ||
			(nID == ID_TOOLBAR_PRINT1) || (nID == ID_TOOLBAR_INSPECT) ||
			(nID == ID_TOOLBAR_FILTER) || (nID == ID_TOOLBAR_HELP) ||
			(nID == ID_TOOLBAR_HELP1))
		{
			AfxLoadString(nID, szFullText);
			strTipText = szFullText;

#ifndef _UNICODE
			if (pNMHDR->code == TTN_NEEDTEXTA)
			{
				lstrcpyn(pTTTA->szText, const_cast<TCHAR*>(strTipText.GetString()), sizeof(pTTTA->szText));
			}
			else
			{
				_mbstowcsz(pTTTW->szText, const_cast<TCHAR*>(strTipText.GetString()), sizeof(pTTTW->szText));
			}
#else
			if (pNMHDR->code == TTN_NEEDTEXTA)
			{
				_wcstombsz(pTTTA->szText, const_cast<TCHAR*>(strTipText.GetString()) ,sizeof(pTTTA->szText));
			}
			else
			{
				lstrcpyn(pTTTW->szText, const_cast<TCHAR*>(strTipText.GetString()), sizeof(pTTTW->szText));
			}
#endif

			*pResult = 0;

			// bring the tooltip window above other popup windows
			::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
				SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

			return TRUE;
		}

		return FALSE;
	}

	void CEquipmentStatusViewerDlg::CreateToolbar()
	{
		// Create toolbar at the top of the dialog window
		if (!m_WndToolBar.CreateEx(this, TBSTYLE_FLAT , WS_CHILD | WS_VISIBLE | CBRS_TOP | 
										CBRS_TOOLTIPS | CBRS_FLYBY ) ||
			!m_WndToolBar.LoadToolBar(IDR_MAINTOOLBAR2))
		{
			TRACE0("Failed to create toolbar\n");
		}

		TAUnicodeString txtPrint = TALoadString(STRING_ID_EDSV_0002);
		AddTextToButton(ID_TOOLBAR_PRINT1,	txtPrint);

		TAUnicodeString txtHelp = TALoadString(STRING_ID_EDSV_0003);
		AddTextToButton(ID_TOOLBAR_HELP1,	txtHelp);

		TATranslateToolBar( IDR_MAINTOOLBAR2, &m_WndToolBar );
	}

	void CEquipmentStatusViewerDlg::CreateStatusBar()
	{
		if (!m_WndStatusBar.CreateEx(this, SBT_TOOLTIPS, WS_CHILD | WS_VISIBLE  | CBRS_BOTTOM | CBRS_RIGHT, AFX_IDW_STATUS_BAR) ||
			!m_WndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(indicators[0])))
		{
			TRACE0("Failed to create status bar\n");
		}

		m_WndStatusBar.SetPaneInfo(0, m_WndStatusBar.GetItemID(0), SBPS_STRETCH | SBPS_POPOUT | SBPS_NOBORDERS, NULL);
		m_WndStatusBar.SetPaneInfo(1, m_WndStatusBar.GetItemID(1), SBPS_NORMAL, 84);
		m_WndStatusBar.SetPaneInfo(2, m_WndStatusBar.GetItemID(2), SBPS_NORMAL, 120);

		TAUnicodeString txtnull(_T(""));
		SetStatusBarText(0, txtnull);
		SetStatusBarText(1, txtnull);
		SetStatusBarText(2, txtnull);
	}

	void CEquipmentStatusViewerDlg::AddTextToButton(UINT buttonId, TAUnicodeString buttonLabel )
	{
		int index = m_WndToolBar.CommandToIndex(buttonId);
		if( index >=0 )
		{
			m_WndToolBar.SetButtonText(index, buttonLabel.GetString());
		}
	}

	void CEquipmentStatusViewerDlg::OnViewToolbar() 
	{
		bool isVisible = (m_WndToolBar.IsWindowVisible()==TRUE);
		m_WndToolBar.ShowWindow(isVisible ? SW_HIDE : SW_SHOW);
		
 		GetMenu()->CheckMenuItem(ID_VIEW_TOOLBAR,isVisible ? MF_UNCHECKED : MF_CHECKED);
		RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);

		Resize();
	}

	void CEquipmentStatusViewerDlg::OnViewStatusbar() 
	{
		bool isVisible = (m_WndStatusBar.IsWindowVisible() == TRUE);
		m_WndStatusBar.ShowWindow(isVisible ? SW_HIDE : SW_SHOW);
 		
		GetMenu()->CheckMenuItem(ID_VIEW_STATUSBAR,isVisible ? MF_UNCHECKED : MF_CHECKED);
		RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);

		Resize();
	}


	void CEquipmentStatusViewerDlg::OnExport() 
	{
		CWaitCursor waitCursor;

		TAUnicodeString fileExtension(_T(".csv"));
		TAUnicodeString fileName = TALoadString(STRING_ID_EDSV_0015);

		TAUnicodeString m_defaultExportPath;
		m_defaultExportPath.fromMultiByteString(m_realPModel->getDefaultExportPath().c_str());

	    if (m_defaultExportPath.Right(1).Compare(_T("/")) == 0 || m_defaultExportPath.Right(1).Compare(_T("\\")) == 0)
		{
        
			m_defaultExportPath = m_defaultExportPath.Left( m_defaultExportPath.GetLength()-1);
		}

		// Show print dialog for user to choose a file name
		TA_Base_Bus::FileDialogArialDisableFolders exportFileDlg(FALSE,  // We want a Save dialog not an Open one
					fileExtension.GetString(),				// The default file extension
					fileName.GetString(),					// This is the initial path and file name
					OFN_HIDEREADONLY |			// Hide the readonly check box
					OFN_OVERWRITEPROMPT |		// Prompt to ask the user if they want to overwrite existing files
					OFN_PATHMUSTEXIST |			// Check that the path entered is correct and warn if it is not
					OFN_NOCHANGEDIR,
					_T("CSV (Comma delimited) (*.csv)|*.csv||"),	// Only allow them to save in csv format
					AfxGetMainWnd());			  // Parent window is the actual alarm viewer

		TAUnicodeString lpstrTitle = TALoadString(STRING_ID_EDSV_0016);
		exportFileDlg.m_ofn.lpstrTitle = lpstrTitle.GetString();
		// Open the file read-only. Create and set size to 0. Will overwrite existing files.
		if (m_defaultExportPath.IsEmpty()) // Path set in configuration is empty
		{
			TCHAR szMaxpath[MAX_PATH];

		    if (SHGetSpecialFolderPath(NULL, szMaxpath, CSIDL_PERSONAL, FALSE))
			{
				m_defaultExportPath = szMaxpath;
				exportFileDlg.m_ofn.lpstrInitialDir = m_defaultExportPath.GetString();
			}

		}

		TAUnicodeString pathName =  m_defaultExportPath + fileName;
		
		CFile outFile;
		// Check the path and file name given for availability
		if ( 0 == outFile.Open(pathName.GetString(), CFile::modeCreate | CFile::modeReadWrite) )
		{

			TCHAR szMaxpath[MAX_PATH];

			if (SHGetSpecialFolderPath(NULL, szMaxpath, CSIDL_PERSONAL, FALSE))
			{
				m_defaultExportPath = szMaxpath;
				exportFileDlg.m_ofn.lpstrInitialDir = m_defaultExportPath.GetString();

			}
		}
		else
		{
			exportFileDlg.m_ofn.lpstrInitialDir = m_defaultExportPath.GetString();
			// remove the file if it is created
			if (outFile != NULL) 
			{	
				outFile.Close();
				CFile::Remove(pathName.GetString());
			}
		}


		TAUnicodeString locationName, reportName;

		m_Location.GetWindowText(locationName.str());
		m_ReportName.GetWindowText(reportName.str());

		// show the comment dialog
		CReportCommentDialog commentDlg;

		if (commentDlg.DoModal() == IDOK)
		{
            // Get the export file path.
            if (IDOK == exportFileDlg.DoModal())
			{
                TAUnicodeString fileNameFullPath;
				fileNameFullPath.str() = exportFileDlg.GetPathName();
				CFileUtility fileUtil;

				fileUtil.SetFileName(fileNameFullPath);
				fileUtil.SetLocationName(locationName);
				fileUtil.SetReportName(reportName);
				fileUtil.SetReportComment(commentDlg.GetComment());

				if (!fileUtil.ExportToCSV((CListCtrl *)&m_EquipmentStatusList))
				{
                    TA_Base_Bus::TransActiveMessage userMsg;
                    userMsg.showMsgBox(IDS_UE_083006);
                }
                else
				{
                    TA_Base_Bus::TransActiveMessage userMsg;
                    userMsg << fileNameFullPath;
                    userMsg.showMsgBox(IDS_UI_083007);
				}
   			}
		}
	}

	void CEquipmentStatusViewerDlg::OnPrint() 
	{
		CWaitCursor waitCursor;
		CReportCommentDialog dlg;

		if (dlg.DoModal() != IDOK)
		{
			return;
		}

		TAUnicodeString locDisplayName, repName;

		m_Location.GetWindowText(locDisplayName.str());
		m_ReportName.GetWindowText(repName.str());

		std::string selectReportKey = m_realPModel->getSelectReportKey();
		if ( ( ACTIVE_PROFILE_ALL_REPORT ==  selectReportKey) ||
			( ACTIVE_PROFILE_MMS_REPORT == selectReportKey ) )
		{
			locDisplayName = TAUnicodeString::CreateFromMultitByteString(m_realPModel->getCurSessionLocDisplayName().c_str());
		}

		//we need set default sort(sort by Assert column and )
		bool bPreAssertSortOrder = m_EquipmentStatusList.GetSortOrder(ASSET_COLUMN);
		int  bPreSortColumn = m_EquipmentStatusList.getCurrentSortCol();
		bool bPreSortColumnSortOrder = m_EquipmentStatusList.GetSortOrder(bPreSortColumn);
		bool bReSorted = false;
		if(bPreSortColumn != ASSET_COLUMN || bPreAssertSortOrder != true)
		{
			bReSorted = true;
			m_EquipmentStatusList.SetSortOrder(ASSET_COLUMN, true);
			m_EquipmentStatusList.SortColumn(ASSET_COLUMN, false);
		}

		CListCtrlPrint cJob;
		cJob.pList = &m_EquipmentStatusList;

		// Set the header information.
		// ListCtrlPrint uses DT_WORDBREAK in CDC::DrawText() to enable multiple lines
		TAUnicodeString header = TALoadString(STRING_ID_EDSV_0017);
		header += TAUnicodeString(CTime::GetCurrentTime().Format(_T("%a %#d/%#m/%Y %#H:%M:%S"))); 
		header += TAAfxFormatString3(STRING_ID_EDSV_0018, repName.GetString(), 
			locDisplayName.GetString(), dlg.GetComment().GetString());
		cJob.csPageHead = header.c_str();
		cJob.csPage = TALoadString(STRING_ID_EDSV_0019).c_str();

		//	At present, the ES Report is monochrome, so the CellColourMap needs to be
		//  the same size as the list control but all cells are just black on white.
		TA_Base_Bus::CellColourMap colourMap( ESL_COLUMN_MAX, m_EquipmentStatusList.GetItemCount(),
			0,  //  the foreground colour: black
			m_EquipmentStatusList.GetBkColor() );
		cJob.pColourMap = &colourMap;

		// create a new col width array for print job to process the width
		int * colWidths = new int [ ESL_COLUMN_MAX+1];
		for (int i=0 ; i< ESL_COLUMN_MAX; i++ )
		{
			int wd = m_EquipmentStatusList.GetColumnWidth( i);

			if (wd <= 0)
			{   //  Hidden column
				if ( QUALITY_COLUMN == i)
				{
					wd = m_EquipmentStatusList.GetColumnWidth( VALUE_COLUMN);
				}
				else
				{
					if ( ACTIVE_PROFILE_MMS_REPORT == selectReportKey )
					{
						//  Keep the other hidden columns hidden by making them very narrow
						wd = 1;
					}
				}
			}
			colWidths[i+1] = wd;
		}
		cJob.pColWidthArray = colWidths;

		cJob.OnFilePrint();
		if(bReSorted)
		{
			m_EquipmentStatusList.SetSortOrder(ASSET_COLUMN, bPreAssertSortOrder);
			m_EquipmentStatusList.SetSortOrder(bPreSortColumn, bPreSortColumnSortOrder);
			m_EquipmentStatusList.SortColumn(bPreSortColumn, false);
		}

		// write audit message
		std::string reportName(repName.toMultiByteStdString());
		std::string locationName(locDisplayName.toMultiByteStdString());
		m_realPModel->logAuditMessage(reportName, locationName, EAM_PRINT_REPORT);
	}

	void CEquipmentStatusViewerDlg::OnColumnClickEquipmentList(NMHDR* pNMHDR, LRESULT* pResult) 
	{
        CWaitCursor waitCursor;
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

		if (m_EquipmentStatusList.GetItemCount() > 0)
		{
			int nColumnClickedOn = pNMListView->iSubItem;

			m_EquipmentStatusList.SetSortOrder(nColumnClickedOn, !m_EquipmentStatusList.GetSortOrder(nColumnClickedOn));
			m_EquipmentStatusList.SortColumn(nColumnClickedOn);
		}

		*pResult = 0;
	}

	void CEquipmentStatusViewerDlg::PopulateLocations()
	{		
		m_Location.ResetContent();
		
		std::map<unsigned long, std::string> locs;
		m_realPModel->populateLocation(locs);

		int maxTextLen = 0;
		CDC* locationDc = m_Location.GetDC();
	
		std::map<unsigned long, std::string>::iterator itLoc;
		for (itLoc = locs.begin(); itLoc != locs.end(); ++itLoc)
		{
			unsigned long locKey    = itLoc->first;
			TAUnicodeString  displayName = TAUnicodeString::CreateFromMultitByteString(itLoc->second.c_str());
			int index = m_Location.AddString(displayName.GetString());
			m_Location.SetItemData(index, locKey);
			if (locationDc != NULL)
			{
				int textLen = locationDc->GetTextExtent(displayName.str()).cx;
				if (textLen > maxTextLen)
				{
					maxTextLen = textLen;
				}
			}
		}		

		if (locationDc != NULL)
		{
			int spaceTextLen = locationDc->GetTextExtent(_T(" ")).cx * 2;
			m_Location.SetDroppedWidth(maxTextLen + spaceTextLen);
			m_Location.ReleaseDC(locationDc);
			locationDc = NULL;
		}
	}

	void CEquipmentStatusViewerDlg::PopulateEquipmentStatusList(unsigned long locationKey)
    {
        CWaitCursor waitCursor;
        SetStatusBarText(2, _T(""));
		AdjustGUIWhileLoading(false);

		m_realPModel->populateEquipmentStatusList(locationKey);

		AdjustGUIWhileLoading(true);
	}

	LRESULT CEquipmentStatusViewerDlg::OnSessionIdChange(WPARAM wParam, LPARAM lParam)
	{
		LRESULT result(0);
		CWaitCursor waitCursor;

		m_EquipmentStatusList.DeleteAllItems();
		m_ReportName.ResetContent();
		PopulateLocations();
		AdjustGUIAccrodingPermission();

		return result;
	}

	LRESULT CEquipmentStatusViewerDlg::OnListItemUpdate(WPARAM wParam, LPARAM lParam)
	{
        CWaitCursor waitCursor;
		LRESULT result(0);

		EquipmentItemUpdateEvent* pEvent = (EquipmentItemUpdateEvent*)(wParam);      
		if (pEvent != NULL)
		{
			result = m_EquipmentStatusList.UpdateInList(pEvent);
			delete pEvent;
		}

        UpdateStatusBar();

		//here we use post message instead of calling m_EquipmentStatusList.SortColumn()
		//directly to avoid too many call sorting m_EquipmentStatusList, it is a better way
		//when a sudden Item Update storm appear.
        if (m_EquipmentStatusList.getCurrentSortCol() != ESL_COLUMN_MAX && !m_needSortListCtrl)
        {
			m_needSortListCtrl = true;
			PostMessage(WM_SORT_LIST_CTRL);
        }

        return result;
	}

	LRESULT CEquipmentStatusViewerDlg::OnRepopulateItem(WPARAM wParam, LPARAM lParam)
	{
		LRESULT result(0);
		CWaitCursor waitCursor;
		m_EquipmentStatusList.DeleteAllItems();
		m_EquipmentStatusList.prepareColumns();
		return result;
	}

	LRESULT CEquipmentStatusViewerDlg::OnSortListCtrl(WPARAM wParam, LPARAM lParam)
	{
		LRESULT result(0);
		CWaitCursor waitCursor;
		m_EquipmentStatusList.SortColumn(m_EquipmentStatusList.getCurrentSortCol());
		m_needSortListCtrl = false;
		return result;
	}

	LRESULT CEquipmentStatusViewerDlg::OnSetLanguage(WPARAM wParam, LPARAM lParam)
	{
		// Translate Menu
		// Translate ToolBar
		// Do something special
		return AbstractDialog::OnSetLanguage( wParam, lParam );
	}

	void CEquipmentStatusViewerDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
	{
		AbstractTransActiveDialog::OnShowWindow(bShow, nStatus);

		Resize();
    }

	void CEquipmentStatusViewerDlg::OnHelpEquipmentstatusviewerhelp() 
	{
		TA_Base_Bus::HelpLauncher::getInstance().displayHelp();
	}

	void CEquipmentStatusViewerDlg::OnToolbarHelp() 
	{
		OnHelpEquipmentstatusviewerhelp();
	}

	void CEquipmentStatusViewerDlg::CalculateToolBarStatusBarHeights()
	{
		CRect rcClientStart;
		CRect rcClientNow;

		GetClientRect(rcClientStart);
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);

		m_ToolBarHeight   = rcClientNow.top;
		m_StatusBarHeight = rcClientStart.bottom - rcClientNow.bottom;
	}

	void CEquipmentStatusViewerDlg::OnSelchangeLocationCombo() 
	{
        CWaitCursor waitCursor;
		int curSel = m_Location.GetCurSel(); 
		if (curSel != CB_ERR)
		{
			unsigned long aSelectedLocationKey = m_Location.GetItemData(curSel);
			m_realPModel->populateEquipmentStatusList(aSelectedLocationKey);
		}
	}

	void CEquipmentStatusViewerDlg::OnExit() 
	{
		OnClose();        
	}

	void CEquipmentStatusViewerDlg::OnHelpAboutequipmentstatusviewer() 
	{
		TA_Base_Bus::HelpLauncher::getInstance().displayAboutBox();
	}

	void CEquipmentStatusViewerDlg::SetStatusBarText(int aIndex, const TAUnicodeString &message)
	{
		CDC *dc = m_WndStatusBar.GetDC();

		if (dc != NULL)
		{
			int width; 
			unsigned int a, b;

			m_WndStatusBar.GetPaneInfo(aIndex, a, b, width);

			int textExtent		= dc->GetTextExtent(message.GetString()).cx;
			int spaceExtent		= dc->GetTextExtent(_T(" ")).cx;
			int numSpacesNeeded = ((width - textExtent) / 2) / spaceExtent;

			TAUnicodeString spaceStr;
			for (int i = 1; i <= numSpacesNeeded + 2; i++)
				spaceStr += _T(" ");
			spaceStr += message;

			m_WndStatusBar.SetPaneText(aIndex, spaceStr.GetString());

			m_WndStatusBar.ReleaseDC(dc);
			dc = NULL;
		}
	}

	void CEquipmentStatusViewerDlg::AdjustGUIAccrodingPermission()
	{
		if (this->m_hWnd == NULL) return;

		//int repNameIndex = m_ReportName.GetCurSel();

		// get a reference to the toolbar control
		CToolBarCtrl &tlbCtrl = m_WndToolBar.GetToolBarCtrl();

		// get a pointer to the menu
		CMenu *menu = GetMenu();

		// if there is no report name selected
		//if (repNameIndex == CB_ERR)
		//{
		//	tlbCtrl.EnableButton(ID_TOOLBAR_PRINT1, FALSE);
		//}	
		//else
		//{
			if (m_realPModel->isAllowPrintOrExport())
			{
				menu->EnableMenuItem(ID_FILE_PRINT, MF_ENABLED);
				tlbCtrl.EnableButton(ID_TOOLBAR_PRINT1, TRUE);
			} 
			else
			{
				menu->EnableMenuItem(ID_FILE_PRINT, MF_GRAYED);
				tlbCtrl.EnableButton(ID_TOOLBAR_PRINT1, FALSE);
			}
		//}

		m_WndToolBar.Invalidate();
		//UpdateWindow();
	}

   	void CEquipmentStatusViewerDlg::OnClose() 
	{
		CWaitCursor waitCursor;
		m_realPModel->cleanup();
		AbstractTransActiveDialog::OnClose();
	}

	void CEquipmentStatusViewerDlg::AdjustGUIWhileLoading(bool aEnableAllGUIComponents)
	{
		CMenu *menu = GetMenu();
		CToolBarCtrl &tlbCtrl = m_WndToolBar.GetToolBarCtrl();
        UINT nEnable = aEnableAllGUIComponents?MF_ENABLED:MF_GRAYED;

        CMenu* sysMenu = GetSystemMenu(FALSE);
		sysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | nEnable);

		menu->EnableMenuItem(ID_FILE_PRINT, nEnable);
		menu->EnableMenuItem(IDEXIT, nEnable);

		tlbCtrl.EnableButton(ID_TOOLBAR_PRINT1, aEnableAllGUIComponents);

		m_Location.EnableWindow(aEnableAllGUIComponents);

        if (aEnableAllGUIComponents)
        {  
			AdjustGUIAccrodingPermission();
        }

		m_WndToolBar.Invalidate();
		UpdateWindow();
	}

	void CEquipmentStatusViewerDlg::OnSize(UINT nType, int cx, int cy) 
	{
		AbstractTransActiveDialog::OnSize(nType, cx, cy);

		if (nType != SIZE_MAXHIDE && nType != SIZE_MINIMIZED)
		{
			Resize();
		}
	}

	void CEquipmentStatusViewerDlg::Resize()
	{
		// get the client size of our main dialog window
		// this is where we need to fit in our controls.
		CRect thisRect;
		GetClientRect(&thisRect);
		int clientHeight = thisRect.bottom - thisRect.top;
		int clientWidth = thisRect.right - thisRect.left;

		// resize the menu line
        CWnd* menuLine = GetDlgItem(IDC_MENU_LINE_CHEAT);
        if (menuLine != NULL)
            menuLine->MoveWindow(0, 0, clientWidth, 2);

		BOOL m_hasToolbar;
		BOOL m_hasStatusbar;

		if (GetMenu()->GetMenuState(ID_VIEW_TOOLBAR, MF_BYCOMMAND) == MF_CHECKED)
			m_hasToolbar = true;
		else
			m_hasToolbar = false;

		if (GetMenu()->GetMenuState(ID_VIEW_STATUSBAR, MF_BYCOMMAND) == MF_CHECKED)
			m_hasStatusbar = true;
		else
			m_hasStatusbar = false;

		int comboHeight = 0;
		int comboWidth = 0;
		int GAP = 3;

		// get the location label
		CWnd *locLabel = GetDlgItem(IDC_LOC_LABEL);
		if (locLabel)
		{
			CRect locRect;
			locLabel->GetWindowRect(&locRect);
			ScreenToClient(&locRect);
			locLabel->MoveWindow(5, 
				(m_hasToolbar ? m_ToolBarHeight : 0) + (2 * GAP),
				locRect.right - locRect.left,
				locRect.bottom - locRect.top);
		}

		// get dimensions of the location combo box
		if (m_Location.m_hWnd != NULL)
		{
			CRect comboRect;
			m_Location.GetWindowRect(&comboRect);
			ScreenToClient(&comboRect);
			comboHeight = comboRect.bottom - comboRect.top;
			comboWidth = comboRect.right - comboRect.left;
			m_Location.MoveWindow(
				75, 
				(m_hasToolbar ? m_ToolBarHeight : 0) + GAP, 
				comboWidth, 
				comboHeight);
		}

		// get the report name label
		CWnd *repLabel = GetDlgItem(IDC_REPORT_LABEL);
		if (repLabel)
		{
			CRect repRect;
			repLabel->GetWindowRect(&repRect);
			ScreenToClient(&repRect);
			repLabel->MoveWindow(150,
				(m_hasToolbar ? m_ToolBarHeight : 0) + (2 * GAP),
				repRect.right - repRect.left,
				repRect.bottom - repRect.top);
		}

		// get the report name combo
		if (m_ReportName.m_hWnd != NULL)
		{
			CRect comboRect;
			m_ReportName.GetWindowRect(&comboRect);
			ScreenToClient(&comboRect);
			comboHeight = comboRect.bottom - comboRect.top;
			comboWidth = comboRect.right - comboRect.left;

			m_ReportName.MoveWindow(
				210, 
				(m_hasToolbar ? m_ToolBarHeight : 0) + GAP, 
				comboWidth, 
				comboHeight);
		}

		// get the stop button
		if (m_StopButton.m_hWnd)
		{
			CRect stopRect;
			m_StopButton.GetWindowRect(&stopRect);
			ScreenToClient(&stopRect);
			m_StopButton.MoveWindow(
				600, 
				(m_hasToolbar ? m_ToolBarHeight : 0) + GAP, 
				stopRect.right - stopRect.left, 
				stopRect.bottom - stopRect.top);
		}

		if (m_EquipmentStatusList.m_hWnd != NULL)
		{
			CRect listRect;

			m_EquipmentStatusList.GetWindowRect(&listRect);
			ScreenToClient(&listRect);
			m_EquipmentStatusList.MoveWindow(
				2, 
				(m_hasToolbar ? m_ToolBarHeight : 0) + comboHeight + (2 * GAP), 
				clientWidth - 2,  
				clientHeight - (2 * GAP) - (comboHeight + (m_hasStatusbar ? m_StatusBarHeight : 0) + (m_hasToolbar ? m_ToolBarHeight : 0)));
		}

		if (m_WndToolBar.m_hWnd && m_hasToolbar)
		{
			m_WndToolBar.MoveWindow(0, 0, clientWidth, m_ToolBarHeight);
		}

		if (m_WndStatusBar.m_hWnd && m_hasStatusbar)
		{
			m_WndStatusBar.MoveWindow(
				0,
				clientHeight - m_StatusBarHeight,
				clientWidth,
				m_StatusBarHeight);
		}

		RedrawWindow(thisRect);
	}

	void CEquipmentStatusViewerDlg::UpdateStatusBar()
	{
		// get the total after filter is applied
		int FilteredTotal = m_EquipmentStatusList.GetItemCount();

		TAUnicodeString s = _T("");
		TAUnicodeString formatStr = TALoadString(STRING_ID_EDSV_0020);
		s.str().Format(formatStr.c_str(), FilteredTotal);

		SetStatusBarText(2, s);
	}

	void CEquipmentStatusViewerDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		std::string type(updateType);

		if (type == EDSV::DOMAIN_DATA_CHANGE_SESSION_UPDATE)
		{
			PostMessage( WM_SESSION_CHANGE, 0, 0);
		}
		else if (type == EDSV::DOMAIN_DATA_CHANGE_ITEM_UPDATE)
		{
			TA_ASSERT(args != NULL, "missing item update args");
			TA_ASSERT(args->getTypeCode() == EDSV::EVENT_ARG_ITEM_UPDATE, "wrong event type code");
			EquipmentItemUpdateEvent* pArgs = dynamic_cast<EquipmentItemUpdateEvent*>(args);
			TA_ASSERT(pArgs != NULL, "dynamic convert EquipmentItemUpdateEvent failed");
			EquipmentItemUpdateEvent* pEvent = new EquipmentItemUpdateEvent(pArgs->getEntityKey(), 
				pArgs->getColUpdateFlags(), pArgs->isFiltered());
			PostMessage( WM_LIST_ITEM_UPDATE, (WPARAM)(pEvent), 0);
		}
		else if (type == EDSV::DOMAIN_DATA_CHANGE_REPOPULATE_ITEM)
		{
			PostMessage( WM_REPOPULATE_ITEM, 0, 0);
		}
	}

	void CEquipmentStatusViewerDlg::requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent)
	{

	}

	std::string CEquipmentStatusViewerDlg::getMyPModelName ()
	{
		return std::string(EDSV::PMODEL_MAIN_DIALOG);
	}

	void CEquipmentStatusViewerDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<EquipmentStatusViewerDlgModel*>(m_pModel);
		TA_ASSERT(m_realPModel != NULL, "m_pmodel can't convert to EquipmentStatusViewerDlgModel");

		m_pModel->attachView(this, EDSV::DOMAIN_DATA_CHANGE_SESSION_UPDATE.c_str());
		m_pModel->attachView(this, EDSV::DOMAIN_DATA_CHANGE_ITEM_UPDATE.c_str());
		m_pModel->attachView(this, EDSV::DOMAIN_DATA_CHANGE_REPOPULATE_ITEM.c_str());
	}

	void CEquipmentStatusViewerDlg::initUIControl()
	{
		CreateStatusBar();
		CreateToolbar();

		CalculateToolBarStatusBarHeights();
		EnableToolTips(TRUE);

		m_WndToolBar.ShowWindow(SW_SHOW);
		m_WndStatusBar.ShowWindow(SW_SHOW);

		TAAppendLangMenu( GetMenu() );
		TATranslateMenu( IDR_MENU1, GetMenu() );
		DrawMenuBar();

		AdjustGUIAccrodingPermission();
		PopulateLocations();

		std::string reportType(ACTIVE_PROFILE_ALL_REPORT);
		m_realPModel->setSelectReportKey(reportType);
		m_realPModel->clearCurrentFiltersForActive();

		m_EquipmentStatusList.SortColumn(ASSET_COLUMN);
		for (int i= 0; i < m_Location.GetCount(); i++)
		{
			if (m_Location.GetItemData(i) == m_realPModel->getCurrentLoginLocation())
			{
				m_Location.SetCurSel(i);
				break;
			}
		}
		OnSelchangeLocationCombo();
	}

} // end of namespace
