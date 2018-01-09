//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanInstanceListCtrl.cpp $
// @author:  Bart Golab
// @version: $Revision: #2 $
//
// Last modification: $DateTime: 2017/05/22 15:28:33 $
// Last modified by:  $Author: hoa.le $
//
// <description>

#if defined(_MSC_VER)
    #pragma warning(disable:4250)
#endif // defined (_MSC_VER)

#include "core/utilities/src/TAAssert.h"
//#include "app/response_plans/plan_manager/src/PlanManagerApp.h"
//#include "app/response_plans/plan_manager/src/PlanAgentAccess.h"
#include "app/response_plans/plan_manager/src/PlanInstanceListCtrl.h"
//#include "app/response_plans/plan_manager/src/TreeNodeFactory.h"
//#include "app/response_plans/plan_manager/src/TreeNode.h"
//#include "app/response_plans/plan_manager/src/PlanNode.h"
#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"
#include "app/response_plans/plan_manager/src/MessageBox.h"
//#include "app/response_plans/plan_manager/src/Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;
using namespace TA_Base_Bus;


/////////////////////////////////////////////////////////////////////////////
// PlanInstanceListCtrl

PlanInstanceListCtrl::PlanInstanceListCtrl(bool planPathVisible) :
    PlanActiveInstanceListCtrl(planPathVisible)
{
    FUNCTION_ENTRY( "PlanInstanceListCtrl" );
    FUNCTION_EXIT;
}


PlanInstanceListCtrl::~PlanInstanceListCtrl()
{
    FUNCTION_ENTRY( "~PlanInstanceListCtrl" );
    FUNCTION_EXIT;
}


//BEGIN_MESSAGE_MAP(PlanInstanceListCtrl, PlanActiveInstanceListCtrl)
//    // {{AFX_MSG_MAP(PlanInstanceListCtrl)
//    // }}AFX_MSG_MAP
//    ON_MESSAGE(WM_ACTIVE_PLAN_DETAIL_UPDATE, OnActivePlanDetailUpdate)
//END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PlanInstanceListCtrl message handlers
//BOOL PlanInstanceListCtrl::PreCreateWindow( CREATESTRUCT& cs )
//{
//    BOOL bRes = TA_Base_Bus::PlanActiveInstanceListCtrl::PreCreateWindow( cs );
//
//    ApplicationFont::getInstance().adjustChildrenFont( GetSafeHwnd() );
//
//    return bRes;
//}


//LRESULT PlanInstanceListCtrl::OnActivePlanDetailUpdate(WPARAM wParam, LPARAM lParam)
//{
//    FUNCTION_ENTRY( "OnActivePlanDetailUpdate" );
//
//    // Automatically select every new instance activated by this session
//    TA_Base_Core::ActivePlanDetail* activePlanDetail = reinterpret_cast<TA_Base_Core::ActivePlanDetail *>(wParam);
//    bool newInstance = (FindInstance(activePlanDetail->plan) == -1);
//
//    PlanActiveInstanceListCtrl::OnActivePlanDetailUpdate(wParam, lParam); // add or update instance
//
//    if (newInstance)
//    {
//        int item = FindInstance(activePlanDetail->plan);
//
//        if (item != -1)
//        {
//            try
//            {
//                if (PlanAgentAccess::getInstance().isOwner(activePlanDetail->plan))
//                {
//                    SelectItem(item);
//                }
//            }
//            catch (...) {}
//        }
//    }
//
//    FUNCTION_EXIT;
//    return 0;
//}
//
//
//void PlanInstanceListCtrl::SetSelectedInstance(PlanInstanceSharedPtr planInstance)
//{
//    FUNCTION_ENTRY( "SetSelectedInstance" );
//
//    int selectedItem = GetNextItem(-1, LVNI_SELECTED);
//    if (selectedItem != -1)
//    {
//        SelectItem(selectedItem, false);
//    }
//
//    if (!planInstance)
//    {
//        FUNCTION_EXIT;
//        return;
//    }
//
//    if (planInstance != GetSelectedInstance())
//    {
//        for (int item = 0; item < GetItemCount(); item++)
//        {
//            TA_Base_Core::ActivePlanId activePlanId = GetPlanInstanceId(item);
//
//            PlanNode *planNode = dynamic_cast<PlanNode *>(TreeNodeFactory::getInstance().getTreeNode(activePlanId.plan));
//
//            if (planNode)
//            {
//                PlanInstanceSharedPtr tmpInstance = planNode->getPlanInstance(PlanInstanceId(activePlanId.instance, activePlanId.location));
//
//                if (tmpInstance.get() == planInstance.get())
//                {
//                    SelectItem(item);
//
//                    FUNCTION_EXIT;
//                    return;
//                }
//            }
//        }
//    }
//
//    FUNCTION_EXIT;
//}
//
//
//PlanInstanceSharedPtr PlanInstanceListCtrl::GetSelectedInstance()
//{
//    FUNCTION_ENTRY( "GetSelectedInstance" );
//
//    PlanNode *planNode = GetSelectedPlan();
//
//    if (planNode)
//    {
//        TA_Base_Core::ActivePlanId selectedActivePlanId = GetSelectedPlanInstanceId();
//
//        FUNCTION_EXIT;
//        return planNode->getPlanInstance(PlanInstanceId(selectedActivePlanId.instance, selectedActivePlanId.location));
//    }
//
//    FUNCTION_EXIT;
//    return PlanInstanceSharedPtr();
//}
//
//
//PlanNode *PlanInstanceListCtrl::GetSelectedPlan()
//{
//    FUNCTION_ENTRY( "GetSelectedPlan" );
//
//    TA_Base_Core::ActivePlanId selectedActivePlanId = GetSelectedPlanInstanceId();
//
//    if (selectedActivePlanId.plan != (TreeNodeId) -1)
//    {
//        try
//        {
//            TreeNode *treeNode = TreeNodeFactory::getInstance().getTreeNode(selectedActivePlanId.plan);
//
//            if (!treeNode)
//            {
//                treeNode = TreeNodeFactory::getInstance().createTreeNode(selectedActivePlanId.plan);
//            }
//
//            FUNCTION_EXIT;
//            return dynamic_cast<PlanNode *>(treeNode);
//        }
//        catch (TA_Base_Core::TransactiveException &ex)
//        {
//            MessageBox::error(IDS_RETRIEVE_PLAN, ex);
//        }
//    }
//
//    FUNCTION_EXIT;
//    return NULL;
//}
//
//
//void PlanInstanceListCtrl::SelectItem(int item, bool selected)
//{
//    FUNCTION_ENTRY( "SelectItem" );
//
//    TA_ASSERT(item >= 0 && item < GetItemCount(), "Invalid item");
//
//    int selectionState = selected? (LVIS_SELECTED | LVIS_FOCUSED) : ~(LVIS_SELECTED | LVIS_FOCUSED);
//
//    SetItemState(item, selectionState, LVIS_SELECTED | LVIS_FOCUSED);
//
//    if (selected)
//    {
//        EnsureVisible(item, FALSE);
//    }
//
//    FUNCTION_EXIT;
//}


