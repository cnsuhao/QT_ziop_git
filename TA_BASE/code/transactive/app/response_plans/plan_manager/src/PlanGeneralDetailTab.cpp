//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanGeneralDetailTab.cpp $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// <description>

#include "app/response_plans/plan_manager/src/PlanGeneralDetailTab.h"

#include "core/utilities/src/TAAssert.h"
#include "core/exceptions/src/TransactiveException.h"

//#include "app/response_plans/plan_manager/pmodel/src/PlanBrowserDoc.h"
#include "app/response_plans/plan_manager/pmodel/src/PlanNode.h"
//#include "app/response_plans/plan_manager/src/MessageBox.h"

#include "app/response_plans/plan_manager/src/PlanTreeFrame.h"
//
//#include "app/response_plans/plan_manager/pmodel/src/PlanInstanceFilter.h"
#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;
using namespace TA_Base_Bus;
using namespace TA_Base_Core;

//as specified in GDN
//note that DB allows for an extra 5 characters
//static const UINT NAME_TXT_LIMIT    = 80;

/////////////////////////////////////////////////////////////////////////////
// PlanGeneralDetailTab dialog


PlanGeneralDetailTab::PlanGeneralDetailTab(QWidget* parent)
: PlanDetailTab(parent)
, m_isLoadingControls(false)
{
    FUNCTION_ENTRY( "PlanGeneralDetailTab" );

	setupUi(this);
	//splitter->setContentsMargins();

	// Replace CltTableWidget from ui with Subclass
	delete activeInstancesList;
	activeInstancesList = &m_planInstanceListCtrl;
	verticalLayout->addWidget(activeInstancesList);

	// Enable the plan instance filter on the active plan instance list control.
	PlanInstanceFilter *planInstanceFilter = getPlanInstanceFilter();
	m_planInstanceListCtrl.setPlanInstanceFilter(reinterpret_cast<IPlanInstanceFilter*>(getPlanInstanceFilter()));

    FUNCTION_EXIT;
}


PlanGeneralDetailTab::~PlanGeneralDetailTab()
{
    FUNCTION_ENTRY( "~PlanGeneralDetailTab" );
    FUNCTION_EXIT;
}

void PlanGeneralDetailTab::updatePlanView()
{
	FUNCTION_ENTRY("updatePlanView");	
	nameEdit->setText(tr(getTreeModel()->getPlanDetail().name));
	//planModifiedByEdit->setText(getTreeModel()->getPlanDetail().name);
	//nameEdit->setText(getTreeModel()->getPlanDetail().name);
	//nameEdit->setText(getTreeModel()->getPlanDetail().name);
	//nameEdit->setText(getTreeModel()->getPlanDetail().name);
	FUNCTION_EXIT;
}

CtlCheckBox* PlanGeneralDetailTab::getAllOwnersCheck()
{
	FUNCTION_ENTRY("getAllOwnersCheck");
	FUNCTION_EXIT;
	return allOwnersCheck;
}

CtlCheckBox* PlanGeneralDetailTab::getAllLocationsCheck()
{
	FUNCTION_ENTRY("getAllLocationsCheck");
	FUNCTION_EXIT;
	return allLocationsCheck;
}

