//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/TreeNode.cpp $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// This is a representation of a node within the plan tree.

//#include "StdAfx.h"
#include "core/utilities/src/TAAssert.h"
//#include "Resource.h"
#include "TreeNode.h"
#include "TreeNodeFactory.h"
//#include "ITreeNodeFilter.h"
//#include "BlankView.h"
#include "app/response_plans/plan_manager/pmodel/src/PlanAgentAccess.h"
#include "RootNode.h"

//#include "bus\unicode_common_lib\src\TAUnicodeString.h"

#include "LabelMaker.h"

using namespace TA_Base_App;

/////////////////////////////////////////////////////////////////////////////
// TreeNode

TreeNode::TreeNode(const TA_Base_Core::NodeSummary &nodeSummary) :
    m_nodeId(nodeSummary.node),
    m_parentId(nodeSummary.parent),
    m_nodeName(new std::string(nodeSummary.name.in())),
    m_nodeSummary(new TA_Base_Core::NodeSummary(nodeSummary)),
    m_nodeDetail(NULL),
    m_nodeVisible(TRUE),
	m_copyNodeAction(CNA_NO_ACTION)
{
    FUNCTION_ENTRY( "TreeNode" );
    FUNCTION_EXIT;
}


TreeNode::~TreeNode()
{
    FUNCTION_ENTRY( "~TreeNode" );

    delete m_nodeName;
    delete m_nodeSummary;
    delete m_nodeDetail;

    FUNCTION_EXIT;
}


bool TreeNode::isPlanPaste()
{
	return m_copyNodeAction != CNA_NO_ACTION;
}


TreeNodeId TreeNode::getNodeId() const
{
    FUNCTION_ENTRY( "getNodeId" );
    FUNCTION_EXIT;
    return m_nodeId;
}


TreeNodeId TreeNode::getParentId() const
{
    FUNCTION_ENTRY( "getParentId" );
    FUNCTION_EXIT;
    return m_parentId;
}


const std::string &TreeNode::getNodeName()
{
    FUNCTION_ENTRY( "getNodeName" );

    if (!m_nodeName)
    {
        m_nodeName = new std::string(getNodeSummary().name.in());
    }

    FUNCTION_EXIT;
    return *m_nodeName;
}


bool TreeNode::isApproved()
{
    FUNCTION_ENTRY( "isApproved" );
    FUNCTION_EXIT;
    return getNodeSummary().approvalState == TA_Base_Core::APPROVED_APPROVAL_STATE;
}


TA_Base_Core::NodeSummary TreeNode::getNodeSummary()
{
    FUNCTION_ENTRY( "getNodeSummary" );

    if (!m_nodeSummary)
    {
        m_nodeSummary = PlanAgentAccess::getInstance().getNodeSummary(getNodeId());
    }

    FUNCTION_EXIT;
    return *m_nodeSummary;
}


TA_Base_Core::NodeDetail& TreeNode::getNodeDetail()
{
    FUNCTION_ENTRY( "getNodeDetail" );

    if (!m_nodeDetail)
    {
        m_nodeDetail = PlanAgentAccess::getInstance().getNodeDetail(getNodeId());
    }

    FUNCTION_EXIT;
    return *m_nodeDetail;
}


TreeNode *TreeNode::getParentNode()
{
    FUNCTION_ENTRY( "getParentNode" );

    TreeNodeId parentNodeId = getNodeSummary().parent;

    FUNCTION_EXIT;
    return TreeNodeFactory::getInstance().getTreeNode(parentNodeId);
}


//CRuntimeClass *TreeNode::getNodeViewClass()
//{
//    FUNCTION_ENTRY( "getNodeViewClass" );
//    FUNCTION_EXIT;
//    return RUNTIME_CLASS(CBlankView);
//}


//void TreeNode::updateCmdUI(CCmdUI &cmdUI)
//{
//    FUNCTION_ENTRY( "updateCmdUI" );
//
//    cmdUI.Enable(FALSE);
//
//    FUNCTION_EXIT;
//}




//std::string& TreeNode::getRootNodeLabel(EPlanTreeType eTreeType)
//{
//	FUNCTION_ENTRY("GetRootNodeLabel");
//
//	static std::string ROOT_NODE_LABEL = "";
//
//	switch (eTreeType)
//	{
//	case PT_ALLPLANS:
//		ROOT_NODE_LABEL = LabelMaker::getAllPlanTreeRootNodeLabel();
//	case PT_APPROVEDPLANS:
//		ROOT_NODE_LABEL = LabelMaker::getApprovedPlanTreeRootNodeLabel();
//	case PT_UNAPPROVEDPLANS:
//		ROOT_NODE_LABEL = LabelMaker::getUnapprovedPlanTreeRootNodeLabel();
//	}
//
//	FUNCTION_EXIT;
//
//	return ROOT_NODE_LABEL;
//}


