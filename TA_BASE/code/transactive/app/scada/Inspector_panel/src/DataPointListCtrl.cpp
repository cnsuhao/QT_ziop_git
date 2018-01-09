/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/DataPointListCtrl.cpp $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the list display used to show the basic information in
  * the CInformationDialog class.
  *
  */

#include "app/scada/Inspector_Panel/src/stdafx.h"

#include "app/scada/inspector_panel/src/InspectorPanel.h"
#include "app/scada/inspector_panel/src/InspectorPanelDlg.h"
#include "app/scada/inspector_panel/src/DataPointListCtrl.h"

#include "../pmodel/src/inspectorpanelmodeleventargs.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/DebugUtil.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// CAbstractDataPointListCtrl

	const int CAbstractDataPointListCtrl::LVS_EX_LABELTIP_XXX = 0x00004000;

	CAbstractDataPointListCtrl::CAbstractDataPointListCtrl()
		: m_pEquipmentPModel(NULL), CEditableListCtrl(true), m_greenIconIndex(-1), m_redIconIndex(-1), m_orangeIconIndex(-1), 
		  m_blueIconIndex(-1), m_accessAllowed(false)
	{
		FUNCTION_ENTRY ("CAbstractDataPointListCtrl()");

		FUNCTION_EXIT;
	}

	CAbstractDataPointListCtrl::~CAbstractDataPointListCtrl()
	{
		FUNCTION_ENTRY ("~CAbstractDataPointListCtrl()");

		DestroyIcon(m_greenIcon);
		DestroyIcon(m_redIcon);
		DestroyIcon(m_orangeIcon);
		DestroyIcon(m_blueIcon);

		FUNCTION_EXIT;
	}


	HWND CAbstractDataPointListCtrl::getViewHandle() 
	{
		return GetSafeHwnd();
	}



	/* SetEquipment
	 *
	 * This function will set the name of the equipment that the data points are to be displayed for.It can 
	 * be called before or after the windows control is on the screen.
	 */
	void CAbstractDataPointListCtrl::refreshAllData()
	{
		FUNCTION_ENTRY ("setEquipment()");

		//m_equipment = equipment;

		// Update the displayed control
		if (m_hWnd!=0 && m_pEquipmentPModel != NULL)
		{
			displayDataPoints();
		}
	}

	void	CAbstractDataPointListCtrl::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		if( _stricmp(updateType, PMODEL_NOTIFY_REFRESH_ALL_DATA) == 0 )
		{
			BOOL err = this->PostMessage(MESSAGE_REFRESH_ALL_DATA);
		}
		else if(_stricmp(updateType, PMODEL_NOTIFY_UPDATE_DATA) == 0 )
		{
			InspectorPanelModelEventArgs* realArgs = dynamic_cast<InspectorPanelModelEventArgs*>(args);
			if(realArgs)
			{
				BOOL err = this->PostMessage(MESSAGE_UPDATE_DATA,realArgs->getDataPoint());
			}
		}
		else if(_stricmp(updateType, PMODEL_NOTIFY_PERTMISSION) == 0 )
		{
			this->PostMessage(MESSAGE_UPDATE_PERMISSION);
		}
	}


	LRESULT CAbstractDataPointListCtrl::onRefreshAllData(WPARAM wparam, LPARAM lparam)
	{
		refreshAllData();

		AfxGetApp()->m_pMainWnd->PostMessage(RESIZE_WINDOW_MESSAGE, 0, 0);
		return 0;
	}

	LRESULT CAbstractDataPointListCtrl::onUpdateData(WPARAM wparam, LPARAM lparam)
	{
		unsigned int iPoint =(unsigned int)wparam;
		updateDataPoint(iPoint);
		return 0;
	}

	LRESULT CAbstractDataPointListCtrl::onUpdatePermission(WPARAM wparam, LPARAM lparam)
	{
		this->updatePermissions();
		return 0;
	}

	BEGIN_MESSAGE_MAP(CAbstractDataPointListCtrl, CEditableListCtrl)
		//{{AFX_MSG_MAP(CAbstractDataPointListCtrl)
		ON_WM_CREATE()
		//}}AFX_MSG_MAP
        ON_NOTIFY(HDN_DIVIDERDBLCLICKW, 0, onDividerDoubleClick)
		ON_MESSAGE( MESSAGE_REFRESH_ALL_DATA, onRefreshAllData)
		ON_MESSAGE( MESSAGE_UPDATE_DATA, onUpdateData)
		ON_MESSAGE( MESSAGE_UPDATE_PERMISSION, onUpdatePermission)
	END_MESSAGE_MAP()



	/////////////////////////////////////////////////////////////////////////////
	// CAbstractDataPointListCtrl message handlers

	int CAbstractDataPointListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	{
		FUNCTION_ENTRY ("OnCreate()");

		if (CWnd::OnCreate(lpCreateStruct) == -1)
		{
			return -1;
		}
		setupList();

		displayDataPoints();

		FUNCTION_EXIT;
		
		return 0;
	}


	void CAbstractDataPointListCtrl::PreSubclassWindow() 
	{
		FUNCTION_ENTRY ("PreSubclassWindow()");

		CWnd::PreSubclassWindow();
		
		attachPModel();
		setupList();
//		displayDataPoints();

		FUNCTION_EXIT;
	}


	// Helper functions

	void CAbstractDataPointListCtrl::updateDataPoint(unsigned int iPoint)
	{
		FUNCTION_ENTRY ("updateDataPoint()");

		if (m_hWnd==0 || m_pEquipmentPModel==NULL ) //wenching++ (TD13353)
		{
			return;
		}

		//if (iPoint<m_pEquipmentPModel->getChildDataPointCount())
		{
			//  Find the data point in the list view
			for (int nItem=0;nItem<this->GetItemCount();nItem++)
			{
				if (this->GetItemData(nItem)==iPoint)
				{
					updateDataPointDisplay(iPoint, nItem);

					// Done
					return;
				}
			}
		}

		FUNCTION_EXIT;
	}

	int CAbstractDataPointListCtrl::getDataPointStatusImageIndex(UINT iPoint)
	{
		FUNCTION_ENTRY ("getDataPointStatusImageIndex()");

		int iconIndex = -1;

		// if equipment is not defined, there is no point going any further
		if (m_pEquipmentPModel == NULL)
		{
			return iconIndex;
		}

		// if icons have not been loaded, load them up now
		if (m_greenIconIndex == -1 || m_redIconIndex == -1 && m_orangeIconIndex == -1 || m_blueIconIndex == -1)
		{
					// Load status icons
			m_greenIcon  = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_GREEN));
			m_redIcon    = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_RED));
			m_orangeIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ORANGE));
			m_blueIcon   = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_BLUE));

			m_greenIconIndex = addIcon(m_greenIcon);
			m_redIconIndex = addIcon(m_redIcon);
			m_orangeIconIndex = addIcon(m_orangeIcon);
			m_blueIconIndex = addIcon(m_blueIcon);

			TA_ASSERT(m_greenIconIndex != -1 && m_redIconIndex != -1 && m_orangeIconIndex != -1 && m_blueIconIndex != -1,
						"Insufficient space in Image List (DataPointListCtrl)");
		}
		//
		// determine the type of icons to be displayed on screen
		//

		EDataPointStatusColor color = m_pEquipmentPModel->getDataPointStatusColor(iPoint);
		switch(color)
		{
		case Green:
			iconIndex = m_greenIconIndex;
			break;
		case Red:
			iconIndex = m_redIconIndex;
			break;
		case Orange:
			iconIndex = m_orangeIconIndex;
			break;
		default:
			iconIndex = m_blueIconIndex;
			break;
		}

		FUNCTION_EXIT;
		return iconIndex;
	}

	int CAbstractDataPointListCtrl::insertItemTextImage(int iPoint, int insertNItem, std::string& value, bool addImage)
	{
		FUNCTION_ENTRY ("insertItemTextImage()");

		if (m_pEquipmentPModel == NULL)
		{
			return 0;
		}

		LVITEM   listItem;
		int nItem;

		// Description
		nItem = InsertItem(insertNItem,CONVERT_FROM_STD(m_pEquipmentPModel->getChildDataPointDescription(iPoint)));
		SetItemData(nItem,iPoint);
		
		listItem.iItem = nItem;
		listItem.iSubItem = 0;

		if (addImage)
		{
			listItem.iImage = getDataPointStatusImageIndex(iPoint);
		}
		else
		{
			listItem.iImage = -1;
		}

		listItem.mask = LVIF_IMAGE;
		SetItem(&listItem);

		FUNCTION_EXIT;
		return nItem;
	}

	void CAbstractDataPointListCtrl::setItemTextImage(int nItem, int nSubItem, const std::string& value, bool addImage)
	{
		FUNCTION_ENTRY ("setItemTextImage()");

		LVITEM   listItem;

		SetItemText(nItem,nSubItem,CONVERT_FROM_STD(value));
		listItem.iItem = nItem;
		listItem.iSubItem = nSubItem;

		if (addImage)
		{
			listItem.iImage = getDataPointStatusImageIndex(GetItemData(nItem));
		}
		else
		{
			listItem.iImage = -1;
		}
		listItem.mask = LVIF_IMAGE;
		SetItem(&listItem);

		FUNCTION_EXIT;
	}


	void CAbstractDataPointListCtrl::setItemTextYesNo(int nItem, int nSubItem, bool yes)
	{
		FUNCTION_ENTRY ("setItemTextYesNo()");

		if (yes)
		{
			SetItemText(nItem, nSubItem,_T("Yes"));
		}
		else
		{
			SetItemText(nItem, nSubItem,_T("No"));
		}

		FUNCTION_EXIT;
	}

	void CAbstractDataPointListCtrl::updatePermissions()
	{
		//m_accessAllowed;
		displayDataPoints();
		//EnableWindow(m_accessAllowed);
	}


    std::string CAbstractDataPointListCtrl::getCurrentValueAsString(UINT iPoint)
    {
		
        return m_pEquipmentPModel->getDisplayValueWithUnit(iPoint);
    }


	bool CAbstractDataPointListCtrl::isAnyDataPointsDisplayedOnScreen() const
	{
		// always true, unless override by specific classes
		return true;
	}


    void CAbstractDataPointListCtrl::onDividerDoubleClick(NMHDR* pNMHDR, LRESULT* pResult) 
	{
        const int IMAGE_SIZE = 16;

		HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;

		// The list control does not resize the column to include the image. Need
		// to do this ourselves.

		int numItems = GetItemCount();
		int column = phdn->iItem;
		int maxWidth = 0;
		int currWidth;

        LVITEM currentItem;
        currentItem.mask = LVIF_IMAGE;
        currentItem.iSubItem = column;

		for (int row = 0; row < numItems; row++)
		{
			currWidth = GetStringWidth(GetItemText(row, column).GetBuffer(0));

            // Check if the item contains an image.
            currentItem.iItem = row;
            GetItem(&currentItem);
            if (currentItem.iImage != -1)
            {
                // Row contains an image. 
                currWidth += IMAGE_SIZE;
            }

			if (currWidth > maxWidth)
			{
				maxWidth = currWidth;
			}
		}

        // First column leaves an extra space.
        if (column == 0)
        {
            maxWidth += IMAGE_SIZE;
        }

        // Set the column's width. Add on an extra image width to make it look better.
		SetColumnWidth(column, maxWidth + IMAGE_SIZE);
		
		*pResult = 0;
	}


	ICellManager::CellType CAbstractDataPointListCtrl::convertPModelCellType(EPModelCellType type)
	{
		switch(type)
		{
		case PMCT_NOTEDITABLE:
			return ICellManager::CT_NOTEDITABLE;
		case PMCT_TEXT:
			return ICellManager::CT_TEXT;
		case PMCT_NUMBER:
			return ICellManager::CT_NUMBER;
		case PMCT_SELECTION:
			return ICellManager::CT_SELECTION;
		case PMCT_DECISION:
			return ICellManager::CT_DECISION;
		case PMCT_CHECKBOX:
			return ICellManager::CT_CHECKBOX;
		case PMCT_CHECKBOX_DISABLED:
			return ICellManager::CT_CHECKBOX_DISABLED;
		case PMCT_DATE:
			return ICellManager::CT_DATE;
		case PMCT_TIME:
			return ICellManager::CT_TIME;
		case PMCT_MASK:
			return ICellManager::CT_MASK;
		case PMCT_BUTTON:
			return ICellManager::CT_BUTTON;
		}
		return ICellManager::CT_NOTEDITABLE;
	}
}
