/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/MeterListCtrl.cpp $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the list display used to show the basic control information in
  * the CControlDialog class.
  *
  */

#include "app/scada/Inspector_Panel/src/stdafx.h"

#include "app/scada/inspector_panel/src/InspectorPanel.h"
#include "app/scada/inspector_panel/src/InspectorPanelDlg.h"
#include "app/scada/inspector_panel/src/ConfirmCommandDialog.h"
#include "app/scada/inspector_panel/src/MeterListCtrl.h"
#include "app/scada/Inspector_Panel/src/ConfirmAdjustDialog.h"
#include "AppLogicExceptionConverter.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;

namespace TA_Base_App
{
    const COLORREF CMeterListCtrl::COLOUR_ALARM = 0x000000FF;  // colour to be used for showing alarms
    const COLORREF CMeterListCtrl::COLOUR_DISABLE = 0x00808080; // colour to be used for showing disabled data points
    const COLORREF CMeterListCtrl::COLOUR_NORMAL = 0x00000000; // colour to be used for showing normal data points

    /////////////////////////////////////////////////////////////////////////////
	// CDataPointListCtrl

	enum columns
	{
		DescriptionColumn			= 0,
		DataPointTypeColumn			= 1,
		CurrentValueColumn			= 2,
		MeterAdjustColumn			= 3,
	};

	CMeterListCtrl::CMeterListCtrl()
	:
	CAbstractDataPointListCtrl(),
	m_dataPointsDisplayedOnScreen ( false ),
	m_isInMeterAdjust ( false )
	{
		FUNCTION_ENTRY ("CMeterListCtrl()");

		// We will manage the cell requests
		SetManager(this);

		FUNCTION_EXIT;
	}

	CMeterListCtrl::~CMeterListCtrl()
	{
		FUNCTION_ENTRY ("~CMeterListCtrl()");
		FUNCTION_EXIT;
	}


	std::string		CMeterListCtrl::getMyPModelName()
	{
		return PMODEL_METER_LISTCTRL;
	}


	void	CMeterListCtrl::setupPModel()
	{
		m_realPModel = dynamic_cast<MeterListCtrlModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
		
		m_pEquipmentPModel = dynamic_cast<AbstractEquipmentPModel*>(m_realPModel);
		TA_ASSERT(m_pEquipmentPModel != NULL, "PModel should be valid");

		m_realPModel->attachView(this,PMODEL_NOTIFY_REFRESH_ALL_DATA);
		m_realPModel->attachView(this,PMODEL_NOTIFY_UPDATE_DATA);
	}


	BEGIN_MESSAGE_MAP(CMeterListCtrl, CAbstractDataPointListCtrl)
		//{{AFX_MSG_MAP(CMeterListCtrl)
		ON_WM_LBUTTONDOWN()
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()
	// Helper functions

	void CMeterListCtrl::setupList()
	{
		FUNCTION_ENTRY ("setupList()");

		SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_SUBITEMIMAGES | CAbstractDataPointListCtrl::LVS_EX_LABELTIP_XXX);

		// Headings
   		this->InsertColumn(DescriptionColumn,	TALoadString(STRING_ID_INSPECTOR_PANEL_0036).GetString() );//_T("Description"));
		this->InsertColumn(DataPointTypeColumn,	TALoadString(STRING_ID_INSPECTOR_PANEL_0037).GetString() );//_T("Type"));
		this->InsertColumn(CurrentValueColumn,	TALoadString(STRING_ID_INSPECTOR_PANEL_0038).GetString() );//_T("Current Value"));
		this->InsertColumn(MeterAdjustColumn,	TALoadString(STRING_ID_INSPECTOR_PANEL_0039).GetString() );//_T("Adjust Meter"));


		this->SetColumnWidth(DescriptionColumn,		210);
		this->SetColumnWidth(DataPointTypeColumn,	50);
		this->SetColumnWidth(CurrentValueColumn,	180);
		// TES995 - Specify LVSCW_AUTOSIZE_USEHEADER to fill rest of list control's width
		this->SetColumnWidth(MeterAdjustColumn,		LVSCW_AUTOSIZE_USEHEADER);