void TreeNode::updateNode(const TA_Base_Core::NodeSummary &nodeSummary)
{
    FUNCTION_ENTRY( "updateNode" );

    invalidateNodeName();
    invalidateNodeSummary();
    invalidateNodeDetail();

    m_nodeSummary = new TA_Base_Core::NodeSummary(nodeSummary);

    FUNCTION_EXIT;
}


//void TreeNode::updateNode(const TA_Base_Core::PlanConfigUpdateData &planConfigData)
//{
//    FUNCTION_ENTRY( "updateNode" );
//
//    switch (planConfigData.configUpdate._d())
//    {
//        case TA_Base_Core::PCUT_APPROVAL_STATE:
//        {
//            TA_ASSERT(getNodeId() == planConfigData.configUpdate.approvalState().plan, "Node ID mismatch");
//            TA_Base_Core::NodeSummary nodeSummary = getNodeSummary();
//            nodeSummary.approvalState = planConfigData.configUpdate.approvalState().approvalState;
//            updateNode(nodeSummary);
//            break;
//        }
//
//        case TA_Base_Core::PCUT_UPDATE_CATEGORY:
//        {
//            TA_ASSERT(getNodeId() == planConfigData.configUpdate.updateCategory().node, "Node ID mismatch");
//            updateNode(planConfigData.configUpdate.updateCategory());
//            break;
//        }
//
//        case TA_Base_Core::PCUT_UPDATE_PLAN:
//        {
//            TA_ASSERT(getNodeId() == planConfigData.configUpdate.updatePlan().node, "Node ID mismatch");
//            updateNode(planConfigData.configUpdate.updatePlan());
//            break;
//        }
//
//        case TA_Base_Core::PCUT_MOVE_PLAN:
//        {
//            TA_ASSERT(getNodeId() == planConfigData.configUpdate.movePlan().summary.node, "Node ID mismatch");
//            updateNode(planConfigData.configUpdate.movePlan().summary);
//            break;
//        }
//
//        case TA_Base_Core::PCUT_MOVE_PLAN_TO_ROOT:
//        {
//            TA_ASSERT(getNodeId() == planConfigData.configUpdate.movePlanToRoot().summary.node, "Node ID mismatch");
//            updateNode(planConfigData.configUpdate.movePlanToRoot().summary);
//            break;
//        }
//
//        case TA_Base_Core::PCUT_MOVE_PLAN_FROM_ROOT:
//        {
//            TA_ASSERT(getNodeId() == planConfigData.configUpdate.movePlanFromRoot().summary.node, "Node ID mismatch");
//            updateNode(planConfigData.configUpdate.movePlanFromRoot().summary);
//            break;
//        }
//
//        default:
//            ;
//    }
//
//    FUNCTION_EXIT;
//}


void TreeNode::invalidateNodeName()
{
    FUNCTION_ENTRY( "invalidateNodeName" );

    delete m_nodeName;
    m_nodeName = NULL;

    FUNCTION_EXIT;
}


void TreeNode::invalidateNodeSummary()
{
    FUNCTION_ENTRY( "invalidateNodeSummary" );

    delete m_nodeSummary;
    m_nodeSummary = NULL;

    FUNCTION_EXIT;
}


void TreeNode::invalidateNodeDetail()
{
    FUNCTION_ENTRY( "invalidateNodeDetail" );

    delete m_nodeDetail;
    m_nodeDetail = NULL;

    FUNCTION_EXIT;
}


//int CALLBACK TreeNode::compareSiblingNodes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
//{
//    FUNCTION_ENTRY( "compareSiblingNodes" );
//
//    TreeNode *treeNode1 = reinterpret_cast<TreeNode *>(lParam1);
//    TA_ASSERT(treeNode1, "First tree node pointer is null");
//
//    TreeNode *treeNode2 = reinterpret_cast<TreeNode *>(lParam2);
//    TA_ASSERT(treeNode1, "Second tree node pointer is null");
//
//    if (treeNode1 == treeNode2)
//    {
//        FUNCTION_EXIT;
//        return 0;
//    }
//
//    FUNCTION_EXIT;
//    return (treeNode1->hasPrecedence(*treeNode2)? -1 : 1);
//}


bool TreeNode::isVisible()
{
    return m_nodeVisible;
}

void TreeNode::setVisible( bool bVisible, bool writeToDb )
{
    m_nodeVisible = bVisible;
    if (writeToDb)
    {
        PlanAgentAccess::getInstance().setPlanType(getNodeId(), (m_nodeVisible? TA_Base_Core::VISIBLE_TYPE : TA_Base_Core::INVISIBLE_TYPE));
    }
}
