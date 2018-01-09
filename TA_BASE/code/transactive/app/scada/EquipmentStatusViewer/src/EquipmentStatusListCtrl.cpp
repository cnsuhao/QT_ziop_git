/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/EquipmentStatusListCtrl.cpp $
 * @author:  Rod Rolirad
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/09/03 14:11:12 $
 * Last modified by:  $Author: CM $
 * 
 * Class is responsible for displaying the ListCtrl in the main window of the application.
 * It handles the column preparation and supports list sorting, column hiding/unhiding and
 * the image list (icons) to display.
 *
 */
#include "app/scada/EquipmentStatusViewer/src/resource.h"
#include "app/scada/EquipmentStatusViewer/src/EquipmentStatusListCtrl.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/CustomDefines.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentStatusViewerDlgModel.h"
#include "bus/mfc_extensions/src/fixed_column_header_ctrl/FixedHeaderCtrl.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

namespace TA_Base_App
{

	BEGIN_MESSAGE_MAP(CEquipmentStatusListCtrl, CListCtrl)
		//{{AFX_MSG_MAP(CEquipmentStatusListCtrl)
        ON_WM_DESTROY()
		ON_WM_SIZE()
	//}}AFX_MSG_MAP
	END_MESSAGE_MAP()


	CEquipmentStatusListCtrl::CEquipmentStatusListCtrl(TA_Base_Bus::ITransActiveWinApp * pApp)
        :
		AbstractTransActiveView(pApp),
		m_currentSortCol(ESL_COLUMN_MAX)
	{
		for (int i=0; i < ESL_COLUMN_MAX; i++)
			m_bAscending[i] = true;

	}

	CEquipmentStatusListCtrl::~CEquipmentStatusListCtrl()
	{
	}


	void CEquipmentStatusListCtrl::PreSubclassWindow() 
	{
        attachPModel();

		SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES | LVS_EX_INFOTIP);

