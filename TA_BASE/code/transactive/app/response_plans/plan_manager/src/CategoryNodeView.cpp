/*
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/CategoryNodeView.cpp $
* @author:  Bart Golab
* @version: $Revision: #3 $
*
* Last modification: $DateTime: 2017/05/22 15:28:33 $
* Last modified by:  $Author: hoa.le $
*
* Process right pane user interactive and update QTableWidget view accordingly.
*/

#include "app/response_plans/plan_manager/src/CategoryNodeView.h"

#include <sstream>

#include "app/response_plans/plan_manager/pmodel/src/PlanTreeModel.h"
#include "app/response_plans/plan_manager/pmodel/src/CategoryNode.h"
#include "app/response_plans/plan_manager/pmodel/src/StringDefn.h"

#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"

#include "app/response_plans/plan_manager/src/PlanTreeFrame.h"
#include "app/response_plans/plan_manager/src/MessageBox.h"
#include "app/response_plans/plan_manager/src/TableDelegate.h"

using namespace TA_Base_App;

/////////////////////////////////////////////////////////////////////////////
// CategoryNodeView

CategoryNodeView::CategoryNodeView(PlanTreeFrame* parent)
: CtlTableWidget(qobject_cast<QWidget*>(parent)), m_pTreeFrame(parent)
{
	FUNCTION_ENTRY("CategoryNodeView");

	m_pTreeFrame = parent;

	// Initialise table widget
	this->setShowGrid(false);
	this->setColumnCount(3);
	this->verticalHeader()->close();

	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->setSelectionMode(QAbstractItemView::ExtendedSelection);
	this->setSelectionBehavior(QAbstractItemView::SelectItems);

	rubberBand = NULL;

	connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(onCategoryListItemSelectionChanged()));

	//PlanManagerFrame* pmf = dynamic_cast<PlanManagerFrame*>(parent->m_pWinApp->getViewManager()->getMainView());
	////connect(view, SIGNAL(categoryViewSelected(TreeNode*)), dynamic_cast<PlanManagerFrame*>(m_pWinApp->getViewManager()->getMainView()), SLOT(onCategoryViewSelectionChanged(TreeNode*)));
	connect(this, SIGNAL(pfCategoryViewSelected(TreeNode*)), parent, SLOT(onCategoryViewSelectionChanged(TreeNode*)));

	FUNCTION_EXIT;
}

CategoryNodeView::~CategoryNodeView()
{
	FUNCTION_ENTRY("~CategoryNodeView");

	getTreeModel()->PurgeCategoryViewNodes();

	FUNCTION_EXIT;
}

//CCategoryListCtrl& CategoryNodeView::GetListCtrl()
//{
//    FUNCTION_ENTRY( "GetListCtrl" );
//    FUNCTION_EXIT;
//    return m_categoryListCtrl;
//}

PlanTreeModel* CategoryNodeView::getTreeModel()
{
	FUNCTION_ENTRY("GetDocument");

	PlanTreeModel *planTreeModel = m_pTreeFrame->getTreeModel();
	TA_ASSERT(planTreeModel != NULL, "Associated document is not a PlanTreeModel");

	FUNCTION_EXIT;
	return planTreeModel;
}

CategoryNode *CategoryNodeView::getCategoryNode()
{
	FUNCTION_ENTRY("getCategoryNode");

	CategoryNode *categoryNode = dynamic_cast<CategoryNode *>(getTreeModel()->GetCurrentNode());
	TA_ASSERT(categoryNode != NULL, "Category node pointer is null");

	FUNCTION_EXIT;
	return categoryNode;
}

//void CategoryNodeView::RemoveNode(TreeNode *treeNode)
//{
//	int item = FindNode(treeNode);
//	if (item != -1)
//	{
//		DeleteItem(item);
//	}
//}
//
//int CategoryNodeView::FindNode(TreeNode *treeNode)
//{
//	LVFINDINFO lvfi;
//
//	lvfi.flags = LVFI_PARAM;
//	lvfi.lParam = (LPARAM)treeNode;
//
//	return FindItem(&lvfi);
//}

