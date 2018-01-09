/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/RegionSelectionDlg.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This dialog enables the operator to select from a list of regions for which
  * they currently hold region duties.
  *
  */

#include "StdAfx.h"
#include "app/security/DutyManager/src/DutyManager.h"
#include "app/security/DutyManager/pmodel/src/RegionSelectionDlgModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
    const int REGION_COLUMN(0);
    //const CString REGION_COLUMN_HEADING("");
    const int REGION_COLUMN_WIDTH(208);
}

using TA_Base_Bus::DataCache;
using TA_Base_Bus::SessionCache;
using TA_Base_Bus::RegionDutyMatrix;
using TA_Base_Bus::RegionDutyMatrixElement;
using TA_Base_Bus::RegionKeyList;

using namespace TA_Base_App;
using namespace TA_Base_Bus;

/////////////////////////////////////////////////////////////////////////////
// RegionSelectionDlg dialog

RegionSelectionDlg::RegionSelectionDlg(TA_Base_Bus::ITransActiveWinApp * app,  CWnd* parent)
	: TA_Base_Bus::AbstractDialog(app, RegionSelectionDlg::IDD, parent)
{
	//{{AFX_DATA_INIT(RegionSelectionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


RegionSelectionDlg::~RegionSelectionDlg()
{
    m_selectedRegionKeys.clear();
	//DutyDataStore::cleanUp();
}

void RegionSelectionDlg::setupPModel()
{
	m_realPModel = dynamic_cast<RegionSelectionDlgModel*> (m_pModel);
	TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
	//m_realPModel->setProcesses(m_processes);
}

std::string RegionSelectionDlg::getMyPModelName()
{
	return PMODEL_REGION_SELECTION_DLG;
}

void RegionSelectionDlg::initUIControl()
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


TA_Base_Bus::RegionKeyList RegionSelectionDlg::getSelectedRegionKeys() const
{
    return m_selectedRegionKeys;
}


void RegionSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RegionSelectionDlg)
	DDX_Control(pDX, IDC_ASSIGNED_RGN_LIST, m_assignedRegionList);
	//}}AFX_DATA_MAP
}




/////////////////////////////////////////////////////////////////////////////
// RegionSelectionDlg message handlers

BEGIN_MESSAGE_MAP(RegionSelectionDlg, CDialog)
	//{{AFX_MSG_MAP(RegionSelectionDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ASSIGNED_RGN_LIST, OnRegionListItemChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL RegionSelectionDlg::OnInitDialog() 
{
	AbstractDialog::OnInitDialog();
	
    if ( m_realPModel->isUseLocationLabel() ) 
    {
		SetWindowText(TALoadString(STRING_ID_DUTY_MANAGER_0026).c_str());
    }

    m_assignedRegionList.SetExtendedStyle(m_assignedRegionList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
    m_assignedRegionList.InsertColumn(REGION_COLUMN, REGION_COLUMN_HEADING_NLL, LVCFMT_LEFT, REGION_COLUMN_WIDTH);

    GetDlgItem(IDOK)->EnableWindow(FALSE);

    try
    {
        // Insert into the list box the regions for which the operator has duty.
        RegionKeyList regionKeys(m_realPModel->getRegionDutyMatrix().getColumnIdsByDuties(m_realPModel->getCurrentSessionId()));

        for (RegionKeyList::const_iterator regionIt = regionKeys.begin(); regionIt != regionKeys.end(); regionIt++)
        {
            try
            {
                std::string regionName(m_realPModel->getDataCache().getRegionDisplayName(*regionIt));

                int item(m_assignedRegionList.InsertItem(m_assignedRegionList.GetItemCount(), g_string2CString(regionName.c_str())));
                m_assignedRegionList.SetItemData(item, *regionIt);
            }
            catch (...)
            { 
                LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, "Ignoring invalid region [r: %lu]", *regionIt);
            }
        }
 
        // Disable the list if there are no regions for which the operator has duty.
        if (m_assignedRegionList.GetItemCount() == 0)
        {
            m_assignedRegionList.EnableWindow(FALSE);

            if (m_realPModel->isUseLocationLabel() ) 
            {
                m_assignedRegionList.InsertItem(0, TALoadString(STRING_ID_DUTY_MANAGER_0027).c_str() );
            }
            else
            {
                m_assignedRegionList.InsertItem(0, TALoadString(STRING_ID_DUTY_MANAGER_0028).c_str() );
            }
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


void RegionSelectionDlg::OnRegionListItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);

	if ((pNMListView->uNewState & LVIS_SELECTED) == LVIS_SELECTED ||
        (pNMListView->uOldState & LVIS_SELECTED) == LVIS_SELECTED)
    {
        GetDlgItem(IDOK)->EnableWindow(m_assignedRegionList.GetSelectedCount() > 0);
    }
	
	*pResult = 0;
}


void RegionSelectionDlg::OnOK() 
{
    m_selectedRegionKeys.reserve(m_assignedRegionList.GetSelectedCount());

    int item(LV_ERR);
    while ((item = m_assignedRegionList.GetNextItem(item, LVNI_SELECTED)) != LV_ERR)
    {
        m_selectedRegionKeys.push_back(m_assignedRegionList.GetItemData(item));
    }

	CDialog::OnOK();
}
