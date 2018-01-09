// SelectSesSubDutyDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "dutymanager.h"
#include "app/security/DutyManager/src/DutyManager.h"
#include "app/security/DutyManager/pmodel/src/SelectSesSubDutyDlgModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// SelectSesSubDutyDlg dialog
using TA_Base_Bus::DutyAgentAccess;
using TA_Base_Bus::DataCache;
using TA_Base_Bus::SessionCache;
using TA_Base_Bus::RegionKeyList;
using TA_Base_Bus::TransferableRegionList;
using TA_Base_Core::DebugUtil;

using namespace TA_Base_App;
using namespace TA_Base_Bus;
/////////////////////////////////////////////////////////////////////////////
// SelectSesSubDutyDlg dialog


SelectSesSubDutyDlg::SelectSesSubDutyDlg(TA_Base_Bus::ITransActiveWinApp * app,  CWnd* parent)
	: TA_Base_Bus::AbstractDialog(app, SelectSesSubDutyDlg::IDD, parent)
{
	//{{AFX_DATA_INIT(SelectSesSubDutyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,"new SelectSesSubDutyDlg()");

	m_selectedSessionInfo.SessionId = "";
}


void SelectSesSubDutyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SelectSesSubDutyDlg)
	DDX_Control(pDX, IDOK, m_selectSesBtn);
	DDX_Control(pDX, IDC_SELECT_SESSION_LIST, m_selectSessionList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SelectSesSubDutyDlg, CDialog)
	//{{AFX_MSG_MAP(SelectSesSubDutyDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SELECT_SESSION_LIST, OnItemchangedSelectSessionList)
	ON_BN_CLICKED(IDOK, OnSelectBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SelectSesSubDutyDlg message handlers

void SelectSesSubDutyDlg::setupPModel()
{
	LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,"SelectSesSubDutyDlg::setupPModel()");

	m_realPModel = dynamic_cast<SelectSesSubDutyDlgModel*> (m_pModel);
	TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
	//m_realPModel->setProcesses(m_processes);

	
}

std::string SelectSesSubDutyDlg::getMyPModelName()
{
	LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,"PMODEL_SELECT_SE_SUB_DUTY_DLG");

	return PMODEL_SELECT_SE_SUB_DUTY_DLG;
}

void SelectSesSubDutyDlg::initUIControl()
{
	//// Set the label containing the processes to stop.
	//std::string processList;
	//for(std::vector<std::string>::iterator iter = m_realPModel->m_processes.begin(); 
	//	iter != m_realPModel->m_processes.end(); iter++)
	//{
	//	TAUnicodeString uniStr = TAUnicodeString::CreateFromMultitByteString((*iter).c_str());
	//	m_processList.AddString(uniStr.c_str());
	//}
}

void SelectSesSubDutyDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL SelectSesSubDutyDlg::OnInitDialog() 
{
	AbstractDialog::OnInitDialog();

	LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,"SelectSesSubDutyDlg.OnInitDialog()");

	//m_dataCache = DutyDataStore::getDataCache();
	//m_sessionCache = DutyDataStore::getSessionCache();

	SessionIdList sessionIds(m_realPModel->getRegionDutyMatrix().getRowIds());
	
	// TODO: Add extra initialization here
	m_selectSessionList.SetExtendedStyle(m_selectSessionList.GetExtendedStyle()| LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	m_selectSessionList.InsertColumn(SES_OPERATOR_COLUMN,SES_OPERATOR_COLUMN_HEADING,LVCFMT_LEFT,SES_OPERATOR_COLUMN_WIDTH);
	m_selectSessionList.InsertColumn(SES_PROFILE_COLUMN,SES_PROFILE_COLUMN_HEADING,LVCFMT_LEFT,SES_PROFILE_COLUMN_WIDTH);
	m_selectSessionList.InsertColumn(SES_WORKSTATION_COLUMN,SES_WORKSTATION_COLUMN_HEADING,LVCFMT_LEFT,SES_WORKSTATION_COLUMN_WIDTH);
	
	m_selectSesBtn.EnableWindow(FALSE);
	
	SessionInfoEx sessionListInfo;
	try
	{
		for (SessionIdList::const_iterator sessionIt = sessionIds.begin(); sessionIt != sessionIds.end(); sessionIt++)
		{
			try
			{
				sessionListInfo = m_realPModel->getSessionInfo(*sessionIt);
				
				// No need to display current session for select
				if(sessionListInfo.SessionId == m_realPModel->getCurrentSessionId())
				{
					continue;
				}
				
				std::string operatorName("");
				std::string profileName("");
				std::string locationName("");
				std::string workstationName("");

				bool isGet =m_realPModel->getSessionInfoName(sessionListInfo,operatorName,profileName,locationName,workstationName);
				if (false == isGet)
				{
					continue;
				}
				else
				{
				}
				
				//if ( sessionListInfo.OperatorId.empty() 
				//	|| sessionListInfo.ProfileId.empty() )
				//{
				//	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, "[CL-21882] OperatorId is empty, or ProfileId is empty.");
				//	continue;
				//}
				//
				//operatorName = m_realPModel->getOperatorDescription(sessionListInfo.OperatorId[0]);
				//profileName = m_realPModel->getProfileName(sessionListInfo.ProfileId[0]);
				//if ( (sessionListInfo.OperatorId.size() > 1) && (sessionListInfo.ProfileId.size() > 1) )
				//{
				//	operatorName += "/";
				//	operatorName += m_realPModel->getOperatorDescription(sessionListInfo.OperatorId[1]);

				//	profileName += "/";
				//	profileName += m_realPModel->getProfileName(sessionListInfo.ProfileId[1]);
				//}
				//workstationName = m_realPModel->getWorkstationName(sessionListInfo.WorkstationId);
				//
				CString onCStr(operatorName.c_str());
				CString pnCStr(profileName.c_str());
				CString wnCStr(workstationName.c_str());
				CString nullCStr("");
				int item(m_selectSessionList.InsertItem(m_selectSessionList.GetItemCount(), nullCStr));

                m_selectSessionList.SetItemText(item, SES_OPERATOR_COLUMN, onCStr);
                m_selectSessionList.SetItemText(item, SES_PROFILE_COLUMN, pnCStr);
                m_selectSessionList.SetItemText(item, SES_WORKSTATION_COLUMN, wnCStr);
                //m_selectSessionList.SetItemData(item, sessionListInfo.OperatorId[0]);
				m_selectSessionList.SetItemData(item, sessionListInfo.sessionKey);
				
			}
			catch(...)
			{
                LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, "[CL-21882] Error while get session list.");
			}
		}
		if (m_selectSessionList.GetItemCount() == 0)
        {
            m_selectSessionList.EnableWindow(FALSE);
            CString nullCStr("");

            int item(m_selectSessionList.InsertItem(m_selectSessionList.GetItemCount(),nullCStr));
			
            m_selectSessionList.SetItemText(item, SES_PROFILE_COLUMN, TALoadString( STRING_ID_DUTY_MANAGER_0003 ).c_str());
        }
		else
		{
			AdjustColumnWidth();
		}
		
	}
	catch(...)
	{
        EndDialog(IDABORT);
        throw;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void SelectSesSubDutyDlg::OnItemchangedSelectSessionList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ((pNMListView->uNewState & LVIS_SELECTED) == LVIS_SELECTED ||
        (pNMListView->uOldState & LVIS_SELECTED) == LVIS_SELECTED)
    {
        m_selectSesBtn.EnableWindow(m_selectSessionList.GetSelectedCount() > 0);
    }


	*pResult = 0;
}

