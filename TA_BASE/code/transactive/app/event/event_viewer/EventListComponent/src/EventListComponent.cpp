/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/event/event_viewer/EventListComponent/src/EventListComponent.cpp $
 * @author:  Nick Jardine
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2015/10/09 15:42:35 $
 * Last modified by:  $Author: CM $
 * 
 * EventListComponent is the main list class. It is a subclass of CListCtrl, and is responsible for controlling the display
 * of list data, and providing functions to the main application such as print, sort, and export.
 */

#include "app/event/event_viewer/EventListComponent/src/stdafx.h"
#include "app/event/event_viewer/EventListComponent/src/EventListComponent.h"

#include  "app/event/event_viewer/pmodel/src/ConstantItems.h"

#include "bus/resources/resource.h"
#include "bus/mfc_extensions/src/print_list_ctrl/ListCtrlPrint.h"
#include "bus/mfc_extensions/src/standard_dialogs_arial_bold/FileDialogArial.h"
//TD17888 marc_bugfix for save as dialog
//#include "bus/mfc_extensions/src/file_dialog_no_folder_change/FileDialogArialDisableFolders.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TA_MinMax.h"
#include "core/utilities/src/DateFormat.h" // TD12474


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using TA_Base_Core::DebugUtil;

/////////////////////////////////////////////////////////////////////////////
// EventListComponent

EventListComponent::EventListComponent()
:CListCtrl(),m_reapplyingSelection(false),m_isColourCoded(true)
{
	m_bToolTipCtrlCustomizeDone = false;
}

EventListComponent::~EventListComponent()
{
	
}

BEGIN_MESSAGE_MAP(EventListComponent, CListCtrl)
	//{{AFX_MSG_MAP(EventListComponent)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
    ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
    ON_NOTIFY_REFLECT(NM_CLICK, OnItemSelected)
    ON_NOTIFY_REFLECT(NM_DBLCLK, onDblclk)
	//}}AFX_MSG_MAP
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdrawEventList)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EventListComponent message handlers


void EventListComponent::PreSubclassWindow() 
{
	attachPModel();
    EnableToolTips();
    CListCtrl::PreSubclassWindow();
}

int EventListComponent::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
    // Borrowed from: http://www.codeguru.com/Cpp/controls/listview/tooltiptitletip/article.php/c909/

    // Determine the coordinates of the cell.
    int row, col;
    RECT cellrect;
    row = CellRectFromPoint(point, &cellrect, &col );

    if ( row == -1 )
    {
        return -1;
    }

    // Encode the cell coordinates the ID.
    pTI->hwnd = m_hWnd;
    pTI->uId = (UINT)((row<<10)+(col&0x3ff)+1);
    pTI->lpszText = LPSTR_TEXTCALLBACK;
    pTI->rect = cellrect;

    return pTI->uId;
}
int EventListComponent::CellRectFromPoint(CPoint & point, RECT * cellrect, int * col) const
{
    // Borrowed from: http://www.codeguru.com/Cpp/controls/listview/tooltiptitletip/article.php/c909/

    int colnum;

    // Make sure that the ListView is in LVS_REPORT
    if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
    {
        return -1;
    }

    // Get the top and bottom row visible
    int row = GetTopIndex();
    int bottom = row + GetCountPerPage();
    if ( bottom > GetItemCount() )
    {
        bottom = GetItemCount();
    }

    // Get the number of columns
    CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
    int nColumnCount = pHeader->GetItemCount();

    // Loop through the visible rows
    for ( ; row<=bottom; row++ )
    {
        // Get bounding rect of item and check whether point falls in it.
        CRect rect;
        GetItemRect( row, &rect, LVIR_BOUNDS );
        if ( rect.PtInRect(point) )
        {
            // Now find the column
            for ( colnum = 0; colnum < nColumnCount; colnum++ )
            {
                int colwidth = GetColumnWidth(colnum);
                if ( ( point.x >= rect.left ) && ( point.x <= (rect.left + colwidth ) ) )
                {
                    RECT rectClient;
                    GetClientRect( &rectClient );
                    if ( col ) *col = colnum;
                    rect.right = rect.left + colwidth;

                    // Make sure that the right extent does not exceed the client area
                    if( rect.right > rectClient.right )
                    {
                        rect.right = rectClient.right;
                    }
                    *cellrect = rect;
                    return row;
                }
                rect.left += colwidth;
            }
        }
    }
    return -1;
}

