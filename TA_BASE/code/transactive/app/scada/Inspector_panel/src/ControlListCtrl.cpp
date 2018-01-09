/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/ControlListCtrl.cpp $
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
 
#include "app/scada/inspector_panel/src/stdafx.h"


#include "app/scada/inspector_panel/src/InspectorPanel.h"
#include "app/scada/inspector_panel/src/InspectorPanelDlg.h"

#include "app/scada/inspector_panel/src/ConfirmCommandDialog.h"
#include "app/scada/inspector_panel/src/ControlListCtrl.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "AppLogicExceptionConverter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;

namespace TA_Base_App
{
    const COLORREF CControlListCtrl::COLOUR_ALARM = 0x000000FF;  // colour to be used for showing alarms
    const COLORREF CControlListCtrl::COLOUR_DISABLE = 0x00808080; // colour to be used for showing disabled data points
    const COLORREF CControlListCtrl::COLOUR_NORMAL = 0x00000000; // colour to be used for showing normal data points

    /////////////////////////////////////////////////////////////////////////////
	// CDataPointListCtrl

	//TD15649, simplified this class 
	//by just responsing to the control state events of the visiable output datapoints
	//instead of using triggering entity of all datapoints to refresh the control state of visiable output datapoints
	
	enum columns
	{
		EQUIPMENT_ATTRIBUTES_COLUMN	= 0,
		DATAPOINT_TYPE_COLUMN		= 1,
		CONTROL_STATE_COLUMN		= 2,
		NEW_VALUE_COLUMN			= 3,
		UNITS_COLUMN				= 4
	};

	CControlListCtrl::CControlListCtrl()
	:CAbstractDataPointListCtrl(),
	m_initialState(false), //wenching++ (TD14179)
	m_dataPointsDisplayedOnScreen ( false ),
	m_realPModel(NULL)
	{
		FUNCTION_ENTRY ("CControlListCtrl()");

		// We will manage the cell requests
		SetManager(this);
		//LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "begin create imagelist");
		//m_imagelist.Create(48, 48, ILC_COLOR32 , 5, 5);
		
		FUNCTION_EXIT;
	}

	CControlListCtrl::~CControlListCtrl()
	{
		FUNCTION_ENTRY ("~CControlListCtrl()");
		
		FUNCTION_EXIT;
	}

	std::string CControlListCtrl::getMyPModelName()
	{
		return PMODEL_CONTROL_LISTCTRL; 
	}

	void CControlListCtrl::setupPModel()
	{
		m_realPModel = dynamic_cast<ControlListCtrlModel*> (m_pModel);

 		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
 		m_pEquipmentPModel = dynamic_cast<AbstractEquipmentPModel*>(m_realPModel);
 		TA_ASSERT(m_pEquipmentPModel != NULL, "PModel should be valid");

		m_realPModel->attachView(this,PMODEL_NOTIFY_REFRESH_ALL_DATA);
		m_realPModel->attachView(this,PMODEL_NOTIFY_UPDATE_DATA);
	}



	BEGIN_MESSAGE_MAP(CControlListCtrl, CAbstractDataPointListCtrl)
		//{{AFX_MSG_MAP(CControlListCtrl)
		ON_WM_CREATE()
		//}}AFX_MSG_MAP
    	//ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, onCustomDrawList )
	END_MESSAGE_MAP()



	/////////////////////////////////////////////////////////////////////////////
	// CDataPointListCtrl message handlers

	int CControlListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	{
		FUNCTION_ENTRY ("OnCreate()");

		if (CWnd::OnCreate(lpCreateStruct) == -1)
		{
			FUNCTION_EXIT;
			return -1;
		}

		setupList();
		displayDataPoints();
		
		FUNCTION_EXIT;
		return 0;
	}


	void CControlListCtrl::setupList()
	{
		FUNCTION_ENTRY ("SetupList()");

		SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | 
											  LVS_EDITLABELS | LVS_EX_SUBITEMIMAGES | 
											  CAbstractDataPointListCtrl::LVS_EX_LABELTIP_XXX );
	