//BEGIN_MESSAGE_MAP(CategoryNodeView, CView)
//    // {{AFX_MSG_MAP(CategoryNodeView)
//    ON_WM_CREATE()
//    ON_WM_SIZE()
//    ON_NOTIFY(LVN_ITEMCHANGED, IDC_CATEGORY_CONTENT_LIST, OnCategoryListItemChanged)
//    ON_NOTIFY(NM_DBLCLK, IDC_CATEGORY_CONTENT_LIST, OnCategoryListItemDblclk)
//    // }}AFX_MSG_MAP
//    ON_MESSAGE(WM_WINDOW_HAS_BECOME_VISIBLE, OnViewShown)
//    ON_MESSAGE(WM_RESET_TREE_NODE_VIEW, onResetCategoryNodeView)
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CategoryNodeView message handlers

void CategoryNodeView::onCategoryListItemSelectionChanged()
{
	QList<QTableWidgetItem *> items = this->selectedItems();
	QTableWidgetItem* item = NULL;
	TreeNode *treeNode = NULL;
	if (!items.isEmpty())
	{
		getTreeModel()->PurgeCategoryViewNodes();

		foreach(item, items)
		{
			CtlTableWidgetItem* ctlItem = dynamic_cast<CtlTableWidgetItem*>(item);
			treeNode = static_cast<TreeNode*>(ctlItem->getDataPtr());
			TA_ASSERT(treeNode, "Tree node is null");
			getTreeModel()->addToCategoryViewNodes(treeNode);
		}
		emit pfCategoryViewSelected(treeNode);
	}
}

//void CategoryNodeView::OnCategoryListItemDblclk(NMHDR* pNMHDR, LRESULT* pResult)
//{
//    FUNCTION_ENTRY( "OnCategoryListItemDblclk" );
//
//    // If the selected nodes are not all plans, the double click has no effect.
//    if (!GetDocument()->ArePlanNodesSelected())
//    {
//        *pResult = 0;
//
//        FUNCTION_EXIT;
//        return;
//    }
//
//    TreeNodeList selectedNodes = GetDocument()->GetSelectedNodes();
//
//    bool selectedNodesAllApproved = !selectedNodes.empty();
//    bool selectedNodesAllUnapproved = !selectedNodes.empty();
//
//    // Iterate over selected nodes and determine whether they are all approved or unapproved plans.
//    // Where a mixture of approved and unapproved plans is selected, the double click has no effect.
//    for (TreeNodeList::iterator iter = selectedNodes.begin(); iter != selectedNodes.end(); iter++)
//    {
//        TreeNode *treeNode = *iter;
//
//        selectedNodesAllApproved = selectedNodesAllApproved && treeNode->isApproved();
//        selectedNodesAllUnapproved = selectedNodesAllUnapproved && !treeNode->isApproved();
//    }
//
//    if (selectedNodesAllApproved)
//    {
//        // For approved plans launch the Plan Controller
//        GetParentFrame()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_PLAN_CONTROL, 0), (LPARAM) NULL);
//    }
//    else if (selectedNodesAllUnapproved)
//    {
//        // For unapproved plans launch the Plan Editor
//        GetParentFrame()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_PLAN_EDIT, 0), (LPARAM) NULL);
//    }
//
//    *pResult = 0;
//
//    FUNCTION_EXIT;
//}

//LRESULT CategoryNodeView::OnViewShown(WPARAM wParam, LPARAM lParam)
//{
//    FUNCTION_ENTRY( "OnViewShown" );
//
//    // User switched from another plan tree view - update main toolbar according to current selection
//    AfxGetMainWnd()->SendMessage(WM_CATEGORY_VIEW_SELECTION_CHANGED);
//
//    FUNCTION_EXIT;
//    return 0;
//}

