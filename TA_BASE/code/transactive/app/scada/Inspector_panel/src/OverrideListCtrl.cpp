/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/OverrideListCtrl.cpp $
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

#ifdef __WIN32__
#pragma warning(disable:4786)
#endif

#include "app/scada/Inspector_Panel/src/stdafx.h"

#include "app/scada/inspector_panel/src/InspectorPanel.h"
#include "app/scada/inspector_panel/src/InspectorPanelDlg.h"
#include "app/scada/inspector_panel/src/ConfirmOverrideDialog.h" 
#include "app/scada/inspector_panel/src/OverrideListCtrl.h"
#include "AppLogicExceptionConverter.h"

#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "core/message/types/InspectorPanelAudit_MessageTypes.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/DebugUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace TA_Base_App
{
	enum columns
	{
		DUMMY_COLUMN				= 0,
		DESCRIPTION_COLUMN			= 1,
		DATAPOINT_TYPE_COLUMN		= 2,
		OVERRIDE_ENABLE_COLUMN		= 3,
		OVERRIDE_VALUE_COLUMN		= 4,
		CURRENT_VALUE_COLUMN		= 5
	};

	/////////////////////////////////////////////////////////////////////////////
	// COverrideListCtrl

	COverrideListCtrl::COverrideListCtrl() 
	:
	CAbstractDataPointListCtrl(),
	m_auditMessageUtility (TA_Base_Core::InspectorPanelAudit::Context),
	m_dataPointsDisplayedOnScreen ( false )
	{
		FUNCTION_ENTRY ("COverrideListCtrl()");

		// We will manage the cell requests
		SetManager(this);

		FUNCTION_EXIT;
	}

	COverrideListCtrl::~COverrideListCtrl()
	{
		FUNCTION_ENTRY ("~COverrideListCtrl()");
		FUNCTION_EXIT;
	}

	std::string		COverrideListCtrl::getMyPModelName()
	{
		return PMODEL_OVERRIDE_LISTCTRL;
	}

	void	COverrideListCtrl::setupPModel()
	{
		m_realPModel = dynamic_cast<OverrideListCtrlModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");

		m_pEquipmentPModel = dynamic_cast<AbstractEquipmentPModel*>(m_realPModel);
		TA_ASSERT(m_pEquipmentPModel != NULL, "PModel should be valid");

		m_realPModel->attachView(this,PMODEL_NOTIFY_REFRESH_ALL_DATA);
		m_realPModel->attachView(this,PMODEL_NOTIFY_UPDATE_DATA);
	}

	/* SetEquipment
	 *
	 * This function will set the name of the equipment that the data points are to be displayed for.It can 
	 * be called before or after the windows control is on the screen.
	 */

	// Helper functions

	void COverrideListCtrl::setupList()
	{
		FUNCTION_ENTRY ("setupList()");

		SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_SUBITEMIMAGES | CAbstractDataPointListCtrl::LVS_EX_LABELTIP_XXX);

		// Headings
   		this->InsertColumn(DUMMY_COLUMN,			TALoadString(STRING_ID_INSPECTOR_PANEL_OVERRIDE_DUMMY).GetString());//_T("Dummy"));
		this->InsertColumn(DESCRIPTION_COLUMN,		TALoadString(STRING_ID_INSPECTOR_PANEL_OVERRIDE_DESC).GetString());//_T("Description"));
		this->InsertColumn(DATAPOINT_TYPE_COLUMN,	TALoadString(STRING_ID_INSPECTOR_PANEL_OVERRIDE_TYPE).GetString());//_T("Type"));
		this->InsertColumn(OVERRIDE_ENABLE_COLUMN,	TALoadString(STRING_ID_INSPECTOR_PANEL_OVERRIDE_OVRRED).GetString());//_T("Ovr"));
		this->InsertColumn(OVERRIDE_VALUE_COLUMN,	TALoadString(STRING_ID_INSPECTOR_PANEL_OVERRIDE_OVRRV).GetString());//_T("Override Value"));
		this->InsertColumn(CURRENT_VALUE_COLUMN,	TALoadString(STRING_ID_INSPECTOR_PANEL_OVERRIDE_CURRV).GetString());//_T("Current Value")); // FAT TES# 228

		// Using a 'dummy' column for index 0 prevents
		// MFC from indenting the text by the image size for the
		// 'real' first column
		this->SetColumnWidth(DUMMY_COLUMN,				0);
		this->SetColumnWidth(DESCRIPTION_COLUMN,		210);
		this->SetColumnWidth(DATAPOINT_TYPE_COLUMN,		50);
		this->SetColumnWidth(OVERRIDE_ENABLE_COLUMN,	40);
		this->SetColumnWidth(OVERRIDE_VALUE_COLUMN,		120);
		this->SetColumnWidth(CURRENT_VALUE_COLUMN,		180);

		FUNCTION_EXIT;
	}

	void COverrideListCtrl::updateDataPointDisplay(int iPoint, int nItem)
	{
		displayItem(nItem);
	}

	// By using the preprocessed index vector, reduce linear search and ensure only those
	// datapoints had been sorted and config valid were displayed.	
	void COverrideListCtrl::displayDataPoints()
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

		int				nItem = 0;
		std::string		value;

		std::vector<OverrideItem> OverrideItems = m_realPModel->getAllOverrideItem();
		if(OverrideItems.size() ==0 )
		{
			DeleteAllItems();
			return;
		}

		for(unsigned int index =0; index < OverrideItems.size(); ++index)
		{
			validItem++;
			OverrideItem tmpItem = OverrideItems[index];
			if( validItem > GetItemCount() )
			{
				nItem = insertItemTextImage(tmpItem.iPoint, validItem-1, tmpItem.description, false);
			}
			else
			{
				nItem = validItem-1;
			}
			SetItemData(nItem, tmpItem.iPoint);
			displayItem(nItem, REFRESH_ALL);							

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

	void COverrideListCtrl::refreshDataPoints()
	{
		FUNCTION_ENTRY ("refreshDataPoints()");

		// No equipment defined yet.
		if (m_realPModel==NULL) 
			return;
		
		displayDataPoints();	

		FUNCTION_EXIT;
	}

	// ICellManager Implementation
	ICellManager::CellType COverrideListCtrl::GetCellType(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("GetCellType()");

		// Check if item is valid  (We can get -1 sometimes but not sure why, should check editable list control)
		// or No access to this dialog
		if (item<0 )	
		{
			return CT_NOTEDITABLE;
		}

		unsigned int iPoint = GetItemData(item);

		switch (subItem)
		{
		case OVERRIDE_ENABLE_COLUMN:
			if (m_realPModel->isNotWritableDp(iPoint))
			{
				return (isCellEnabled (item, subItem) ? CT_CHECKBOX : CT_CHECKBOX_DISABLED);
			}
			return CT_NOTEDITABLE;

		case OVERRIDE_VALUE_COLUMN:
		default:
			return CT_NOTEDITABLE;
		}

		FUNCTION_EXIT;

		return CT_NOTEDITABLE;
	}


    unsigned long COverrideListCtrl::GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem)
    {
        return 0;
    }


	ICellManager::CellData COverrideListCtrl::GetCellData(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("GetCellData()");

		TA_ASSERT (m_realPModel!=NULL,"Equipment not defined");

		ICellManager::CellData   data;

		FUNCTION_EXIT;

		return data;
	}

    TAUnicodeString COverrideListCtrl::GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return _T("");
    }

    char COverrideListCtrl::GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return 0;
    }

	bool COverrideListCtrl::inhibitOverrideConfirm(const int inhibitOverrideItem, unsigned int iPoint, bool enable)
	{
		FUNCTION_ENTRY ("inhibitOverrideConfirm()");

        TAUnicodeString actionName, actionType;
        actionType.LoadString(inhibitOverrideItem);
		
		TAUnicodeString dpDescription =TAUnicodeString::CreateFromMultitByteString(m_realPModel->getDataPointDescription(iPoint).c_str());
		int pos = dpDescription.Find ( _T("%"), 0 );
		if ( -1 != pos )
		{
			dpDescription.str().Insert ( pos, _T("%") );
		}

        AfxFormatString2(actionName.str(), enable?IDS_ENABLE_DATAPOINT:IDS_DISABLE_DATAPOINT, actionType.str(), dpDescription.str() );

        TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << actionName;
        UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);        
        FUNCTION_EXIT;
		return (selectedButton == IDYES);
	}

	bool COverrideListCtrl::UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value)
	{
		FUNCTION_ENTRY ("UpdateCellValue()");

		TA_ASSERT (m_realPModel!=NULL,"Equipment not defined.");

		std::string  strValue = value.toMultiByteStdString();
		std::string message;
		bool cellValueUpdated = false;

		// Get the data point array position
		UINT iPoint = listCtrl->GetItemData(item);

		std::string overrideValue ("");
		
		if (m_realPModel->isNotWritableDp(iPoint) == false)
		{
			return false;
		}

		try
		{
			// only applicable to input data points
			switch (subItem)
			{
				case OVERRIDE_ENABLE_COLUMN:
				{
					bool bRequestOverride = strValue == "Yes";
					if(bRequestOverride)//override the datapoint
					{
						bool isOkToOverrid = m_realPModel->isOkToOverride(iPoint);
						// if the override state is different
						if(!isOkToOverrid)
						{
							return false;
						}
						CConfirmOverrideDialog dlg(NULL, iPoint, m_realPModel);

						// if acknowledged to go ahead
						if (dlg.DoModal() == IDOK)
						{
							try
							{
								overrideValue = dlg.getOverrideValue();
								cellValueUpdated = m_realPModel->setManuallyOverrideValue (iPoint,overrideValue);
								TAUnicodeString *statusLineStr = new TAUnicodeString();
								*statusLineStr = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0190, CONVERT_FROM_STD(overrideValue));
								AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
							}
							catch(OutputLogicException&ex)
							{
								TAUnicodeString *statusLineStr = new TAUnicodeString();
								*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0191, CONVERT_FROM_STD(overrideValue),AppLogicExceptionConverter::ConvertException(ex).str());
								AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
							}
						}
					}
					else//ovverride off
					{					
						bool isOkToRemoveOverride = m_realPModel->isOkToRemoveOverride(iPoint);

						// get confirmation from the operator
						if (isOkToRemoveOverride && inhibitOverrideConfirm ( IDS_OVERRIDE, iPoint,  false ) )
						{
							try
							{
								cellValueUpdated =m_realPModel->removeOverride(iPoint);
								TAUnicodeString *statusLineStr = new TAUnicodeString();
								*statusLineStr = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0192, CONVERT_FROM_STD(m_realPModel->getChildDataPointDescription(iPoint)));
								AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
							}
							catch(OutputLogicException&ex)
							{
								TAUnicodeString *statusLineStr = new TAUnicodeString();
								*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0193, CONVERT_FROM_STD(m_realPModel->getChildDataPointDescription(iPoint)),AppLogicExceptionConverter::ConvertException(ex).str());
								AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
							}
						}
					}
				}
				break;

				case OVERRIDE_VALUE_COLUMN:
				default:
					break;
			
			}
			// end of switch statement

		}
		catch (...)
		{
			std::string message = "Error (unknown) when overriding DataPoint \"";
			message += m_realPModel->getDataPointDescription(iPoint) + "\".";

			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, 
				"UpdateCellValue() - %s", message.c_str());
		}

		// if the flag indicates that screen animation needs to be updated
		if ( true == cellValueUpdated )
		{
			// TD#3351 - force re-query of datapoint to get overriden value, in case override fails
			// TES #115 - force the 'Value' column to be refreshed
			displayItem ( item, REFRESH_ALL );
		}

		return cellValueUpdated;

		FUNCTION_EXIT;
	}

	void COverrideListCtrl::displayItem(int nItem, DWORD refreshType)
	{
		FUNCTION_ENTRY ("displayItem()");

		unsigned int iPoint = GetItemData(nItem);

		OverrideItem overrideItem = m_realPModel->getOverrideItem(iPoint);
		std::string  value;


		if( (m_realPModel->isNotWritableDp(iPoint) == true) && m_realPModel->isDpDiaplayable(iPoint)  )
		{
			// If a read only datapoint, able to refresh all fields
			// Otherwise we only refresh items specified as argument
			refreshType |= REFRESH_ALL;
		}

		if (REFRESH_DESCRIPTION & refreshType)
		{
			// Description (Equipment Attributes)
			setItemTextImage(nItem, DESCRIPTION_COLUMN, overrideItem.description, false);
		}

		if (REFRESH_TYPE & refreshType)
		{
			// Type of the data point
			setItemTextImage(nItem, DATAPOINT_TYPE_COLUMN, overrideItem.type, false);
		}

		if (REFRESH_OVERRIDE & refreshType)
		{
			bool isChecked = false, isEnabled = false;
			// Override Enable
			isChecked = overrideItem.isOverride;
			isEnabled = isCellEnabled(nItem, OVERRIDE_ENABLE_COLUMN);
			showCheckIcon(nItem, OVERRIDE_ENABLE_COLUMN, isChecked, isEnabled);
		}

		if (REFRESH_OVERRIDE_VALUE & refreshType)
		{
			SetItemText(nItem, OVERRIDE_VALUE_COLUMN, CONVERT_FROM_STD(overrideItem.overrideValue));
		}

 		setItemTextImage(nItem, CURRENT_VALUE_COLUMN, overrideItem.currentValue, true);         
		FUNCTION_EXIT;
	}


	bool COverrideListCtrl::isCellEnabled (int nItem, int nSubItem)
	{
		FUNCTION_ENTRY ("isCellEnabled()");

		// Check if item is valid  (We can get -1 sometimes but not sure why, should check editable list control)
		if (nItem<0)
		{
			return false;
		}
		unsigned int iPoint = GetItemData(nItem);

		try
		{
			// only applicable to input data points
			if (m_realPModel->isNotWritableDp(iPoint))
			{
				switch (nSubItem)
				{
					case OVERRIDE_ENABLE_COLUMN:
					{
						return m_realPModel->isOverrideColumnEnable(iPoint);
					}

					case OVERRIDE_VALUE_COLUMN:
					default:
					{
						return false;
					}						
				}
			}
		}
		catch (...)
		{
			// Not always available
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, 
				"isCellEnabled() - catch Unknown exception");
		}		

		return false;

		FUNCTION_EXIT;
	}

	

	bool COverrideListCtrl::isAnyDataPointsDisplayedOnScreen() const
	{
		return m_dataPointsDisplayedOnScreen;
	}

	//TD15438, override the function to get a chance to adjust columns width.
	BOOL COverrideListCtrl::SetWindowPos( const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags )
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
				
				int columnWidth = GetColumnWidth(CURRENT_VALUE_COLUMN);
				int vsrollWidth = GetSystemMetrics(SM_CXVSCROLL);
				
				//the minimum width is 100
				if (columnWidth - vsrollWidth > 100)
				{
					SetColumnWidth(CURRENT_VALUE_COLUMN, columnWidth - vsrollWidth);
				}
			}
		}
		
		return isSucceeded;
		
	}
	

}