BOOL EventListComponent::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
        // Borrowed from: http://www.codeguru.com/Cpp/controls/listview/tooltiptitletip/article.php/c909/

    UINT nID = pNMHDR->idFrom;

    *pResult = 0;

    // We want to disable automatic tooltips completely.
    if( 0 == nID )
    {
        return TRUE;
    }

    // I want to implement this in PreSubclassWindow(), but it crashes.
    if(!m_bToolTipCtrlCustomizeDone)
    {
#if _MSC_VER >= 1300
		CToolTipCtrl* pToolTip = AfxGetModuleState()->m_thread.GetDataNA()->m_pToolTip;
#else
		_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
		CToolTipCtrl* pToolTip = pThreadState->m_pToolTip;
#endif

        // Set max tip width in pixel.
        // you can change delay time, tip text or background color also. enjoy yourself!
        pToolTip->SetMaxTipWidth(1000); //TD14876
        m_bToolTipCtrlCustomizeDone = TRUE;
    }

    // need to handle both ANSI and UNICODE versions of the message
    TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
    TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;

    // Extract the row and column numbers.
    int row = ((nID-1) >> 10) & 0x3fffff ;
    int col = (nID-1) & 0x3ff;
    TAUnicodeString strTip;
    strTip = GetItemText( row, col );
	
	//TD16138++ wangyn
	int columnLength = GetColumnWidth(col);
	
	int textWidth = GetStringWidth(strTip.str().GetBuffer(0)) + 12;
	
	if(columnLength >= textWidth)
	{
		return FALSE;
	}
	//++TD16138

    // Convert the text as required.
   /* if(pNMHDR->code == TTN_NEEDTEXTA)
    {
        m_pchTip = std::auto_ptr<TCHAR>( new TCHAR[strTip.GetLength()+1] );
        lstrcpyn( m_pchTip.get(), strTip, strTip.GetLength() );
        m_pchTip.get()[strTip.GetLength()] = NULL;
        pTTTW->lpszText = (WCHAR*)m_pchTip.get();
    }
    else
    {
        m_pwchTip = std::auto_ptr<WCHAR>( new WCHAR[strTip.GetLength()+1] );
        _mbstowcsz( m_pwchTip.get(), strTip.GetString(), strTip.GetLength() );
        m_pwchTip.get()[strTip.GetLength()] = NULL;
        pTTTW->lpszText = (WCHAR*)m_pwchTip.get();
    }*/

	//TODO Check
	 int text_length = strTip.GetLength() *  sizeof(TCHAR);
	 m_pchTip = std::auto_ptr<TCHAR>( new TCHAR[text_length+1] );
	 lstrcpyn( m_pchTip.get(), strTip, text_length );
	 m_pchTip.get()[text_length] = NULL;
	 pTTTW->lpszText = (WCHAR*)m_pchTip.get();
	 return TRUE;
}


void EventListComponent::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem=&(pDispInfo)->item;	

	int iItemIndx= pItem->iItem;

	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		CHeaderCtrl* header = GetHeaderCtrl();
		HDITEM headerItem;
		TCHAR headerName[1000];		
		headerItem.mask = HDI_TEXT;
		headerItem.pszText = headerName;
		headerItem.cchTextMax = 1000;
		header->GetItem(pItem->iSubItem,&headerItem);

		//TD14876++
		// Pattern code from AlarmListCtrl

		// Remove any non-printable charcters and copy to control
		//lstrcpy(pItem->pszText, replaceNonPrintableCharactersWithSpace(m_filter->getItemData(iItemIndx,headerName)).c_str());

		TAUnicodeString headerNameStr = headerName;
		TAUnicodeString text;
		text = TAUnicodeString::CreateFromMultitByteString(m_realPModel->getFilterItemData(iItemIndx,headerNameStr.toMultiByteStdString()).c_str());
		//std::string text = replaceNonPrintableCharactersWithSpace(m_filter->getItemData(iItemIndx,headerName));

		int maxLen = pItem->cchTextMax;

		int	text_length	= text.GetLength() *  sizeof(TCHAR);
		if (text_length > maxLen)
		{
			text = text.Left(maxLen); ///------------TODO Check
			text_length = maxLen;
		}
		text_length = ta_min(text_length, pItem->cchTextMax);

		memset(pItem->pszText, 0, pItem->cchTextMax);
		memcpy(pItem->pszText, text.c_str(), text_length);	
		//++TD14876
	}    

	*pResult = 0;
}

