/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanTreeView.cpp $
* @author:  Bart Golab
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2017/05/09 13:33:55 $
* Last modified by:  $Author: hoa.le $
*
* Subclass of CtlTreeWidget to display and manipulate the QTreeWidget.
*/

#include "app/response_plans/plan_manager/src/PlanTreeView.h"

#include "core/utilities/src/TAAssert.h"
#include "bus/response_plans/active_plans_display/src/StringUtilities.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
//#include "FloatingWindowThreadRegister.h" //QTTODO: seprate, used in edit and control plans

#include "app/response_plans/plan_manager/pmodel/src/TreeNode.h"
#include "app/response_plans/plan_manager/pmodel/src/RootNode.h"
#include "app/response_plans/plan_manager/pmodel/src/PlanTreeModel.h"

#include "app/response_plans/plan_manager/src/MessageBox.h"
#include "app/response_plans/plan_manager/src/PlanTreeFrame.h"

using namespace TA_Base_App;

/////////////////////////////////////////////////////////////////////////////
// PlanTreeView construction/destruction

PlanTreeView::PlanTreeView(EPlanTreeType ePlanType, PlanTreeFrame* planTreeFrame)
: CtlTreeWidget(planTreeFrame), m_ePlanType(ePlanType), m_planTreeFrame(planTreeFrame)/*, AbstractTransActiveView(this, "")*/
{
	FUNCTION_ENTRY("PlanTreeView");

	setMouseTracking(true);
	viewport()->setMouseTracking(true);

	FUNCTION_EXIT;
}

PlanTreeView::~PlanTreeView()
{
	FUNCTION_ENTRY("~PlanTreeView");
	FUNCTION_EXIT;
}

PlanTreeModel* PlanTreeView::getTreeModel()
{
	return m_planTreeFrame->getTreeModel();
}

void PlanTreeView::onViewShown()
{
	FUNCTION_ENTRY("OnViewShown");

	if (this->topLevelItemCount() <= 0) // Tree is empty - must get root node data to display
	{
		try
		{
			QStringList treeLabels;
			treeLabels << tr("");
			setHeaderLabels(treeLabels);
			// Pretend we have got an update of the lock category parameter.
			// This will have the effect of populating the root category and, if necessary,
			// locking the tree at a specific sub-category.
			ProcessRparamLockCategoryUpdate();
		}
		catch (TA_Base_Core::TransactiveException  &ex)
		{
			//MessageBox::error(IDS_DISPLAY_TREE_CONTENTS, ex);
		}
	}
	else // User switched from another view - update toolbar and menu according to the type of selected node // QTTODO2
	{
		emit mfTreeNodeChanged(getTreeModel()->GetCurrentNode());
	}

	FUNCTION_EXIT;
}

CtlTreeWidgetItem* PlanTreeView::addToTree(TreeNode* treeNode, CtlTreeWidget &treeWidget, CtlTreeWidgetItem* parent)
{
	FUNCTION_ENTRY("addToTree");

	if (m_planTreeFrame->getTreeModel() && !m_planTreeFrame->getTreeModel()->isRelevant(*treeNode))
	{
		FUNCTION_EXIT;
		return NULL;
	}

	int itemIndex = 0;
	if (parent && parent->childCount() > 0)
	{
		CtlTreeWidgetItem* siblingItem = treeWidget.itemBelow(parent);
		 TA_ASSERT(siblingItem != NULL, "Sibling tree item is null");

		while (siblingItem)
		{
			TreeNode* siblingNode = static_cast<TreeNode*>(siblingItem->getDataPtr()); // convert the data to TreeNode*
			TA_ASSERT(siblingNode, "Sibling node pointer is null");

			// See if node is already in the tree
			if (siblingNode == treeNode)
			{
				FUNCTION_EXIT;
				return NULL;
			}

			// See if node should go below its sibling.
			if (siblingNode->hasPrecedence(*treeNode))
			{
				itemIndex++;;  // this node must go below its sibling
			}
			else
			{
				break;
			}

			siblingItem = parent->child(itemIndex);// treeWidget.topLevelItem(treeWidget.indexOfTopLevelItem(siblingItem) + 1);
		}
	}

	CtlTreeWidgetItem* newItem = NULL;

	if (treeWidget.topLevelItemCount() <= 0)
	{
		newItem = new CtlTreeWidgetItem(&treeWidget);
		newItem->setIcon(0, QIcon(QString(treeNode->getNodeImageType(newItem->isExpanded()).c_str())));
	}
	else
	{
		newItem = new CtlTreeWidgetItem();
		parent->insertChild(itemIndex, newItem);
		// Display string according to locale
		newItem->setText(0, QTextCodec::codecForLocale()->toUnicode(treeNode->getNodeName().c_str()));
		newItem->setIcon(0, QIcon(QString(treeNode->getNodeImageType(newItem->isExpanded()).c_str())));
		if (treeNode->getNodeSummary().hasChildren)
		{
			newItem->setChildIndicatorPolicy(CtlTreeWidgetItem::ShowIndicator);
		}
	}

	if (newItem)
	{
		newItem->setDataPtr(treeNode);
	}

	FUNCTION_EXIT;

	return newItem;
}