		//this->SetImageList(&m_imagelist, LVSIL_SMALL);
		// Headings
   		this->InsertColumn(EQUIPMENT_ATTRIBUTES_COLUMN,		TALoadString(STRING_ID_INSPECTOR_PANEL_0001).GetString());//_T("Description"));
		this->InsertColumn(DATAPOINT_TYPE_COLUMN,			TALoadString(STRING_ID_INSPECTOR_PANEL_0002).GetString());//_T("Type"));
		this->InsertColumn(CONTROL_STATE_COLUMN,			TALoadString(STRING_ID_INSPECTOR_PANEL_0003).GetString());//_T("State"));
   		this->InsertColumn(NEW_VALUE_COLUMN,				TALoadString(STRING_ID_INSPECTOR_PANEL_0004).GetString());//_T("New Value"));
   		this->InsertColumn(UNITS_COLUMN,					TALoadString(STRING_ID_INSPECTOR_PANEL_0005).GetString());//_T("Units"));

		this->SetColumnWidth(EQUIPMENT_ATTRIBUTES_COLUMN,	210);
		this->SetColumnWidth(DATAPOINT_TYPE_COLUMN,			50);
		this->SetColumnWidth(CONTROL_STATE_COLUMN,			100);
		this->SetColumnWidth(NEW_VALUE_COLUMN,				180);
		this->SetColumnWidth(UNITS_COLUMN,					50);

