/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/LimitListCtrl.cpp $
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
#include "app/scada/inspector_panel/src/LimitDetailsListCtrl.h"
#include "app/scada/inspector_panel/src/LimitListCtrl.h"

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
	// CLimitListCtrl


	enum columns
	{
		DESCRIPTION_COLUMN		= 0,
	};


	CLimitListCtrl::CLimitListCtrl(CEdit& limitInfo, CLimitDetailsListCtrl& limitDetails) 
	:
	CAbstractDataPointListCtrl(), 
	m_limitInfo (limitInfo),
	m_limitDetails (limitDetails),
	m_dataPointsDisplayedOnScreen ( false )
	{
		FUNCTION_ENTRY ("CLimitListCtrl()");

		// We will manage the cell requests
		m_accessAllowed = true;
		SetManager(this);

		FUNCTION_EXIT;
	}

	CLimitListCtrl::~CLimitListCtrl()
	{
		FUNCTION_ENTRY ("~CLimitListCtrl()");
		FUNCTION_EXIT;
	}

	std::string		CLimitListCtrl::getMyPModelName()
	{
		return PMODEL_LIMITS_LISTCTRL;
	}

	void	CLimitListCtrl::setupPModel()
	{
		m_realPModel = dynamic_cast<LimitListCtrlModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");

		m_pEquipmentPModel = dynamic_cast<AbstractEquipmentPModel*>(m_realPModel);
		TA_ASSERT(m_pEquipmentPModel != NULL, "PModel should be valid");

		m_realPModel->attachView(this,PMODEL_NOTIFY_REFRESH_ALL_DATA);
		m_realPModel->attachView(this,PMODEL_NOTIFY_UPDATE_DATA);
	}


	BEGIN_MESSAGE_MAP(CLimitListCtrl, CAbstractDataPointListCtrl)
		//{{AFX_MSG_MAP(CLimitListCtrl)
		ON_WM_CREATE()
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////////////////
	// CLimitListCtrl message handlers

	// Helper functions

	void CLimitListCtrl::setupList()
	{
		FUNCTION_ENTRY ("setupList()");

		SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_SUBITEMIMAGES | CAbstractDataPointListCtrl::LVS_EX_LABELTIP_XXX );

		// Headings
   		this->InsertColumn(DESCRIPTION_COLUMN, TALoadString(STRING_ID_INSPECTOR_PANEL_0026).GetString()); //_T("Description"));
		this->SetColumnWidth(DESCRIPTION_COLUMN,210);

		FUNCTION_EXIT;
	}


	void CLimitListCtrl::updateDataPointDisplay(int iPoint, int nItem)
	{
		FUNCTION_ENTRY ("updateDataPointDisplay()");

		std::string		value;
		value = m_realPModel->getChildDataPointDescription(iPoint);
		SetItemText(nItem, DESCRIPTION_COLUMN, CONVERT_FROM_STD(value));

		FUNCTION_EXIT;
	}

	void CLimitListCtrl::refreshDataPoints()
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

	// By using the preprocessed index vector, reduce linear search and ensure only those
	// datapoints had been sorted and config valid were displayed.
	void CLimitListCtrl::displayDataPoints()
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

		int				nItem;
		unsigned int	iPoint;
		std::string		value;

		

		// Get the preprocessed indexes with the required config.
		std::vector<unsigned long>& vectorIndex = m_realPModel->getLimitIndex();

		if (0 == vectorIndex.size())
		{
			DeleteAllItems();
			m_limitDetails.clearList();
			return;
		}

		for (unsigned int index=0;index < vectorIndex.size();index++)
		{
			iPoint = vectorIndex.at(index);
			try
			{
				// 4669 FAT TES 205: Limit Page of Inspector Panel should only applicable to AI
				validItem++;

				// Description
				value = m_realPModel->getChildDataPointDescription(iPoint);
				if ( validItem > GetItemCount() )
				{
					nItem = insertItemTextImage(iPoint, validItem-1, value, false);
					setItemTextImage(nItem, DESCRIPTION_COLUMN, value, false);
					SetItemData(nItem, iPoint);
				}
				else
				{
					nItem = validItem-1;
					SetItemData(nItem, iPoint);
					SetItemText(nItem, DESCRIPTION_COLUMN, CONVERT_FROM_STD(value) );
				}

				m_dataPointsDisplayedOnScreen = true;
			
			}
			catch (...)
			{
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, "Unknown Exception");
			}
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

		if ( GetItemCount() > 0)
		{
			iPoint = GetItemData(0);
			{
				SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);			
				// Show type
				m_limitInfo.SetWindowText(_T(""));
				m_limitInfo.SetWindowText(CONVERT_FROM_STD(m_realPModel->getLimitInfoString(iPoint)) );
				// Show limit details list
				m_limitDetails.setDataPoint(iPoint);
			}
		}

		FUNCTION_EXIT;
	}

	// ICellManager Implementation
	ICellManager::CellType CLimitListCtrl::GetCellType(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("GetCellType()");

		// Check if item is valid  (We can get -1 sometimes but not sure why, should check editable list control)
		// or No access to this dialog
		if (item<0 || !m_accessAllowed)	
		{
			return CT_NOTEDITABLE;
		}

		std::vector<unsigned long>& vectorIndex = m_realPModel->getLimitIndex();
		if (0 == vectorIndex.size())
		{
			return CT_NOTEDITABLE;
		}
		// We actually don't want to edit anything, but a row has been clicked on, so we will use this trigger
		// to display the correct limit information
		unsigned int iPoint = GetItemData(item);

		// Clear the edit box
		m_limitInfo.SetWindowText(_T(""));

		m_limitInfo.SetWindowText( CONVERT_FROM_STD(m_realPModel->getLimitInfoString(iPoint) ));

		//
		// determine the right to change the alarm limits of the data point.  According to
		// GUI note, only consider for AI, everything else has no rights
		//

		// find if the operator profile has the right to change the alarm limits
		bool isLimitsTabAccessAllowed = m_realPModel->isLimitsAccessPermitted(iPoint);

		// update screen accordingly
		m_limitDetails.updatePermissions ( isLimitsTabAccessAllowed );

		// display the alarm limits detail
		m_limitDetails.setDataPoint(GetItemData(item));

		FUNCTION_EXIT;

		return CT_NOTEDITABLE;
	};

    unsigned long CLimitListCtrl::GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem)
    {
        return 0;
    }


	ICellManager::CellData CLimitListCtrl::GetCellData(CEditableListCtrl* listCtrl, int item, int subItem)
	{
		FUNCTION_ENTRY ("GetCellData()");

		ICellManager::CellData   data;

		FUNCTION_EXIT;

		return data;
	}

    TAUnicodeString CLimitListCtrl::GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return _T("");
    }

    char CLimitListCtrl::GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem)
    {
	    TA_ASSERT(false, "There are no cells of type CT_MASK");

        return 0;
    }

	bool CLimitListCtrl::UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value)
	{
		FUNCTION_ENTRY ("UpdateCellValue()");
		FUNCTION_EXIT;

		return false;
	}

	bool CLimitListCtrl::isAnyDataPointsDisplayedOnScreen() const
	{
		return m_dataPointsDisplayedOnScreen;
	}
}