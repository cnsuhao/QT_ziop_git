//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanDetailTab.cpp $
// @author:  Bart Golab
// @version: $Revision: #2 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// <description>

#include "app/response_plans/plan_manager/src/PlanDetailTab.h"

#include "core/utilities/src/TAAssert.h"

#include "app/response_plans/plan_manager/src/PlanTreeFrame.h"

#include "app/response_plans/plan_manager/src/PlanManagerApp.h"
#include "app/response_plans/plan_manager/pmodel/src/PlanTreeModel.h"
#include "app/response_plans/plan_manager/src/PlanNode.h"
#include "app/response_plans/plan_manager/pmodel/src/PlanInstanceFilter.h"
//#include "app/response_plans/plan_manager/src/OperatorRights.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// PlanDetailTab dialog

PlanDetailTab::PlanDetailTab(QWidget* parent) 
: QWidget(parent)
, m_isRefreshPending(true)
{
    FUNCTION_ENTRY( "PlanDetailTab" );
    FUNCTION_EXIT;
}


PlanDetailTab::~PlanDetailTab()
{
    FUNCTION_ENTRY( "~PlanDetailTab" );
    FUNCTION_EXIT;
}


//BEGIN_MESSAGE_MAP(PlanDetailTab, CPropertyPage)
//    // {{AFX_MSG_MAP(PlanDetailTab)
//    ON_BN_CLICKED(IDC_ALL_OWNERS_CHECK, OnAllOwnersCheck)
//    ON_BN_CLICKED(IDC_ALL_LOCATIONS_CHECK, OnAllLocationsCheck)
//    // }}AFX_MSG_MAP
//    ON_MESSAGE(WM_RESET_TREE_NODE_VIEW, OnResetTreeNodeView)
//    ON_MESSAGE_VOID(WM_ADJUST_CONTROL_POSITIONS, AdjustControlPositions)
//    ON_MESSAGE_VOID(WM_ADJUST_PLAN_FILTER_BUTTON_STATES, SetPlanFilterButtonStates)
//END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PlanDetailTab message handlers

//void PlanDetailTab::OnAllOwnersCheck()
//{
//    FUNCTION_ENTRY( "OnAllOwnersCheck" );
//
//    PlanInstanceFilter *planInstanceFilter = GetPlanInstanceFilter();
//    planInstanceFilter->setAllOwnersNeeded(m_allOwnersCheck.GetCheck() == BST_CHECKED);
//
//    if (!m_isRefreshPending)
//    {
//        CWaitCursor waitCursor;
//
//        if (planInstanceFilter->allOwnersNeeded())
//        {
//            RefreshActiveInstances();
//        }
//        else
//        {
//            PurgeActiveInstances();
//        }
//    }
//
//    FUNCTION_EXIT;
//}
//
//
//void PlanDetailTab::OnAllLocationsCheck()
//{
//    FUNCTION_ENTRY( "OnAllLocationsCheck" );
//
//    PlanInstanceFilter *planInstanceFilter = GetPlanInstanceFilter();
//    planInstanceFilter->setAllLocationsNeeded(m_allLocationsCheck.GetCheck() == BST_CHECKED);
//
//    if (!m_isRefreshPending)
//    {
//        CWaitCursor waitCursor;
//
//        if (planInstanceFilter->allLocationsNeeded())
//        {
//            RefreshActiveInstances();
//        }
//        else
//        {
//            PurgeActiveInstances();
//        }
//    }
//
//    FUNCTION_EXIT;
//}


void PlanDetailTab::onResetPlanNodeView(PlanTreeModel* treeModel, QTabWidget* pPlanTab)
{
    FUNCTION_ENTRY( "OnResetTreeNodeView" );

    // Reset active instance filtering rules
	treeModel->getPlanInstanceFilter()->setDefaults();

    // Reset active instance seletion
    PlanInstanceSharedPtr instance;
	treeModel->setCurrentInstance(instance);

    // For efficiency update only the active page. Other page(s) will update once activated.
    m_isRefreshPending = true;

	if (this == pPlanTab->currentWidget())
    {
        updatePlanDetail();
    }

    FUNCTION_EXIT;
}


//BOOL PlanDetailTab::OnSetActive()
//{
//    FUNCTION_ENTRY( "OnSetActive" );
//
//    if (CPropertyPage::OnSetActive())
//    {
//        GetParentFrame()->SendMessage(WM_PLAN_DETAIL_PAGE_SELECTION_CHANGED, (WPARAM) this);
//
//        UpdatePlanDetail();
//
//        FUNCTION_EXIT;
//        return TRUE;
//    }
//
//    FUNCTION_EXIT;
//    return FALSE;
//}


