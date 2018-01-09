/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/SubsystemDutyTransferDlg.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This dialog enables the operator to select from a list of transferable subsystems
  * and request duties for these subsystems.
  *
  */

#include "StdAfx.h"
#include "app/security/DutyManager/src/DutyManager.h"
#include "app/security/DutyManager/pmodel/src/SubsystemDutyTransferDlgModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
    const int REGION_COLUMN(0);
    const int SUBSYSTEM_COLUMN(1);
    const int OPERATOR_COLUMN(2);
    const int PROFILE_COLUMN(3);

	CString REGION_COLUMN_HEADING_SUBSYSTEM_DLG	= TALoadString( STRING_ID_DUTY_MANAGER_0014 ).c_str();

    //CString REGION_COLUMN_HEADING("Region");
    //const CString SUBSYSTEM_COLUMN_HEADING("Subsystem");
    //const CString OPERATOR_COLUMN_HEADING("Operator");
    //const CString PROFILE_COLUMN_HEADING("Profile");

	//CString REGION_COLUMN_HEADING	= TALoadString( STRING_ID_DUTY_MANAGER_0014 ).c_str();
	//CString SUBSYSTEM_COLUMN_HEADING = TALoadString( STRING_ID_DUTY_MANAGER_0018 ).c_str();
	//CString OPERATOR_COLUMN_HEADING	= TALoadString( STRING_ID_DUTY_MANAGER_0016 ).c_str();
	//CString PROFILE_COLUMN_HEADING	= TALoadString( STRING_ID_DUTY_MANAGER_0017 ).c_str();

    //const int REGION_COLUMN_WIDTH(120);
    //const int SUBSYSTEM_COLUMN_WIDTH(200);
	const int REGION_COLUMN_WIDTH(70);
    const int SUBSYSTEM_COLUMN_WIDTH(310);
    const int OPERATOR_COLUMN_WIDTH(120);
    const int PROFILE_COLUMN_WIDTH(140);

    const UINT TIMER_AUTO_POPULATE_EVENT(1001);
    const UINT TIMER_AUTO_POPULATE_TIME(50);   // ms


    //struct TransferableSubsystemComparator
    //{
    //    TransferableSubsystemComparator(TA_Base_Bus::DataCache& dataCache) : m_dataCache(dataCache) {}

    //    bool operator()(const TA_Base_Bus::DutyAgentTypeCorbaDef::TransferableSubsystem& ts1, 
    //                    const TA_Base_Bus::DutyAgentTypeCorbaDef::TransferableSubsystem& ts2) const
    //    {
    //        if (ts1.regionKey == ts2.regionKey)
    //        {
    //            try
    //            {
    //                std::string subName1(m_dataCache.getSubsystemName(ts1.subsystemKey));
    //                std::string subName2(m_dataCache.getSubsystemName(ts2.subsystemKey));

    //                return (_stricmp(subName1.c_str(), subName2.c_str()) < 0);
    //            }
    //            catch (...)
    //            {
    //                return false;
    //            }
    //        }

    //        return ts1.regionKey < ts2.regionKey;
    //    }

    //    TA_Base_Bus::DataCache& m_dataCache;
    //};
}

using TA_Base_Bus::DutyAgentAccess;
using TA_Base_Bus::DataCache;
using TA_Base_Bus::SessionCache;
using TA_Base_Bus::RegionDutyMatrix;
using TA_Base_Bus::RegionKeyList;
using TA_Base_Bus::SubsystemKeyList;
using TA_Base_Bus::TransferableSubsystemList;
using TA_Base_Bus::SubsystemTreeMap;

using namespace TA_Base_App;
using namespace TA_Base_Bus;

/////////////////////////////////////////////////////////////////////////////
// SubsystemDutyTransferDlg dialog

SubsystemDutyTransferDlg::SubsystemDutyTransferDlg(TA_Base_Bus::ITransActiveWinApp * app,  CWnd* parent)
	: TA_Base_Bus::AbstractDialog(app, SubsystemDutyTransferDlg::IDD, parent)

{
	//{{AFX_DATA_INIT(SubsystemDutyTransferDlg)
	//}}AFX_DATA_INIT
}


SubsystemDutyTransferDlg::~SubsystemDutyTransferDlg()
{
    m_transferableSubsystems.clear();
	//DutyDataStore::cleanUp();
}