//void CategoryNodeView::onItemSelectionChanged() // Not needed
//{
//	CtlTableWidgetItem* itemRow = dynamic_cast<CtlTableWidgetItem*>(this->currentItem());
//	QVariant itemData = itemRow->data(Qt::UserRole);
//
//	TreeNode* treeNode = itemData.value<TreeNode*>();
//	TA_ASSERT(treeNode, "Tree node pointer is null");
//
//	if (dynamic_cast<CategoryNode*>(treeNode))
//	{
//		this->clearSelection();
//		this->clearFocus();
//	}
//}

void CategoryNodeView::mousePressEvent(QMouseEvent *event)
{
	QAbstractItemView::mousePressEvent(event);
	origin = event->pos();
	if (!rubberBand)
		rubberBand = new QRubberBand(QRubberBand::Rectangle, viewport());
	rubberBand->setGeometry(QRect(origin, QSize()));
	rubberBand->show();
}

void CategoryNodeView::mouseMoveEvent(QMouseEvent *event)
{
	if (rubberBand)
		rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
	QAbstractItemView::mouseMoveEvent(event);
}

void CategoryNodeView::mouseReleaseEvent(QMouseEvent *event)
{
	QAbstractItemView::mouseReleaseEvent(event);
	if (rubberBand)
		rubberBand->hide();
	viewport()->update();
}

CtlTableWidgetItem* CategoryNodeView::addRowItem(TreeNode* treeNode, CtlTableWidget& tableWidget, int row, int column, std::string text)
{
	CtlTableWidgetItem* newItem = new CtlTableWidgetItem();
	newItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	tableWidget.setItem(row, column, newItem);
	// Display string according to locale
	newItem->setText(QTextCodec::codecForLocale()->toUnicode(text.c_str()).toStdString());

	if ((column > 0) || (dynamic_cast<CategoryNode*>(treeNode) && column == 0))
	{
		newItem->setFlags(Qt::ItemIsEnabled);
	}

	newItem->setDataPtr(treeNode);

	return newItem;
}

void CategoryNodeView::addToList(TreeNode* treeNode, CtlTableWidget &tableWidget)
{
	FUNCTION_ENTRY("addToList");

	if (m_pTreeFrame->getTreeModel() && !m_pTreeFrame->getTreeModel()->isRelevant(*treeNode))
	{
		FUNCTION_EXIT;
		return;
	}

	int row = 0;
	for (int i = 0; i < tableWidget.rowCount(); i++)
	{
		CtlTableWidgetItem* itemRow = dynamic_cast<CtlTableWidgetItem*>(tableWidget.item(row, 0));
		TreeNode* siblingNode = static_cast<TreeNode*>(itemRow->getDataPtr());
		TA_ASSERT(siblingNode, "Sibling node pointer is null");

		// See if node is already in the list
		if (siblingNode == treeNode)
		{
			FUNCTION_EXIT;
			return;
		}

		// See if node should go below its sibling.
		if (siblingNode->hasPrecedence(*treeNode))
		{
			row++;  // this node must go below its sibling
		}
		else
		{
			break;
		}
	}

	tableWidget.insertRow(row);
	CtlTableWidgetItem* newItem = addRowItem(treeNode, tableWidget, row, 0, treeNode->getNodeName());
	newItem->setIcon(QIcon(QString(treeNode->getNodeImageType(false).c_str())));
	addRowItem(treeNode, tableWidget, row, 1, treeNode->getNodeType());
	addRowItem(treeNode, tableWidget, row, 2, treeNode->getNodeStatus());

	FUNCTION_EXIT;
}

void CategoryNodeView::addChildrenToList(TreeNode* treeNode, CtlTableWidget &tableWidget)
{
	FUNCTION_ENTRY("addChildrenToList");

	TreeNodeList childNodes;
	CategoryNode* catNode = dynamic_cast<CategoryNode*>(treeNode);
	if (catNode)
	{
		catNode->getChildren(childNodes);

		for (TreeNodeList::iterator iter = childNodes.begin(); iter != childNodes.end(); iter++)
		{
			TreeNode *childNode = (TreeNode *)*iter;
			addToList(childNode, tableWidget);
		}
	}

	FUNCTION_EXIT;
}

