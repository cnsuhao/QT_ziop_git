/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/InhibitListCtrl.cpp $
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
#include "app/scada/inspector_panel/src/InhibitListCtrl.h"

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
		CONTROL_INHIBIT_COLUMN		= 3,
		SCAN_INHIBIT_COLUMN			= 4,
		ALARM_INHIBIT_COLUMN		= 5,
		MMS_INHIBIT_COLUMN			= 6,
		CURRENT_VALUE_COLUMN		= 7
	};

	/////////////////////////////////////////////////////////////////////////////
	// CInhibitListCtrl

	CInhibitListCtrl::CInhibitListCtrl() 
	:
	CAbstractDataPointListCtrl(), 
	m_dataPointsDisplayedOnScreen ( false )
	{
		FUNCTION_ENTRY ("CInhibitListCtrl()");

		// We will manage the cell requests
		SetManager(this);
		//m_accessAllowed = true;

		FUNCTION_EXIT;
	}

	CInhibitListCtrl::~CInhibitListCtrl()
	{
		FUNCTION_ENTRY ("~CInhibitListCtrl()");
		FUNCTION_EXIT;
	}

	/* SetEquipment
	 *
	 * This function will set the name of the equipment that the data points are to be displayed for.It can 
	 * be called before or after the windows control is on the screen.
	 */

	// Helper functions

	void CInhibitListCtrl::setupList()
	{
		FUNCTION_ENTRY ("setupList()");

		SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_SUBITEMIMAGES | CAbstractDataPointListCtrl::LVS_EX_LABELTIP_XXX);

		// Headings
		this->InsertColumn(DUMMY_COLUMN,				TALoadString(STRING_ID_INSPECTOR_PANEL_0013).GetString());//_T("Dummy"));
   		this->InsertColumn(DESCRIPTION_COLUMN,			TALoadString(STRING_ID_INSPECTOR_PANEL_0014).GetString());//_T("Description"));
		this->InsertColumn(DATAPOINT_TYPE_COLUMN,		TALoadString(STRING_ID_INSPECTOR_PANEL_0015).GetString());//_T("Type"));
   		this->InsertColumn(CONTROL_INHIBIT_COLUMN,		TALoadString(STRING_ID_INSPECTOR_PANEL_0016).GetString());//_T("Ctrl"));
		this->InsertColumn(SCAN_INHIBIT_COLUMN,			TALoadString(STRING_ID_INSPECTOR_PANEL_0017).GetString());//_T("Scan"));
		this->InsertColumn(ALARM_INHIBIT_COLUMN,		TALoadString(STRING_ID_INSPECTOR_PANEL_0018).GetString());//_T("Alarm"));
		this->InsertColumn(MMS_INHIBIT_COLUMN,			TALoadString(STRING_ID_INSPECTOR_PANEL_0019).GetString());//_T("MMS"));
		this->InsertColumn(CURRENT_VALUE_COLUMN,		TALoadString(STRING_ID_INSPECTOR_PANEL_0020).GetString());//_T("Current Value"));

		// Using a 'dummy' column for index 0 prevents
		// MFC from indenting the text by the image size for the 'real' first column
		this->SetColumnWidth(DUMMY_COLUMN,				0);
		this->SetColumnWidth(DESCRIPTION_COLUMN,		210);
		this->SetColumnWidth(DATAPOINT_TYPE_COLUMN,		50);
		this->SetColumnWidth(CONTROL_INHIBIT_COLUMN,	50);
		this->SetColumnWidth(SCAN_INHIBIT_COLUMN,		50);
		this->SetColumnWidth(ALARM_INHIBIT_COLUMN,		55);
		this->SetColumnWidth(MMS_INHIBIT_COLUMN,		50);
		this->SetColumnWidth(CURRENT_VALUE_COLUMN,		130);

		FUNCTION_EXIT;
	}


	std::string CInhibitListCtrl::getMyPModelName()
	{
		return PMODEL_INHIBIT_LISTCTRL; 
	}

	void CInhibitListCtrl::setupPModel()
	{
		m_realPModel = dynamic_cast<InhibitListCtrlModel*> (m_pModel);

		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
		m_pEquipmentPModel = dynamic_cast<AbstractEquipmentPModel*>(m_realPModel);
		TA_ASSERT(m_pEquipmentPModel != NULL, "PModel should be valid");

		m_realPModel->attachView(this,PMODEL_NOTIFY_REFRESH_ALL_DATA);
		m_realPModel->attachView(this,PMODEL_NOTIFY_UPDATE_DATA);
	}


	void CInhibitListCtrl::updateDataPointDisplay(int iPoint, int nItem)
	{
		FUNCTION_ENTRY ("updateDataPointDisplay()");
		displayItem(nItem);
	}

	// By using the preprocessed index vector, reduce linear search and ensure only those
	// datapoints had been sorted and config valid were displayed.
	void CInhibitListCtrl::displayDataPoints()
	{
		FUNCTION_ENTRY ("displayDataPoints()");

		int validItem = 0;
		int difference = 0;
		int totalItem = GetItemCount();

		if (m_realPModel==NULL)
		{
			return;
		}
		int				nItem = 0;
		std::string		value;
		unsigned int	iPoint;


		std::vector<InhibitItem> allInhibitItems = m_realPModel->getAllInhibitDisplayItem();
		if(allInhibitItems.size() == 0)
		{
			DeleteAllItems();
			return;
		}

		for(unsigned int index =0; index < allInhibitItems.size(); ++index)
		{
			iPoint = allInhibitItems[index].iPoint;

			validItem++;

			if( validItem > GetItemCount() )
			{
				std::string description = allInhibitItems[index].description;
				nItem = insertItemTextImage(iPoint, validItem-1, description, false);						
				SetItemData(nItem, iPoint);
			}
			else
			{
				nItem = validItem-1;
				SetItemData(nItem, iPoint);
			}

			displayItem(nItem);

			// set the flag to indicate data points are displayed on screen
			m_dataPointsDisplayedOnScreen = true;
		}
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

	void CInhibitListCtrl::refreshDataPoints()
	{
		FUNCTION_ENTRY ("refreshDataPoints()");

		// No equipment defined yet.
		if (m_realPModel==NULL)
		{
			return;
		}
		displayDataPoints(); 

		FUNCTION_EXIT;
	}


	// ICellManager Implementation
	ICellManager::CellType CInhibitListCtrl::GetCellType(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("GetCellType()");

		// Check if item is valid  (We can get -1 sometimes but not sure why, should check editable list control)
		if (item<0)	
		{
			return CT_NOTEDITABLE;
		}

		unsigned int iPoint = GetItemData(item);
	
		switch (subItem)
		{
		case SCAN_INHIBIT_COLUMN:
		case ALARM_INHIBIT_COLUMN:
		case MMS_INHIBIT_COLUMN:
			{
				if (m_realPModel->isWritableDp(iPoint) == false)
					return (isCellEnabled (item, subItem) ? CT_CHECKBOX : CT_CHECKBOX_DISABLED);
				else
					return CT_NOTEDITABLE;
			}
		case CONTROL_INHIBIT_COLUMN:
			{
				if (m_realPModel->isWritableDp(iPoint) != false)
					return (isCellEnabled (item, subItem) ? CT_CHECKBOX : CT_CHECKBOX_DISABLED);
				else
					return CT_NOTEDITABLE;
			}

		default:
			return CT_NOTEDITABLE;
		}

		FUNCTION_EXIT;

		return CT_NOTEDITABLE;
	}


    unsigned long CInhibitListCtrl::GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem)
    {
        return 0;
    }


	ICellManager::CellData CInhibitListCtrl::GetCellData(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("GetCellData()");

		TA_ASSERT (m_realPModel!=NULL,"Equipment not defined");

		ICellManager::CellData   data;

		FUNCTION_EXIT;

		return data;
	}

    TAUnicodeString CInhibitListCtrl::GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return _T("");
    }

    char CInhibitListCtrl::GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return 0;
    }

	bool CInhibitListCtrl::inhibitOverrideConfirm(const int inhibitOverrideItem, unsigned int iPoint, bool enable)
	{
		FUNCTION_ENTRY ("inhibitOverrideConfirm()");

        TAUnicodeString actionName, actionType;
        actionType.LoadString(inhibitOverrideItem);

		TAUnicodeString dpDescription = TAUnicodeString::CreateFromMultitByteString( m_realPModel->getDataPointDescription(iPoint).c_str() );
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

	bool CInhibitListCtrl::UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value)
	{
		FUNCTION_ENTRY ("UpdateCellValue()");

		TA_ASSERT (m_realPModel!=NULL,"Equipment not defined.");

		std::string  strValue = value.toMultiByteStdString();
		std::string message;
		bool updateAnimation = false;

		// Get the data point array position
		UINT iPoint = listCtrl->GetItemData(item);
		
		std::stringstream statusLine;
		statusLine << m_realPModel->getDataPointDescription(iPoint);


		switch (subItem)
		{
			case SCAN_INHIBIT_COLUMN:
			{	
				bool bRequestScanInhibit =  strValue == "Yes";
				bool isOkToSetScanInhibit = m_realPModel->isOkToSetScanInhibit(iPoint,bRequestScanInhibit);

				if(isOkToSetScanInhibit && inhibitOverrideConfirm(IDS_SCAN_INHIBIT, iPoint, bRequestScanInhibit))
				{
					try
					{
						updateAnimation = m_realPModel->setScanInhibit(iPoint,bRequestScanInhibit);
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0122, CONVERT_FROM_STD( m_realPModel->getChildDataPointDescription(iPoint) ), bRequestScanInhibit?TALoadString(STRING_ID_INSPECTOR_PANEL_0120).GetString() : TALoadString(STRING_ID_INSPECTOR_PANEL_0121).GetString() );
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					catch(OutputLogicException&ex)
					{
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0123,CONVERT_FROM_STD( m_realPModel->getChildDataPointDescription(iPoint) ), bRequestScanInhibit?TALoadString(STRING_ID_INSPECTOR_PANEL_0120).GetString() : TALoadString(STRING_ID_INSPECTOR_PANEL_0121).GetString() );
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					
				}

				// if the flag is set
				if ( true == updateAnimation )
				{
					listCtrl->showCheckIcon(item, subItem, strValue == "Yes", true);
					displayItem(item);
				}

				break;
			} 

			case ALARM_INHIBIT_COLUMN:
			{
				bool bRequestAlarmInhibit =  strValue == "Yes";
				bool isOkToSetAlarmInhibit = m_realPModel->isOkToSetAlarmInhibit(iPoint,bRequestAlarmInhibit);

					// get confirmation from the operator
				if (isOkToSetAlarmInhibit && inhibitOverrideConfirm(IDS_ALARM_INHIBIT, iPoint, bRequestAlarmInhibit) )
				{
					try
					{
						updateAnimation = m_realPModel->setAlarmInhibit(iPoint, bRequestAlarmInhibit);
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0124, CONVERT_FROM_STD( m_realPModel->getChildDataPointDescription(iPoint) ), bRequestAlarmInhibit?TALoadString(STRING_ID_INSPECTOR_PANEL_0120).GetString() : TALoadString(STRING_ID_INSPECTOR_PANEL_0121).GetString() );
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					catch(OutputLogicException&ex)
					{
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0125,CONVERT_FROM_STD( m_realPModel->getChildDataPointDescription(iPoint) ), bRequestAlarmInhibit?TALoadString(STRING_ID_INSPECTOR_PANEL_0120).GetString() : TALoadString(STRING_ID_INSPECTOR_PANEL_0121).GetString() );
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					
				}

				// if the flag is set
				if ( true == updateAnimation )
				{
					listCtrl->showCheckIcon(item, subItem, strValue == "Yes", true);
					displayItem(item);
				}
			
				break;
			}

			case MMS_INHIBIT_COLUMN:
			{
				bool bRequestMmsInhibit =  strValue == "Yes";
				bool isOkToSetAlarmInhibit = m_realPModel->isOkToSetMmsInhibit(iPoint,bRequestMmsInhibit);
					
				if (isOkToSetAlarmInhibit && inhibitOverrideConfirm(IDS_MMS_INHIBIT, iPoint, bRequestMmsInhibit))
				{
					try
					{
						updateAnimation = m_realPModel->setMmsInhibit(iPoint, bRequestMmsInhibit);
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0126, CONVERT_FROM_STD( m_realPModel->getChildDataPointDescription(iPoint) ), bRequestMmsInhibit?TALoadString(STRING_ID_INSPECTOR_PANEL_0120).GetString() : TALoadString(STRING_ID_INSPECTOR_PANEL_0121).GetString() );
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					catch(OutputLogicException&ex)
					{
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0127,CONVERT_FROM_STD( m_realPModel->getChildDataPointDescription(iPoint) ), bRequestMmsInhibit?TALoadString(STRING_ID_INSPECTOR_PANEL_0120).GetString() : TALoadString(STRING_ID_INSPECTOR_PANEL_0121).GetString() );
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
				}

				// if the flag is set
				if ( true == updateAnimation )
				{
					listCtrl->showCheckIcon(item, subItem, strValue == "Yes", true);
					displayItem(item);
				}
			
				break;
			}

			case CONTROL_INHIBIT_COLUMN:
			{
				bool bRequestControlInhibit =  strValue == "Yes";
				bool isOkToSetControlInhibit = m_realPModel->isOkToSetControlInhibit(iPoint,bRequestControlInhibit);
					// get confirmation from the operator
				if (isOkToSetControlInhibit && inhibitOverrideConfirm(IDS_CONTROL_INHIBIT, iPoint, bRequestControlInhibit))
				{
					try
					{
						updateAnimation = m_realPModel->setControlInhibit(iPoint,bRequestControlInhibit);
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0128, CONVERT_FROM_STD( m_realPModel->getChildDataPointDescription(iPoint) ), bRequestControlInhibit?TALoadString(STRING_ID_INSPECTOR_PANEL_0120).GetString() : TALoadString(STRING_ID_INSPECTOR_PANEL_0121).GetString() );
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					catch(OutputLogicException&ex)
					{
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0129,CONVERT_FROM_STD( m_realPModel->getChildDataPointDescription(iPoint) ), bRequestControlInhibit?TALoadString(STRING_ID_INSPECTOR_PANEL_0120).GetString() : TALoadString(STRING_ID_INSPECTOR_PANEL_0121).GetString() );
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
				}

				// if the flag is set
				if ( true == updateAnimation )
				{
					listCtrl->showCheckIcon(item, subItem, strValue == "Yes", true);
					displayItem(item);
				}

				break;
			}

			default:
			{
				break;
			}
		}
		
	
		FUNCTION_EXIT;

		// Display is always handled here.
		return false;
	}

	void CInhibitListCtrl::displayItem(int nItem)
	{
		FUNCTION_ENTRY ("displayItem()");
		unsigned int iPoint = GetItemData(nItem);

		InhibitItem inhibitItem = m_realPModel->getInhibitListCtrlItem(iPoint);

		if (inhibitItem.isWritable == false)
		{
			setItemTextImage(nItem, DESCRIPTION_COLUMN, inhibitItem.description, false);

			setItemTextImage(nItem, DATAPOINT_TYPE_COLUMN, inhibitItem.type, false);

			// Control Inhibit - not used for Information Data Points
			// Scan Inhibit
			bool isChecked = false, isEnabled =false;

			isChecked = inhibitItem.isScanInhibit;
			isEnabled = m_realPModel->isScanInhibitColumnEnable(iPoint);
			showCheckIcon(nItem, SCAN_INHIBIT_COLUMN, isChecked, isEnabled);

			// Alarm Inhibit

			isChecked|= inhibitItem.isAlarmInhibit;
			//isEnabled = isCellEnabled(nItem, ALARM_INHIBIT_COLUMN);
			isEnabled = m_realPModel->isAlarmInhibitColumnEnable(iPoint);
			showCheckIcon(nItem, ALARM_INHIBIT_COLUMN, isChecked, isEnabled);
			
			// MMS Inhibit
			isChecked |= inhibitItem.isMmsInhibit;
			isEnabled = m_realPModel->isMmsInhibitColumnEnable(iPoint);
			showCheckIcon(nItem, MMS_INHIBIT_COLUMN, isChecked, isEnabled);

			// should not show control inhibit checkbox & always update value
			destroyCheckIcon(nItem, CONTROL_INHIBIT_COLUMN);
			setItemTextImage(nItem, CURRENT_VALUE_COLUMN, inhibitItem.valueString, true);
		}
		else
		{
			// Description (Equipment Attributes)
			setItemTextImage(nItem, DESCRIPTION_COLUMN, inhibitItem.description, false);
			setItemTextImage(nItem, DATAPOINT_TYPE_COLUMN, inhibitItem.type, false);
			
			// Control Inhibit
			bool isChecked = false, isEnabled = false;
			
			isChecked = inhibitItem.isCtrlInhibit;
			isEnabled = m_realPModel->isControlInhibitColumnEnable(iPoint);
			showCheckIcon(nItem, CONTROL_INHIBIT_COLUMN, isChecked, isEnabled);

			// remove scan, alarm, mms inhibit checkboxes and no current value displayed
			destroyCheckIcon(nItem, SCAN_INHIBIT_COLUMN);
			destroyCheckIcon(nItem, ALARM_INHIBIT_COLUMN);
			destroyCheckIcon(nItem, MMS_INHIBIT_COLUMN);
			setItemTextImage(nItem, CURRENT_VALUE_COLUMN, "", false); 
		}
	
		FUNCTION_EXIT;
	}

	bool CInhibitListCtrl::isCellEnabled (int nItem, int nSubItem)
	{
		FUNCTION_ENTRY ("isCellEnabled()");
		// Check if item is valid  (We can get -1 sometimes but not sure why, should check editable list control)
		if (nItem<0)	
		{	
			return false;
		}
		unsigned int iPoint = GetItemData(nItem);
		
		if (m_realPModel->isWritableDp(iPoint) == false)
		{
			switch (nSubItem)
			{
			case SCAN_INHIBIT_COLUMN:
				{
					return m_realPModel->isScanInhibitColumnEnable(iPoint);
				}
				break;
			case ALARM_INHIBIT_COLUMN:
				{
					return m_realPModel->isAlarmInhibitColumnEnable(iPoint);
				}
				break;
			case MMS_INHIBIT_COLUMN:
				{
					return m_realPModel->isMmsInhibitColumnEnable(iPoint);
				}
				break;
			default:
				{
					return false;
				}					
			}
		}
		else
		{
			if (nSubItem == CONTROL_INHIBIT_COLUMN)
			{
				return m_realPModel->isControlInhibitColumnEnable(iPoint);		
			}
			else
			{
				return false;
			}
		}
	
		FUNCTION_EXIT;

		return false;
	}


	bool CInhibitListCtrl::isAnyDataPointsDisplayedOnScreen() const
	{
		//todo->
		//return m_equipment->hasInformationPoint();
		return true;
	}

	//TD15438, override the function to get a chance to adjust columns width.
	BOOL CInhibitListCtrl::SetWindowPos( const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags )
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
	
	void CInhibitListCtrl::updateDataNode(TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataPoint()");

		if (m_hWnd==0 || m_realPModel == NULL ) 
		{
			return;
		}

		if (updateType == CombinedInhibitMMSUpdate)
		{
			for (int nItem=0;nItem<this->GetItemCount();nItem++)
			{
				displayItem(nItem);
			}
		}

		FUNCTION_EXIT;
	}
}