void PlanTreeView::addChildrenToTree(TreeNode* treeNode, CtlTreeWidget &treeWidget, CtlTreeWidgetItem* parent)
{
	FUNCTION_ENTRY("addChildrenToTree");

	TreeNodeList childNodes;
	CategoryNode* catNode = dynamic_cast<CategoryNode*>(treeNode);
	if (catNode)
	{
		catNode->getChildren(childNodes);

		for (TreeNodeList::iterator iter = childNodes.begin(); iter != childNodes.end(); iter++)
		{
			TreeNode *childNode = (TreeNode *)*iter;
			addToTree(childNode, treeWidget, parent);
		}
	}

	FUNCTION_EXIT;
}

void PlanTreeView::onTreeItemExpanded(QTreeWidgetItem* item)
{
	FUNCTION_ENTRY("itemExpanded");
	CtlTreeWidgetItem* ctlItem = dynamic_cast<CtlTreeWidgetItem*>(item);
	TreeNode* treeNode = static_cast<TreeNode*>(ctlItem->getDataPtr()); // convert the data to TreeNode*
	TA_ASSERT(treeNode, "Tree node pointer is null");

	// Ensure that the node is expandable. Note that if the item has been selected then it's also expandable.
	if (m_guardTreeAccess && ctlItem && !getTreeModel()->IsNodeAccessible(treeNode->getNodeId()))
	{
		TA_Base_Bus::TransActiveMessage userMsg;
		userMsg << TA_Base_Core::RunParams::getInstance().get(RPARAM_LOCKCATEGORY);
		UINT selectedButton = userMsg.showMsgBox(IDS_UW_210018);

		FUNCTION_EXIT;
		return;
	}

	// If item is being expanded for the first time
	if (ctlItem->isExpanded() && ctlItem->childCount() == 0)
	{
		try
		{
			QApplication::setOverrideCursor(Qt::WaitCursor);

			// Insert the node's children into the tree under the expanding item
			addChildrenToTree(treeNode, *this, ctlItem);

			QApplication::restoreOverrideCursor();
		}
		catch (const TA_Base_Core::TransactiveException  &ex)
		{
			//MessageBox::error(IDS_EXPAND_CATEGORY, ex);

			FUNCTION_EXIT;
			return;
		}
	}

	FUNCTION_EXIT;
}

void PlanTreeView::onTreeItemSelected(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	FUNCTION_ENTRY("currentItemChanged");

	CtlTreeWidgetItem* ctlItem = dynamic_cast<CtlTreeWidgetItem*>(current);

	TreeNode* treeNode = static_cast<TreeNode*>(ctlItem->getDataPtr()); // convert the data to TreeNode*
	TA_ASSERT(treeNode, "Tree node pointer is null");

	// Store away the selected node
	getTreeModel()->SetCurrentNode(treeNode);

	// Notify the main frame so that the toolbar can be updated for the selected node
	emit mfTreeNodeChanged(treeNode);

	// Notify the parent tree frame so that the right hand pane can be updated for the selected node
	emit pfTreeNodeChanged(treeNode);

	FUNCTION_EXIT;
}

void PlanTreeView::ProcessRparamLockCategoryUpdate()
{
	FUNCTION_ENTRY("ProcessRparamLockCategoryUpdate");

	QApplication::setOverrideCursor(Qt::WaitCursor);

	// Disable the tree access guard while populating and searching the tree for the locked category.
	// The search may involve expanding the tree, which is normally protected by the guard.
	m_guardTreeAccess = false;

	// Create the root of the tree.
	RootNode *rootNode = getTreeModel()->createRootNode();
	this->clear(); // Delete all items from tree

	this->header()->close(); // Remove tree widget header

	CtlTreeWidgetItem* newItem = addToTree(rootNode, *this);

	newItem->setText(0, QString(getTreeModel()->GetRootNodeLabel(m_ePlanType).c_str()));

	this->setCurrentItem(newItem); // Trigger currentItemChanged()

	this->expandItem(newItem);  // Trigger itemExpanded()

	// Check if plan tree needs to be locked
	if (getTreeModel()->IsPlanTreeLocked())
	{
		// Fing the locked category, expanding the tree as necessary.
		TreeNodeId lockCategoryId = getTreeModel()->GetLockedCategoryId();
		CtlTreeWidgetItem* treeItem = topLevelItem(lockCategoryId);

		// If the locked category is not found, then that means the tree has not been populated yet.
		if (treeItem)
		{
			setCurrentItem(treeItem);
			expandItem(treeItem);
			// Now the tree access guard can be activated.
			m_guardTreeAccess = true;
		}
	}

	QApplication::restoreOverrideCursor();

	FUNCTION_EXIT;
}