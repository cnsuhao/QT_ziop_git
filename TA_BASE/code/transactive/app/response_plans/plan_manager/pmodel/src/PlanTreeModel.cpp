//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanTreeModel.cpp $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// <description>
//

//#include "StdAfx.h"
//#include "core/utilities/src/TAAssert.h"
//#include "core/utilities/src/DataConversion.h"
//#include "core/exceptions/src/TransactiveException.h"
//#include "core/exceptions/src/PlanAgentException.h"
//#include "bus/mfc_extensions/src/message_box/XMessageBox.h"
//#include "bus/generic_gui_view/src/TransActiveMessage.h"
//TD17888 marc_bugfix for save as dialog
//#include "bus/mfc_extensions/src/file_dialog_no_folder_change/FileDialogArialDisableFolders.h"
//#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
//#include "core/data_access_interface/entity_access/src/IEntityData.h"
//#include "core/data_access_interface/entity_access/src/PlanManagerEntityData.h"
//#include "Resource.h"
#include "PlanTreeModel.h"
#include "TreeNode.h"
#include "PlanNode.h"
#include "CategoryNode.h"
//#include "RootNode.h"
//#include "PlanStep.h"
//#include "MessageBox.h"
//#include "PlanAgentUpdateProcessor.h"

#include "LabelMaker.h"

//#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
//#include "ChangeImportPlanName.h"

//#include "PlanAgentAccess.h"

//// CL 15087 (3) ++
//#include "core/message/types/PlanAgentAudit_MessageTypes.h"
//#include "core/message/src/AuditMessageSender.h"
//#include "core/message/src/MessagePublicationManager.h"
//// CL 15087 (3) ++
//#include "core/utilities/src/DateFormat.h" // TD12474

#include "PlanService.h"

#include "TreeNodeFactory.h"

using namespace TA_Base_App;

/////////////////////////////////////////////////////////////////////////////
// PlanTreeModel

PlanTreeModel::PlanTreeModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
: TA_Base_Bus::AbstractPModel(appController, pmodelName), m_ePlanType(PT_UNDEFINED)
{
	FUNCTION_ENTRY("PlanTreeModel");

	// TODO: add one-time construction code here
	if (_stricmp(pmodelName, PMODEL_ALLPLANTREEFRAME) == 0)
	{
		m_ePlanType = PT_ALLPLANS;
	}
	else if (_stricmp(pmodelName, PMODEL_APPROVEDPLANTREEFRAME) == 0)
	{
		m_ePlanType = PT_APPROVEDPLANS;
	}
	else if (_stricmp(pmodelName, PMODEL_UNAPPROVEDPLANTREEFRAME) == 0)
	{
		m_ePlanType = PT_UNAPPROVEDPLANS;
	}


	FUNCTION_EXIT;
}

PlanTreeModel::~PlanTreeModel()
{
	FUNCTION_ENTRY("~PlanTreeModel");
	FUNCTION_EXIT;
}

void PlanTreeModel::allocViewResource()
{
	registerPlanSettings();
}

void PlanTreeModel::releaseViewResource()
{
	deregisterPlanSettings();
}

std::string& PlanTreeModel::GetRootNodeLabel(EPlanTreeType eTreeType)
{
	FUNCTION_ENTRY("GetRootNodeLabel");

	static std::string ROOT_NODE_LABEL = "";

	switch (eTreeType)
	{
	case PT_ALLPLANS:
		ROOT_NODE_LABEL = LabelMaker::getAllPlanTreeRootNodeLabel();
		break;
	case PT_APPROVEDPLANS:
		ROOT_NODE_LABEL = LabelMaker::getApprovedPlanTreeRootNodeLabel();
		break;
	case PT_UNAPPROVEDPLANS:
		ROOT_NODE_LABEL = LabelMaker::getUnapprovedPlanTreeRootNodeLabel();
		break;
	}

	FUNCTION_EXIT;

	return ROOT_NODE_LABEL;
}