		FUNCTION_EXIT;
	}

	void CControlListCtrl::displayDataPoints()
	{
		FUNCTION_ENTRY ("DisplayDataPoints()");

		if (m_realPModel==NULL)
		{
			FUNCTION_EXIT;
			return;
		}

		int validItem = 0;
		int difference = 0;
		int totalItem = GetItemCount();

		int				nItem = 0;
		std::string		value;
		

		std::vector<ControlItem> allControlItems = m_realPModel->getAllControlDisplayItem();
		if(allControlItems.size() == 0)
		{
			DeleteAllItems();
			FUNCTION_EXIT;
			return;
		}

		if (m_initialState == false)
		{
			m_initialState = true;
			//std::string controlStateString = m_realPModel->updateControlState();
			//todo->postmessage.
		}

		for(unsigned int index =0; index < allControlItems.size(); ++index)
		{

			ControlItem controlItem = allControlItems[index];
			validItem++;

				// Description (Equipment Attributes)
			if( validItem > GetItemCount() )
			{
				nItem = this->InsertItem(validItem-1, CONVERT_FROM_STD(controlItem.description) );
			}
			else
			{
				nItem = validItem-1;
			}
			SetItemData(nItem,controlItem.iPoint);
			SetItemText(nItem, EQUIPMENT_ATTRIBUTES_COLUMN, CONVERT_FROM_STD(controlItem.description) );
			SetItemText(nItem, DATAPOINT_TYPE_COLUMN, CONVERT_FROM_STD(controlItem.type) );
			SetItemText(nItem, CONTROL_STATE_COLUMN, CONVERT_FROM_STD(controlItem.state) );


				// Value - only displayed if the control state is displayed
				// 				if (controlItem.state.size() > 0 )
				// 				{	
			SetItemText(nItem,NEW_VALUE_COLUMN, CONVERT_FROM_STD(controlItem.newValue) );
				// 				}
				//SetItemText(nItem,NEW_VALUE_COLUMN,"");

			SetItemText(nItem,UNITS_COLUMN,CONVERT_FROM_STD(controlItem.units) );
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

	void CControlListCtrl::refreshDataPoints()
	{
		FUNCTION_ENTRY ("refreshDataPoints()");

		// No equipment defined yet.
		if (m_realPModel==NULL) 
		{
			FUNCTION_EXIT;
			return;
		}

		displayDataPoints(); 

		FUNCTION_EXIT;
	}

	// ICellManager Implementation
	ICellManager::CellType CControlListCtrl::GetCellType(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("GetCellType()");

		// Check if item is valid  (We can get -1 sometimes but not sure why, should check editable list control)
		// Also check if there is any pending control command or Only value column is editable
		if (item<0 || subItem!=NEW_VALUE_COLUMN) 
		{
			FUNCTION_EXIT;
			return CT_NOTEDITABLE;
		}

		// Get the DataPoint array position
		int iPoint = listCtrl->GetItemData(item);
	    return convertPModelCellType( m_realPModel->getPMCellTypeFromPoint(iPoint) );
		//return convertPModelCellType( PMCT_BUTTON );
		
		/*else//todo->
		{
			std::stringstream message;
			message << m_realPModel->getChildDataPointDescription(iPoint) << ": Control Inhibited.";

			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", message.str().c_str() );

			std::string* statusLineStr = new std::string(message.str());
			AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);

			FUNCTION_EXIT;
			return CT_NOTEDITABLE;
		}
		
		return CT_NOTEDITABLE;*/
	}


    unsigned long CControlListCtrl::GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem)
    {
        return 0;
    }


	ICellManager::CellData CControlListCtrl::GetCellData(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("GetCellData()");

		TA_ASSERT (m_realPModel!=NULL,"CControlListCtrl::GetCellData() - m_realPModel not defined");

		ICellManager::CellData   datas;

		if (subItem == NEW_VALUE_COLUMN)
		{
			unsigned int iPoint = listCtrl->GetItemData(item);
			std::vector<std::string> values = m_realPModel->getCellValues(iPoint);
			
			for(int index = 0; index < values.size();++index)
			{
				datas.push_back(TAUnicodeString::CreateFromMultitByteString( values[index].c_str() ));
			}
		}

		FUNCTION_EXIT;
		return datas;
	}

    TAUnicodeString CControlListCtrl::GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return _T("");
    }

    char CControlListCtrl::GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return 0;
    }

	bool CControlListCtrl::UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value)
	{
		FUNCTION_ENTRY ("UpdateCellValue()");

        bool updateListCell = false;
		try 
		{
			UINT iPoint = listCtrl->GetItemData(item);
			std::string  strValue = value.toMultiByteStdString();
			bool isOkToSendControl = m_realPModel->isOkToSendControl(iPoint, strValue);
			
			if(!isOkToSendControl)
			{
				//Control value [%1] inValid
				TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0102,value.c_str());
				return false;
			}

			CConfirmCommandDialog     dlg;
			dlg.m_equipment = CONVERT_FROM_STD( m_realPModel->getEquipmentDescription() );
			dlg.m_dataPoint = CONVERT_FROM_STD( m_realPModel->getChildDataPointDescription(iPoint) );
			dlg.m_newValue  = value;

			if (dlg.DoModal()==IDOK)
			{
				try
				{
					m_realPModel->setValue(iPoint,strValue);
					//"[%1] Update value to [%2] succefully"
					TAUnicodeString *statusLineStr = new TAUnicodeString();
					*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0100, CONVERT_FROM_STD(m_realPModel->getChildDataPointDescription(iPoint)), value.c_str() );

					AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
				}
				catch(OutputLogicException&ex)
				{
					//"[%1] Update value to [%2] Fail,dueto [%3]"
					int exCode = ex.getExceptionCode();
					TAUnicodeString *statusLineStr = new TAUnicodeString();
					*statusLineStr = TAAfxFormatString3(STRING_ID_INSPECTOR_PANEL_0101, CONVERT_FROM_STD(m_realPModel->getChildDataPointDescription(iPoint)),value.c_str(), AppLogicExceptionConverter::ConvertException(ex).str() );
				
					AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
				}
				updateListCell = true;
			}
		}
		catch (...)
		{
			AfxMessageBox( TALoadString(STRING_ID_INSPECTOR_PANEL_0006).GetString() );//_T("updatecell exeption"));
		}
		FUNCTION_EXIT;
		return updateListCell;
	}

	void CControlListCtrl::updateDataNode(ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataNode()");

		// No datanode details here

		FUNCTION_EXIT;
	}

	void CControlListCtrl::updateDataPointDisplay ( int iPoint, int nItem)
	{
		FUNCTION_ENTRY ("UpdateDataPointDisplay()");

		if (m_realPModel == NULL)
		{
			FUNCTION_EXIT;
			return;
		}

		try
		{
			//if(m_realPModel->isValid(iPoint) && !m_realPModel->isChildControlInhibited(iPoint))//todo->
			if(m_realPModel->isValid(iPoint) )
			{
				ControlItem controlItem = m_realPModel->getControlListCtrlItem(iPoint);
				SetItemText(nItem,EQUIPMENT_ATTRIBUTES_COLUMN, CONVERT_FROM_STD(controlItem.description) );
				SetItemText(nItem, DATAPOINT_TYPE_COLUMN, CONVERT_FROM_STD(controlItem.type));
				SetItemText(nItem, CONTROL_STATE_COLUMN, CONVERT_FROM_STD(controlItem.state));
 				SetItemText(nItem,NEW_VALUE_COLUMN, CONVERT_FROM_STD(controlItem.newValue));
				SetItemText(nItem,UNITS_COLUMN, CONVERT_FROM_STD(controlItem.units));
			}
		}
		catch (...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", "Unknown Exception" );
		}

		FUNCTION_EXIT;
	}


	bool CControlListCtrl::isAnyDataPointsDisplayedOnScreen() const
	{
		return	m_dataPointsDisplayedOnScreen;
	}

	//TD15438, override the function to get a chance to adjust columns width.
	BOOL CControlListCtrl::SetWindowPos( const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags )
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
				
				int columnWidth = GetColumnWidth(NEW_VALUE_COLUMN);
				int vsrollWidth = GetSystemMetrics(SM_CXVSCROLL);
				
				//the minimum width is 100
				if (columnWidth - vsrollWidth > 100)
				{
					SetColumnWidth(NEW_VALUE_COLUMN, columnWidth - vsrollWidth);
				}
			}
		}
		
		return isSucceeded;
	}

	//add by tercel
	void CControlListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		TCHAR lpBuffer[256]; 

		LV_COLUMN lvc;
		::ZeroMemory(&lvc, sizeof(lvc));
		lvc.mask = LVCF_WIDTH | LVCF_FMT;
		long t_width = 0;


		for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
		{
			lpDrawItemStruct->rcItem.left += t_width ;
			lpDrawItemStruct->rcItem.right += t_width ;
			t_width = lvc.cx ;

			LV_ITEM lvi; 
			::ZeroMemory(&lvi, sizeof(lvi));
			lvi.iItem = lpDrawItemStruct->itemID;
			lvi.mask = LVIF_TEXT | LVIF_PARAM;
			lvi.iSubItem = nCol;
			lvi.pszText = lpBuffer;
			lvi.cchTextMax = sizeof(lpBuffer);
			VERIFY(GetItem(&lvi));

			CDC* pDC;
			pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

			if ( lpDrawItemStruct->itemState & ODS_SELECTED )
			{
				pDC->FillSolidRect(&lpDrawItemStruct->rcItem,GetSysColor(COLOR_HIGHLIGHT)) ;
			}
			else
			{
				pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_WINDOW)) ;
			}

			if ( CONTROL_STATE_COLUMN == nCol &&
				( !_tcscmp(lpBuffer , CONVERT_FROM_PCHAR(DPT_CONTROL_LCC_FAILED) ) || 
				!_tcscmp(lpBuffer , CONVERT_FROM_PCHAR(DPT_CONTROL_RCC_FAILED) ) || 
				!_tcscmp(lpBuffer , CONVERT_FROM_PCHAR(DPT_CONTROL_COMMS_FAILED) )))
			{				
				pDC->SetTextColor(COLOUR_ALARM) ;// red							
			}
			else
			{
				pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT)) ;
			}
			CRect lrect = lpDrawItemStruct->rcItem;
			lrect.DeflateRect(6, 0, 0, 0);
			::DrawText(lpDrawItemStruct->hDC, lpBuffer, _tcslen(lpBuffer), &lrect, DT_LEFT) ;
		}
	}

}