void EventListComponent::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;


    // Want to get the header item text (to get the column name)
    HDITEM headerItem;
    TCHAR headerName[1000];
    headerItem.mask = HDI_TEXT;
    headerItem.pszText = headerName;
    headerItem.cchTextMax = 1000;
	CHeaderCtrl* headerCtrl = GetHeaderCtrl(); 
	headerCtrl->GetItem(pNMListView->iSubItem, &headerItem); 
	
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	int currentSortColumn = m_realPModel->getCurrentSortColumn();
	
	clearColumnMask(currentSortColumn);

	TAUnicodeString headerNameStr = headerName;
	m_realPModel->sort(headerNameStr.toMultiByteStdString(),pNMListView->iSubItem);

	resetSortBitmap(headerNameStr.toMultiByteStdString(),pNMListView->iSubItem);  
	*pResult = 0;
}


//todo remove this func
void EventListComponent::resetSortBitmap(const std::string& columnName, const int columnId)
{ 
	// Add sort arrows to list columns
    HDITEM headerItem;

    // Want to get the header item text (to get the column name), the format (to position the bitmap), and the bitmap handle
    headerItem.mask = HDI_FORMAT | HDI_BITMAP;
	CHeaderCtrl* headerCtrl = GetHeaderCtrl(); 
	headerCtrl->GetItem(columnId, &headerItem);

    // If the header currently has a bitmap set, delete it.
	if (headerItem.hbm != 0) 
	{ 
		DeleteObject(headerItem.hbm); 
		headerItem.hbm = 0; 
	} 
	
    // Add the bitmap and on right formats to the header item
	headerItem.fmt |= HDF_BITMAP | HDF_BITMAP_ON_RIGHT; 
    
	// Load the required image
	headerItem.hbm = (HBITMAP)LoadImage(AfxGetInstanceHandle(), 
        MAKEINTRESOURCE((m_realPModel->getCurrentSortOrder()) ? IDB_ASCENDING : IDB_DESCENDING), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS); 

	// Set the header item with the new image
	headerCtrl->SetItem(columnId, &headerItem); 	
}


void EventListComponent::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // We will also be notified of selection changes when we do it ourselves, so
    // make sure we ignore expected events.
    // NOTE: The AlarmListCtrl keeps track of selection by storing the selection
    //       state for every alarm. While this is possible for the EventListComponent
    //       the large number of events means linear searching of all events should
    //       be avoided. Instead we'll just keep a vector of selected indices, which
    //       will be dynamically modified on refresh to ensure correct items remain
    //       selected.
    if ( ! m_reapplyingSelection )
    {
        // Create a vector of selected item indices.
        std::vector<unsigned long> selectedItems;
	    POSITION pos = GetFirstSelectedItemPosition();
	    while ( pos!=NULL )
	    {
            selectedItems.push_back(  GetNextSelectedItem(pos) );
	    }

        // Update the filter.
        m_realPModel->setSelectedItems( selectedItems );
		
		// for test
		//m_filter->setRowSelected(selectedItems);

    }

    *pResult = 0;
}

BOOL EventListComponent::SetItemCountEx( int size, DWORD flags /*=LVSICF_NOINVALIDATEALL*/ )
{
    // Let the parent class handle the request.
    BOOL returnValue = CListCtrl::SetItemCountEx( size, flags );

    // Make sure the following select changes to not affect current selections.
    m_reapplyingSelection = true;

    // Unselect all currently selected items.
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos!=NULL)
	{
        SetItemState( GetNextSelectedItem(pos), 0, LVIS_SELECTED );
        pos = GetFirstSelectedItemPosition();
    }

    // Reapply the selection.
    const std::vector<unsigned long>& selectedItems = m_realPModel->getSelectedItems();
    std::vector<unsigned long>::const_iterator it = selectedItems.begin();
    for ( ; it!=selectedItems.end(); it++ )
    {
        /*SetItemState( *it+1, LVIS_SELECTED, LVIS_SELECTED );*/
		SetItemState( *it+1, LVIS_SELECTED, LVIS_SELECTED );  // for test ir->ir+1
    }

    m_reapplyingSelection = false;

    return returnValue;
}