void SelectSesSubDutyDlg::OnSelectBtn() 
{
	LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,
		"begin SelectSesSubDutyDlg::OnSelSesBtn");
	// TODO: Add your control notification handler code here
	TA_ASSERT(m_selectSessionList.GetSelectedCount() > 0, "No sessions selected.");
	
	try
	{
		int item(LV_ERR);
		//	DWORD dwItem = 0;
		SessionInfoEx sessionListInfo;
		if ((item = m_selectSessionList.GetNextItem(item, LVNI_SELECTED)) != LV_ERR)
		{
			//selectedRegionKeys.push_back(m_transferableRegionList.GetItemData(item));
			DWORD dwItem = m_selectSessionList.GetItemData(item);
			
			sessionListInfo = m_realPModel->getSessionCashe().getSessionInfo(dwItem);
			
			m_selectedSessionInfo = sessionListInfo;
			
			LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,
				"in SelectSesSubDutyDlg::OnSelSesBtn, before PostMessage WM_OTHER_SES_SUB_DUTY_UPDATE");
			
			//GetParent()->PostMessage(WM_OTHER_SES_SUB_DUTY_UPDATE,reinterpret_cast<WPARAM>(&sessionListInfo),0);
			
			if ((!sessionListInfo.OperatorId.empty()) && (!sessionListInfo.ProfileId.empty()))
			{
				LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,
					"in SelectSesSubDutyDlg::OnSelSesBtn, after PostMessage WM_OTHER_SES_SUB_DUTY_UPDATE, the SessionInfoEx in the parameter is: OperatorId=%ld, ProfileId=%ld, LocationId=%ld, WorkStationId=%ld",
					sessionListInfo.OperatorId[0],sessionListInfo.ProfileId[0],sessionListInfo.LocationId,sessionListInfo.WorkstationId);
			}
			
			EndDialog(IDOK);
		}
	}
	catch (const TA_Base_Core::TransactiveException& ex)
    {
        MessageBox::error(TALoadString(STRING_ID_DUTY_MANAGER_0029).toMultiByteStdString(), ex.what(),"");
    }
    catch (...)
    {
        MessageBox::error(TALoadString(STRING_ID_DUTY_MANAGER_0030).toMultiByteStdString());
	}
	LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,
		"end SelectSesSubDutyDlg::OnSelSesBtn");
}

int SelectSesSubDutyDlg::GetColumnCount()
{
	CHeaderCtrl* pHeaderCtrl = m_selectSessionList.GetHeaderCtrl();
	return(pHeaderCtrl->GetItemCount());
}

void SelectSesSubDutyDlg::AdjustColumnWidth()
{
	int nColumnCount = GetColumnCount();
	m_selectSessionList.SetColumnWidth(nColumnCount-1, LVSCW_AUTOSIZE_USEHEADER);
	
	m_selectSessionList.Invalidate();
}

TA_Base_Bus::SessionInfoEx SelectSesSubDutyDlg::getSelectedSessionInfo()
{
	/*
	try
	{
		return m_selectedSessionInfo;
	}
	catch (const TA_Base_Core::TransactiveException& ex)
    {
        MessageBox::error("Error while get Selected SessionInfo.", ex);
    }
    catch (...)
    {
        MessageBox::error("Error while get Selected SessionInfo.");
	}
	*/
	return m_selectedSessionInfo;
}