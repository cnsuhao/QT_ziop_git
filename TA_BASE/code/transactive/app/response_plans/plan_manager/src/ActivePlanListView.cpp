//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/ActivePlanListView.cpp $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2017/05/22 15:28:33 $
// Last modified by:  $Author: hoa.le $
//
// Used for viewing all currently active plans.
//

#include "app/response_plans/plan_manager/src/ActivePlanListView.h"

//#include "core/utilities/src/TAAssert.h"
//#include "bus/generic_gui_view/src/TransActiveMessage.h"

//#include "app/response_plans/plan_manager/pmodel/src/PlanNode.h"

#include "app/response_plans/plan_manager/pmodel/src/ActivePlanModel.h"

#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"

//#include "app/response_plans/plan_manager/src/MessageBox.h"
//#include "app/response_plans/plan_manager/src/ViewUpdate.h" // QTTODO:

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

using namespace TA_Base_App;

/////////////////////////////////////////////////////////////////////////////
// ActivePlanListView

ActivePlanListView::ActivePlanListView(ActivePlanListFrame* planListFrame)
: QDialog(planListFrame)
, m_planInstanceListCtrl(true /*plan path visible*/)
, m_initialised(false)
, m_planListFrame(planListFrame)
{
	FUNCTION_ENTRY("ActivePlanListView");

	setupUi(this);
	connect(allOwnersCheck, SIGNAL(clicked()), this, SLOT(onAllOwnersCheck()));
	connect(allLocationsCheck, SIGNAL(clicked()), this, SLOT(onAllLocationsCheck()));
	connect(activeInstancesList, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onDblclkActiveInstanceList()));

	// Replace CltTableWidget from ui with Subclass
	delete activeInstancesList;
	activeInstancesList = &m_planInstanceListCtrl;
	verticalLayout->addWidget(activeInstancesList);

	FUNCTION_EXIT;
}

ActivePlanListView::~ActivePlanListView()
{
	FUNCTION_ENTRY("~ActivePlanListView");
	FUNCTION_EXIT;
}

/////////////////////////////////////////////////////////////////////////////
// ActivePlanListView message handlers

//void ActivePlanListView::OnSize(UINT nType, int cx, int cy)
//{
//    FUNCTION_ENTRY( "OnSize" );
//
//    CFormView::OnSize(nType, cx, cy);
//
//    // TODO: Add your message handler code here
//    if (::IsWindow(m_activeInstancesGroupBox.GetSafeHwnd()))
//    {
//        CRect activeInstancesBoxRect;
//        m_activeInstancesGroupBox.GetWindowRect(activeInstancesBoxRect);
//        ScreenToClient(activeInstancesBoxRect);
//        activeInstancesBoxRect.right = cx - 8;
//        activeInstancesBoxRect.bottom = cy - 8;
//        m_activeInstancesGroupBox.MoveWindow(activeInstancesBoxRect);
//
//        CRect activeInstancesListRect;
//        m_planInstanceListCtrl.GetWindowRect(activeInstancesListRect);
//        ScreenToClient(activeInstancesListRect);
//        activeInstancesListRect.right = activeInstancesBoxRect.right - 13;
//        activeInstancesListRect.bottom = activeInstancesBoxRect.bottom - 13;
//        m_planInstanceListCtrl.MoveWindow(activeInstancesListRect);
//    }
//
//    FUNCTION_EXIT;
//}

ActivePlanModel* ActivePlanListView::getTreeModel()
{
	return m_planListFrame->getTreeModel();
}

void ActivePlanListView::onAllOwnersCheck()
{
    FUNCTION_ENTRY( "OnAllOwnersCheck" );

    // TODO: Add your control notification handler code here
	PlanInstanceFilter *planInstanceFilter = getTreeModel()->getPlanInstanceFilter();
	planInstanceFilter->setAllOwnersNeeded(allOwnersCheck->isChecked());

    //if (planInstanceFilter->allOwnersNeeded())
    //{
    //    m_planInstanceListCtrl.refresh();
    //}
    //else
    //{
    //    m_planInstanceListCtrl.purge();
    //}

    FUNCTION_EXIT;
}

void ActivePlanListView::onAllLocationsCheck()
{
    FUNCTION_ENTRY( "OnAllLocationsCheck" );

    // TODO: Add your control notification handler code here
	PlanInstanceFilter *planInstanceFilter = getTreeModel()->getPlanInstanceFilter();
	planInstanceFilter->setAllLocationsNeeded(allOwnersCheck->isChecked());

    //if (planInstanceFilter->allLocationsNeeded())
    //{
    //    m_planInstanceListCtrl.refresh();
    //}
    //else
    //{
    //    m_planInstanceListCtrl.purge();
    //}

    FUNCTION_EXIT;
}

