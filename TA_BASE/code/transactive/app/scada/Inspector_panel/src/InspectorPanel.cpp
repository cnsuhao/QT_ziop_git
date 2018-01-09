/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/InspectorPanel.cpp $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class provides the base application class for the inspector panel.  It is implemented
  * as a managed process.
  */

#include "app/scada/Inspector_Panel/src/stdafx.h"

#include "app/scada/Inspector_Panel/src/InspectorPanel.h"
#include "app/scada/Inspector_Panel/src/InspectorPanelDlg.h"
//#include "app/scada/Inspector_Panel/src/InspectorPanelGUI.h"

#include "core\utilities\src\DebugUtil.h"
#include "../pmodel/src/OutputLogicException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_Bus;
using TA_Base_Bus::TransActiveWinApp;

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// CInspectorPanelApp

	BEGIN_MESSAGE_MAP(CInspectorPanelApp, TransActiveWinApp)
		//{{AFX_MSG_MAP(CInspectorPanelApp)
			// NOTE - the ClassWizard will add and remove mapping macros here.
			//    DO NOT EDIT what you see in these blocks of generated code!
		//}}AFX_MSG
		ON_COMMAND(ID_HELP, CWinApp::OnHelp)
		ON_COMMAND(ID_HELP_INDEX, CWinApp::OnHelpIndex)
	END_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////////////////
	// CInspectorPanelApp construction

	CInspectorPanelApp::CInspectorPanelApp() : TransActiveWinApp( TA_Base_Bus::MVT_DIALOG, false )
	{
	}


	TA_Base_Bus::IApplicationController * CInspectorPanelApp::createApplicationController()
	{
		TA_Base_Bus::IApplicationController* pIGUIApplicationController = new InspectorpanelAppController(this,"InspectorPanel");
		return pIGUIApplicationController;
	}


	CWnd* CInspectorPanelApp::createMainView(UINT & dlgID)
	{
		TALoadLang( APPLICATION_ID_InspectorPanel );
		//TASetCurrentLang(LANGUAGE_ID_EN_US);
		//TASetCurrentLang(LANGUAGE_ID_ZH_CN);
		CInspectorPanelDlg* dlg = new CInspectorPanelDlg(this);
		dlgID = CInspectorPanelDlg::IDD;
		return dlg;
	}


	void CInspectorPanelApp::invoke(const char * methodName, const TA_Base_Bus::NameValueParams & params)
	{
		 if(_stricmp(methodName, APP_METHOD_SHOW_STATUS_MESSAGE) == 0)
		 {
			TA_Base_Bus::NameValueParams newParams(params);
			if (newParams.end() == newParams.find("MessageId"))
			{
				TAUnicodeString *statusString = new TAUnicodeString();
				statusString->fromMultiByteString((const char *)newParams["Message"]);
				AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, (WPARAM)statusString, 0);
				return;
			}
			int msgId = (int)newParams["MessageId"];
			std::vector<std::string> parameters = *(std::vector<std::string> *)newParams["Parameters"];
			TAUnicodeString *statusString = new TAUnicodeString();
			switch(msgId)
			{
			case CONTROL_STATUS_SENT_0:
				{
					*statusString = TALoadString(STRING_ID_CONTROL_STATUS_SENT_0);
				}
				break;
			case CONTROL_STATUS_PENDING_0:
				{
					*statusString = TALoadString(STRING_ID_CONTROL_STATUS_PENDING_0);
				}
				break;
			case CONTROL_STATUS_SUCCESSFUL_0:
				{
					*statusString = TALoadString(STRING_ID_CONTROL_STATUS_SUCCESSFUL_0);
				}
				break;
			case CONTROL_STATUS_FAIL_0:
				{
					*statusString = TALoadString(STRING_ID_CONTROL_STATUS_FAIL_0);
				}
				break;
			case CONTROL_STATUS_LCC_FAIL_4:
				{
					*statusString = TAAfxFormatString4(STRING_ID_CONTROL_STATUS_LCC_FAIL_4, CONVERT_FROM_STD(parameters[0]), CONVERT_FROM_STD(parameters[1]),CONVERT_FROM_STD(parameters[2]),CONVERT_FROM_STD(parameters[3]));
				}
				break;
			case CONTROL_STATUS_RCC_FAIL_3:
				{
					*statusString = TAAfxFormatString3(STRING_ID_CONTROL_STATUS_RCC_FAIL_3, CONVERT_FROM_STD(parameters[0]),CONVERT_FROM_STD(parameters[1]), CONVERT_FROM_STD(parameters[2]));
				}
				break;
			case CONTROL_STATUS_COMMS_FAIL_0:
				{
					*statusString = TALoadString(STRING_ID_CONTROL_STATUS_COMMS_FAIL_0);
				}
				break;
			default:
				return;
				break;
			}
			AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, (WPARAM)statusString, 0);
			
		 }
	}

	/*void	CInspectorPanelApp::requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent)
	{
		int typeCode = uiEvent->getTypeCode();
		TA_Base_Bus::UINotificationDataContextPtr dataContext = uiEvent->getMyDataContext();
		if (dataContext->getParamEncode() == TA_Base_Bus::PARAM_ENCODE_STR_1)
		{
			switch(typeCode)
			{
			case UserReplyControlConfirm:
				{
					this->showControlConfirmDialg(dataContext->getParamValue(0),dataContext->getParamValue(1),dataContext->getParamValue(2));
				}
				break;
			default:

			}
		}
	}

	bool CInspectorPanelApp::showControlConfirmDialg(const std::string& equipmentName,const std::string& datapointDescription, const std::string& newValue)
	{
		CConfirmCommandDialog     dlg;

		dlg.m_equipment = equipmentName.c_str();
		dlg.m_dataPoint = datapointDescription.c_str();
		dlg.m_newValue  = newValue.c_str();

		return dlg.DoModal()==IDOK;
	}*/


// 	bool CInspectorPanelApp::showOverrideConfirmDialog()
// 	{
// 
// 	}

	/////////////////////////////////////////////////////////////////////////////
	// The one and only CInspectorPanelApp object

	CInspectorPanelApp theApp;
}

