/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/NotesDialog.cpp $
  * @author:  Darren Sampson
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the display and update of notes associated with equipment.
  */

#ifdef __WIN32__
#pragma warning(disable:4786)
#endif

#include "app/scada/Inspector_Panel/src/stdafx.h"

#include "app/scada/inspector_panel/src/InspectorPanel.h"
#include "app/scada/inspector_panel/src/InspectorPanelDlg.h"
#include "app/scada/inspector_panel/src/NotesDialog.h"
#include "AppLogicExceptionConverter.h"

#include "bus/generic_gui_view/src/TransActiveMessage.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/message/types/InspectorPanelAudit_MessageTypes.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// CNotesDialog dialog


	CNotesDialog::CNotesDialog(TA_Base_Bus::ITransActiveWinApp*app,CWnd* pParent /*=NULL*/)
		: TA_Base_Bus::AbstractTransActiveDialog(app, CNotesDialog::IDD, NULL),
		  m_accessAllowed(false), 
		  m_notesAccessible(false),
		  m_originalHeight (0)
	{
		FUNCTION_ENTRY ("CNotesDialog()");

		//{{AFX_DATA_INIT(CNotesDialog)
			// NOTE: the ClassWizard will add member initialization here
		//}}AFX_DATA_INIT

		FUNCTION_EXIT;
	}


	void CNotesDialog::setupPModel()
	{
		m_realPModel = dynamic_cast<NotesDialogModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");

		m_realPModel->attachView(this,PMODEL_NOTIFY_REFRESH_ALL_DATA);
		m_realPModel->attachView(this,PMODEL_NOTIFY_UPDATE_DATA);
	}

	void	CNotesDialog::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		if( _stricmp(updateType, PMODEL_NOTIFY_REFRESH_ALL_DATA) == 0 )
		{
			BOOL err = this->PostMessage(MESSAGE_REFRESH_ALL_DATA);
		}
	}

	std::string CNotesDialog::getMyPModelName()
	{
		return PMODEL_NOTES_DLG;
	}

	LRESULT CNotesDialog::onRefreshAllData(WPARAM wparam, LPARAM lparam)
	{
		this->refreshAllData();
		return 0;
	}

	void CNotesDialog::DoDataExchange(CDataExchange* pDX)
	{
		FUNCTION_ENTRY ("DoDataExchange()");

		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CNotesDialog)
		DDX_Control(pDX, IDC_EDITNOTES, m_notesEdit);
		//}}AFX_DATA_MAP

		FUNCTION_EXIT;
	}


	BEGIN_MESSAGE_MAP(CNotesDialog, CDialog)
		//{{AFX_MSG_MAP(CNotesDialog)
		ON_BN_CLICKED(IDC_REMOVE, OnRemove)
		ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
		ON_MESSAGE( MESSAGE_REFRESH_ALL_DATA, onRefreshAllData)
		ON_EN_CHANGE(IDC_EDITNOTES, OnChangeEditnotes)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////////////////
	// CNotesDialog message handlers

	void CNotesDialog::OnRemove() 
	{
		FUNCTION_ENTRY ("OnRemove()");

		if (m_realPModel != NULL)
		{
            TAUnicodeString actionName;
            actionName.LoadString(IDS_REMOVE_EQUIPMENT_NOTES);
            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << actionName;
            UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);        
			
			try
			{
				m_realPModel->setNotes("");
				TAUnicodeString *statusLineStr = new TAUnicodeString();
				*statusLineStr = TALoadString(STRING_ID_INSPECTOR_PANEL_0180);
				AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
				DisplayEquipmentNotes();
			}
			catch(OutputLogicException&ex)
			{
				TAUnicodeString *statusLineStr = new TAUnicodeString();
				*statusLineStr = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0181, AppLogicExceptionConverter::ConvertException(ex).str());
				AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
			}
		}
		FUNCTION_EXIT;
	}

	void CNotesDialog::refreshAllData()
	{
		FUNCTION_ENTRY ("setEquipment()");
		// Update the displayed control
		if (m_hWnd!=0 && m_realPModel != NULL) 
		{
			updatePermissions();
			DisplayEquipmentNotes();
		}

		FUNCTION_EXIT;
	}

	void CNotesDialog::updateDataPoint(unsigned long key, ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataPoints()");

		// No data points on this dialog

		FUNCTION_EXIT;
	}

	void CNotesDialog::updateDataNode(ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataNode(ScadaEntityUpdateType updateType)");

		if (m_hWnd!=0) 
		{
			switch (updateType)
			{
			case NotesStateUpdate:
				DisplayEquipmentNotes();
				break;

			default:
				// No updates for this tab
				break;
			}
		}

		FUNCTION_EXIT;
	}

	bool CNotesDialog::isUnsavedChangeOnTab()
	{
		FUNCTION_ENTRY ("isUnsavedChangeOnTab()");
		FUNCTION_EXIT;

		return m_notesChanged;
	}

	bool CNotesDialog::isOkToChangeTab()
	{
		FUNCTION_ENTRY ("isOkToChangeTab()");

		if (m_realPModel != NULL)
		{
			if (m_notesChanged && m_accessAllowed)
			{
                TAUnicodeString actionName;
                actionName.LoadString(IDS_UPDATE_EQUIPMENT_NOTES);
                TA_Base_Bus::TransActiveMessage userMsg;
                userMsg << actionName;
                UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);        

				switch (selectedButton)
				{
				case IDYES:
					return updateNotes();

				case IDNO:
					DisplayEquipmentNotes();
					return true;

				case IDCANCEL:
					return false;

				default:
					DisplayEquipmentNotes();
					return true;
				}
			}
			else
			{
				DisplayEquipmentNotes();
			}
		}

		FUNCTION_EXIT;

		return true;
	}

	void CNotesDialog::DisplayEquipmentNotes()
	{
		FUNCTION_ENTRY ("DisplayEquipmentNotes()");

		std::string notes = "";

		if (m_realPModel != NULL)
		{
			m_notesAccessible = false;
			
			try
			{
				notes = m_realPModel->getNotes();
				m_notesAccessible = true;
			}
			catch (...)
			{
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
					"DisplayEquipmentNotes() - ");
			}
		}

		m_notesEdit.SetLimitText(2000);
		m_notesEdit.SetWindowText(CONVERT_FROM_STD(notes));
		m_notesChanged = false;
		
		updateButtons();

		FUNCTION_EXIT;
	}

	void CNotesDialog::OnUpdate() 
	{
		FUNCTION_ENTRY ("OnUpdate()");

		// TODO: Add your control notification handler code here

        TAUnicodeString actionName;
        actionName.LoadString(IDS_UPDATE_EQUIPMENT_NOTES);
        TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << actionName;
        UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);        
		if (selectedButton == IDYES)
		{
			updateNotes();
		}

		FUNCTION_EXIT;
	}

	void CNotesDialog::OnChangeEditnotes() 
	{
		FUNCTION_ENTRY ("OnChangeEditnotes()");

		// TODO: If this is a RICHEDIT control, the control will not
		// send this notification unless you override the CDialog::OnInitDialog()
		// function and call CRichEditCtrl().SetEventMask()
		// with the ENM_CHANGE flag ORed into the mask.
		
		// TODO: Add your control notification handler code here

		m_notesChanged = true;
		TAUnicodeString notesText;
		m_notesEdit.GetWindowText(notesText.str());
		
		updateButtons();

		FUNCTION_EXIT;
	}

	void CNotesDialog::updateButtons()
	{
		FUNCTION_ENTRY ("updateButtons()");

		if (m_realPModel != NULL)
		{
			TAUnicodeString notesText;
			
			m_notesEdit.GetWindowText(notesText.str());

			GetDlgItem(IDC_REMOVE)->EnableWindow(notesText.GetLength() != 0 && m_accessAllowed && m_notesAccessible);									
			GetDlgItem(IDC_UPDATE)->EnableWindow(m_notesChanged && m_accessAllowed && m_notesAccessible);

			m_notesEdit.EnableWindow(m_accessAllowed && m_notesAccessible);
		}

		FUNCTION_EXIT;
	}

	bool CNotesDialog::updateNotes()
	{
		FUNCTION_ENTRY ("updateNotes()");

		if (m_realPModel != NULL)
		{
			TAUnicodeString notesText;
			m_notesEdit.GetWindowText(notesText.str());
			std::string notes = notesText.toMultiByteStdString();
			
			try
			{
				m_realPModel->setNotes(notes);

				TAUnicodeString *statusLineStr = new TAUnicodeString();
				*statusLineStr = TALoadString(STRING_ID_INSPECTOR_PANEL_0182);

				DisplayEquipmentNotes();
				AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
			}
			catch(OutputLogicException&ex)
			{
				TAUnicodeString *statusLineStr = new TAUnicodeString();
				*statusLineStr = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0183, AppLogicExceptionConverter::ConvertException(ex).str());
				AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
			}


			return true;
		}

		FUNCTION_EXIT;

		return false;
	}
	
	void CNotesDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
	{
		FUNCTION_ENTRY ("OnShowWindow()");

		CDialog::OnShowWindow(bShow, nStatus);
			
		if (bShow)
		{
			DisplayEquipmentNotes();
		}

		FUNCTION_EXIT;
	}

	void CNotesDialog::getWindowRect(CRect* rect)
	{
		GetWindowRect(rect);
	}

	unsigned int CNotesDialog::getMinimumHeight()
	{
		if (m_originalHeight == 0)
		{
			CRect dialogRect;
			GetWindowRect(&dialogRect);
			dialogRect.NormalizeRect();

			m_originalHeight = dialogRect.Height();
		}

		return m_originalHeight;
	}		

	void CNotesDialog::resize(unsigned int height)
	{
		CRect updateRect, removeRect, editRect, dialogRect;
		m_notesEdit.GetWindowRect(&editRect);
		GetDlgItem(IDC_REMOVE)->GetWindowRect(&removeRect);									
		GetDlgItem(IDC_UPDATE)->GetWindowRect(&updateRect);
		GetWindowRect(&dialogRect);

		ScreenToClient(updateRect);
		ScreenToClient(removeRect);
		dialogRect.NormalizeRect();
		editRect.NormalizeRect();

		int diff = height - dialogRect.Height();

		if (diff > 0)
		{
			dialogRect.InflateRect(0, 0, 0, diff);
			editRect.InflateRect(0, 0, 0, diff);
		}
		else
		{
			dialogRect.DeflateRect(0, 0, 0, -diff);
			editRect.InflateRect(0, 0, 0, diff);
		}

		updateRect.OffsetRect(0, diff);
		removeRect.OffsetRect(0, diff);


		GetDlgItem(IDC_REMOVE)->SetWindowPos(NULL, removeRect.left, removeRect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
		GetDlgItem(IDC_UPDATE)->SetWindowPos(NULL, updateRect.left, updateRect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
		m_notesEdit.SetWindowPos(NULL, 0, 0, editRect.Width(), editRect.Height(), SWP_NOMOVE|SWP_NOZORDER);
		SetWindowPos(NULL, 0, 0, dialogRect.Width(), dialogRect.Height(), SWP_NOMOVE|SWP_NOZORDER);
	}

	void CNotesDialog::updatePermissions(/*TA_Base_Bus::RightsLibrary* rightsMgr*/)
	{
		m_accessAllowed = false;

		if (m_realPModel != NULL)
		{
			m_accessAllowed = m_realPModel->isNotesPermitted();
		}
	
		DisplayEquipmentNotes();
	}


	bool CNotesDialog::isAnyDataPointsDisplayedInTab() const
	{
		// no data point on the Notes page, default to true
		return true;
	}

	void CNotesDialog::resetList()
	{

	}
}