void CategoryNodeView::onResetCategoryNodeView(PlanTreeModel* treeDoc)
{
	FUNCTION_ENTRY("onResetCategoryNodeView");

	treeDoc->PurgeCategoryViewNodes();
	CategoryNode *categoryNode = getCategoryNode();

	try
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);

		this->clear();
		this->setRowCount(0);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		this->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
		this->setSortingEnabled(false);
		// Remove focus from table items
		this->setItemDelegate(new TableDelegate());

		QStringList labels;
		labels << tr(IDS_NODE_LIST_NAME_COLUMN)
			<< tr(IDS_NODE_LIST_TYPE_COLUMN)
			<< tr(IDS_NODE_LIST_STATUS_COLUMN);
		this->setHorizontalHeaderLabels(labels);
		this->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

		addChildrenToList(categoryNode, *qobject_cast<CtlTableWidget*>(this));

		this->resizeColumnsToContents();
		this->setColumnWidth(0, this->columnWidth(0) * 2);
		this->setColumnWidth(1, this->columnWidth(1) * 2);
		this->setColumnWidth(2, this->columnWidth(2) * 2);

		//this->horizontalHeader()->setDefaultSectionSize(this->horizontalHeader()->minimumSectionSize() * 5);
		this->resizeRowsToContents();
		this->verticalHeader()->setDefaultSectionSize(this->verticalHeader()->minimumSectionSize());

		QApplication::restoreOverrideCursor();
	}
	catch (TA_Base_Core::TransactiveException  &ex)
	{
		MessageBox::error(IDS_DISPLAY_CATEGORY, ex);
	}

	FUNCTION_EXIT;
}

//void CategoryNodeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
//{
//    FUNCTION_ENTRY( "OnUpdate" );
//
//    // TODO: Add your specialized code here and/or call the base class
//    CViewUpdate<TA_Base_Core::PlanConfigUpdateData> *pcdUpdate = dynamic_cast<CViewUpdate<TA_Base_Core::PlanConfigUpdateData> *>(pHint);
//    if (pcdUpdate)
//    {
//        TA_Base_Core::PlanConfigUpdateData planConfigData = pcdUpdate->GetUpdateData();
//
//        switch (planConfigData.configUpdate._d())
//        {
//            case TA_Base_Core::PCUT_APPROVAL_STATE:
//                ProcessPlanApprovalStateUpdate(planConfigData.configUpdate.approvalState());
//                break;
//
//            case TA_Base_Core::PCUT_UPDATE_PLAN:
//                ProcessModifyNodeUpdate(planConfigData.configUpdate.updatePlan(), planConfigData.session.in());
//                break;
//
//            case TA_Base_Core::PCUT_INSERT_PLAN:
//                ProcessInsertNodeUpdate(planConfigData.configUpdate.insertPlan(), planConfigData.session.in());
//                break;
//
//            case TA_Base_Core::PCUT_INSERT_PLAN_INTO_ROOT:
//                ProcessInsertNodeIntoRootUpdate(planConfigData.configUpdate.insertPlanIntoRoot(), planConfigData.session.in());
//                break;
//
//            case TA_Base_Core::PCUT_COPY_PLAN: // a plan copy is essentially an insert
//                ProcessInsertNodeUpdate(planConfigData.configUpdate.copyPlan(), planConfigData.session.in());
//                break;
//
//            case TA_Base_Core::PCUT_COPY_PLAN_TO_ROOT:
//                ProcessInsertNodeIntoRootUpdate(planConfigData.configUpdate.copyPlanToRoot(), planConfigData.session.in());
//                break;
//
//            case TA_Base_Core::PCUT_MOVE_PLAN:
//                ProcessMoveNodeUpdate(planConfigData.configUpdate.movePlan(), planConfigData.session.in());
//                break;
//
//            case TA_Base_Core::PCUT_MOVE_PLAN_TO_ROOT:
//                ProcessMoveNodeToRootUpdate(planConfigData.configUpdate.movePlanToRoot(), planConfigData.session.in());
//                break;
//
//            case TA_Base_Core::PCUT_MOVE_PLAN_FROM_ROOT:
//                ProcessMoveNodeFromRootUpdate(planConfigData.configUpdate.movePlanFromRoot(), planConfigData.session.in());
//                break;
//
//            case TA_Base_Core::PCUT_DELETE_PLANS:
//                ProcessDeleteNodeUpdate(planConfigData.configUpdate.deletePlans());
//                break;
//
//            case TA_Base_Core::PCUT_DELETE_PLANS_FROM_ROOT:
//                ProcessDeleteNodeFromRootUpdate(planConfigData.configUpdate.deletePlansFromRoot());
//                break;
//
//            case TA_Base_Core::PCUT_UPDATE_CATEGORY:
//                ProcessModifyNodeUpdate(planConfigData.configUpdate.updateCategory(), planConfigData.session.in());
//                break;
//
//            case TA_Base_Core::PCUT_INSERT_CATEGORY:
//                ProcessInsertNodeUpdate(planConfigData.configUpdate.insertCategory(), planConfigData.session.in());
//                break;
//
//            case TA_Base_Core::PCUT_INSERT_CATEGORY_INTO_ROOT:
//                ProcessInsertNodeIntoRootUpdate(planConfigData.configUpdate.insertCategoryIntoRoot(), planConfigData.session.in());
//                break;
//
//            case TA_Base_Core::PCUT_DELETE_CATEGORY:
//                ProcessDeleteNodeUpdate(planConfigData.configUpdate.deleteCategory());
//                break;
//
//            case TA_Base_Core::PCUT_DELETE_CATEGORY_FROM_ROOT:
//                ProcessDeleteNodeFromRootUpdate(planConfigData.configUpdate.deleteCategoryFromRoot());
//                break;
//
//            default:
//                FUNCTION_EXIT;
//                return;
//        }
//
//        // Force a refresh of the plan tree
//        Invalidate();
//    }
//
//    FUNCTION_EXIT;
//}

