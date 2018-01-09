/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/RegionDutyTransferDlg.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This dialog enables the operator to select from a list of transferable regions
  * and take duties for these regions.
  *
  */

#include "StdAfx.h"
#include "app/security/DutyManager/src/DutyManager.h"
#include "app/security/DutyManager/pmodel/src/RegionDutyTransferDlgModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
    const int REGION_COLUMN(0);
    const int OPERATOR_COLUMN(1);
    const int PROFILE_COLUMN(2);

	CString REGION_COLUMN_HEADING_REGION_DLG	= TALoadString( STRING_ID_DUTY_MANAGER_0014 ).c_str();

    //CString REGION_COLUMN_HEADING("Region");
    //const CString OPERATOR_COLUMN_HEADING("Operator");
    //const CString PROFILE_COLUMN_HEADING("Profile");
	//CString REGION_COLUMN_HEADING	= TALoadString( STRING_ID_DUTY_MANAGER_0014 ).c_str();
	//CString OPERATOR_COLUMN_HEADING = TALoadString( STRING_ID_DUTY_MANAGER_0016 ).c_str();
	//CString PROFILE_COLUMN_HEADING	= TALoadString( STRING_ID_DUTY_MANAGER_0017 ).c_str();

    //const int REGION_COLUMN_WIDTH(120);
    //const int OPERATOR_COLUMN_WIDTH(180);
	const int REGION_COLUMN_WIDTH(70);
    const int OPERATOR_COLUMN_WIDTH(250);
    const int PROFILE_COLUMN_WIDTH(120);

    //struct TransferableRegionComparator
    //{
    //    bool operator()(const TA_Base_Bus::DutyAgentTypeCorbaDef::TransferableRegion& tr1, 
    //                    const TA_Base_Bus::DutyAgentTypeCorbaDef::TransferableRegion& tr2) const
    //    {
    //        return tr1.regionKey < tr2.regionKey;
    //    }
    //};
}

using TA_Base_Bus::DutyAgentAccess;
using TA_Base_Bus::DataCache;
using TA_Base_Bus::SessionCache;
using TA_Base_Bus::RegionKeyList;
using TA_Base_Bus::TransferableRegionList;

using namespace TA_Base_App;
using namespace TA_Base_Bus;

/////////////////////////////////////////////////////////////////////////////
// RegionDutyTransferDlg dialog

RegionDutyTransferDlg::RegionDutyTransferDlg(TA_Base_Bus::ITransActiveWinApp * app,  CWnd* parent)
	: TA_Base_Bus::AbstractDialog(app, RegionDutyTransferDlg::IDD, parent)
{
	//{{AFX_DATA_INIT(RegionDutyTransferDlg)
	//}}AFX_DATA_INIT
}


RegionDutyTransferDlg::~RegionDutyTransferDlg()
{
	//DutyDataStore::cleanUp();
}


void RegionDutyTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RegionDutyTransferDlg)
	DDX_Control(pDX, IDC_TAKE_DUTY_BTN, m_takeDutyBtn);
	DDX_Control(pDX, IDC_TRANSFERABLE_RGN_LIST, m_transferableRegionList);
	//}}AFX_DATA_MAP
}




/////////////////////////////////////////////////////////////////////////////
// RegionDutyTransferDlg message handlers