		FUNCTION_EXIT;
	}

	// By using the preprocessed index vector, reduce linear search and ensure only those
	// datapoints had been sorted and config valid were displayed.
	void CMeterListCtrl::displayDataPoints()
	{
		FUNCTION_ENTRY ("DisplayDataPoints()");

		int validItem = 0;
		int difference = 0;
		int totalItem = GetItemCount();
	
		if (NULL == m_realPModel)
		{
			return;
		}

		int				nItem = 0;
		std::string		value;

		std::vector<MeterItem> allMeterItems = m_realPModel->getAllMeterDisplayItem();

		if(0 == allMeterItems.size())
		{
			DeleteAllItems();
			return;
		}

		for(unsigned int index =0; index < allMeterItems.size(); ++index)
		{
			unsigned int iPoint = allMeterItems[index].iPoint;
			MeterItem  tmpItem = allMeterItems[index];
			validItem++;
			if( validItem > GetItemCount() )
			{
				std::string description = m_realPModel->getChildDataPointDescription(iPoint);
				nItem = insertItemTextImage(iPoint, validItem-1, tmpItem.description, false);	
			}
			else
			{
				nItem = validItem-1;
			}
			SetItemData(nItem, iPoint);

			displayItem ( nItem );

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

	//TD15879
	void CMeterListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (false == m_isInMeterAdjust)
		{
			CListCtrl::OnLButtonDown(nFlags, point);
		}
		else
		{
			// Prevent the LButton Down hold by the list before ConfirmAjust Dialog DoModal
			CPoint ptCursor;
			GetCursorPos(&ptCursor);
			ScreenToClient(&ptCursor);
			ReleaseCapture();
			SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(ptCursor.x, ptCursor.y));	
			SendMessage(WM_MOUSEMOVE, 0, MAKELPARAM(ptCursor.x, ptCursor.y));
			SendMessage(WM_KILLFOCUS, 0, MAKELPARAM(ptCursor.x, ptCursor.y));
		}
	}


	// ICellManager Implementation
	ICellManager::CellType CMeterListCtrl::GetCellType(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("GetCellType()");

		// if the specified item is valid
		if (NULL != m_realPModel && item >= 0)	
		{
			// only MeterAdjustColumn column is editable
			if ( ( subItem >= 0 ) && ( subItem == MeterAdjustColumn ) )
			{
				unsigned int iPoint = listCtrl->GetItemData ( item ) ;
				// get the DataPoint array from the equipment
				std::stringstream statusLine;
				statusLine << m_realPModel->getChildDataPointDescription(iPoint) << ": ";

				try
				{
					double doubleValue = m_realPModel->getChildValueAsDouble(iPoint);
				}
				catch (...)
				{
					// get double value error, so can't adjust the real value;
					LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, 
						"dataPoint getValueAsDouble exception happened.");
					return CT_NOTEDITABLE;
				}

				// check the operator profile for access
				bool isMeterAdjustAllowed = m_realPModel->isMeterAdjustPermitted(iPoint);

				// if the operator is allowed to adjust the totalised value
				if ( true == isMeterAdjustAllowed )
				{
					m_isInMeterAdjust = true;
					return CT_NUMBER;
				}
				else
				{
					statusLine << "No Access.";
				}
			}
		}

		return CT_NOTEDITABLE;

		FUNCTION_EXIT;
	};


    unsigned long CMeterListCtrl::GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem)
    {
        return 0;
    }


	ICellManager::CellData CMeterListCtrl::GetCellData(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("GetCellData()");

		TA_ASSERT (m_realPModel!=NULL,"Equipment not defined");

		ICellManager::CellData   data;

		FUNCTION_EXIT;

		return data;
	}

    TAUnicodeString CMeterListCtrl::GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return _T("");
    }

    char CMeterListCtrl::GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return 0;
    }

	bool CMeterListCtrl::UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value)
	{
		//todo
		FUNCTION_ENTRY ("UpdateCellValue()");

		TA_ASSERT (m_realPModel!=NULL,"Equipment not defined.");

		bool cellValueUpdated = false;
		bool adjustFailed = false;

		// if the specified item is valid
		if (item >= 0)	
		{
			// only MeterAdjustColumn column is editable
			if ( ( subItem >= 0 ) && ( subItem == MeterAdjustColumn ) )
			{
				std::string  strValue = value.toMultiByteStdString();
				// Get the data point array position
				UINT iPoint = listCtrl->GetItemData(item);
				std::stringstream statusLine;
				statusLine.precision(10);
				try
				{
					double correction = 0;
					double adjustedValue = 0;

					//statusLine << m_realPModel->getChildDataPointDescription(iPoint) << ": ";
					//statusLine << "Adjusted From " << m_realPModel->getChildValueAsString(iPoint);
					//statusLine << " By " << strValue;

					//TAAfxFormatString3(STRING_ID_INSPECTOR_PANEL_0170, CONVERT_FROM_STD(m_realPModel->getChildDataPointDescription(iPoint)), CONVERT_FROM_STD(m_realPModel->getChildValueAsString(iPoint)), value.c_str());

					adjustFailed = !m_realPModel->checkValueAdjustable(iPoint,strValue,correction,adjustedValue);
					// if the flag is set
					if ( false == adjustFailed )
					{
						CConfirmAdjustDialog     dlg;

						// Confirm update
						//std::stringstream displayValue;
						//displayValue.precision(12); //14420++
						//displayValue << "Adjust Meter: "  << correction << " - Adjusted Current Value: " << adjustedValue;
						std::stringstream correctionStream, adjustedValueStream;
						correctionStream<<correction;
						adjustedValueStream<<adjustedValue;
						TAUnicodeString displayValue = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0171, CONVERT_FROM_STD(correctionStream.str()), CONVERT_FROM_STD(adjustedValueStream.str()));

						dlg.m_equipment = TAUnicodeString::CreateFromMultitByteString( m_realPModel->getDataNodeAssetName().c_str() );
						dlg.m_equipment += _T("\r\n");
						dlg.m_equipment += TAUnicodeString::CreateFromMultitByteString( m_realPModel->getDataNodeLabel().c_str() );
						dlg.m_dataPoint = TAUnicodeString::CreateFromMultitByteString( m_realPModel->getChildDataPointDescription(iPoint).c_str() );
						//dlg.m_newValue  = TAUnicodeString::CreateFromMultitByteString( displayValue.str().c_str() );
						dlg.m_newValue  = displayValue;

						// if acknowledged to go ahead
						if (dlg.DoModal()==IDOK)
						{
							try
							{
								m_realPModel->setMeteredCorrectionValue(iPoint,correction);

								TAUnicodeString *statusLineStr = new TAUnicodeString();
								*statusLineStr = TAAfxFormatString3(STRING_ID_INSPECTOR_PANEL_0170, CONVERT_FROM_STD(m_realPModel->getChildDataPointDescription(iPoint)), CONVERT_FROM_STD(m_realPModel->getChildValueAsString(iPoint)), value.c_str());
								AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
								cellValueUpdated = true;
							}
							catch(OutputLogicException&ex)
							{
								TAUnicodeString errStr = AppLogicExceptionConverter::ConvertException(ex);
								TAUnicodeString *statusLineStr = new TAUnicodeString();
								*statusLineStr = TAAfxFormatString4(STRING_ID_INSPECTOR_PANEL_0172, CONVERT_FROM_STD(m_realPModel->getChildDataPointDescription(iPoint)), CONVERT_FROM_STD(m_realPModel->getChildValueAsString(iPoint)), value.c_str(), errStr.str());
								AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
							}
						}
					}
					else
					{
						statusLine << " Failed.";
					}
				}
				catch (...)
				{
					adjustFailed = true;
					statusLine << " Failed.";
				}

				// Post status message
				if (true == adjustFailed || true == cellValueUpdated)
				{
					std::string* statusLineStr = new std::string(statusLine.str());
					//AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
				}
			}
		}

		// Reset the flag so that the LBUTTONDOWN will be performed normally
		m_isInMeterAdjust = false; 

		return cellValueUpdated;

		FUNCTION_EXIT;
	}

	void CMeterListCtrl::updateDataPointDisplay(int iPoint, int nItem)
	{
		FUNCTION_ENTRY ("UpdateDataPointDisplay()");

		if (NULL == m_realPModel)
		{
			return;
		}

		displayItem ( nItem );

		FUNCTION_EXIT;
	}
	

	void CMeterListCtrl::updatePermissions()
	{
		EnableWindow(true);

	}

	void CMeterListCtrl::displayItem(int nItem)
	{
		unsigned int iPoint = GetItemData( nItem );
		
		MeterItem meterItem = m_realPModel->getMeterListCtrlItem(iPoint);
		setItemTextImage(nItem, DescriptionColumn, meterItem.description, false);
		setItemTextImage(nItem, DataPointTypeColumn, meterItem.type, false);
		setItemTextImage(nItem, CurrentValueColumn, meterItem.currentValue, true);
	}

	bool CMeterListCtrl::isAnyDataPointsDisplayedOnScreen() const
	{
		return m_dataPointsDisplayedOnScreen;
	}

	//TD15438, override the function to get a chance to adjust columns width.
	BOOL CMeterListCtrl::SetWindowPos( const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags )
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
				
				int columnWidth = GetColumnWidth(CurrentValueColumn);
				int vsrollWidth = GetSystemMetrics(SM_CXVSCROLL);
				
				//the minimum width is 100
				if (columnWidth - vsrollWidth > 100)
				{
					SetColumnWidth(CurrentValueColumn, columnWidth - vsrollWidth);
				}
			}
		}
		
		return isSucceeded;	
	}
}