void EventListComponent::OnItemSelected(NMHDR* pNMHDR, LRESULT* pResult)
{
    unsigned int numSelectedItems = 0;
    unsigned long index = 0;

    // First test if only one event is selected
    const std::vector<unsigned long>& selectedItems = m_realPModel->getSelectedItems();
    numSelectedItems = selectedItems.size();

    if(selectedItems.size() > 0)
    {
        index = selectedItems[0];
    }
    
	// Redraw to get rid of the blue line on previously selected items
	RedrawWindow();
	UpdateWindow();
    // Post the message to the Event Viewer
    ::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_USER_LIST_SELECTION_UPDATE, numSelectedItems, index);
}

void EventListComponent::onDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
    FUNCTION_ENTRY("onDblclk");

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // Get the row number and validate it.
    int item = pNMListView->iItem;
    if ( item < 0 || item >= GetItemCount() )
    {
        *pResult = 0;
        FUNCTION_EXIT;
        return;
    }

    
    // Get the column number and validate it.
   /* int subItem = pNMListView->iSubItem;
    if ( subItem < 0 || subItem >= static_cast<int>(columns.size()) )
    {
        *pResult = 0;
        FUNCTION_EXIT;
        return;
    }*/

    // Extract the of the column from the header.
    CHeaderCtrl* header = GetHeaderCtrl();
    HDITEM headerItem;
    TCHAR headerName[1000];
    headerItem.mask = HDI_TEXT;
    headerItem.pszText = headerName;
    headerItem.cchTextMax = 1000;
    header->GetItem(pNMListView->iSubItem,&headerItem);

	TAUnicodeString headerNameStr = headerName;
	m_realPModel->executeColumnAction( pNMListView->iSubItem,headerNameStr.toMultiByteStdString());
	
    *pResult = 0;
}

void EventListComponent::OnCustomdrawEventList ( NMHDR* pNMHDR, LRESULT* pResult )
{

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	//unsigned long index = pLVCD->nmcd.dwItemSpec;
	*pResult = CDRF_DODEFAULT;
	// Used to remember if an item is selected across function calls
	static bool isSelected = false;
	switch(pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		// First thing - check the draw stage. If it's the control's prepaint
		// stage, then tell Windows we want messages for every item.
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_POSTPAINT:
		break;
	case CDDS_POSTERASE:
		break;
	case CDDS_ITEMPREPAINT:
		// This is the prepaint stage for an item. Here's where we set the
		// item's text color. Our return value will tell Windows to draw the
		// item itself, but it will use the new color we set here.
		// We'll cycle the colors through red, green, and light blue.
		// Get the index for the item
		{
			isSelected = false;
			unsigned long index= pLVCD->nmcd.dwItemSpec;

// 			if(/*GetItemState(index, LVIS_SELECTED) == LVIS_SELECTED*/
// 				/*m_filter->isRowSeleted(index)*/ 		)	
				if(GetItemState(index, LVIS_SELECTED) == LVIS_SELECTED)

			{
				// If the item is selected then we turn the Windows selection state off so that
				// our drawing colours are used instead of the default. This will be turned back on
				// in the custom draw method in the post paint stage.
				isSelected = true;
				SetItemState(index, 0, LVIS_SELECTED);
			}
			if ( m_isColourCoded )
			{
				// If alarm severities are colour coded pass the event onto the subitems.
				*pResult = CDRF_NOTIFYSUBITEMDRAW; 
			}
			else
			{
				// Otherwise just draw with row foreground and background colour.
				m_realPModel->getFilterRowColour(index, pLVCD->clrText, pLVCD->clrTextBk );

				*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
			}
		}
		break;
	case (CDDS_ITEMPREPAINT | CDDS_SUBITEM):
		// This is the prepaint stage for a subitem. Here's where we set the
		// item's text and background colors. Our return value will tell 
		// Windows to draw the subitem itself, but it will use the new colors
		// we set here.
		{
			CHeaderCtrl* header = GetHeaderCtrl();
			HDITEM headerItem;
			TCHAR headerName[1000];
			headerItem.mask = HDI_TEXT;
			headerItem.pszText = headerName;
			headerItem.cchTextMax = 1000;
			header->GetItem(pLVCD->iSubItem,&headerItem);
			TAUnicodeString headerNameStr = headerName;
			pLVCD->clrTextBk = m_realPModel->getFilterItemColour(pLVCD->nmcd.dwItemSpec,headerNameStr.toMultiByteStdString());
			*pResult = CDRF_DODEFAULT;
		}
		break;
	case CDDS_ITEMPOSTPAINT:
		if(isSelected)
		{
			int index = pLVCD->nmcd.dwItemSpec;
			CRect rect;
			GetItemRect(index, &rect,LVIR_BOUNDS);
			CDC* newDC = CDC::FromHandle(pLVCD->nmcd.hdc);

			// Blue Border selection highlight
			CPen   newPen  (PS_SOLID, 3, RGB(0,0,255));
			CPen* oldPen = newDC->SelectObject(&newPen);

			newDC->SelectStockObject(NULL_BRUSH);
			newDC->Rectangle(&rect);

			newDC->SelectObject(oldPen);

			// Turn selecting back on if necessary
			SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
			//setCurrentSelection(index);

			// Validate this item so it is not redrawn again
			ValidateRect(&rect);
			*pResult = CDRF_SKIPDEFAULT;//CDRF_DODEFAULT;
		}
	}	
}