//BEGIN_MESSAGE_MAP(PlanGeneralDetailTab, CPlanDetailPage)
//    // {{AFX_MSG_MAP(PlanGeneralDetailTab)
//    ON_WM_SIZE()
//    ON_NOTIFY(LVN_ITEMCHANGED, IDC_ACTIVE_INSTANCES_LIST, OnPlanInstanceListItemChanged)
//    ON_EN_CHANGE(IDC_NAME_EDIT, OnChangeNameEdit)
//    ON_EN_CHANGE(IDC_DESCRIPTION_EDIT, OnChangeDescriptionEdit)
//    // }}AFX_MSG_MAP
//    ON_MESSAGE(WM_REFRESH_PLAN_GENERAL_DETAIL, OnRefreshPlanGeneralDetail)
//END_MESSAGE_MAP()
//
//
///////////////////////////////////////////////////////////////////////////////
//// PlanGeneralDetailTab message handlers
//
//BOOL PlanGeneralDetailTab::PreTranslateMessage(MSG* pMsg)
//{
//    FUNCTION_ENTRY( "PreTranslateMessage" );
//
//    // TD#1070/2616
//    // This is relevant only to plan editing. It enables Del/Ctrl+X/Ctrl+C/Ctrl+V key presses
//    // to be processed within the General tab. These keys are normally used as accelerators
//    // for the ID_STEP_DELETE/ID_STEP_CUT/ID_STEP_COPY/ID_STEP_PASTE commands, which the General
//    // tab disables.
//    if (pMsg->message == WM_KEYDOWN)
//    {
//        WPARAM key = pMsg->wParam;
//        bool isCtrlPressed = (GetAsyncKeyState(VK_CONTROL) < 0);
//
//        if ((key == VK_DELETE) || (isCtrlPressed && (key == 'C' || key == 'X' || key == 'V')))
//        {
//            ::TranslateMessage(pMsg);
//            ::DispatchMessage(pMsg);
//
//            FUNCTION_EXIT;
//            return TRUE;
//        }
//    }
//
//    // TD#1739/1740/1741: Route the message via CPlanDetailPage to ensure the parent
//    // frame gets a go at it.
//    FUNCTION_EXIT;
//    return CPlanDetailPage::PreTranslateMessage(pMsg);
//}
//
//
//BOOL PlanGeneralDetailTab::OnInitDialog()
//{
//    FUNCTION_ENTRY( "OnInitDialog" );
//
//    CPlanDetailPage::OnInitDialog();
//
//    // Enable the plan instance filter on the active plan instance list control.
//    m_planInstanceListCtrl.SetPlanInstanceFilter(GetPlanInstanceFilter());
//
//    // Separate step and parameter lists with a movable splitter bar
//    m_splitterBar.BindWithControl(this, IDC_SPLITTER_BAR);
//    m_splitterBar.SetMinHeight(235, 0);
//    m_splitterBar.SetSplitRatio(0.5);
//    m_splitterBar.AttachAbove(m_detailsGroupBox.GetDlgCtrlID(), 10);
//    m_splitterBar.AttachAbove(m_descriptionEdit.GetDlgCtrlID(), 25);
//    m_splitterBar.AttachBelow(m_activeInstancesGroupBox.GetDlgCtrlID(), 6);
//    m_splitterBar.AttachBelow(m_allOwnersCheck.GetDlgCtrlID(), 28, TRUE);
//    m_splitterBar.AttachBelow(m_allLocationsCheck.GetDlgCtrlID(), 54, TRUE);
//    m_splitterBar.AttachBelow(m_planInstanceListCtrl.GetDlgCtrlID(), 82);
//    m_splitterBar.RecalcLayout();
//
//     // as per TD 11659
//     m_nameEdit.SetLimitText (NAME_TXT_LIMIT);
//
//	 //CL-14921++ Item1
//	 const unsigned int MAX_DESCRIPTION_BYTES = 950;
//	 m_descriptionEdit.SetLimitText(MAX_DESCRIPTION_BYTES);
//	 //++CL-14921
//
//    return TRUE;  // return TRUE unless you set the focus to a control
//                  // EXCEPTION: OCX Property Pages should return FALSE
//}
//
//
//BOOL PlanGeneralDetailTab::OnSetActive()
//{
//    FUNCTION_ENTRY( "OnSetActive" );
//
//    if (CPlanDetailPage::OnSetActive())
//    {
//        PlanInstanceSharedPtr currentInstance = GetDocument()->GetCurrentInstance();
//        m_planInstanceListCtrl.SetSelectedInstance(currentInstance);
//
//        FUNCTION_EXIT;
//        return TRUE;
//    }
//
//    FUNCTION_EXIT;
//    return FALSE;
//}
//
//
//void PlanGeneralDetailTab::OnSize(UINT nType, int cx, int cy)
//{
//    FUNCTION_ENTRY( "OnSize" );
//
//    CPlanDetailPage::OnSize(nType, cx, cy);
//
//    if (::IsWindow(m_detailsGroupBox.GetSafeHwnd()))
//    {
//        PlanNode *planNode = GetPlanNode();
//
//        CRect detailsBoxRect;
//        m_detailsGroupBox.GetWindowRect(detailsBoxRect);
//        ScreenToClient(detailsBoxRect);
//        detailsBoxRect.right = cx - 8;
//        if (!planNode->isApproved())
//        {
//            detailsBoxRect.bottom = cy - 8;
//        }
//        m_detailsGroupBox.MoveWindow(detailsBoxRect);
//
//        CRect nameEditRect;
//        m_nameEdit.GetWindowRect(nameEditRect);
//        ScreenToClient(nameEditRect);
//        nameEditRect.right = detailsBoxRect.right - 13;
//        m_nameEdit.MoveWindow(nameEditRect);
//
//        CRect descriptionEditRect;
//        m_descriptionEdit.GetWindowRect(descriptionEditRect);
//        ScreenToClient(descriptionEditRect);
//        descriptionEditRect.right = detailsBoxRect.right - 13;
//        descriptionEditRect.bottom = detailsBoxRect.bottom - 13;
//        m_descriptionEdit.MoveWindow(descriptionEditRect);
//
//        if (planNode->isApproved())
//        {
//            CRect activeInstancesBoxRect;
//            m_activeInstancesGroupBox.GetWindowRect(activeInstancesBoxRect);
//            ScreenToClient(activeInstancesBoxRect);
//            activeInstancesBoxRect.right = cx - 8;
//            activeInstancesBoxRect.bottom = cy - 8;
//            m_activeInstancesGroupBox.MoveWindow(activeInstancesBoxRect);
//
//            CRect activeInstancesListRect;
//            m_planInstanceListCtrl.GetWindowRect(activeInstancesListRect);
//            ScreenToClient(activeInstancesListRect);
//            activeInstancesListRect.right = activeInstancesBoxRect.right - 13;
//            activeInstancesListRect.bottom = activeInstancesBoxRect.bottom - 13;
//            m_planInstanceListCtrl.MoveWindow(activeInstancesListRect);
//
//            m_splitterBar.RecalcLayout();
//        }
//    }
//
//    FUNCTION_EXIT;
//}
//
//
//void PlanGeneralDetailTab::OnPlanInstanceListItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
//{
//    FUNCTION_ENTRY( "OnPlanInstanceListItemChanged" );
//
//    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//
//    if ((pNMListView->uNewState & LVIS_SELECTED) == LVIS_SELECTED &&
//        (pNMListView->uOldState & LVIS_SELECTED) != LVIS_SELECTED)
//    {
//        PlanInstanceSharedPtr planInstance = m_planInstanceListCtrl.GetSelectedInstance();
//        GetDocument()->SetCurrentInstance(planInstance);
//    }
//    else if ((pNMListView->uNewState & LVIS_SELECTED) != LVIS_SELECTED &&
//             (pNMListView->uOldState & LVIS_SELECTED) == LVIS_SELECTED)
//    {
//        GetDocument()->SetCurrentInstance(PlanInstanceSharedPtr());
//    }
//
//    *pResult = 0;
//
//    FUNCTION_EXIT;
//}
//
//
//LRESULT PlanGeneralDetailTab::OnRefreshPlanGeneralDetail(WPARAM wParam, LPARAM lParam)
//{
//    FUNCTION_ENTRY( "OnRefreshPlanGeneralDetail" );
//
//    RefreshControlContents();
//
//    FUNCTION_EXIT;
//    return 0;
//}
//
//
//void PlanGeneralDetailTab::OnChangeNameEdit()
//{
//    FUNCTION_ENTRY( "OnChangeNameEdit" );
//
//    PlanNode* planNode = GetPlanNode();
//
//    if (planNode->isCopiedPlan() && !m_isLoadingControls)
//    {
//        CString text;
//        m_nameEdit.GetWindowText(text);
//
//        planNode->setPlanName((LPCTSTR)text);
//        GetDocument()->SetModifiedFlag();
//    }
//
//    FUNCTION_EXIT;
//}
//
//
//void PlanGeneralDetailTab::OnChangeDescriptionEdit()
//{
//    FUNCTION_ENTRY( "OnChangeDescriptionEdit" );
//
//    PlanNode* planNode = GetPlanNode();
//
//    if (planNode->isCopiedPlan() && !m_isLoadingControls)
//    {
//        CString text;
//        m_descriptionEdit.GetWindowText(text);
//
//        planNode->setPlanDescription((LPCTSTR)text);
//        GetDocument()->SetModifiedFlag();
//    }
//
//    FUNCTION_EXIT;
//}


