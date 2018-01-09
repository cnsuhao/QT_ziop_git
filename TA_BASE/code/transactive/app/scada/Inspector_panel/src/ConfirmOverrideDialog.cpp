/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/ConfirmOverrideDialog.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  */
#include "app/scada/Inspector_Panel/src/stdafx.h"

#include "app/scada/Inspector_Panel/src/InspectorPanel.h"
#include "app/scada/Inspector_Panel/src/ConfirmOverrideDialog.h"

#include "bus/scada/proxy_library/src/DataNodeProxySmartPtr.h"
#include "bus/scada/proxy_library/src/DataPointProxySmartPtr.h"
#include "bus/scada/proxy_library/src/IDataNodeProxy.h"
#include "bus/scada/proxy_library/src/IDataPointProxy.h"
#include "bus/generic_gui_view/src/HelpLauncher.h" //TD15124 azenitha
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include "core/exceptions/src/ScadaProxyException.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NEW_VALUE_CONTROL			5001



using namespace TA_Base_Bus;

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// CConfirmOverrideDialog dialog


	
	CConfirmOverrideDialog::CConfirmOverrideDialog(CWnd* pParent, int iPoint, OverrideListCtrlModel*		realPModel)
		: CDialog(CConfirmOverrideDialog::IDD, pParent), m_realPModel(realPModel), m_iPoint(iPoint), m_overrideValue("")
	{
		//{{AFX_DATA_INIT(CConfirmOverrideDialog)
		//}}AFX_DATA_INIT
	}

	
	void CConfirmOverrideDialog::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CConfirmOverrideDialog)
		//}}AFX_DATA_MAP
	}

	BEGIN_MESSAGE_MAP(CConfirmOverrideDialog, CDialog)
		//{{AFX_MSG_MAP(CConfirmOverrideDialog)
		//}}AFX_MSG_MAP
		ON_EN_CHANGE(NEW_VALUE_CONTROL, OnChangeNewValue)
		ON_CBN_SELCHANGE(NEW_VALUE_CONTROL, OnChangeNewValue)
		ON_BN_CLICKED(ID_HELP_CO, OnHelpButton) //TD15124 azenitha
	END_MESSAGE_MAP()
	
	/////////////////////////////////////////////////////////////////////////////
	// CConfirmOverrideDialog message handlers

	//TD15124 azenitha++
	void CConfirmOverrideDialog::OnHelpButton() 
	{
		TA_Base_Bus::HelpLauncher::getInstance().displayHelp(true);
	}
	//TD15124 ++azenitha

	BOOL CConfirmOverrideDialog::OnInitDialog() 
	{
		CDialog::OnInitDialog();


		GetDlgItem(IDC_EQUIPMENT)->SetWindowText(CONVERT_FROM_STD(m_realPModel->getEquipmentDescription()));
		
		//uniStr = TAUnicodeString::CreateFromMultitByteString(m_realPModel->getChildDataPointDescription(m_iPoint).c_str());
		//GetDlgItem(IDC_DATA_POINT)->SetWindowText(uniStr.c_str());

		GetDlgItem(IDC_DATA_POINT)->SetWindowText(CONVERT_FROM_STD(m_realPModel->getChildDataPointDescription(m_iPoint)));
		
		CRect newValueRect;
		GetDlgItem(IDC_NEW_VALUE)->GetWindowRect(&newValueRect);
		ScreenToClient(&newValueRect);

		GetDlgItem(IDOK)->EnableWindow(false);

		EPModelCellType type = m_realPModel->getCellTypeFromPoint(m_iPoint);
		// Check if boolean, text or combo
		if (type == PMCT_SELECTION)
		{
			// Combo box
			// Give space to scroll
			newValueRect.bottom += 200;

			m_valueComboBox.Create(WS_CHILD | CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL, newValueRect, this, NEW_VALUE_CONTROL);
			
			std::vector<std::string> valueStrings = m_realPModel->getCellValues(m_iPoint);

			for(unsigned int index = 0; index < valueStrings.size(); ++index)
			{
				TAUnicodeString uniStr = TAUnicodeString::CreateFromMultitByteString(valueStrings[index].c_str());
				m_valueComboBox.AddString(uniStr.c_str());
			}
			m_valueComboBox.SetFont(GetFont(), FALSE);
			m_valueComboBox.SetCurSel(-1);
			m_valueComboBox.SetFocus();
			m_valueComboBox.ShowWindow(SW_SHOW);

			return FALSE;
		}
		else if (type == PMCT_NUMBER)
		{
			// Number edit box
			m_valueNumericEdit.Create(WS_VISIBLE | WS_BORDER, newValueRect, this, NEW_VALUE_CONTROL);
			m_valueNumericEdit.SetFocus();
			m_valueNumericEdit.ShowWindow(SW_SHOW);

			return FALSE;
		}
		else if (type == PMCT_TEXT)
		{
			// Text edit box
			m_valueEdit.Create(WS_VISIBLE | WS_BORDER, newValueRect, this, NEW_VALUE_CONTROL);
			m_valueEdit.SetFocus();
			m_valueEdit.ShowWindow(SW_SHOW);

			return FALSE;
		}
		return TRUE;  // return TRUE unless you set the focus to a control
					  // EXCEPTION: OCX Property Pages should return FALSE
	}

	std::string CConfirmOverrideDialog::getOverrideValue()
	{
		return m_overrideValue;
	}

	void CConfirmOverrideDialog::OnOK() 
	{
		// TODO: Add extra validation here
		
		//CString str;
		TAUnicodeString uniStr(_T(""));
		EPModelCellType type = m_realPModel->getCellTypeFromPoint(m_iPoint);
		if(type == PMCT_SELECTION)
		{
			m_valueComboBox.GetLBText(m_valueComboBox.GetCurSel(), uniStr.str());
		}
		else if(type == PMCT_NUMBER)
		{
			m_valueNumericEdit.GetWindowText(uniStr.str());
		}
		else if(type == PMCT_TEXT)
		{
			m_valueEdit.GetWindowText(uniStr.str());
		}
		m_overrideValue = uniStr.toMultiByteStdString();
		CDialog::OnOK();
	}

	void CConfirmOverrideDialog::OnChangeNewValue() 
	{
		// TODO: If this is a RICHEDIT control, the control will not
		// send this notification unless you override the CDialog::OnInitDialog()
		// function and call CRichEditCtrl().SetEventMask()
		// with the ENM_CHANGE flag ORed into the mask.
		
		TAUnicodeString str;
		EPModelCellType type = m_realPModel->getCellTypeFromPoint(m_iPoint);

		// Check if boolean, text or combo
		if(type == PMCT_SELECTION)
		{
			// Combo box
			GetDlgItem(IDOK)->EnableWindow(m_valueComboBox.GetCurSel() >= 0);
		}
		else if(type == PMCT_NUMBER)
		{
			// Number edit box
			m_valueNumericEdit.GetWindowText(str.str());

			GetDlgItem(IDOK)->EnableWindow(str.GetLength() != 0);
		}
		else if(type == PMCT_TEXT)
		{
			// Text edit box
			m_valueEdit.GetWindowText(str.str());

			GetDlgItem(IDOK)->EnableWindow(str.GetLength() != 0);
		}

	}
	
}