void SubsystemDutyTransferDlg::setupPModel()
{
	m_realPModel = dynamic_cast<SubsystemDutyTransferDlgModel*> (m_pModel);
	TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
	//m_realPModel->setProcesses(m_processes);
}

std::string SubsystemDutyTransferDlg::getMyPModelName()
{
	return PMODEL_SUBSYSTEM_DUTY_TRANSFER_DLG;
}

void SubsystemDutyTransferDlg::initUIControl()
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

//TA_Base_Bus::DataCache& SubsystemDutyTransferDlg::getDataCache()
//{
//	return m_realPModel->getDataCache();
//}
//
//TA_Base_Bus::SessionCache& SubsystemDutyTransferDlg::getSessionCashe()
//{
//	return m_realPModel->getSessionCashe();
//}
//
//TA_Base_Bus::RegionDutyMatrix&		SubsystemDutyTransferDlg::getRegionDutyMatrix()
//{
//	return m_realPModel->getRegionDutyMatrix();
//}
//
//
//TA_Base_Bus::SubsystemDutyMatrix& SubsystemDutyTransferDlg::getSubsystemDutyMatrix()
//{
//	return m_realPModel->getSubsystemDutyMatrix();
//}
//
//TA_Base_Bus::SessionInfoEx  SubsystemDutyTransferDlg::getCurrentSessionInfo()
//{
//	return m_realPModel->getCurrentSessionInfo();
//}
//
//TA_Base_Bus::ProfileKeyList	SubsystemDutyTransferDlg::getPreviousProfileId()
//{
//	return m_realPModel->getPreviousProfileId();
//}
//
//std::string SubsystemDutyTransferDlg::getRegionDisplayName(RegionKey key)
//{
//	return m_realPModel->getRegionDisplayName(key);
//	// m_dataCache.getRegionDisplayName(*regionIt).c_str();
//}
//
//std::string SubsystemDutyTransferDlg::getRegionName(RegionKey key)
//{
//	return m_realPModel->getRegionName(key);
//	// m_dataCache.getRegionName(*regionIt).c_str();
//}

TransferableSubsystemList SubsystemDutyTransferDlg::getTransferableSubsystems(const SessionInfo& session, const RegionKeyList& regionKeys)
{
	return m_realPModel->getTransferableSubsystems(session,  regionKeys);
}