void EventListComponent::setFilter()
{
	//clear previous columns
	m_realPModel->initializePrintColWidth();

	// Create the columns.
    std::vector<Filter::Column> columns = m_realPModel->getFilterColumns();	    
    for (unsigned int i = 0;i < columns.size();i++)
    {	
		//TD14897 azenitha++
		TAUnicodeString columnName = TAUnicodeString::CreateFromMultitByteString( columns[i].m_name.c_str());
		TAUnicodeString sevColName = TAUnicodeString::CreateFromMultitByteString(EVDisplayStrRes::GetInstance()->COLUMN_SEVERITY.c_str());
		if (columnName.Compare(sevColName) == 0)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "[TD14897] Setting severity column to center justified");
			InsertColumn(i,columnName,LVCFMT_CENTER,columns[i].m_width);
		}
		else
		{
			InsertColumn(i,columnName,LVCFMT_LEFT,columns[i].m_width);
		}
		//TD14897 ++azenitha
    }

	// Set any filter dependent styles.
    SetBkColor( m_realPModel->getBackgroundColour());	
}

void EventListComponent::ChangeColumnsCaption()
{
	m_realPModel->loadColumns();
	std::vector<Filter::Column> columns = m_realPModel->getFilterColumns();	    
	for (unsigned int i = 0;i < columns.size();i++)
    {
		TAUnicodeString columnName = TAUnicodeString::CreateFromMultitByteString( columns[i].m_name.c_str());
		LVCOLUMN col;		
		col.mask = LVCF_TEXT;		
		col.pszText= (LPTSTR)(LPCTSTR) columnName;
		SetColumn(i,&col);
	}
}

