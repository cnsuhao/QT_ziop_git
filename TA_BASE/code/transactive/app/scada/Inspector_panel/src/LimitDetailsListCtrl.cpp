/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/LimitDetailsListCtrl.cpp $
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
#include "app/scada/inspector_panel/src/LimitDetailsListCtrl.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "AppLogicExceptionConverter.h"

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
		ALARM_TYPE_COLUMN			= 1,
		ALARM_ENABLED_COLUMN		= 2,
		CURRENT_VALUE_COLUMN		= 3,
		THRESHOLD_VALUE_COLUMN		= 4
	};

	/////////////////////////////////////////////////////////////////////////////
	// CLimitDetailsListCtrl

	CLimitDetailsListCtrl::CLimitDetailsListCtrl()
	:
	CAbstractDataPointListCtrl(),
	m_realPModel(NULL),
	m_dataPointsDisplayedOnScreen ( false ),
	m_iPoint (-1)
	{
		FUNCTION_ENTRY ("CLimitDetailsListCtrl::CLimitDetailsListCtrl()");

		// We will manage the cell requests
		SetManager(this);

		FUNCTION_EXIT;
	}

	CLimitDetailsListCtrl::~CLimitDetailsListCtrl()
	{
		FUNCTION_ENTRY ("CLimitDetailsListCtrl::~CLimitDetailsListCtrl()");
		FUNCTION_EXIT;
	}

	std::string		CLimitDetailsListCtrl::getMyPModelName()
	{
		return PMODEL_LIMIT_DETAIL_LISTCTRL;
	}

	void	CLimitDetailsListCtrl::setupPModel()
	{
		m_realPModel = dynamic_cast<LimitDetaillListCtrlModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");

		m_pEquipmentPModel = dynamic_cast<AbstractEquipmentPModel*>(m_realPModel);
		TA_ASSERT(m_pEquipmentPModel != NULL, "PModel should be valid");

		m_realPModel->attachView(this,PMODEL_NOTIFY_REFRESH_ALL_DATA);
		m_realPModel->attachView(this,PMODEL_NOTIFY_UPDATE_DATA);
	}
	/* setEquipment
	 *
	 * This function will set the name of the equipment that the details are to be displayed for.It can 
	 * be called before or after the windows control is on the screen.
	 */
	void CLimitDetailsListCtrl::refreshAllData()
	{
		FUNCTION_ENTRY ("setEquipment()");

		// call base class
		//CAbstractDataPointListCtrl::setEquipment ( equipment );

		FUNCTION_EXIT;
	}

	/* setDataPoint
	 *
	 * This function will set the entity key of the dataPoint that the details are to be displayed for.It can 
	 * be called before or after the windows control is on the screen.
	 */
	void CLimitDetailsListCtrl::setDataPoint(int iPoint)
	{
		FUNCTION_ENTRY ("setDataPoint()");

		m_iPoint = iPoint;

		displayLimitDetails();

		FUNCTION_EXIT;
	}

	BEGIN_MESSAGE_MAP(CLimitDetailsListCtrl, CAbstractDataPointListCtrl)
		//{{AFX_MSG_MAP(CLimitDetailsListCtrl)
		ON_WM_CREATE()
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()



	/////////////////////////////////////////////////////////////////////////////
	// CDataPointListCtrl message handlers

	int CLimitDetailsListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	{
		FUNCTION_ENTRY ("OnCreate()");

		if (CWnd::OnCreate(lpCreateStruct) == -1)
		{
			return -1;
		}

		setupList();
		
		FUNCTION_EXIT;

		return 0;
	}