//void CategoryNodeView::ProcessPlanApprovalStateUpdate(const TA_Base_Core::ApprovalStateUpdate &approvalStateUpdate)
//{
//    FUNCTION_ENTRY( "ProcessPlanApprovalStateUpdate" );
//
//    TreeNode *treeNode = TreeNodeFactory::getInstance().getTreeNode(approvalStateUpdate.plan);
//    if (treeNode)
//    {
//        // If the node does not belong to the current category we'll need to have a look further down
//        if (treeNode->getNodeSummary().parent != getCategoryNode()->getNodeId())
//        {
//            treeNode = NULL;
//        }
//    }
//
//    if (!treeNode)
//    {
//        try
//        {
//            // The node could belong to a descendant of the current category.
//            TreeNodeIdList nodeIds;
//            TreeNodeFactory::getInstance().getNodeIdsInNodePath(approvalStateUpdate.plan, nodeIds);
//
//            // skip the plan node ID
//            for (unsigned int i = 0; i < (nodeIds.size() - 1); i++)
//            {
//                if (nodeIds[i] == getCategoryNode()->getNodeId())
//                {
//                    // Get the next sub-category in the path
//                    treeNode = TreeNodeFactory::getInstance().getTreeNode(nodeIds[i+1]);
//
//                    break;
//                }
//            }
//        }
//        catch (TA_Base_Core::TransactiveException  &ex)
//        {
//            std::ostringstream errorMessage;
//            MessageBox::error(IDS_SEARCH_CATEGORY_PATH, ex);
//        }
//    }
//
//    if (treeNode)
//    {
//        // Add or remove the node from the view as per the filter. If the node is already part of the view,
//        // it remains untouched.
//        if (GetDocument()->isRelevant(*treeNode))
//        {
//            treeNode->addToList(GetListCtrl(), GetDocument());
//        }
//        else
//        {
//            GetListCtrl().RemoveNode(treeNode);
//        }
//    }
//
//    FUNCTION_EXIT;
//}