void SubsystemDutyTransferDlg::updateTransferableSubsystems(const TA_Base_Bus::RegionKeyList& regionKeys)
{
    TA_ASSERT(!regionKeys.empty(), "Region key list is empty");

    try
    {
        // Update the target regions display.
        m_regionsEdit.SetWindowText(g_string2CString(GridAssistant::generateRegionString(regionKeys).c_str()));
        m_regionsEdit.UpdateWindow();

        // Remove any previous transferable subsystems from the list.
        m_transferableSubsystemList.DeleteAllItems();
        m_transferableSubsystemList.UpdateWindow();

        {
            CWaitCursor waitCursor;

            // Retrieve from the Duty Agent the list of transferable subsystems for the current session and the selected regions.
            // Sort by region key and subsystem name.

			//TD15346++
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
				"TD15346: Before call to m_dutyAgent.getTransferableSubsystems: region key size:%lu", 
				regionKeys.size());
			std::string temp = "TD15346: session: ";
			temp += m_realPModel->getCurrentSessionId();
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, temp.c_str());
			//++TD15346
			TA_Base_Bus::SessionInfo sessionInfo = m_realPModel->getSessionInfo(m_realPModel->getCurrentSessionId());

            m_transferableSubsystems = getTransferableSubsystems(sessionInfo, regionKeys);
            std::sort(m_transferableSubsystems.begin(), m_transferableSubsystems.end(), TransferableSubsystemComparator( m_realPModel->getDataCache() ));

			{
			//TD15346++
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
				"TD15346: After call to m_dutyAgent.getTransferableSubsystems: region key size:%lu", 
				regionKeys.size());
			std::string temp = "TD15346: session: ";
			temp += m_realPModel->getCurrentSessionId();
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, temp.c_str());
			//++TD15346
			}
        }
    
        // Insert into the list control the transferable subsystems retrieved from the Duty Agent.
        for (TransferableSubsystemList::const_iterator subsystemIt = m_transferableSubsystems.begin(); 
             subsystemIt != m_transferableSubsystems.end(); subsystemIt++)
        {
            try
            {
                std::string regionName(m_realPModel->getRegionDisplayName(subsystemIt->regionKey));
                std::string subsystemName(m_realPModel->getSubsystemName(subsystemIt->subsystemKey));
                std::string operatorName(""), profileName("");

                // Unassigned subsystems have the operator key and profile key set to "invalid". 
                // In such case operator name and profile name should be left blank.
                if (subsystemIt->operatorKey != TA_Base_Bus::DutyAgentTypeCorbaDef::INVALID_KEY)
                {
                    operatorName = m_realPModel->getOperatorDescription(subsystemIt->operatorKey);
                }

                if (subsystemIt->profileKey != TA_Base_Bus::DutyAgentTypeCorbaDef::INVALID_KEY)
                {
                    profileName = m_realPModel->getProfileName(subsystemIt->profileKey);
                }

                int item(m_transferableSubsystemList.InsertItem(m_transferableSubsystemList.GetItemCount(), STRING_NULL));

                m_transferableSubsystemList.SetItemText(item, REGION_COLUMN, g_string2CString(regionName.c_str()));
                m_transferableSubsystemList.SetItemText(item, SUBSYSTEM_COLUMN, g_string2CString(subsystemName.c_str()));
                m_transferableSubsystemList.SetItemText(item, OPERATOR_COLUMN, g_string2CString(operatorName.c_str()));
                m_transferableSubsystemList.SetItemText(item, PROFILE_COLUMN, g_string2CString(profileName.c_str()));
                m_transferableSubsystemList.SetItemData(item, reinterpret_cast<DWORD>(&(*subsystemIt)));
            }
            catch (...)
            {
                LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, 
                            "Ignoring invalid transferable subsystem [r: %lu, s: %lu, o: %lu, p: %lu]",
                            subsystemIt->regionKey, subsystemIt->subsystemKey, subsystemIt->operatorKey, subsystemIt->profileKey);
            }
        }

        // Disable the list if there are no subsystems that can be transfered to the operator.
        if (m_transferableSubsystemList.GetItemCount() == 0)
        {
            m_transferableSubsystemList.EnableWindow(FALSE);
            
            int item(m_transferableSubsystemList.InsertItem(m_transferableSubsystemList.GetItemCount(), STRING_NULL));
            m_transferableSubsystemList.SetItemText(item, SUBSYSTEM_COLUMN, TALoadString(STRING_ID_DUTY_MANAGER_0034).c_str());
        }
        else
        {
			AdjustColumnWidth();//cl-CL-19416--lkm
            m_transferableSubsystemList.EnableWindow(TRUE);
        }
    }
    catch (const TA_Base_Bus::DutyMonitorFrameworkException& ex)
    {
        MessageBox::error(TALoadString(STRING_ID_DUTY_MANAGER_0031).toMultiByteStdString(), ex.what(),"");
    }
    catch (const TA_Base_Core::TransactiveException& ex)
    {
        MessageBox::error(TALoadString(STRING_ID_DUTY_MANAGER_0032).toMultiByteStdString(), ex.what(),"");
    }
    catch (...)
    {
        MessageBox::error(TALoadString(STRING_ID_DUTY_MANAGER_0033).toMultiByteStdString());
    }	
}


void SubsystemDutyTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SubsystemDutyTransferDlg)
	DDX_Control(pDX, IDC_REQUEST_DUTY_BTN, m_requestDutyBtn);
	DDX_Control(pDX, IDC_REQUEST_ALLDUTY_BTN, m_requestAllDutyBtn);
	DDX_Control(pDX, IDC_TRANSFERABLE_SUB_LIST, m_transferableSubsystemList);
	DDX_Control(pDX, IDC_SELECT_RGNS_BTN, m_selectRegionsBtn);
	DDX_Control(pDX, IDC_REGIONS_EDIT, m_regionsEdit);
	DDX_Control(pDX, IDC_REGIONS_BOX, m_regionsBox);
	//}}AFX_DATA_MAP
}




/////////////////////////////////////////////////////////////////////////////
// SubsystemDutyTransferDlg message handlers