void PlanGeneralDetailTab::showRelevantControls()
{
    FUNCTION_ENTRY( "ShowRelevantControls" );

	if (getPlanNode()->isApproved())
	{
		splitter->show();
		activeInstancesGrpbox->show();
		allOwnersCheck->show();
		allLocationsCheck->show();
		activeInstancesList->show();
	}
	else
	{
		splitter->hide();
		activeInstancesGrpbox->hide();
		allOwnersCheck->hide();
		allLocationsCheck->hide();
		activeInstancesList->hide();
	}

	nameEdit->setReadOnly(!getTreeModel()->editingEnabled());
	descriptionEdit->setReadOnly(!getTreeModel()->editingEnabled());
	m_planInstanceListCtrl.setDisabled(!getTreeModel()->controlEnabled());

    FUNCTION_EXIT;
}


void PlanGeneralDetailTab::refreshControlContents()
{
    FUNCTION_ENTRY( "refreshControlContents" );

    m_isLoadingControls = true;

    try
    {
		QApplication::setOverrideCursor(Qt::WaitCursor);

        PlanNode *planNode = getPlanNode();
        PlanInstanceFilter *planInstanceFilter = getPlanInstanceFilter();
		///
        //planNode->updatePlanView(*this, *planInstanceFilter);

		PlanNode* planNode = dynamic_cast<PlanNode*>(getTreeModel()->GetCurrentNode());
		TA_ASSERT(planNode != NULL, "Plan node pointer is null");

		if (planNode)
		{
			nameEdit->setText(tr(planNode->getPlanDetail().name));
			descriptionEdit->setText(tr(planNode->getPlanDetail().name));
			approvalStatusEdit->setText(tr(planNode->getPlanDetail().name));
			planModifiedByEdit->setText(tr(planNode->getPlanDetail().name));
			planModificationTimeEdit->setText(tr(planNode->getPlanDetail().name));
			approvalModifiedByEdit->setText(tr(planNode->getPlanDetail().name));
			approvalModificationTimeEdit->setText(tr(planNode->getPlanDetail().name));

			if (planNode->isApproved())
			{
				allOwnersCheck->setChecked(planInstanceFilter->allOwnersNeeded());
				allLocationsCheck->setChecked(planInstanceFilter->allLocationsNeeded());

				m_planInstanceListCtrl.clear() // Delete table items... Work?
				updatePlanInstanceList(generalDetailPage.m_planInstanceListCtrl, filter);
			}
		}



		PlanInstanceSharedPtr currentInstance = getTreeModel()->GetCurrentInstance();
        m_planInstanceListCtrl.SetSelectedInstance(currentInstance);

		QApplication::restoreOverrideCursor();

    }
    catch (TA_Base_Core::TransactiveException  &ex)
    {
        MessageBox::error(IDS_REFRESH_CONTENTS, ex);
    }
    m_isLoadingControls = false;

    FUNCTION_EXIT;
}

