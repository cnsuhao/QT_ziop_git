/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/InformationListCtrl.cpp $
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

#include "app/scada/inspector_panel/src/InformationListCtrl.h"
#include "../pmodel/src//InfoTabHelper.h"


#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/DebugUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WM_INFOMATION_TEST (WM_USER + 952)
namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// CInformationListCtrl


	enum EColumns
	{
		DummyColumn				= 0,
		DescriptionColumn		= 1,
		TypeColumn				= 2,
		ValueColumn				= 3
	};


	CInformationListCtrl::CInformationListCtrl()
	:
	CAbstractDataPointListCtrl(),
	m_isReset( false ),
	m_dataPointsDisplayedOnScreen ( false )
	{
		FUNCTION_ENTRY ("CInformationListCtrl()");

		// We will manage the cell requests
		SetManager(this);

		QualityColumn = -1;
		TimestampColumn = -1;
		FUNCTION_EXIT;
	}

	CInformationListCtrl::~CInformationListCtrl()
	{
		FUNCTION_ENTRY ("~CInformationListCtrl()");
		FUNCTION_EXIT;
	}

	BEGIN_MESSAGE_MAP(CInformationListCtrl, CAbstractDataPointListCtrl)
		//{{AFX_MSG_MAP(CInformationListCtrl)
		ON_WM_CREATE()
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////////////////
	// CInformationListCtrl message handlers

	// Helper functions

	void CInformationListCtrl::setupList()
	{
		FUNCTION_ENTRY ("setupList()");	

		
		if (true == m_isReset)
		{
			// TD18728, Delete the columns before re-setup the list
			if( TimestampColumn != -1 )
				DeleteColumn(TimestampColumn);
			if( QualityColumn != -1)
				DeleteColumn(QualityColumn);
			this->DeleteColumn(ValueColumn);
			this->DeleteColumn(TypeColumn);
			this->DeleteColumn(DescriptionColumn);
			this->DeleteColumn(DummyColumn);

			Invalidate();
		}
		

		if( InfoTabHelper::getShowQuality() && InfoTabHelper::getShowTimestamp() )
		{
			QualityColumn = 4;
			TimestampColumn = 5;
		}
		else if( !InfoTabHelper::getShowQuality() && InfoTabHelper::getShowTimestamp() )
		{
			QualityColumn = -1;
			TimestampColumn = 4;
		}
		else if( !InfoTabHelper::getShowTimestamp() && InfoTabHelper::getShowQuality())
		{
			QualityColumn = 4;
			TimestampColumn = -1;
		}
		

		SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_SUBITEMIMAGES | CAbstractDataPointListCtrl::LVS_EX_LABELTIP_XXX);//|0x00010000 );

		// Headings
		this->InsertColumn(DummyColumn,				TALoadString(STRING_ID_INSPECTOR_PANEL_0007).GetString() );//_T("DeleteMe") );
   		this->InsertColumn(DescriptionColumn,		TALoadString(STRING_ID_INSPECTOR_PANEL_0008).GetString() );//_T("Description"));
		this->InsertColumn(TypeColumn,				TALoadString(STRING_ID_INSPECTOR_PANEL_0009).GetString() );//_T("Type"));
   		this->InsertColumn(ValueColumn,				TALoadString(STRING_ID_INSPECTOR_PANEL_0010).GetString() );//_T("Current Value"));

		
		if( QualityColumn != -1)
			this->InsertColumn(QualityColumn,			TALoadString(STRING_ID_INSPECTOR_PANEL_0011).GetString() );//_T("Quality"));
		if( TimestampColumn != -1 )
			this->InsertColumn(TimestampColumn,			TALoadString(STRING_ID_INSPECTOR_PANEL_0012).GetString() );//_T("Time"));
		

		// Using a 'dummy' column for index 0 prevents
		// MFC from indenting the text by the image size for the 'real' first column
		this->SetColumnWidth(DummyColumn, 0);

		if ( QualityColumn == -1 && TimestampColumn == -1)
		{
			this->SetColumnWidth(DescriptionColumn,		350);
			this->SetColumnWidth(TypeColumn,			100);
			this->SetColumnWidth(ValueColumn,			140);
		}
		else
		{
			this->SetColumnWidth(DescriptionColumn,		210);
			this->SetColumnWidth(TypeColumn,			50);

			if ( QualityColumn != -1 && TimestampColumn != -1)
			{
				this->SetColumnWidth(ValueColumn,			110);
				this->SetColumnWidth(QualityColumn,			110);
				this->SetColumnWidth(TimestampColumn,		110);
			}
			else 
			{
				this->SetColumnWidth(ValueColumn,			180);

				if ( QualityColumn != -1 )
					this->SetColumnWidth(QualityColumn,			150);
				else
					this->SetColumnWidth(TimestampColumn,		150);
			}
		}

		FUNCTION_EXIT;
	}


	std::string CInformationListCtrl::getMyPModelName()
	{
		return PMODEL_INFORMATION_LISTCTRL; 
	}

	void CInformationListCtrl::setupPModel()
	{
		m_realPModel = dynamic_cast<InformationListCtrlModel*> (m_pModel);
		
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
 		m_pEquipmentPModel = dynamic_cast<AbstractEquipmentPModel*>(m_realPModel);
 		TA_ASSERT(m_pEquipmentPModel != NULL, "PModel should be valid");

		m_realPModel->attachView(this,PMODEL_NOTIFY_REFRESH_ALL_DATA);
		m_realPModel->attachView(this,PMODEL_NOTIFY_UPDATE_DATA);
	}


	void CInformationListCtrl::updateDataPointDisplay(int iPoint, int nItem)
	{
		FUNCTION_ENTRY ("updateDataPointDisplay()");

		if (NULL == m_realPModel)
		{
			return;
		}

		InformationItem displayItem = m_realPModel->getInformationItem(iPoint);

    	SetItemText(nItem, DescriptionColumn, CONVERT_FROM_STD(displayItem.description));

    	setItemTextImage(nItem, TypeColumn, displayItem.shortName, false);
    									
		if ( TimestampColumn != -1 )
		{
    		setItemTextImage(nItem, TimestampColumn, displayItem.timestamp, false);
		}

                // TES#1034: Always update status (quality) / current value columns of datapoints
    	setItemTextImage(nItem, ValueColumn, displayItem.valueString, true);

		if( QualityColumn != -1 )
		{
    		setItemTextImage(nItem, QualityColumn, displayItem.qualityString, false);
		}

		FUNCTION_EXIT;
	}

	// By using the preprocessed index vector, reduce linear search and ensure only those
	// datapoints had been sorted and config valid were displayed.
	void CInformationListCtrl::displayDataPoints()
	{
		FUNCTION_ENTRY ("displayDataPoints()");
	
		int validItem = 0;
		int difference = 0;
		int totalItem = GetItemCount();

		// No equipment defined yet.
		if (m_realPModel==NULL) 
		{
			return;
		}

		int		 nItem = 0;

		std::vector<InformationItem> allDisplayItems = m_realPModel->getAllInfomationDisplayItem();
		if(allDisplayItems.size()==0)
		{
			DeleteAllItems();
			return;
		}

		for(unsigned int index =0; index < allDisplayItems.size(); ++index)
		{
			InformationItem displayItem = allDisplayItems[index];

			validItem++;

			// Description
			if( validItem > GetItemCount() )
			{	
				nItem = insertItemTextImage(displayItem.iPoint, validItem-1, displayItem.description, false);	
				setItemTextImage(nItem, DescriptionColumn, displayItem.description, false);
				SetItemData(nItem, displayItem.iPoint);
			}
			else
			{
				nItem = validItem-1;
				SetItemData(nItem, displayItem.iPoint);
				SetItemText(nItem, DescriptionColumn, CONVERT_FROM_STD(displayItem.description));

			}

			// Type of the data point
			setItemTextImage(nItem, TypeColumn, displayItem.shortName, false);

			//timestamp
			if( TimestampColumn != -1 )
			{
				setItemTextImage(nItem, TimestampColumn, displayItem.timestamp, false);
			}		

			// TES#1034: Always update status (quality) / current value columns of datapoints
			// Status of data point
			setItemTextImage(nItem, ValueColumn, displayItem.valueString, true);

			if( QualityColumn != -1 )
			{
				setItemTextImage(nItem, QualityColumn, displayItem.qualityString, false);
			}

			// set the flag to indicate data points are displayed on screen
			m_dataPointsDisplayedOnScreen = true;    	
		}

		// Delete invalid item
		if( validItem > 0 )
		{
			difference = totalItem - validItem;

			if ( difference > 0 )
			{
				for ( int i=0; i<difference; i++)
					DeleteItem(totalItem-1-i);
			}
		}
		else if( validItem == 0 && totalItem != 0 )
		{
			for ( int i=0; i<totalItem; i++)
				DeleteItem(i);
		}

		FUNCTION_EXIT;
	}

	// ICellManager Implementation
	ICellManager::CellType CInformationListCtrl::GetCellType(CEditableListCtrl* listCtrl, int item, int subItem)
	{
	//	FUNCTION_ENTRY ("GetCellType()");
	//	FUNCTION_EXIT;

		return CT_NOTEDITABLE;
	};


    unsigned long CInformationListCtrl::GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem)
    {
        return 0;
    }


	ICellManager::CellData CInformationListCtrl::GetCellData(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		//FUNCTION_ENTRY ("GetCellData()");

		ICellManager::CellData   data;

		//FUNCTION_EXIT;

		return data;
	}

    TAUnicodeString CInformationListCtrl::GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return _T("");
    }

    char CInformationListCtrl::GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return 0;
    }

	bool CInformationListCtrl::UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value)
	{
	//	FUNCTION_ENTRY ("UpdateCellValue()");
	//	FUNCTION_EXIT;

		return false;
	}

	bool CInformationListCtrl::isAnyDataPointsDisplayedOnScreen() const
	{
		return m_dataPointsDisplayedOnScreen;
	}

	//TD15438, override the function to get a chance to adjust columns width.
	BOOL CInformationListCtrl::SetWindowPos( const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags )
	{

		//Call the base class member first
		BOOL isSucceeded = CListCtrl::SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);
		
		//To adjust the column width if needed.
		if (isSucceeded)
		{
			CRect rectItem, rectHeader, rectList;
			GetItemRect(0, &rectItem, LVIR_BOUNDS);
			GetHeaderCtrl()->GetWindowRect(&rectHeader);
			GetClientRect(&rectList);
			
			//if horizontal and scroll bar appear, adjust a column to let horizontal scroll bar dispear
			if (rectList.Height() < rectItem.Height() * GetItemCount() + rectHeader.Height() &&
				rectList.Width() < rectItem.Width())
			{
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Column Width is adjusted: itemH=%d; itemNo=%d; itemW=%d; headH=%d; listH=%d; listW=%d", 
					rectItem.Height(), GetItemCount(), rectItem.Width(), rectHeader.Height(), rectList.Height(), rectList.Width());

				int columnWidth = GetColumnWidth(DescriptionColumn);
				int vsrollWidth = GetSystemMetrics(SM_CXVSCROLL);
				
				//the minimum width is 100
				if (columnWidth - vsrollWidth > 100)
				{
					SetColumnWidth(DescriptionColumn, columnWidth - vsrollWidth);
				}
			}
		}
		
		return isSucceeded;		
	}

	void CInformationListCtrl::resetList()
	{
		m_isReset = true;
		setupList();
		m_isReset = false;
	}

}