BEGIN_MESSAGE_MAP(SubsystemDutyTransferDlg, CDialog)
	//{{AFX_MSG_MAP(SubsystemDutyTransferDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TRANSFERABLE_SUB_LIST, OnSubsystemListItemChanged)
	ON_BN_CLICKED(IDC_SELECT_RGNS_BTN, OnSelectRgnsBtn)
	ON_BN_CLICKED(IDC_REQUEST_DUTY_BTN, OnRequestDutyBtn)
	ON_BN_CLICKED(IDC_REQUEST_ALLDUTY_BTN, OnRequestAlldutyBtn)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL SubsystemDutyTransferDlg::OnInitDialog() 
{
	AbstractDialog::OnInitDialog();
	
    if ( m_realPModel->isUseLocationLabel() ) 
    {
		// "Location"
        m_regionsBox.SetWindowText( TALoadString( STRING_ID_DUTY_MANAGER_0019 ).c_str() );
        REGION_COLUMN_HEADING_SUBSYSTEM_DLG = TALoadString( STRING_ID_DUTY_MANAGER_0019 ).c_str();
    }

    m_transferableSubsystemList.SetExtendedStyle(m_transferableSubsystemList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
    m_transferableSubsystemList.InsertColumn(REGION_COLUMN, REGION_COLUMN_HEADING_SUBSYSTEM_DLG, LVCFMT_LEFT, REGION_COLUMN_WIDTH);
    m_transferableSubsystemList.InsertColumn(SUBSYSTEM_COLUMN, SUBSYSTEM_COLUMN_HEADING, LVCFMT_LEFT, SUBSYSTEM_COLUMN_WIDTH);
    m_transferableSubsystemList.InsertColumn(OPERATOR_COLUMN, OPERATOR_COLUMN_HEADING, LVCFMT_LEFT, OPERATOR_COLUMN_WIDTH);
    m_transferableSubsystemList.InsertColumn(PROFILE_COLUMN, PROFILE_COLUMN_HEADING, LVCFMT_LEFT, PROFILE_COLUMN_WIDTH);

    m_transferableSubsystemList.EnableWindow(FALSE);
    m_requestDutyBtn.EnableWindow(FALSE);
	m_requestAllDutyBtn.EnableWindow(FALSE);

    // When the dialog comes up automatically initialise controls where appropriate.
    SetTimer(TIMER_AUTO_POPULATE_EVENT, TIMER_AUTO_POPULATE_TIME, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void SubsystemDutyTransferDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_AUTO_POPULATE_EVENT)
    {
        // Wait for the dialog to show before attempting to populate it.
        if (IsWindowVisible())
        {
            KillTimer(TIMER_AUTO_POPULATE_EVENT);

            // If the operator currently has duty for exactly one region, then pre-select that region
            // for him and auto-populate the transferable subsystem list.
            RegionKeyList regionKeys(m_realPModel->getRegionDutyMatrix().getColumnIdsByDuties(m_realPModel->getCurrentSessionId()));
            if (regionKeys.size() == 1)
            {
                updateTransferableSubsystems(regionKeys);
            }
        }

        return;
    }

	CDialog::OnTimer(nIDEvent);
}


void SubsystemDutyTransferDlg::OnSubsystemListItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);

	if ((pNMListView->uNewState & LVIS_SELECTED) == LVIS_SELECTED ||
        (pNMListView->uOldState & LVIS_SELECTED) == LVIS_SELECTED)
    {
        m_requestDutyBtn.EnableWindow(m_transferableSubsystemList.GetSelectedCount() > 0);
    }

	// cf++ TD15111
	m_requestAllDutyBtn.EnableWindow(m_transferableSubsystemList.GetItemCount() > 0);
	// ++cf

	*pResult = 0;
}


void SubsystemDutyTransferDlg::OnSelectRgnsBtn() 
{
    // Get the operator to select the regions for subsystem duty transfer.
	RegionSelectionDlg rgnSelectDlg(m_pWinApp);

    if (rgnSelectDlg.DoModal() == IDOK)
    {
        updateTransferableSubsystems(rgnSelectDlg.getSelectedRegionKeys());
    }
}