bool PlanTreeModel::isRelevant(TreeNode &treeNode)
{
	FUNCTION_ENTRY("isRelevant");

	// PW#2920: If the operator is locked into a particular category, then all that is relevant are
	// nodes at or above the locked category or below it.

	bool isPTLocked = IsPlanTreeLocked();

	bool retVal = m_ePlanType == PT_ALLPLANS ? true : false;

	if (isPTLocked)
	{
		bool isCategoryPathLocked = IsNodeOnLockedCategoryPath(treeNode.getNodeId());
		bool isNodeAccess = IsNodeAccessible(treeNode.getNodeId());

		if (m_ePlanType == PT_ALLPLANS)
		{
			FUNCTION_EXIT
				return isCategoryPathLocked || isNodeAccess;
		}
		else
		{
			if (!isCategoryPathLocked || !isNodeAccess)
			{
				FUNCTION_EXIT
					return false;
			}
		}
	}

	if (m_ePlanType == PT_APPROVEDPLANS || m_ePlanType == PT_UNAPPROVEDPLANS)
	{
		PlanNode *planNode = dynamic_cast<PlanNode *>(&treeNode);

		if (planNode)
		{
			retVal = planNode->isApproved();
			if (m_ePlanType == PT_UNAPPROVEDPLANS)
			{
				retVal = !retVal;
			}
		}

		CategoryNode *categoryNode = dynamic_cast<CategoryNode *>(&treeNode);

		if (categoryNode)
		{
			// A category is relevant to the unapproved plan tree if either of the following applies:
			// 1. the category has unapproved descendants, or
			// 2. a plan tree lock is active and the category lies on the path leading to the locked category - this
			// allows the locked category to appear in the unapproved plan tree, even if it has no unapproved descendants.
			retVal = categoryNode->getNodeSummary().hasUnapprovedDescendant ||
				(IsPlanTreeLocked() && IsNodeOnLockedCategoryPath(categoryNode->getNodeId()));
		}
	}

	FUNCTION_EXIT;
	return retVal;
}

RootNode* PlanTreeModel::createRootNode()
{
	FUNCTION_ENTRY("createRootNode");
	FUNCTION_EXIT;

	return TreeNodeFactory::getInstance().createRootNode();
}

void PlanTreeModel::PurgeCategoryViewNodes()
{
	FUNCTION_ENTRY("PurgeCategoryViewNodes");

	m_categoryViewNodeMap.clear();

	FUNCTION_EXIT;
}

bool PlanTreeModel::ArePlanNodesSelected()
{
	FUNCTION_ENTRY("ArePlanNodesSelected");

	TreeNodeList selectedNodes = GetSelectedNodes();

	if (selectedNodes.empty())
	{
		FUNCTION_EXIT;
		return false;
	}

	for (TreeNodeList::iterator iter = selectedNodes.begin(); iter != selectedNodes.end(); iter++)
	{
		PlanNode* planNode = reinterpret_cast<PlanNode*>(*iter);
		if (!planNode)
		{
			FUNCTION_EXIT;
			return false;
		}
	}

	FUNCTION_EXIT;
	return true;
}

TreeNodeList PlanTreeModel::GetSelectedNodes()
{
	FUNCTION_ENTRY("GetSelectedNodes");

	// If there are nodes selected in the category node view return those, otherwise return only the node
	// that's currently selected in the plan tree.
	TreeNodeList selectedNodes;

	if (m_categoryViewNodeMap.empty())
	{
		TreeNode *treeNode = GetCurrentNode();
		// Return empty list if current tree node is invalid
		if (treeNode == NULL)
		{
			return selectedNodes;
		}
		selectedNodes.push_back(treeNode);
	}
	else
	{
		selectedNodes.reserve(m_categoryViewNodeMap.size());

		for (TreeNodeMap::iterator iter = m_categoryViewNodeMap.begin(); iter != m_categoryViewNodeMap.end(); iter++)
		{
			selectedNodes.push_back(iter->second);
		}
	}

	FUNCTION_EXIT;
	return selectedNodes;
}

void PlanTreeModel::addToCategoryViewNodes(TreeNode *treeNode)
{
	FUNCTION_ENTRY("AddToCategoryViewNodes");

	TreeNodeMap::iterator iter = m_categoryViewNodeMap.find(treeNode->getNodeId());

	if (iter == m_categoryViewNodeMap.end())
	{
		m_categoryViewNodeMap.insert(std::make_pair(treeNode->getNodeId(), treeNode));
	}

	FUNCTION_EXIT;
}


void PlanTreeModel::removeFromCategoryViewNodes(TreeNode *treeNode)
{
	FUNCTION_ENTRY("RemoveFromCategoryViewNodes");

	TreeNodeMap::iterator iter = m_categoryViewNodeMap.find(treeNode->getNodeId());

	if (iter != m_categoryViewNodeMap.end())
	{
		m_categoryViewNodeMap.erase(iter);
	}

	FUNCTION_EXIT;
}