BEGIN_MESSAGE_MAP(RegionDutyTransferDlg, CDialog)
	//{{AFX_MSG_MAP(RegionDutyTransferDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TRANSFERABLE_RGN_LIST, OnRegionListItemChanged)
	ON_BN_CLICKED(IDC_TAKE_DUTY_BTN, OnTakeDutyBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void RegionDutyTransferDlg::setupPModel()
{
	m_realPModel = dynamic_cast<RegionDutyTransferDlgModel*> (m_pModel);
	TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
	//m_realPModel->setProcesses(m_processes);
}

std::string RegionDutyTransferDlg::getMyPModelName()
{
	return PMODEL_REGION_DUTY_TRANSFER_DLG;
}

void RegionDutyTransferDlg::initUIControl()
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

BOOL RegionDutyTransferDlg::OnInitDialog() 
{
	AbstractDialog::OnInitDialog();
	
    if ( m_realPModel->isUseLocationLabel() ) 
    {
        SetWindowText(TALoadString( STRING_ID_DUTY_MANAGER_0012 ).c_str());
        REGION_COLUMN_HEADING_REGION_DLG = TALoadString( STRING_ID_DUTY_MANAGER_0012 ).c_str();
    }

    m_transferableRegionList.SetExtendedStyle(m_transferableRegionList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
    m_transferableRegionList.InsertColumn(REGION_COLUMN, REGION_COLUMN_HEADING_REGION_DLG, LVCFMT_LEFT, REGION_COLUMN_WIDTH);
    m_transferableRegionList.InsertColumn(OPERATOR_COLUMN, OPERATOR_COLUMN_HEADING, LVCFMT_LEFT, OPERATOR_COLUMN_WIDTH);
    m_transferableRegionList.InsertColumn(PROFILE_COLUMN, PROFILE_COLUMN_HEADING, LVCFMT_LEFT, PROFILE_COLUMN_WIDTH);

    m_takeDutyBtn.EnableWindow(FALSE);

    try
    {
        // Retrieve from the Duty Agent the list of transferable regions for the current session.
        // Sort by region key so that they will appear on the dialog in the order configured in the database.
		//SessionInfo sessionInfo= m_realPModel->getSessionInfo(m_realPModel->getSessionCashe().getCurrentSessionId());
  //      TransferableRegionList transferableRegions(m_realPModel->getTransferableRegions(sessionInfo));
  //      std::sort(transferableRegions.begin(), transferableRegions.end(), TransferableRegionComparator());
		TransferableRegionList transferableRegions;
		m_realPModel->getCurrentSessionTransferableRegionList(transferableRegions);
		
        // Insert into the list control the transferable regions retrieved from the Duty Agent.
        for (TransferableRegionList::const_iterator regionIt = transferableRegions.begin(); regionIt != transferableRegions.end(); regionIt++)
        {
            try
            {
                //std::string regionName(m_realPModel->getRegionDisplayName(regionIt->regionKey));
                //std::string operatorName(""), profileName("");

                //// Unassigned regions have the operator key and profile key set to "invalid". 
                //// In such case operator name and profile name should be left blank.
                //if (regionIt->operatorKey != TA_Base_Bus::DutyAgentTypeCorbaDef::INVALID_KEY)
                //{
                //    operatorName = m_realPModel->getOperatorDescription(regionIt->operatorKey);
                //}

                //if (regionIt->profileKey != TA_Base_Bus::DutyAgentTypeCorbaDef::INVALID_KEY)
                //{
                //    profileName = m_realPModel->getProfileName(regionIt->profileKey);
                //}

				std::string operatorName(""), profileName(""),regionName("");
				m_realPModel->getRegionInfoName(*regionIt,regionName,operatorName,profileName);

                int item(m_transferableRegionList.InsertItem(m_transferableRegionList.GetItemCount(),STRING_NULL));

                m_transferableRegionList.SetItemText(item, REGION_COLUMN, g_string2CString(regionName.c_str()));
                m_transferableRegionList.SetItemText(item, OPERATOR_COLUMN, g_string2CString(operatorName.c_str()));
                m_transferableRegionList.SetItemText(item, PROFILE_COLUMN, g_string2CString(profileName.c_str()));
                m_transferableRegionList.SetItemData(item, regionIt->regionKey);
            }
            catch (...)
            { 
                LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, 
                            "Ignoring invalid transferable region [r: %lu, o: %lu, p: %lu]",
                            regionIt->regionKey, regionIt->operatorKey, regionIt->profileKey);
            }
        }

        // Disable the list if there are no regions that can be transfered to the target operator.
        if (m_transferableRegionList.GetItemCount() == 0)
        {
            m_transferableRegionList.EnableWindow(FALSE);
            
            int item(m_transferableRegionList.InsertItem(m_transferableRegionList.GetItemCount(), STRING_NULL));

            if (m_realPModel->isUseLocationLabel()) 
            {
                m_transferableRegionList.SetItemText(item, OPERATOR_COLUMN, TALoadString(STRING_ID_DUTY_MANAGER_0022).c_str() );
            }
            else
            {
                m_transferableRegionList.SetItemText(item, OPERATOR_COLUMN, TALoadString(STRING_ID_DUTY_MANAGER_0023).c_str() );
            }
        }
		else
		{
			AdjustColumnWidth();//cl-CL-19416--lkm
		}
    }
    catch (...)
    {
        EndDialog(IDABORT);
        throw;
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void RegionDutyTransferDlg::OnRegionListItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);

	if ((pNMListView->uNewState & LVIS_SELECTED) == LVIS_SELECTED ||
        (pNMListView->uOldState & LVIS_SELECTED) == LVIS_SELECTED)
    {
        m_takeDutyBtn.EnableWindow(m_transferableRegionList.GetSelectedCount() > 0);
    }
	
	*pResult = 0;
}


void RegionDutyTransferDlg::OnTakeDutyBtn() 
{
	TA_ASSERT(m_transferableRegionList.GetSelectedCount() > 0, "No regions selected for transfer");

    RegionKeyList selectedRegionKeys;
    selectedRegionKeys.reserve(m_transferableRegionList.GetSelectedCount());

    // Get the region keys corresponding to the selected list items.
    // There may be uncontrolled subsystems associated with these keys.
    int item(LV_ERR);
    while ((item = m_transferableRegionList.GetNextItem(item, LVNI_SELECTED)) != LV_ERR)
    {
        selectedRegionKeys.push_back(m_transferableRegionList.GetItemData(item));
    }

    try
    {
        CWaitCursor waitCursor;

        // Effect region duty transfer. If this succeeds close the dialog and return to the main
        // Region Manager window.
		TA_Base_Bus::SessionInfo sessionInfo = m_realPModel->getSessionCashe().getSessionInfo(m_realPModel->getCurrentSessionId());
        m_realPModel->transferRegionDuty(sessionInfo, selectedRegionKeys);

	    EndDialog(IDOK);
    }
    catch (const TA_Base_Bus::DutyMonitorFrameworkException& ex)
    {
        if (m_realPModel->isUseLocationLabel()) 
        {
            MessageBox::error(TALoadString(STRING_ID_DUTY_MANAGER_0024).toMultiByteStdString(), ex.what(),"");
        }
        else
        {
            MessageBox::error(TALoadString(STRING_ID_DUTY_MANAGER_0025).toMultiByteStdString(), ex.what(),"");
        }
    }	
}


///////////////////////////////////////////////////////////cl-CL-19416--lkm
void RegionDutyTransferDlg::AdjustColumnWidth()
{
	int nColumnCount = GetColumnCount();
	m_transferableRegionList.SetColumnWidth(nColumnCount-1, LVSCW_AUTOSIZE_USEHEADER);

	/*
	for(int i = 0; i < nColumnCount; i++)
	{
		m_transferableRegionList.SetRedraw(FALSE);
		m_transferableRegionList.SetColumnWidth(i, LVSCW_AUTOSIZE);
		int nColumnWidth = m_transferableRegionList.GetColumnWidth(i);
		m_transferableRegionList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		int nHeaderWidth = m_transferableRegionList.GetColumnWidth(i);

		m_transferableRegionList.SetRedraw(TRUE);
		int width = std::max(nColumnWidth, nHeaderWidth);
		if (i==0)
			m_transferableRegionList.SetColumnWidth(i, width - 50);
		else if (i==1)
			m_transferableRegionList.SetColumnWidth(i, width + 50);
		else
			m_transferableRegionList.SetColumnWidth(i, width);		
	}
	*/
	m_transferableRegionList.Invalidate();
}

int RegionDutyTransferDlg::GetColumnCount()
{
	CHeaderCtrl* pHeaderCtrl = m_transferableRegionList.GetHeaderCtrl();
	return(pHeaderCtrl->GetItemCount());
}
////////////////////////////////////////////////////////////cl-CL-19416--lkm