void SubsystemDutyTransferDlg::OnRequestDutyBtn() 
{

	TA_ASSERT(m_transferableSubsystemList.GetSelectedCount() > 0, "No subsystems selected for transfer");

    SubsystemTreeMap selectedSubsystems;

    // Get the transferable subsystems corresponding to the selected list items.
    // Use them to build a subsystem tree required for subsystem duty transfer.
    int item(LV_ERR);
    while ((item = m_transferableSubsystemList.GetNextItem(item, LVNI_SELECTED)) != LV_ERR)
    {
        const TA_Base_Bus::DutyAgentTypeCorbaDef::TransferableSubsystem* transferableSubsystem =
            reinterpret_cast<const TA_Base_Bus::DutyAgentTypeCorbaDef::TransferableSubsystem*>(m_transferableSubsystemList.GetItemData(item));

        SubsystemTreeMap::iterator subsystemIt(selectedSubsystems.find(transferableSubsystem->regionKey));
       
        if (subsystemIt == selectedSubsystems.end())
        {
            selectedSubsystems.insert(
                std::make_pair(transferableSubsystem->regionKey, SubsystemKeyList(1, transferableSubsystem->subsystemKey)));
        }
        else
        {
            subsystemIt->second.push_back(transferableSubsystem->subsystemKey);
        }
    }

    try
    {
        CWaitCursor waitCursor;

        // Request subsystem duty transfer. If this succeeds close the dialog and return 
        // to the main Rights Manager window.
		TA_Base_Bus::SessionInfo sessionInfo = m_realPModel->getSessionInfo(m_realPModel->getCurrentSessionId());
        m_realPModel->transferSubsystemDuty(sessionInfo, selectedSubsystems);
		EndDialog(IDOK);
    }
    catch (const TA_Base_Bus::DutyMonitorFrameworkException& ex)
    {
		std::string exStr(ex.what());
		size_t found;
		found=exStr.find("TRANSIENT_CallTimedout");
		if(found == std::string::npos)
		{
			MessageBox::error(TALoadString( STRING_ID_DUTY_MANAGER_0021 ).toMultiByteStdString(), ex.what(),"");
		}
		else
		{
			MessageBox::warning( TALoadString( STRING_ID_DUTY_MANAGER_0020 ).toMultiByteStdString() );
		}
    }
}

// cf++ TD15111
void SubsystemDutyTransferDlg::OnRequestAlldutyBtn() 
{
	// Select all of the subsystem items
    int item(LV_ERR);
    while ((item = m_transferableSubsystemList.GetNextItem(item, LVNI_ALL)) != LV_ERR)
    {
		m_transferableSubsystemList.SetItemState(item, LVIS_SELECTED, LVIS_SELECTED);
	}

	// Make sure the focus is set to the list view control.
	m_transferableSubsystemList.SetFocus();	
}
// ++cf


///////////////////////////////////////////////////////////cl-CL-19416--lkm
void SubsystemDutyTransferDlg::AdjustColumnWidth()
{
	int nColumnCount = GetColumnCount();
	m_transferableSubsystemList.SetColumnWidth(nColumnCount-1, LVSCW_AUTOSIZE_USEHEADER);

	/*
	for(int i = 0; i < nColumnCount; i++)
	{
		
		m_transferableSubsystemList.SetRedraw(FALSE);
		m_transferableSubsystemList.SetColumnWidth(i, LVSCW_AUTOSIZE);
		int nColumnWidth = m_transferableSubsystemList.GetColumnWidth(i);
		m_transferableSubsystemList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		int nHeaderWidth = m_transferableSubsystemList.GetColumnWidth(i);

		m_transferableSubsystemList.SetRedraw(TRUE);
		int width = std::max(nColumnWidth, nHeaderWidth);
		if (i==0)
			m_transferableSubsystemList.SetColumnWidth(i, width - 50);
		else if (i==1)
			m_transferableSubsystemList.SetColumnWidth(i, width + 50);
		else
			m_transferableSubsystemList.SetColumnWidth(i, width);
		
	}
	*/
	m_transferableSubsystemList.Invalidate();
}

int SubsystemDutyTransferDlg::GetColumnCount()
{
	CHeaderCtrl* pHeaderCtrl = m_transferableSubsystemList.GetHeaderCtrl();
	return(pHeaderCtrl->GetItemCount());
}
////////////////////////////////////////////////////////////cl-CL-19416--lkm