void ActivePlanListView::onDblclkActiveInstanceList()
{
    FUNCTION_ENTRY( "onDblclkActiveInstanceList" );

    //PlanNode *planNode = m_planInstanceListCtrl.GetSelectedPlan();
    //if (planNode)
    //{
    //    // The user may be locked to a specific category and should not be allowed to launch
    //    // the plan controller for a plan that does not live under that category.
    //    if (GetDocument()->IsNodeAccessible(planNode->getNodeId()))
    //    {
    //        PlanInstanceSharedPtr planInstance = m_planInstanceListCtrl.GetSelectedInstance();
    //        TA_ASSERT(NULL != planInstance.get(), "Plan instance is null");

    //        planNode->controlPlan(planInstance->getInstanceId());
    //    }
    //    else
    //    {
    //        TA_Base_Bus::TransActiveMessage userMsg;
    //        userMsg << TA_Base_Core::RunParams::getInstance().get(RPARAM_LOCKCATEGORY);
    //        UINT selectedButton = userMsg.showMsgBox(IDS_UW_210013);
    //    }
    //}

    //*pResult = 0;

    FUNCTION_EXIT;
}

void ActivePlanListView::onViewShown()
{
	FUNCTION_ENTRY("onViewShown");

	if (!m_initialised)
	{
		PlanInstanceFilter *planInstanceFilter = getTreeModel()->getPlanInstanceFilter();

		if (!getTreeModel()->canViewNonLocalActivePlans())
		{
		    planInstanceFilter->setAllLocationsNeeded(false);
			allOwnersCheck->setEnabled(false);
		}

		allOwnersCheck->setChecked(planInstanceFilter->allOwnersNeeded());
		allLocationsCheck->setChecked(planInstanceFilter->allLocationsNeeded());

		m_planInstanceListCtrl.setPlanInstanceFilter(planInstanceFilter);
		m_planInstanceListCtrl.setActivePlanAccess();

		//m_planInstanceListCtrl.refresh();

		m_initialised = true;
	}

	FUNCTION_EXIT;
	//return 0;
}

//void ActivePlanListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
//{
//    FUNCTION_ENTRY( "OnUpdate" );
//
//    // TODO: Add your specialized code here and/or call the base class
//    if (!m_initialised)
//    {
//        FUNCTION_EXIT;
//        return;
//    }
//
//    if (lHint == ID_RPARAM_SESSIONID_UPDATE)
//    {
//        m_allLocationsCheck.EnableWindow(OperatorRights::getInstance().canViewNonLocalActivePlans());
//
//        PlanInstanceFilter *planInstanceFilter = GetDocument()->getPlanInstanceFilter();
//
//        if (planInstanceFilter->allLocationsNeeded() && !OperatorRights::getInstance().canViewNonLocalActivePlans())
//        {
//            planInstanceFilter->setAllLocationsNeeded(false);
//            m_allLocationsCheck.SetCheck(BST_UNCHECKED);
//            SendMessage(WM_COMMAND, MAKEWPARAM(m_allLocationsCheck.GetDlgCtrlID(), BN_CLICKED), (LPARAM) m_allLocationsCheck.GetSafeHwnd());
//        }
//
//        FUNCTION_EXIT;
//        return;
//    }
//
//    CViewUpdate<TA_Base_Core::ActivePlanDetail> *apdUpdate = dynamic_cast<CViewUpdate<TA_Base_Core::ActivePlanDetail> *>(pHint);
//    if (apdUpdate)
//    {
//        SendMessageToDescendants(WM_ACTIVE_PLAN_DETAIL_UPDATE, (WPARAM) &apdUpdate->GetUpdateData(), 0);
//    }
//
//    FUNCTION_EXIT;
//}

//HWND TA_Base_App::ActivePlanListView::getViewHandle()
//{
//    HWND wnd = GetSafeHwnd();
//    TA_ASSERT(wnd != NULL, "View should be initialized successfully");
//    return wnd;
//}

//TA_Base_App::ActivePlanListView::ActivePlanListView():
//CFormView(ActivePlanListView::IDD),
//TA_Base_Bus::AbstractTransActiveFormView(NULL)
//{
//
//}