//void CategoryNodeView::ProcessModifyNodeUpdate(const TA_Base_Core::NodeSummary &nodeSummary, std::string sessionId)
//{
//    FUNCTION_ENTRY( "ProcessModifyNodeUpdate" );
//
//    // If the node belongs to the current category
//    if (nodeSummary.parent == getCategoryNode()->getNodeId())
//    {
//        // Sort the contents of the list in case the node has been renamed
//        GetListCtrl().SortItems(TreeNode::compareSiblingNodes, 0);
//    }
//
//    FUNCTION_EXIT;
//}

//void CategoryNodeView::ProcessInsertNodeUpdate(const TA_Base_Core::NodeSummary &nodeSummary, std::string sessionId)
//{
//    FUNCTION_ENTRY( "ProcessInsertNodeUpdate" );
//
//    // If the node belongs to the current category
//    if (nodeSummary.parent == getCategoryNode()->getNodeId())
//    {
//        TreeNode *treeNode = TreeNodeFactory::getInstance().getTreeNode(nodeSummary.node);
//        TA_ASSERT(treeNode, "Tree node does not exist");
//
//        treeNode->addToList(GetListCtrl(), GetDocument());
//
//        FUNCTION_EXIT;
//        return;
//    }
//
//    try
//    {
//        // The node could belong to a descendant of the current category.
//        TreeNodeIdList nodeIds;
//        TreeNodeFactory::getInstance().getNodeIdsInNodePath(nodeSummary.parent, nodeIds);
//
//        for (unsigned int i = 0; i < (nodeIds.size() - 1); i++)
//        {
//            if (nodeIds[i] == getCategoryNode()->getNodeId())
//            {
//                // Get the next sub-category in the path
//                TreeNode *treeNode = TreeNodeFactory::getInstance().getTreeNode(nodeIds[i+1]);
//                TA_ASSERT(treeNode, "Tree node does not exist");
//
//                treeNode->addToList(GetListCtrl(), GetDocument());
//
//                break;
//            }
//        }
//    }
//    catch (TA_Base_Core::TransactiveException  &ex)
//    {
//        MessageBox::error(IDS_SEARCH_CATEGORY_PATH, ex);
//    }
//
//    FUNCTION_EXIT;
//}

//void CategoryNodeView::ProcessInsertNodeIntoRootUpdate(const TA_Base_Core::NodeSummary &nodeSummary, std::string sessionId)
//{
//    FUNCTION_ENTRY( "ProcessInsertNodeIntoRootUpdate" );
//
//    // If current category is a root one
//    if (dynamic_cast<RootNode *>(getCategoryNode()) != NULL)
//    {
//        TreeNode *treeNode = TreeNodeFactory::getInstance().getTreeNode(nodeSummary.node);
//        TA_ASSERT(treeNode, "Tree node does not exist");
//
//        treeNode->addToList(GetListCtrl(), GetDocument());
//    }
//
//    FUNCTION_EXIT;
//}

//void CategoryNodeView::ProcessMoveNodeUpdate(const TA_Base_Core::TransferNodeUpdate &transferNodeUpdate, std::string sessionId)
//{
//    FUNCTION_ENTRY( "ProcessMoveNodeUpdate" );
//
//    // A node move is essentially a delete followed by an insert.
//    TA_Base_Core::DeleteNodeUpdate deleteNodeUpdate;
//    deleteNodeUpdate.parent = transferNodeUpdate.oldParent;
//    deleteNodeUpdate.nodes.length(1);
//    deleteNodeUpdate.nodes[0] = transferNodeUpdate.summary.node;
//
//    ProcessDeleteNodeUpdate(deleteNodeUpdate);
//    ProcessInsertNodeUpdate(transferNodeUpdate.summary, sessionId);
//
//    FUNCTION_EXIT;
//}