    	m_icons[ESL_GREEN]  = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_GREEN));
		m_icons[ESL_RED]    = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_RED));
		m_icons[ESL_ORANGE] = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ORANGE));
		m_icons[ESL_BLUE]   = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_BLUE));

        m_imageList.Create(16, 16, ILC_COLOR8, 4, 0);

        for (int i = 0; i < ESL_ICON_MAX; ++i )
        {
            HICON icon = getIconHandle(EIcon(i));

            if (0 == icon || m_imageList.Add(icon) < 0)
            {
                TA_ASSERT(FALSE, "Failed to load icon/add to image list");
            }
        }

        SetImageList(&m_imageList, LVSIL_SMALL);

        // DUMMY_COLUMN Used to get rid of indentation on Asset column, as when we set
        // icons for the list control the first column always has indentation by one icon width..
		InsertColumn(DUMMY_COLUMN,			_T(""));

		TAUnicodeString txtShow;
		TALoadString(STRING_ID_EDSV_0004, txtShow);
		InsertColumn(ASSET_COLUMN,			txtShow.GetString());

		TALoadString(STRING_ID_EDSV_0005, txtShow);
		InsertColumn(DESCRIPTION_COLUMN,	txtShow.GetString());

		TALoadString(STRING_ID_EDSV_0006, txtShow);
		InsertColumn(DATAPOINT_COLUMN,		txtShow.GetString());

		TALoadString(STRING_ID_EDSV_0007, txtShow);
		InsertColumn(TYPE_COLUMN,			txtShow.GetString());

		TALoadString(STRING_ID_EDSV_0008, txtShow);
		InsertColumn(VALUE_COLUMN,			txtShow.GetString());

		TALoadString(STRING_ID_EDSV_0009, txtShow);
		InsertColumn(QUALITY_COLUMN,        txtShow.GetString());

		TALoadString(STRING_ID_EDSV_0010, txtShow);
		InsertColumn(STATUS_COLUMN,			txtShow.GetString());

		TALoadString(STRING_ID_EDSV_0011, txtShow);
		InsertColumn(MODIFIED_BY_COLUMN,	txtShow.GetString());
		//InsertColumn(TIMESTAMP_COLUMN,		"Timestamp");

		hideColumn(DUMMY_COLUMN);
		SetColumnWidth(ASSET_COLUMN,		180);   // Used to be 150//240
		SetColumnWidth(DESCRIPTION_COLUMN,	230);   // Used to be 246//285
		SetColumnWidth(DATAPOINT_COLUMN,	330);	// +60//160
		SetColumnWidth(TYPE_COLUMN,			50);	// +40//60
		SetColumnWidth(VALUE_COLUMN,		180);	// +80//120
		hideColumn(QUALITY_COLUMN);
		SetColumnWidth(STATUS_COLUMN,		210);//115
		SetColumnWidth(MODIFIED_BY_COLUMN,  170);//150
      
        // Any extra space can be assigned to description column
        TA_Base_Bus::FixedHeaderCtrl::expandColumnToFitListWidth(*this, DESCRIPTION_COLUMN, true);

		CListCtrl::PreSubclassWindow();
	}


    void CEquipmentStatusListCtrl::prepareColumns()
    {
        //do nothing
    }


	void CEquipmentStatusListCtrl::SortColumn(int aCol, bool isUpdateUI )
	{
        m_sortParameters.ascending = m_bAscending[aCol];
        m_sortParameters.sortColumn = aCol;
		m_sortParameters.realData = m_realModel->getSortDataPtr();
     
        m_currentSortCol = aCol;
        SortItems( CompareFunc, (LPARAM) &m_sortParameters);

		if(isUpdateUI)
		{
			for (int thisCol = -1; thisCol < ESL_COLUMN_MAX; thisCol++)
			{
				HDITEM item;
				GetHeaderCtrl()->GetItem(thisCol, &item);

				// delete the bitmap object
				if (item.hbm != NULL)
				{
					DeleteObject(item.hbm); 
					item.hbm = NULL; 
				}

				if (thisCol == aCol)
				{
					item.mask = HDI_BITMAP | HDI_FORMAT;
					item.fmt |= HDF_BITMAP | HDF_BITMAP_ON_RIGHT;

					if (m_bAscending[aCol] == true)
						item.hbm = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SORTUP), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
					else
						item.hbm = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SORTDOWN), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
				} else
				{
					item.mask = HDI_FORMAT;
					item.fmt &= ~HDF_BITMAP;
					item.fmt &= ~HDF_BITMAP_ON_RIGHT;
				}

				GetHeaderCtrl()->SetItem(thisCol, &item);
			}
		}
	}


	void CEquipmentStatusListCtrl::SetSortOrder(int aCol, bool aAscending)
	{
		m_bAscending[aCol] = aAscending;
	}


	bool CEquipmentStatusListCtrl::GetSortOrder(int aCol)
	{
		return m_bAscending[aCol];
	}

    HICON CEquipmentStatusListCtrl::getIconHandle(const EIcon& icon)
    {
        IconMap::iterator itr = m_icons.find(icon);
        if (itr == m_icons.end())
        {
            return 0;
        }
        else
        {
            return itr->second;
        }
    }


    void CEquipmentStatusListCtrl::OnDestroy()
    {
		//detach from the presentation model
		if (m_pModel != NULL)
		{
			m_pModel->detachView(this);
		}

        CListCtrl::OnDestroy();

        for (IconMap::iterator itr = m_icons.begin(); itr != m_icons.end(); ++itr )
        {
            HICON handle = itr->second;
            if (0 != handle)
            {
                DestroyIcon(handle);
            }
        }
        m_icons.clear();
    }


    void CEquipmentStatusListCtrl::hideColumn(int colIndex)
    {
		if (colIndex >= 0 && colIndex < GetHeaderCtrl()->GetItemCount())
		{
			// Resize the column width to zero as it is now hidden
			SetColumnWidth(colIndex, 0);
			m_hiddenCols.insert(std::set<int>::value_type(colIndex));
		}
    }


    void CEquipmentStatusListCtrl::showColumn(int colIndex, int width)
    {
		if (colIndex >= 0 && colIndex < GetHeaderCtrl()->GetItemCount() && width >= 0)
		{
			// TD12080: hide zero width columns
			if (width == 0)
			{
				hideColumn(colIndex);
				return;
			}

			// Adjust the column to the specified width to make it visible
			SetColumnWidth(colIndex, width);
			m_hiddenCols.erase(std::set<int>::value_type(colIndex)); // TD12080: enable resizing of this column
		}
    }


    BOOL CEquipmentStatusListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
    {
        NMHDR* pNMHDR = (NMHDR*)lParam;

        // Stop any hidden columns from being resized
        if(pNMHDR->code == HDN_BEGINTRACKW || pNMHDR->code == HDN_BEGINTRACKA|| pNMHDR->code == HDN_ITEMCHANGING)
        {
            HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;

            for (std::set<int>::iterator itr = m_hiddenCols.begin(); itr != m_hiddenCols.end(); ++itr )
            {
                if (pHDN->iItem == (*itr))
                {
                    // This is a hidden column, suppress the notification
                    *pResult = TRUE;
                    return TRUE;
                }
            }
        }
        return CListCtrl::OnNotify(wParam, lParam, pResult);
    }

	void CEquipmentStatusListCtrl::OnSize(UINT nType, int cx, int cy) 
	{
		CListCtrl::OnSize(nType, cx, cy);
		if (nType != SIZE_MAXHIDE && nType != SIZE_MINIMIZED)
		{
			int nColumnCount = GetHeaderCtrl()->GetItemCount();

			if (nColumnCount > 0)
			{
				// Determine the amount of space to be fill in by extending the last column
				CRect rc;
				GetClientRect(rc);
				int headerTotalWidth = rc.Width();
				
				int sumColumnWidths = 0;
				for (int i = 0; i < nColumnCount; ++i )
				{
					sumColumnWidths += GetColumnWidth(i);	
				}

				int paddingWidth = headerTotalWidth - sumColumnWidths;
		       
				SetColumnWidth(DESCRIPTION_COLUMN,  GetColumnWidth(DESCRIPTION_COLUMN)+paddingWidth);
			}
		}
	}

	int CALLBACK CEquipmentStatusListCtrl::CompareFunc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		unsigned long key1 = static_cast<unsigned long>(lParam1);
		unsigned long key2 = static_cast<unsigned long>(lParam2);
		
		return EquipmentStatusViewerDlgModel::compareItems(key1, key2, (Sort_Parameters*)lParamSort);
	}

	int CEquipmentStatusListCtrl::UpdateInList(EquipmentItemUpdateEvent* updateEvent)
	{
		if (updateEvent == NULL) return -1;

		bool bFiltered = updateEvent->isFiltered();
		unsigned long entityKey = updateEvent->getEntityKey();
		unsigned long updateFlags = updateEvent->getColUpdateFlags();

		// find this item index first
		LVFINDINFO findInfo;
		findInfo.flags = LVFI_PARAM;
		findInfo.lParam = entityKey;
		int nIndex = FindItem( &findInfo );

		// need filter
		if ( bFiltered )
		{
			if (nIndex >= 0)
			{
				DeleteItem( nIndex);
			}
			return nIndex;
		}

		//  This report line is to be displayed. Is the report line
		//  already in the list control? Update it or add it.
		LVITEM lvItem;
		ZeroMemory( &lvItem, sizeof( lvItem));
		if (nIndex < 0)
		{
			//  Needs to be added first. Just add a stub item and then
			//  update all the columns' text entries.
			TAUnicodeString txtShow = TALoadString(STRING_ID_EDSV_0014);
			lvItem.pszText	= const_cast<TCHAR*>(txtShow.GetString());
			lvItem.mask		= LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE | LVIF_INDENT;
			lvItem.iImage   = -1;
			lvItem.iItem	= GetItemCount();
			lvItem.iSubItem	= 0;
			lvItem.lParam	= entityKey;
			lvItem.iIndent  = 0;
			nIndex = InsertItem( &lvItem);
		}

		if (nIndex >= 0)
		{
			EquipmentItemInfo itemInfo;
			bool isExist = m_realModel->getEquipmentItemInfo(entityKey, itemInfo);
			if (isExist)
			{
				int nStartColumn = ESL_COLUMN_MIN;
				int nEndColumn   = ESL_COLUMN_MAX;
				for (int nColumn = nStartColumn; nColumn < nEndColumn; nColumn++)
				{
					if (m_realModel->isColUpdate(nColumn, updateFlags))
					{
						TAUnicodeString colText = TAUnicodeString::CreateFromMultitByteString(itemInfo.m_text[(EslColumns)nColumn].c_str());
						lvItem.pszText	= const_cast<TCHAR*>(colText.GetString());
						lvItem.mask		= LVIF_TEXT | LVIF_IMAGE;
						lvItem.iImage   = itemInfo.m_icon[(EslColumns)nColumn];
						lvItem.iItem	= nIndex;
						lvItem.iSubItem	= nColumn;
						SetItem( &lvItem);
					}
				}			
			}
		}

		return nIndex;
	}

	void CEquipmentStatusListCtrl::setupPModel()
	{
		m_realModel = dynamic_cast<EquipmentStatusViewerDlgModel*>(m_pModel);
		TA_ASSERT(m_realModel != NULL, "m_pmodel can't convert to EquipmentStatusViewerDlgModel");
	}

	std::string CEquipmentStatusListCtrl::getMyPModelName()
	{
		return std::string(EDSV::PMODEL_MAIN_DIALOG);
	}

	HWND CEquipmentStatusListCtrl::getViewHandle()
	{
		return GetSafeHwnd();
	}
} // TA_Base_App