// 	void CLimitDetailsListCtrl::PreSubclassWindow() 
// 	{
// 		FUNCTION_ENTRY ("PreSubclassWindow()");
// 
// 		CWnd::PreSubclassWindow();
// 		setupList();
// 
// 		FUNCTION_EXIT;
// 	}

	// Helper functions

	void CLimitDetailsListCtrl::setupList()
	{
		FUNCTION_ENTRY ("CLimitDetailsListCtrl::SetupList()");

		SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_SUBITEMIMAGES | CAbstractDataPointListCtrl::LVS_EX_LABELTIP_XXX);

		// Headings
		this->InsertColumn(DUMMY_COLUMN,				TALoadString(STRING_ID_INSPECTOR_PANEL_0021).GetString());//_T("Dummy"));
		this->InsertColumn(ALARM_TYPE_COLUMN,			TALoadString(STRING_ID_INSPECTOR_PANEL_0022).GetString());//_T("Alarm Type"));
		this->InsertColumn(ALARM_ENABLED_COLUMN,		TALoadString(STRING_ID_INSPECTOR_PANEL_0023).GetString());//_T("Enabled"));
		this->InsertColumn(CURRENT_VALUE_COLUMN,		TALoadString(STRING_ID_INSPECTOR_PANEL_0024).GetString());//_T("Current Value"));
		this->InsertColumn(THRESHOLD_VALUE_COLUMN,		TALoadString(STRING_ID_INSPECTOR_PANEL_0025).GetString());//_T("Threshold Value"));

		// Using a 'dummy' column for index 0 prevents
		// MFC from indenting the text by the image size for the
		// 'real' first column
		this->SetColumnWidth(DUMMY_COLUMN,				0);
		this->SetColumnWidth(ALARM_TYPE_COLUMN,			154);
		this->SetColumnWidth(ALARM_ENABLED_COLUMN,		70);
		this->SetColumnWidth(CURRENT_VALUE_COLUMN,		130);
		this->SetColumnWidth(THRESHOLD_VALUE_COLUMN,	140);

		FUNCTION_EXIT;
	}


	void CLimitDetailsListCtrl::displayDataPoints()
	{
		refreshLimitDetails();
	}

	void CLimitDetailsListCtrl::updateDataPoint(unsigned int iPoint)
	{
		FUNCTION_ENTRY ("updateDataPoint()");

		if (m_hWnd==0 || m_pEquipmentPModel==NULL ) //wenching++ (TD13353)
		{
			return;
		}

		displayLimitDetails();

		FUNCTION_EXIT;
	}

	void CLimitDetailsListCtrl::updateDataPointDisplay(int iPoint, int nItem)
	{
		displayLimitDetails();
	}

	void CLimitDetailsListCtrl::clearList()
	{
		DeleteAllItems();
	}


	void CLimitDetailsListCtrl::displayLimitDetails()
	{
		FUNCTION_ENTRY ("CLimitDetailsListCtrl::displayLimitDetails()");

		DeleteAllItems();

		// No equipment defined yet.
		if (m_realPModel==NULL || m_iPoint==-1 )
		{
			return; 
		}

		int nItem = 0;
		try
		{
			std::vector<LimitDetailItem> detailItemVector = m_realPModel->getLimitDetaillListCtrlItem(m_iPoint);	
			for (int index = 0; index < detailItemVector.size(); index++)
			{
				nItem = InsertItem(nItem+1, CONVERT_FROM_STD( detailItemVector[index].typeString) );

				SetItemText(nItem, ALARM_TYPE_COLUMN, CONVERT_FROM_STD( detailItemVector[index].typeString) );
				
				bool isChecked = detailItemVector[index].isEnable;
				bool isEnabled = isCellEnabled(nItem, ALARM_ENABLED_COLUMN);
				showCheckIcon(nItem, ALARM_ENABLED_COLUMN, isChecked, isEnabled);
				SetItemText(nItem, THRESHOLD_VALUE_COLUMN, CONVERT_FROM_STD(detailItemVector[index].thresholdString) );
				//setItemTextImage(nItem,CURRENT_VALUE_COLUMN,detailItemVector[index].currentValueString.c_str(),true);

				LVITEM   listItem;
				listItem.iItem = nItem;
				listItem.iSubItem = CURRENT_VALUE_COLUMN;
				listItem.iImage = getDataPointStatusImageIndex(m_iPoint);
				listItem.mask = LVIF_IMAGE;
				SetItem(&listItem);

				SetItemText(nItem, CURRENT_VALUE_COLUMN, CONVERT_FROM_STD(detailItemVector[index].currentValueString) );
			}
		}
		catch(...)
		{

		}

		FUNCTION_EXIT;
	}

	void CLimitDetailsListCtrl::refreshLimitDetails()
	{
		FUNCTION_ENTRY ("CLimitDetailsListCtrl::refreshDataPoints()");

		// No equipment defined yet.
		if (m_realPModel==NULL)
		{
			return;
		}
		this->displayLimitDetails();
		FUNCTION_EXIT;
	}

	// ICellManager Implementation
	ICellManager::CellType CLimitDetailsListCtrl::GetCellType(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("CLimitDetailsListCtrl::GetCellType()");

		// Check if item is valid  (We can get -1 sometimes but not sure why, should check editable list control)
		if (item<0)	
		{
			return CT_NOTEDITABLE;
		}

		// No access to this dialog
		if (!m_accessAllowed)
		{
			return CT_NOTEDITABLE;
		}

		switch (subItem)
		{
			case ALARM_ENABLED_COLUMN:
			{
				return CT_CHECKBOX;
			}

			case THRESHOLD_VALUE_COLUMN:
			{
				return CT_NUMBER;
			}

			default:
			{
				return CT_NOTEDITABLE;
			}
		}

		FUNCTION_EXIT;

		return CT_NOTEDITABLE;
	}


    unsigned long CLimitDetailsListCtrl::GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem)
    {
        return 0;
    }


	ICellManager::CellData CLimitDetailsListCtrl::GetCellData(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("CLimitDetailsListCtrl::GetCellData()");

		TA_ASSERT (m_realPModel!=NULL,"Equipment not defined");

		ICellManager::CellData   data;

		FUNCTION_EXIT;

		return data;
	}

    TAUnicodeString CLimitDetailsListCtrl::GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return _T("");
    }

    char CLimitDetailsListCtrl::GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return 0;
    }

	bool CLimitDetailsListCtrl::limitDetailChangeConfirm(const int alarmThreshold, unsigned int iPoint, bool enable)
	{
		FUNCTION_ENTRY ("CLimitDetailsListCtrl::inhibitOverrideConfirm()");
    
        TAUnicodeString actionName, actionType;
        actionType.LoadString(alarmThreshold);
		
		TAUnicodeString dpDescription = TAUnicodeString::CreateFromMultitByteString( m_realPModel->getChildDataPointDescription(iPoint).c_str() );
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

	bool CLimitDetailsListCtrl::UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value)
	{
		FUNCTION_ENTRY ("CLimitDetailsListCtrl::UpdateCellValue()");

		TA_ASSERT (m_realPModel!=NULL,"Equipment not defined.");

		bool cellValueUpdated = false;
		std::string  strValue = value.toMultiByteStdString();
		std::string message;

		// Get the data point array position
		//TA_Base_Bus::DataPointProxySmartPtr& dataPoint = m_equipment->getChildDataPoint(m_iPoint);

		std::stringstream statusLine;
		statusLine << m_realPModel->getChildDataPointDescription(m_iPoint) << ": ";
		
		if (m_realPModel->isWritableDp(m_iPoint) == false) //wenching++ (TD14686)
		{
			std::string sessionId ( TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID) );
			bool foundMatchedAlarmType = false;

			// get the alarm type associated with this cell
			TAUnicodeString alarmType = GetItemText( item, ALARM_TYPE_COLUMN );			

			AlarmProperty newAlarmProperty;
			//foundMatchedAlarmType = m_realPModel->isAlarmTypeMatchable(m_iPoint,(LPCTSTR)alarmType);
			foundMatchedAlarmType  = m_realPModel->getAlarmProperty(m_iPoint, alarmType.toMultiByteStdString().c_str(),newAlarmProperty);

			// if we have successfully found a match alarm type to modify its properties
			if ( true == foundMatchedAlarmType )
			{
				switch (subItem)
				{
				case ALARM_ENABLED_COLUMN:
					{
						newAlarmProperty.enabled = strValue == "Yes";
						cellValueUpdated = this->enableAlarmConfirm(newAlarmProperty.enabled , alarmType.toMultiByteStdString());
					}
					break;
				case THRESHOLD_VALUE_COLUMN:
					{
						// Check if there is changing of value
						// Since this cell only allow value entry, no unit checking
						std::string cellValue = value.toMultiByteStdString();

						if( newAlarmProperty.threshold == atof(cellValue.c_str()) )
						{
							break;
						}
						//TD15160, to ensure the input text is valid number without unit string
						//if the input is a numberic value, it must not equil to the one with '9' added
						std::string longerValue = cellValue + "9";
						if (cellValue.empty() || atof(cellValue.c_str()) == atof(longerValue.c_str()))
						{
							// show text in Status box
							//statusLine << "The input value " << cellValue.c_str() << " is not a valid number (should exclude unit).";
							
							TAUnicodeString *statusLineStr = new TAUnicodeString();
							*statusLineStr = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0136,value.c_str());
							AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
							//std::string* statusLineStr = new std::string(statusLine.str());
							//AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
							
							break;
						}

						//add the unit for further display
						cellValue += " ";
						cellValue += m_realPModel->getChildUnit(m_iPoint);

						//TD15160, to check the limit value range
						if (atof(cellValue.c_str()) < m_realPModel->getChildLimit(m_iPoint,true) ||
							atof(cellValue.c_str()) > m_realPModel->getChildLimit(m_iPoint,false))
						{
							// show text in Status box
							//statusLine << "Set new Threshold Value " << cellValue.c_str() << " Failed. ";
							//statusLine << "Valid range is from ";
							//statusLine << m_realPModel->getChildLimit(m_iPoint,true) << " to ";
							//statusLine << m_realPModel->getChildLimit(m_iPoint,false) << " ";
							//statusLine << m_realPModel->getChildUnit(m_iPoint) << ".";

							std::stringstream lowLimitStream;
							std::stringstream highLimitStream;

							lowLimitStream<< m_realPModel->getChildLimit(m_iPoint,true);
							highLimitStream<<m_realPModel->getChildLimit(m_iPoint,false); 
							
							TAUnicodeString *statusLineStr = new TAUnicodeString();
							*statusLineStr = TAAfxFormatString4(STRING_ID_INSPECTOR_PANEL_0137,value.c_str(), CONVERT_FROM_STD(lowLimitStream.str()), CONVERT_FROM_STD(highLimitStream.str()),CONVERT_FROM_STD(m_realPModel->getChildUnit(m_iPoint)));
							AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
							//std::string* statusLineStr = new std::string(statusLine.str());
							//AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
							
							break;
						}
						
						// get confirmation from the operator
						//std::stringstream ss;
						//ss << "change alarm threshold for " << m_realPModel->getChildEntityName(m_iPoint);
						//ss << ", "  << m_realPModel->getChildDataPointDescription(m_iPoint) << ", from ";
						//ss << newAlarmProperty.threshold << " to " << cellValue;

						std::stringstream thresholdStream;
						thresholdStream<<newAlarmProperty.threshold;

						TAUnicodeString dpDescription = TAAfxFormatString4(STRING_ID_INSPECTOR_PANEL_0138,CONVERT_FROM_STD(m_realPModel->getChildEntityName(m_iPoint)), CONVERT_FROM_STD(m_realPModel->getChildDataPointDescription(m_iPoint)), CONVERT_FROM_STD(thresholdStream.str()),value.c_str());
						//TAUnicodeString dpDescription = TAUnicodeString::CreateFromMultitByteString( ss.str().c_str() );
						int pos = dpDescription.Find ( _T("%"), 0 );
						if ( -1 != pos )
						{
							dpDescription.str().Insert ( pos, _T("%") );
						}
						
						TAUnicodeString actionName;
						AfxFormatString1 ( actionName.str(), IDS_OVERRIDE_ALARM_THRESHOLD, dpDescription.GetString() );

						TA_Base_Bus::TransActiveMessage userMsg;
						userMsg << actionName;
						UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);       

						if ( selectedButton == IDYES )
						{
							// update the AlarmValue
							//std::string newValue = cellValue;
							//newAlarmProperty.threshold = atof (newValue.c_str());

							//statusLine << "\"" << newValue << "\"" << " Limit Set ";

							// set the flag to indicate that the cell value will need to be changed
							cellValueUpdated = true;
						}
						break;
					}

				default:
					{
						break;
					}					
				}
			}

			// if the flag indicate the alarm properties need to be updated
			if ( true == cellValueUpdated )
			{
				try
				{
					m_realPModel->setAlarmProperties ( m_iPoint,newAlarmProperty);
					cellValueUpdated = true;
					
					TAUnicodeString *statusLineStr = new TAUnicodeString();
					*statusLineStr = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0130, CONVERT_FROM_STD( m_realPModel->getChildDataPointDescription(m_iPoint)) );
					AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
				}
				catch(OutputLogicException&ex)
				{
					TAUnicodeString *statusLineStr = new TAUnicodeString();
					*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0130, CONVERT_FROM_STD( m_realPModel->getChildDataPointDescription(m_iPoint)), AppLogicExceptionConverter::ConvertException(ex).str() );
					AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
				}
			}
		}

		// if the flag is set
		if ( true == cellValueUpdated )
		{
			// update the screen dialog
			refreshLimitDetails();
		}

		return cellValueUpdated;

		FUNCTION_EXIT;
	}


	bool CLimitDetailsListCtrl::isCellEnabled (int nItem, int nSubItem)
	{
		FUNCTION_ENTRY ("CLimitDetailsListCtrl::isCellEnabled()");

		// Check if item is valid  (We can get -1 sometimes but not sure why, should check editable list control)
		if (nItem<0)
		{
			return false;
		}
		else
		{
			return m_accessAllowed;
		}

		FUNCTION_EXIT;
	}

	void CLimitDetailsListCtrl::updatePermissions(bool allowed)
	{
		// always enable the dialog windows
		if( !(IsWindowEnabled()) )
			EnableWindow ( true );

		// access to individual cell is through this variable
		m_accessAllowed = allowed;
	}

	bool CLimitDetailsListCtrl::overrideAlarmConditionConfirm ( const TAUnicodeString & typeOfChange, unsigned int iPoint, bool enable )
	{
		FUNCTION_ENTRY ("overrideAlarmConditionConfirm()");

        TAUnicodeString actionName;
		TAUnicodeString actionType = typeOfChange;//TAUnicodeString::CreateFromMultitByteString( typeOfChange.c_str() );

		TAUnicodeString dpDescription = TAUnicodeString::CreateFromMultitByteString( m_realPModel->getChildDataPointDescription(iPoint).c_str() );
		int pos = dpDescription.Find ( _T("%"), 0 );
		if ( -1 != pos )
		{
			dpDescription.str().Insert ( pos, _T("%") );
		}

		std::stringstream temp;
		temp << m_realPModel->getChildEntityName(iPoint) << ", " << dpDescription.toMultiByteStdString();
		dpDescription = TAUnicodeString::CreateFromMultitByteString( temp.str().c_str() );

        AfxFormatString2 ( actionName.str(), enable ? IDS_ENABLE_DATAPOINT : IDS_DISABLE_DATAPOINT, actionType.str(), dpDescription.str() );

        TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << actionName;
        UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);        
        
		return ( selectedButton == IDYES );

		FUNCTION_EXIT;
	}

	bool CLimitDetailsListCtrl::isAnyDataPointsDisplayedOnScreen() const
	{
		return	m_dataPointsDisplayedOnScreen;
	}   


	bool CLimitDetailsListCtrl::enableAlarmConfirm(bool isEnable, const std::string& alarmType)
	{
		std::stringstream statusLine;
		bool rtn = false;
			// get confirmation from the operator
		//std::string typeOfChange = "alarm type " + alarmType;
		TAUnicodeString typeOfChange = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0135, CONVERT_FROM_STD(alarmType) );
		if ( overrideAlarmConditionConfirm ( typeOfChange, m_iPoint, isEnable ) )
		{
			//statusLine << "Alarm type \"" << alarmType;
			//statusLine << (isEnable? "\" enabled ":"\" disabled ");
			TAUnicodeString *statusLineStr = new TAUnicodeString();
			*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0134,CONVERT_FROM_STD(alarmType), isEnable?TALoadString(STRING_ID_INSPECTOR_PANEL_0132).GetString() : TALoadString(STRING_ID_INSPECTOR_PANEL_0133).GetString() );
			AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
			rtn = true;
		}
		return rtn;
	}
}