void PlanGeneralDetailTab::updatePlanInstanceList(PlanNode* planNode, const PlanInstanceFilter &filter)
{
	FUNCTION_ENTRY("updatePlanInstanceList");

	if (planNode->isApproved())
	{
		PlanInstanceList* planInstances = planNode->getAllLocationPlans(filter);

		m_planInstanceListCtrl.setRowCount(planInstances->size());

		for (PlanInstanceList::const_iterator iiter = planInstances->begin(); iiter != planInstances->end(); iiter++)
		{
			PlanInstanceSharedPtr planInstance = *iiter;

			if (!getTreeModel()->TreeNodeFactory::getInstance().canTreeNodeBeShowed(planInstance->getActivePlanId().plan))
			{
				continue;
			}

			// Add to PlanInstane list
			planInstance->acquireActivePlanDetailLock();
			m_planInstanceListCtrl.populateInstance(planInstance->getActivePlanDetail());
		}
	}

	FUNCTION_EXIT;
}


//void PlanGeneralDetailTab::RefreshActiveInstances()
//{
//    FUNCTION_ENTRY( "RefreshActiveInstances" );
//
//    PlanNode *planNode = GetPlanNode();
//    PlanInstanceFilter *planInstanceFilter = GetPlanInstanceFilter();
//
//    planNode->updatePlanInstanceList(m_planInstanceListCtrl, *planInstanceFilter);
//
//    FUNCTION_EXIT;
//}
//
//
//void PlanGeneralDetailTab::PurgeActiveInstances()
//{
//    FUNCTION_ENTRY( "PurgeActiveInstances" );
//
//    // The purge may occur as the operator activates the page. The instance that may have
//    // been selected on the previous page should also be selected in the active instances
//    // list to make sure that current instance selection is not reset during the purge.
//    m_planInstanceListCtrl.SetSelectedInstance(GetDocument()->GetCurrentInstance());
//    m_planInstanceListCtrl.Purge();
//
//    FUNCTION_EXIT;
//}