void EventListComponent::print()
{
	CListCtrlPrint cJob;

    // Set the header information.

	// TD12474 ++
	//CString header = "Event Viewer : " + CTime::GetCurrentTime().Format("%a %#d/%#m/%Y %#H:%M:%S");

	TA_Base_Core::DateFormat dateFormat; 
	TAUnicodeString dateFormatStr = TAUnicodeString::CreateFromMultitByteString(dateFormat.getDateFormat(TA_Base_Core::DateFormat::DateTimeFormat3).c_str());
	TAUnicodeString currentTime = CTime::GetCurrentTime().Format(dateFormatStr);

	TAUnicodeString header = TAUnicodeString::CreateFromMultitByteString(EVDisplayStrRes::GetInstance()->PRINT_HEADER_STR.c_str());
	header += currentTime;
	// ++ TD12474

	cJob.csPageHead = header.str();
	cJob.csPage = _T("Page %d of %d");

    // Get some information required to populate the colour map.
    const std::vector<Filter::Column>& columns = m_realPModel->getFilterColumns();	    
    unsigned int numRows = this->GetItemCount();

    // Generate the CellColourMap required for printing the events in colour.
    TA_Base_Bus::CellColourMap colourMap( columns.size(), numRows, 0, GetBkColor() );
    for ( unsigned int row=0; row<numRows; row++ )
    {
        if ( ! m_isColourCoded )
        {
            // If individual cells aren't colour coded apply row colouring.
            colourMap.setRowBkColour( row, m_realPModel->getBackgroundColour() );
        }
        else
        {
            // Otherwise add the individual cell colours.
            unsigned int column = 0;
            std::vector<Filter::Column>::const_iterator it;
            for ( it=columns.begin(); it!=columns.end(); it++,column++ )
            {
                colourMap.setCellBkColour( row, column, m_realPModel->getFilterItemColour( row, it->m_name ) );
            }
        }
    }
    cJob.pColourMap = &colourMap;

	// TD 12763 Determine the column width and adjust where appropriate
	int* printoutColWidths = m_realPModel->getPrintColWidth();
	if (printoutColWidths != NULL)
	{
		// create a new col width array for cJob to process the width, if there are changes
		cJob.pColWidthArray = printoutColWidths;
	}
    // Finally, add the list of data to print and then print it.
	cJob.pList = this;
	cJob.OnFilePrint();	
}

void EventListComponent::clearColumnMask(int columnId)
{
	HDITEM headerItem;
	CHeaderCtrl* headerCtrl = GetHeaderCtrl(); 
	// Remove image from original sort column
	headerCtrl->GetItem(columnId, &headerItem); 
	headerItem.fmt &= ~(HDF_BITMAP | HDF_BITMAP_ON_RIGHT); 
	if (headerItem.hbm != 0) 
	{ 
		DeleteObject(headerItem.hbm); 
		headerItem.hbm = 0; 
	} 
	headerCtrl->SetItem(columnId, &headerItem);	
}

void EventListComponent::clearSortMark()
{
	int columnId = m_realPModel->getCurrentSortColumn();
	if(columnId != -1)
	{
		clearColumnMask(columnId);
		m_realPModel->clearSortMark();
	}
}

BOOL EventListComponent::clearList()
{
    // Before actually deleting all items, need to remove the sort icons from any currently sorted headers
    
	clearSortMark();

    return DeleteAllItems();
}


/*void EventListComponent::exportEvents(const CString& exportLocation)
{
    // Show print dialog for user to choose a file name
	//TD17888 marc_bugfix
	TA_Base_Bus::FileDialogArialDisableFolders dlg(FALSE,            // We want a Save dialog not an Open one
					".csv",           // The default file extension
					"Untitled",       // This is the initial file name
					OFN_HIDEREADONLY| // Hide the readonly check box
					OFN_OVERWRITEPROMPT| // Prompt to ask the user if they want to overwrite existing files
					OFN_PATHMUSTEXIST |	 // Check that the path entered is correct and warn if it is not
					OFN_NOCHANGEDIR,
					"CSV (Comma delimited) (*.csv)|*.csv||",	// Only allow them to save in csv format
					this);			  // Parent window is the actual event viewer

    dlg.m_ofn.lpstrInitialDir = exportLocation;

	if (dlg.DoModal() != IDOK)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,"Export was cancelled");
		return;
	}

	try
	{
		std::string fileName = dlg.GetPathName(); //todo check conversion
		m_realPModel->exportEvents(fileName,GetItemCount());
	}
	catch ( ... )
	{
        TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << std::string(dlg.GetPathName());
        UINT selectedButton = userMsg.showMsgBox(IDS_UE_040018);
		return;
	}

    TA_Base_Bus::TransActiveMessage userMsg;
    UINT selectedButton = userMsg.showMsgBox(IDS_UI_040018);
}*/




void EventListComponent::setColourCoding(const bool isEnabled)
{
    m_isColourCoded = isEnabled;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// AbstractTransActiveView member functions

std::string EventListComponent::getMyPModelName()
{
	//return EVENT_DISPLAY_PMODEL;
	return EVENT_VIEWER_DLG_PMODEL;
}

void EventListComponent::setupPModel()
{
	m_realPModel = dynamic_cast<EventViewerDlgPModel*> (m_pModel);
	TA_ASSERT(m_realPModel != NULL, "PModel has been initialized successfully");
}

void EventListComponent::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
{		
}