//void CategoryNodeView::ProcessMoveNodeToRootUpdate(const TA_Base_Core::TransferNodeUpdate &transferNodeUpdate, std::string sessionId)
//{
//    FUNCTION_ENTRY( "ProcessMoveNodeToRootUpdate" );
//
//    // A node move is essentially a delete followed by an insert.
//    TA_Base_Core::DeleteNodeUpdate deleteNodeUpdate;
//    deleteNodeUpdate.parent = transferNodeUpdate.oldParent;
//    deleteNodeUpdate.nodes.length(1);
//    deleteNodeUpdate.nodes[0] = transferNodeUpdate.summary.node;
//
//    ProcessDeleteNodeUpdate(deleteNodeUpdate);
//    ProcessInsertNodeIntoRootUpdate(transferNodeUpdate.summary, sessionId);
//
//    FUNCTION_EXIT;
//}

//void CategoryNodeView::ProcessMoveNodeFromRootUpdate(const TA_Base_Core::TransferNodeUpdate &transferNodeUpdate, std::string sessionId)
//{
//    FUNCTION_ENTRY( "ProcessMoveNodeFromRootUpdate" );
//
//    // A node move is essentially a delete followed by an insert.
//    ProcessDeleteNodeFromRootUpdate(transferNodeUpdate.summary.node);
//    ProcessInsertNodeUpdate(transferNodeUpdate.summary, sessionId);
//
//    FUNCTION_EXIT;
//}

//void CategoryNodeView::ProcessDeleteNodeUpdate(const TA_Base_Core::DeleteNodeUpdate &deleteNodeUpdate)
//{
//    FUNCTION_ENTRY( "ProcessDeleteNodeUpdate" );
//
//    // If the node(s) were deleted from the current category
//    if (deleteNodeUpdate.parent == getCategoryNode()->getNodeId())
//    {
//        for (CORBA::ULong i = 0; i < deleteNodeUpdate.nodes.length(); i++)
//        {
//            TreeNode *treeNode = TreeNodeFactory::getInstance().getTreeNode(deleteNodeUpdate.nodes[i]);
//            TA_ASSERT(treeNode, "Tree node does not exist");
//
//            GetListCtrl().RemoveNode(treeNode);
//        }
//
//        FUNCTION_EXIT;
//        return;
//    }
//
//    try
//    {
//        // The node could belong to a descendant of the current category.
//        TreeNodeIdList nodeIds;
//        TreeNodeFactory::getInstance().getNodeIdsInNodePath(deleteNodeUpdate.parent, nodeIds);
//
//        for (unsigned int i = 0; i < (nodeIds.size() - 1); i++)
//        {
//            if (nodeIds[i] == getCategoryNode()->getNodeId())
//            {
//                // Get the next sub-category in the path
//                TreeNode *treeNode = TreeNodeFactory::getInstance().getTreeNode(nodeIds[i+1]);
//                TA_ASSERT(treeNode, "Tree node does not exist");
//
//                if (!GetDocument()->isRelevant(*treeNode))
//                {
//                    GetListCtrl().RemoveNode(treeNode);
//                }
//
//                break;
//            }
//        }
//    }
//    catch (TA_Base_Core::TransactiveException  &ex)
//    {
//        MessageBox::error(IDS_SEARCH_CATEGORY_PATH, ex);
//    }
//
//    FUNCTION_EXIT;
//}

//void CategoryNodeView::ProcessDeleteNodeFromRootUpdate(const TA_Base_Core::NodeIds &nodeIds)
//{
//    FUNCTION_ENTRY( "ProcessDeleteNodeFromRootUpdate" );
//
//    for (CORBA::ULong i = 0; i < nodeIds.length(); i++)
//    {
//        ProcessDeleteNodeFromRootUpdate(nodeIds[i]);
//    }
//
//    FUNCTION_EXIT;
//}

//void CategoryNodeView::ProcessDeleteNodeFromRootUpdate(TreeNodeId nodeId)
//{
//    FUNCTION_ENTRY( "ProcessDeleteNodeFromRootUpdate" );
//
//    // If current category is a root one
//    if (dynamic_cast<RootNode *>(getCategoryNode()) != NULL)
//    {
//        TreeNode *treeNode = TreeNodeFactory::getInstance().getTreeNode(nodeId);
//        TA_ASSERT(treeNode, "Tree node does not exist");
//
//        GetListCtrl().RemoveNode(treeNode);
//    }
//
//    FUNCTION_EXIT;
//}