/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/TagDialog.cpp $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the display and update of tag settings associated with equipment.
  */

#ifdef __WIN32__
#pragma warning(disable:4786)
#endif

#include "app/scada/Inspector_Panel/src/stdafx.h"

#include "app/scada/inspector_panel/src/InspectorPanel.h"
#include "app/scada/inspector_panel/src/ConfirmTagDialog.h"
#include "app/scada/inspector_panel/src/InspectorPanelDlg.h"
#include "app/scada/inspector_panel/src/TagDialog.h"
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
	// CTagDialog dialog

	CTagDialog::CTagDialog(TA_Base_Bus::ITransActiveWinApp*app,CWnd* pParent /*=NULL*/)
	: TA_Base_Bus::AbstractTransActiveDialog(app, CTagDialog::IDD, NULL),
        m_tagAccessAllowed(false),
        m_ptwTagAccessAllowed(false),
        m_originalHeight(0)
	{
		FUNCTION_ENTRY ("CTagDialog()");

		//{{AFX_DATA_INIT(CTagDialog)
		//}}AFX_DATA_INIT

		FUNCTION_EXIT;
	}


	void CTagDialog::DoDataExchange(CDataExchange* pDX)
	{
		FUNCTION_ENTRY ("DoDataExchange()");

		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CTagDialog)
		DDX_Control(pDX, IDC_SCANINHIBIT_CHECK, m_scanInhibitCheck);
		DDX_Control(pDX, IDC_MMSINHIBIT_CHECK, m_mmsInhibitCheck);
		DDX_Control(pDX, IDC_CONTROLINHIBIT_CHECK, m_controlInhibitCheck);
		DDX_Control(pDX, IDC_ALARMINHIBIT_CHECK, m_alarmInhibitCheck);
		DDX_Control(pDX, IDC_PTW_ID, m_ptwIdEdit);
		DDX_Control(pDX, IDC_COMMENT, m_commentEdit);
	//}}AFX_DATA_MAP

		FUNCTION_EXIT;
	}


	BEGIN_MESSAGE_MAP(CTagDialog, CDialog)
		//{{AFX_MSG_MAP(CTagDialog)
		ON_BN_CLICKED(IDC_APPLY_TAG, onApplyTag)
		ON_BN_CLICKED(IDC_RADIO_TAG_PTW, OnRadioTagPtw)
		ON_BN_CLICKED(IDC_RADIO_NOT_TAGGED, OnRadioNotTagged)
		ON_BN_CLICKED(IDC_RADIO_TAG, OnRadioTag)
		ON_EN_CHANGE(IDC_PTW_ID, OnChangeEdit)
		ON_WM_SHOWWINDOW()
		ON_WM_PAINT()
		ON_EN_CHANGE(IDC_COMMENT, OnChangeEdit)
		ON_MESSAGE( MESSAGE_REFRESH_ALL_DATA, onRefreshAllData)
		ON_BN_CLICKED(IDC_ALARMINHIBIT_CHECK, OnAlarmInhibitCheck)
		ON_BN_CLICKED(IDC_CONTROLINHIBIT_CHECK, OnControlInhibitCheck)
		ON_BN_CLICKED(IDC_MMSINHIBIT_CHECK, OnMmsInhibitCheck)
		ON_BN_CLICKED(IDC_SCANINHIBIT_CHECK, OnScanInhibitCheck)
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////////////////
	// CTagDialog message handlers

	void CTagDialog::onApplyTag() 
	{
		FUNCTION_ENTRY ("onApplyTag()");

		applyTag();

		FUNCTION_EXIT;
	}

	void CTagDialog::setupPModel()
	{
		m_realPModel = dynamic_cast<TagDialogModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");

		m_realPModel->attachView(this,PMODEL_NOTIFY_REFRESH_ALL_DATA);
		m_realPModel->attachView(this,PMODEL_NOTIFY_UPDATE_DATA);
	}


	std::string CTagDialog::getMyPModelName()
	{
		return PMODEL_TAG_DLG;
	}

	void	CTagDialog::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		if( _stricmp(updateType, PMODEL_NOTIFY_REFRESH_ALL_DATA) == 0 )
		{
			BOOL err = this->PostMessage(MESSAGE_REFRESH_ALL_DATA);
		}
	}

	LRESULT CTagDialog::onRefreshAllData(WPARAM wparam, LPARAM lparam)
	{
		this->refreshAllData();
		return 0;
	}

	//todo->put more logic to model
	bool CTagDialog::applyTag()
	{
		FUNCTION_ENTRY ("applyTag()");

		if (m_realPModel == NULL)
		{
			FUNCTION_EXIT;
			return false;
		}
		int tagRadioButton = GetCheckedRadioButton(IDC_RADIO_NOT_TAGGED, IDC_RADIO_TAG_PTW);
		
		TAUnicodeString tagPtwId, tagComment;
		CConfirmTagDialog   dlg;
		
		m_ptwIdEdit.GetWindowText(tagPtwId.str());
		m_commentEdit.GetWindowText(tagComment.str());


		dlg.m_equipment = TAUnicodeString::CreateFromMultitByteString(m_realPModel->getDataNodeAssetName().c_str());
		dlg.m_equipment += _T("\r\n");
		dlg.m_equipment += TAUnicodeString::CreateFromMultitByteString(m_realPModel->getDataNodeDescription().c_str());
	
		if (!(true == m_tagAccessAllowed || true == m_ptwTagAccessAllowed))
		{
			FUNCTION_EXIT;
			return false;
		}

		// Format Initial status string
		switch (tagRadioButton)
		{
		case IDC_RADIO_TAG:
				dlg.m_tag = TALoadString(STRING_ID_INSPECTOR_PANEL_0040); //_T("Tag without PTW");
				break; 
		case IDC_RADIO_TAG_PTW:
				//dlg.m_tag = _T("Tag with PTW \'");
				//dlg.m_tag += tagPtwId;
				//dlg.m_tag += _T("\'");
				dlg.m_tag = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0041,tagPtwId.str());
				break;
		case IDC_RADIO_NOT_TAGGED:
				//dlg.m_tag = _T("Remove Tag");
				dlg.m_tag = TALoadString(STRING_ID_INSPECTOR_PANEL_0042);
				break;
		}


		// Work out inhibit details
		TA_Base_Bus::InhibitState		inhibitState;
		TAUnicodeString					inhibitString;
		
        inhibitState.inputInhibit = TA_Base_Bus::NoInputInhibit;
		inhibitState.outputInhibit = TA_Base_Bus::NoOutputInhibit;

		if (m_scanInhibitChecked)
		{
			//inhibitString = "Inhibit scan";
			inhibitString = TALoadString(STRING_ID_INSPECTOR_PANEL_0200);
			inhibitState.inputInhibit = TA_Base_Bus::InhibitScan;
		}
		else if (m_alarmInhibitChecked)
		{
			//inhibitString = "Inhibit alarm";
			inhibitString = TALoadString(STRING_ID_INSPECTOR_PANEL_0201);
			inhibitState.inputInhibit = TA_Base_Bus::InhibitAlarm;
		}
		else if (m_mmsInhibitChecked)
		{
			//inhibitString = "Inhibit MMS";
			inhibitString = TALoadString(STRING_ID_INSPECTOR_PANEL_0202);
			inhibitState.inputInhibit = TA_Base_Bus::InhibitMmsAlarm;
		}

		if (m_controlInhibitChecked)
		{
			if (inhibitString.IsEmpty())
			{
				//inhibitString = "Inhibit control";
				inhibitString = TALoadString(STRING_ID_INSPECTOR_PANEL_0203);
			}
			else
			{
				//inhibitString += " and control";
				inhibitString += TALoadString(STRING_ID_INSPECTOR_PANEL_0204);
			}
	
			inhibitState.outputInhibit = TA_Base_Bus::InhibitControl;
		}
		
		if (inhibitString.IsEmpty())
		{
			if (tagRadioButton == IDC_RADIO_NOT_TAGGED)
			{
				if (m_originalScanInhibitChecked)
				{
					//inhibitString = "Enable scan";
					inhibitString = TALoadString(STRING_ID_INSPECTOR_PANEL_0205);
				}
				else if (m_originalAlarmInhibitChecked)
				{
					//inhibitString = "Enable alarm";
					inhibitString = TALoadString(STRING_ID_INSPECTOR_PANEL_0206);
				}
				else if (m_originalMmsInhibitChecked)
				{
					//inhibitString = "Enable MMS";
					inhibitString = TALoadString(STRING_ID_INSPECTOR_PANEL_0207);
				}

				if (m_originalControlInhibitChecked)
				{
					if (inhibitString.IsEmpty())
					{
						//inhibitString = "Enable control";
						inhibitString = TALoadString(STRING_ID_INSPECTOR_PANEL_0208);
					}
					else
					{
						//inhibitString += " and control";
						inhibitString += TALoadString(STRING_ID_INSPECTOR_PANEL_0209);
					}
				}

				if (inhibitString.IsEmpty())
				{
					//inhibitString = "Inhibition unchanged";
					inhibitString = TALoadString(STRING_ID_INSPECTOR_PANEL_0210);
				}
			}
			else
			{
				//inhibitString = "No Inhibition";
				inhibitString = TALoadString(STRING_ID_INSPECTOR_PANEL_0211);
			}
		}
		
		dlg.m_inhibit = inhibitString;//TAUnicodeString::CreateFromMultitByteString( inhibitString.c_str() );

		// Populate dialog
		bool apply = (dlg.DoModal()==IDOK);

		if (apply)
		{
			std::stringstream statusLine;
			statusLine << "Equipment Tag";

			// tag/un-tag equipment
				
			std::string ptwId = tagPtwId.toMultiByteStdString();
			std::string comment = tagComment.toMultiByteStdString();

			std::string message = "Applying tag state \"" + dlg.m_tag.toMultiByteStdString();
			message += "/" + dlg.m_inhibit.toMultiByteStdString();
			message += "\" to Equipment " + m_realPModel->getDataNodeAssetName() + ".";

			TAUnicodeString *statusLineStr = new TAUnicodeString();
			*statusLineStr = TAAfxFormatString3(STRING_ID_INSPECTOR_PANEL_0212, dlg.m_tag.str(), dlg.m_inhibit.str(), CONVERT_FROM_STD(m_realPModel->getDataNodeAssetName()));
			AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);

			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
				"applyTag() - %s", message.c_str());
            
            std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);

			switch (tagRadioButton)
			{
			case IDC_RADIO_TAG:
				{
	
					// tag the equipment now
					try
					{
						m_realPModel->setTagState(Tagged, inhibitState.outputInhibit, inhibitState.inputInhibit,
										 "", comment);
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0213, CONVERT_FROM_STD(m_realPModel->getDataNodeAssetName()));
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					catch(OutputLogicException&ex)
					{
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0214, CONVERT_FROM_STD(m_realPModel->getDataNodeAssetName()), AppLogicExceptionConverter::ConvertException(ex).str());
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					break; 
				}

			case IDC_RADIO_TAG_PTW:
				{
					try
					{
						m_realPModel->setTagState(TaggedPtw, inhibitState.outputInhibit, inhibitState.inputInhibit, 
										 ptwId, comment);
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0215, CONVERT_FROM_STD(m_realPModel->getDataNodeAssetName()));
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					catch(OutputLogicException&ex)
					{
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0216, CONVERT_FROM_STD(m_realPModel->getDataNodeAssetName()), AppLogicExceptionConverter::ConvertException(ex).str());
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					break;
				}

			case IDC_RADIO_NOT_TAGGED:
				{
					try
					{
						m_realPModel->removeTagState();	
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0217, CONVERT_FROM_STD(m_realPModel->getDataNodeAssetName()));
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					catch(OutputLogicException&ex)
					{
						TAUnicodeString *statusLineStr = new TAUnicodeString();
						*statusLineStr = TAAfxFormatString2(STRING_ID_INSPECTOR_PANEL_0218, CONVERT_FROM_STD(m_realPModel->getDataNodeAssetName()), AppLogicExceptionConverter::ConvertException(ex).str());
						AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
					}
					break;
				}
			}
		}
			

		DisplayEquipmentTagStatus();

		FUNCTION_EXIT;
		return apply;
	}

	void CTagDialog::OnRadioTagPtw() 
	{
		FUNCTION_ENTRY ("OnRadioTagPtw()");

		CheckRadioButton(IDC_RADIO_NOT_TAGGED, IDC_RADIO_TAG_PTW, IDC_RADIO_TAG_PTW);	

		/* fTD220 - check all inhibits for tag-ptw by default */
		m_scanInhibitChecked = true;
		m_scanInhibitCheck.SetCheck(m_scanInhibitChecked);
		
		m_alarmInhibitChecked = m_scanInhibitChecked ? true : false;
		m_alarmInhibitCheck.SetCheck(m_alarmInhibitChecked);
		
		m_mmsInhibitChecked = (m_scanInhibitChecked || m_alarmInhibitChecked) ? true : false;
		m_mmsInhibitCheck.SetCheck(m_mmsInhibitChecked);
		
		m_controlInhibitChecked = true;
		m_controlInhibitCheck.SetCheck(m_controlInhibitChecked);

		GetDlgItem(IDC_APPLY_TAG)->EnableWindow(isUnsavedChangeOnTab());

		updateRadioButtonAccess();
		checkPtwTextCommentField();	//wenching (TD13741)

		m_commentEdit.EnableWindow(true);
		m_ptwIdEdit.EnableWindow(true);
		m_ptwIdEdit.SetLimitText(40);

		FUNCTION_EXIT;
	}

	void CTagDialog::OnRadioNotTagged() 
	{
		FUNCTION_ENTRY ("OnRadioNotTagged()");

		CheckRadioButton(IDC_RADIO_NOT_TAGGED, IDC_RADIO_TAG_PTW, IDC_RADIO_NOT_TAGGED);	
		
		GetDlgItem(IDC_APPLY_TAG)->EnableWindow(isUnsavedChangeOnTab());
		
		updateRadioButtonAccess();
		checkPtwTextCommentField(); //wenching (TD13741)

		m_commentEdit.EnableWindow(false);
		m_ptwIdEdit.EnableWindow(false);

		FUNCTION_EXIT;
	}

	void CTagDialog::OnRadioTag() 
	{
		FUNCTION_ENTRY ("OnRadioTag()");

		CheckRadioButton(IDC_RADIO_NOT_TAGGED, IDC_RADIO_TAG_PTW, IDC_RADIO_TAG);

		/* fTD220 - check all inhibits for tag-ptw by default */
		m_scanInhibitChecked = true;
		m_scanInhibitCheck.SetCheck(m_scanInhibitChecked);
		
		m_alarmInhibitChecked = m_scanInhibitChecked ? true : false;
		m_alarmInhibitCheck.SetCheck(m_alarmInhibitChecked);
		
		m_mmsInhibitChecked = (m_scanInhibitChecked || m_alarmInhibitChecked) ? true : false;
		m_mmsInhibitCheck.SetCheck(m_mmsInhibitChecked);
		
		m_controlInhibitChecked = true;
		m_controlInhibitCheck.SetCheck(m_controlInhibitChecked);

		GetDlgItem(IDC_APPLY_TAG)->EnableWindow(isUnsavedChangeOnTab());

		updateRadioButtonAccess();
		checkPtwTextCommentField(); //wenching (TD13741)

		m_commentEdit.EnableWindow(true);
		m_ptwIdEdit.EnableWindow(false);

		FUNCTION_EXIT;
	}

	/* SetEquipment
	 *
	 * This function will set the name of the equipment that the data points are to be displayed for.It can 
	 * be called before or after the windows control is on the screen.
	 */
	void CTagDialog::refreshAllData()
	{
		FUNCTION_ENTRY ("setEquipment()");


		// Update the displayed control
		if (m_hWnd!=0 && m_realPModel != NULL) 
		{
			updatePermissions();
			
			DisplayEquipmentTagStatus();
		}

		FUNCTION_EXIT;
	}

	void CTagDialog::updateDataPoint(unsigned long key, ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataPoints()");

		// No data points on this dialog

		FUNCTION_EXIT;
	}

	void CTagDialog::updateDataNode(ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataNode()");

		if (m_hWnd!=0) 
		{
			switch (updateType)
			{
			
			case TA_Base_Bus::TagStateUpdate:
				// Only update on change of tag state or initialisation/invalidation
				DisplayEquipmentTagStatus();
				break;

			default:
				// Do nothing for these updates
				break;
			}
		}

		FUNCTION_EXIT;
	}

	bool CTagDialog::isUnsavedChangeOnTab()
	{
		FUNCTION_ENTRY ("isUnsavedChangeOnTab()");

		int radioButton = GetCheckedRadioButton(IDC_RADIO_NOT_TAGGED, IDC_RADIO_TAG_PTW);

		bool hasChanged = false;
		TAUnicodeString tagPtwId, tagComment;

		m_ptwIdEdit.GetWindowText(tagPtwId.str());
		m_commentEdit.GetWindowText(tagComment.str());

		switch (radioButton)
		{
		case IDC_RADIO_TAG:
			hasChanged = (m_originalTagStatus != TA_Base_Bus::Tagged) || tagComment.Compare(CONVERT_FROM_STD(m_originalComment)) != 0;
			break; 

		case IDC_RADIO_TAG_PTW:
			if (m_ptwTagAccessAllowed)
            {
                hasChanged = (m_originalTagStatus != TA_Base_Bus::TaggedPtw ||
						  tagComment.Compare( CONVERT_FROM_STD(m_originalComment) ) != 0 ||
						  tagPtwId.Compare(CONVERT_FROM_STD(m_originalPtwId) ) != 0);		
            }
            else
            {
                // Special case - PTW disabled
                hasChanged = false;
            }
            break;

		case IDC_RADIO_NOT_TAGGED:
			hasChanged = (m_originalTagStatus != TA_Base_Bus::NotTagged);
			break;
		}	

		hasChanged |= (m_originalScanInhibitChecked != m_scanInhibitChecked);
		hasChanged |= (m_originalAlarmInhibitChecked != m_alarmInhibitChecked);
		hasChanged |= (m_originalMmsInhibitChecked != m_mmsInhibitChecked);
		hasChanged |= (m_originalControlInhibitChecked != m_controlInhibitChecked);

		if (!(m_tagAccessAllowed || m_ptwTagAccessAllowed))
		{
			// Special case - no permissions
			hasChanged = false;
		}

		FUNCTION_EXIT;
		return hasChanged;
	}

	bool CTagDialog::isOkToChangeTab()
	{
		FUNCTION_ENTRY ("isOkToChangeTab()");

		if (!isUnsavedChangeOnTab())
		{
			FUNCTION_EXIT;
			return true;
		}

        TAUnicodeString actionName;
        actionName.LoadString(IDS_APPLY_TAG_CHANGES);
        TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << actionName;
        UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);        

		switch (selectedButton)
		{
		case IDYES:
			FUNCTION_EXIT;
			return applyTag();

		case IDNO:
			DisplayEquipmentTagStatus();
			FUNCTION_EXIT;
			return true;

		case IDCANCEL:
			FUNCTION_EXIT;
			return false;

		default:
			DisplayEquipmentTagStatus();
			FUNCTION_EXIT;
			return true;
		}
	}

	void CTagDialog::DisplayEquipmentTagStatus()
	{
		FUNCTION_ENTRY ("DisplayEquipmentTagStatus()");

		if (m_realPModel != NULL && m_hWnd != 0)
		{
/*			try*/
			{
				m_commentEdit.SetLimitText(2000);
				m_originalTagStatus = m_realPModel->getDataNodeTaggingState();
				TA_Base_Bus::InhibitState tagInhibitStatus = m_realPModel->getDataNodeTagInhibitState();

				m_originalScanInhibitChecked = (tagInhibitStatus.inputInhibit == TA_Base_Bus::InhibitScan);
				m_originalAlarmInhibitChecked = (tagInhibitStatus.inputInhibit == TA_Base_Bus::InhibitScan || tagInhibitStatus.inputInhibit == TA_Base_Bus::InhibitAlarm);
				m_originalMmsInhibitChecked = (tagInhibitStatus.inputInhibit == TA_Base_Bus::InhibitScan || tagInhibitStatus.inputInhibit == TA_Base_Bus::InhibitAlarm || tagInhibitStatus.inputInhibit == TA_Base_Bus::InhibitMmsAlarm);
				m_originalControlInhibitChecked = (tagInhibitStatus.outputInhibit == TA_Base_Bus::InhibitControl);

				m_originalPtwId = m_realPModel->getDataNodePtwId();
				m_originalComment = m_realPModel->getDataNodeTagComment();
				 
				switch (m_originalTagStatus)
				{
				case TA_Base_Bus::Tagged:
					CheckRadioButton(IDC_RADIO_NOT_TAGGED, IDC_RADIO_TAG_PTW, IDC_RADIO_TAG);
					m_commentEdit.EnableWindow();
					m_commentEdit.SetWindowText( CONVERT_FROM_STD(m_originalComment) );
					m_ptwIdEdit.EnableWindow(false);
					m_ptwIdEdit.SetWindowText(_T(""));
					break; 

				case TA_Base_Bus::TaggedPtw:
					CheckRadioButton(IDC_RADIO_NOT_TAGGED, IDC_RADIO_TAG_PTW, IDC_RADIO_TAG_PTW);
					m_commentEdit.EnableWindow();
					m_commentEdit.SetWindowText( CONVERT_FROM_STD(m_originalComment) );
					m_ptwIdEdit.EnableWindow();
					m_ptwIdEdit.SetWindowText(CONVERT_FROM_STD(m_originalPtwId));
					m_ptwIdEdit.SetLimitText(40);
					break;

				case TA_Base_Bus::NotTagged:
					CheckRadioButton(IDC_RADIO_NOT_TAGGED, IDC_RADIO_TAG_PTW, IDC_RADIO_NOT_TAGGED);
					m_commentEdit.EnableWindow(false);
					m_commentEdit.SetWindowText(_T(""));
					m_ptwIdEdit.EnableWindow(false);
					m_ptwIdEdit.SetWindowText(_T(""));
					break;
				}

				m_scanInhibitChecked = m_originalScanInhibitChecked;
				m_scanInhibitCheck.SetCheck(m_scanInhibitChecked);

				m_alarmInhibitChecked = m_scanInhibitChecked?true:m_originalAlarmInhibitChecked;
				m_alarmInhibitCheck.SetCheck(m_alarmInhibitChecked);
				m_alarmInhibitCheck.EnableWindow(!m_scanInhibitChecked);

				m_mmsInhibitChecked = (m_scanInhibitChecked || m_alarmInhibitChecked)?true:m_originalMmsInhibitChecked;
				m_mmsInhibitCheck.SetCheck(m_mmsInhibitChecked);
				m_mmsInhibitCheck.EnableWindow(!(m_scanInhibitChecked || m_alarmInhibitChecked));

				m_controlInhibitChecked = m_originalControlInhibitChecked;
				m_controlInhibitCheck.SetCheck(m_controlInhibitChecked);
				
				// Set up buttons as per Access Control
				updateRadioButtonAccess();
				updatePtwCommentAccess();

				GetDlgItem(IDC_APPLY_TAG)->EnableWindow(false);
			}
// 			catch (TA_Base_Core::ScadaProxyException &e)
// 			{
// 				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
// 					"DisplayEquipmentTagStatus() - %s", e.what());
// 			}
		}

		FUNCTION_EXIT;
	}

	void CTagDialog::OnChangeEdit() 
	{
		FUNCTION_ENTRY ("OnChangeEdit()");

		if (m_realPModel != NULL)
		{		
			// Check access control first and disable apply button if no permissions
			updateRadioButtonAccess();
			updatePtwCommentAccess();
			
			GetDlgItem(IDC_APPLY_TAG)->EnableWindow(isUnsavedChangeOnTab());
			checkPtwTextCommentField(); //wenching++ (TD13741)
		}

		FUNCTION_EXIT;
	}


	// ++wenching (TD12062,TD13140)
	
	void CTagDialog::updateRadioButtonAccess()
	{
		FUNCTION_ENTRY ("updateRadioButtonAccess()");

		if (m_realPModel != NULL)
		{
			//TD15276
			bool enableReset = m_tagAccessAllowed || m_ptwTagAccessAllowed;

			if ( (Tagged == m_originalTagStatus && false == m_tagAccessAllowed)
				|| (TaggedPtw == m_originalTagStatus && false == m_ptwTagAccessAllowed) )
			{
				// Currently tagged option is not equil to the privelege to set/reset the tag option
				enableReset = false;
			}

			GetDlgItem(IDC_RADIO_TAG)->EnableWindow(enableReset && m_tagAccessAllowed);			
            GetDlgItem(IDC_RADIO_TAG_PTW)->EnableWindow(enableReset && m_ptwTagAccessAllowed);
            GetDlgItem(IDC_RADIO_NOT_TAGGED)->EnableWindow(enableReset);

			bool tagged = (GetCheckedRadioButton(IDC_RADIO_NOT_TAGGED, IDC_RADIO_TAG_PTW) != IDC_RADIO_NOT_TAGGED);

			if (!tagged)
			{
				m_scanInhibitChecked = false;
				m_scanInhibitCheck.SetCheck(false); 
				m_alarmInhibitChecked = false;
				m_alarmInhibitCheck.SetCheck(false);
				m_mmsInhibitChecked = false;
				m_mmsInhibitCheck.SetCheck(false); 
				m_controlInhibitChecked = false;
				m_controlInhibitCheck.SetCheck(false);	
			}

			// wenching++ (TD12062,TD13140)
			// Operator profile checking
			bool isInhibitEnable = false;
		
			isInhibitEnable = m_realPModel->isProfileAccessAllowed(m_scanInhibitChecked, scanInhibit);
			m_scanInhibitCheck.EnableWindow(isInhibitEnable && tagged && (m_tagAccessAllowed || m_ptwTagAccessAllowed) && m_realPModel->hasInformationPoint()); 

			isInhibitEnable = m_realPModel->isProfileAccessAllowed(m_alarmInhibitChecked, alarmInhibit);
			m_alarmInhibitCheck.EnableWindow(isInhibitEnable && tagged && (m_tagAccessAllowed || m_ptwTagAccessAllowed) && !m_scanInhibitChecked && m_realPModel->hasInformationPoint());
		
			isInhibitEnable = m_realPModel->isProfileAccessAllowed(m_mmsInhibitChecked, mmsInhibit);
			m_mmsInhibitCheck.EnableWindow(isInhibitEnable && tagged && (m_tagAccessAllowed || m_ptwTagAccessAllowed) && !(m_scanInhibitChecked || m_alarmInhibitChecked) && m_realPModel->hasInformationPoint()); 

			isInhibitEnable = m_realPModel->isProfileAccessAllowed(m_controlInhibitChecked, controlInhibit);
			m_controlInhibitCheck.EnableWindow(isInhibitEnable && tagged && (m_tagAccessAllowed || m_ptwTagAccessAllowed) && m_realPModel->hasControlPoint());	
			// ++wenching (TD12062,TD13140)
		}

		FUNCTION_EXIT;
	}

	void CTagDialog::updatePtwCommentAccess()
	{
		FUNCTION_ENTRY ("updatePtwCommentAccess()");

		if (m_realPModel != NULL)
		{
			//TD16016, revert TD15276, must disble window when do not have rights
			if (false == m_ptwTagAccessAllowed)
			{
				m_ptwIdEdit.EnableWindow(false);
			}

			if (false == m_tagAccessAllowed && false == m_ptwTagAccessAllowed)
			{
				m_commentEdit.EnableWindow(false);
			}
		}
	}

	//wenching++ (TD13741)
	void CTagDialog::checkPtwTextCommentField()
	{
		TAUnicodeString tagPtwId, tagComment;

		int tagRadioButton = GetCheckedRadioButton(IDC_RADIO_NOT_TAGGED, IDC_RADIO_TAG_PTW);
		m_ptwIdEdit.GetWindowText(tagPtwId.str());
		m_commentEdit.GetWindowText(tagComment.str());

		if (tagRadioButton == IDC_RADIO_TAG_PTW && 
			(tagPtwId.Compare( _T("") ) == 0 || tagComment.Compare( _T("") ) == 0) )
		{
			GetDlgItem(IDC_APPLY_TAG)->EnableWindow(false);
		}
		else if (tagRadioButton == IDC_RADIO_TAG)
		{
			GetDlgItem(IDC_APPLY_TAG)->EnableWindow(true);
		}
	}
	//++wenching (TD13741)

	void CTagDialog::OnAlarmInhibitCheck() 
	{
		FUNCTION_ENTRY ("OnAlarmInhibitCheck()");

		bool previousChecked = m_alarmInhibitChecked;

		m_alarmInhibitChecked = !m_alarmInhibitChecked;
		m_alarmInhibitCheck.SetCheck(m_alarmInhibitChecked);

		m_mmsInhibitChecked = (m_alarmInhibitChecked || m_originalAlarmInhibitChecked || previousChecked)?true:m_originalMmsInhibitChecked;
		m_mmsInhibitCheck.SetCheck(m_mmsInhibitChecked);

		GetDlgItem(IDC_APPLY_TAG)->EnableWindow(isUnsavedChangeOnTab());

		updateRadioButtonAccess();
		updatePtwCommentAccess();
		checkPtwTextCommentField(); //wenching (TD13741)

		FUNCTION_EXIT;
	}

	void CTagDialog::OnControlInhibitCheck() 
	{
		FUNCTION_ENTRY ("OnControlInhibitCheck()");

		m_controlInhibitChecked = !m_controlInhibitChecked;
		m_controlInhibitCheck.SetCheck(m_controlInhibitChecked);

		GetDlgItem(IDC_APPLY_TAG)->EnableWindow(isUnsavedChangeOnTab());

		updateRadioButtonAccess();
		updatePtwCommentAccess();
		checkPtwTextCommentField(); //wenching (TD13741)

		FUNCTION_EXIT;
	}

	void CTagDialog::OnMmsInhibitCheck() 
	{
		FUNCTION_ENTRY ("OnMmsInhibitCheck()");

		m_mmsInhibitChecked = !m_mmsInhibitChecked;
		m_mmsInhibitCheck.SetCheck(m_mmsInhibitChecked);

		GetDlgItem(IDC_APPLY_TAG)->EnableWindow(isUnsavedChangeOnTab());

		updateRadioButtonAccess();
		updatePtwCommentAccess();
		checkPtwTextCommentField(); //wenching (TD13741)

		FUNCTION_EXIT;
	}

	void CTagDialog::OnScanInhibitCheck() 
	{
		FUNCTION_ENTRY ("OnAlarmInhibitCheck()");

		bool previousChecked = m_scanInhibitChecked;

		m_scanInhibitChecked = !m_scanInhibitChecked;
		m_scanInhibitCheck.SetCheck(m_scanInhibitChecked);

		m_alarmInhibitChecked = (m_scanInhibitChecked || m_originalScanInhibitChecked || previousChecked)?true:m_originalAlarmInhibitChecked;
		m_alarmInhibitCheck.SetCheck(m_alarmInhibitChecked);
		
		m_mmsInhibitChecked = (m_scanInhibitChecked || m_alarmInhibitChecked)?true:m_originalMmsInhibitChecked;
		m_mmsInhibitCheck.SetCheck(m_mmsInhibitChecked);
		
		GetDlgItem(IDC_APPLY_TAG)->EnableWindow(isUnsavedChangeOnTab());

		updateRadioButtonAccess();
		updatePtwCommentAccess();
		checkPtwTextCommentField(); //wenching (TD13741)

		FUNCTION_EXIT;
	}

	void CTagDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
	{
		FUNCTION_ENTRY ("OnShowWindow()");

		CDialog::OnShowWindow(bShow, nStatus);
			
		if (bShow)
		{
			DisplayEquipmentTagStatus();
		}

		FUNCTION_EXIT;
	}

	void CTagDialog::getWindowRect(CRect* rect)
	{
		GetWindowRect(rect);
	}

	unsigned int CTagDialog::getMinimumHeight()
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

	void CTagDialog::resize(unsigned int height)
	{
	}


    bool CTagDialog::isPtwTagAccessAllowed()
    {
        // Can only access the PTW Tag components if the PTW privilege present
        // as well as the basic tag access control for this page
		//TD15276
        return m_ptwTagAccessAllowed;
    }


	void CTagDialog::updatePermissions()
	{
		m_tagAccessAllowed = false;
        m_ptwTagAccessAllowed = false;

		if (m_realPModel != NULL)
		{
			m_tagAccessAllowed = m_realPModel->isActionPermitted(DATA_NODE_TAG);
            m_ptwTagAccessAllowed = m_realPModel->isActionPermitted(DATA_NODE_TAG_PTW);
		}		

        // Now that rights updated, update associated controls
        // that are disabled based on rights
        DisplayEquipmentTagStatus();
		checkPtwTextCommentField(); //wenching (TD13741)
	}


	bool CTagDialog::isAnyDataPointsDisplayedInTab() const
	{
		// no data point on the Notes page, default to true
		return true;
	}

	void CTagDialog::resetList()
	{

	}



#if 0
	void CTagDialog::setInhibitCheckboxes(bool scanInhibit, bool alarmInhibit, bool mmsInhibit, bool controlInhibit)
	{
		m_scanInhibitChecked = scanInhibit;
		m_scanInhibitCheck.SetCheck(m_scanInhibitChecked);
		
		m_alarmInhibitChecked = m_scanInhibitChecked ? true : alarmInhibit;
		m_alarmInhibitCheck.SetCheck(m_alarmInhibitChecked);
		m_alarmInhibitCheck.EnableWindow(!m_scanInhibitChecked);
		
		m_mmsInhibitChecked = (m_scanInhibitChecked || m_alarmInhibitChecked) ? true : mmsInhibit;
		m_mmsInhibitCheck.SetCheck(m_mmsInhibitChecked);
		m_mmsInhibitCheck.EnableWindow(!(m_scanInhibitChecked || m_alarmInhibitChecked));
		
		m_controlInhibitChecked = controlInhibit;
		m_controlInhibitCheck.SetCheck(m_controlInhibitChecked);
	}
#endif		
}