void PlanDetailTab::updatePlanDetail()
{
    FUNCTION_ENTRY("updatePlanDetail");

    //AdjustControlPositions();
    setPlanFilterButtonStates();

    if (m_isRefreshPending)
    {
        showRelevantControls();
        refreshControlContents();

        m_isRefreshPending = false;
    }

    FUNCTION_EXIT;
}
//
//
//void PlanDetailTab::AdjustControlPositions()
//{
//    FUNCTION_ENTRY( "AdjustControlPositions" );
//
//    CTabCtrl *tabCtrl = GetPropertySheet()->GetTabControl();
//    CRect tabCtrlRect;
//
//    tabCtrl->GetClientRect(tabCtrlRect);
//    tabCtrl->AdjustRect(FALSE, tabCtrlRect);
//    tabCtrlRect.OffsetRect(CPoint(5, 9));   // BGTODO: Shouldn't have to offset at all. Investigate ...
//
//    // Reposition the dialog and tell it to resize. This will adjust the sizes and positions of individual controls.
//    MoveWindow(tabCtrlRect);
//    SendMessage(WM_SIZE, SIZE_RESTORED, MAKEWPARAM(tabCtrlRect.Width(), tabCtrlRect.Height()));
//
//    FUNCTION_EXIT;
//}


void PlanDetailTab::setPlanFilterButtonStates()
{
    FUNCTION_ENTRY("setPlanFilterButtonStates");

    PlanInstanceFilter *planInstanceFilter = getPlanInstanceFilter();

	bool isLocationView = getTreeModel()->canViewNonLocalActivePlans();
	getAllLocationsCheck()->setEnabled(isLocationView);

	if (!isLocationView)
    {
        planInstanceFilter->setAllLocationsNeeded(false);
    }

	if (planInstanceFilter->allOwnersNeeded() != (getAllOwnersCheck()->checkState() == Qt::Checked))
    {
		getAllOwnersCheck()->setChecked(planInstanceFilter->allOwnersNeeded());
		getAllOwnersCheck()->click();
    }

	if (planInstanceFilter->allLocationsNeeded() != (getAllLocationsCheck()->checkState() == Qt::Checked))
    {
		getAllLocationsCheck()->setChecked(planInstanceFilter->allLocationsNeeded());
		getAllLocationsCheck()->click();
    }

    FUNCTION_EXIT;
}


//BOOL PlanDetailTab::PreTranslateMessage(MSG* pMsg)
//{
//    FUNCTION_ENTRY( "PreTranslateMessage" );
//
//    // TODO: Add your specialized code here and/or call the base class
//    CFrameWnd *parentFrame = GetParentFrame();
//    if (parentFrame)
//    {
//        // Give the frame a chance to process accelerator key presses.
//        if (parentFrame->PreTranslateMessage(pMsg))
//        {
//            FUNCTION_EXIT;
//            return TRUE;
//        }
//    }
//
//    FUNCTION_EXIT;
//    return CPropertyPage::PreTranslateMessage(pMsg);
//}
//
//
//CPropertySheet *PlanDetailTab::GetPropertySheet()
//{
//    FUNCTION_ENTRY( "GetPropertySheet" );
//
//    CPropertySheet *propertySheet = static_cast<CPropertySheet *>(GetParent());
//    TA_ASSERT(propertySheet != NULL, "Parent property sheet is null");
//
//    FUNCTION_EXIT;
//    return propertySheet;
//}


PlanTreeModel* PlanDetailTab::getTreeModel()
{
    FUNCTION_ENTRY( "getTreeModel" );

	PlanTreeModel* pTreeModel = (dynamic_cast<PlanTreeFrame*>(this->parentWidget()))->getTreeModel();
	TA_ASSERT(pTreeModel != NULL, "PlanTreeModel is invalid");

    FUNCTION_EXIT;
	return pTreeModel;
}


PlanNode* PlanDetailTab::getPlanNode()
{
    FUNCTION_ENTRY("getPlanNode");

    PlanNode *planNode = dynamic_cast<PlanNode*>(getTreeModel()->GetCurrentNode());
    TA_ASSERT(planNode != NULL, "Not a plan node");

    FUNCTION_EXIT;
    return planNode;
}


PlanInstanceFilter* PlanDetailTab::getPlanInstanceFilter()
{
    FUNCTION_ENTRY( "GetPlanInstanceFilter" );

    PlanInstanceFilter *planInstanceFilter = dynamic_cast<PlanTreeFrame*>(this->parentWidget())->getTreeModel()->getPlanInstanceFilter();
    TA_ASSERT(planInstanceFilter != NULL, "Plan instance filter is null");

    FUNCTION_EXIT;
    return planInstanceFilter;
}


//BOOL PlanDetailTab::OnInitDialog()
//{
//    CPropertyPage::OnInitDialog();
//
//    ApplicationFont::getInstance().adjustChildrenFont( GetSafeHwnd() );
//
//    return TRUE;
//}