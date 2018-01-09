/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/CategoryNodeView.h $
* @author:  Bart Golab
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2017/05/09 13:33:55 $
* Last modified by:  $Author: hoa.le $
*
* Process right pane user interactive and update QTableWidget view accordingly.
*/

#ifndef CATEGORYNODEVIEW_H
#define CATEGORYNODEVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

#include "bus/common_control/src/itemview/CtlTableWidget.h"

namespace TA_Base_App
{
	class PlanTreeFrame;
	class PlanTreeModel;
	class TreeNode;
	class CategoryNode;

	/////////////////////////////////////////////////////////////////////////////
	// CategoryNodeView view

	class CategoryNodeView : public CtlTableWidget
	{
		Q_OBJECT

		// Operations
	public:
		CategoryNodeView(PlanTreeFrame* parent = NULL);
		virtual ~CategoryNodeView();

		/**
		* getTreeModel
		*
		* Get PlanTreeFrame PModel for tree node modifications
		*
		* @return PlanTreeModel  Pointer to PlanTreeModel
		*/
		PlanTreeModel* getTreeModel();

	public slots:

		/**
		* onResetCategoryNodeView
		*
		* Initialise the setup CtlTableWidget
		*
		* @param treeDoc  Pointer to PlanTreeModel
		*/
		void onResetCategoryNodeView(PlanTreeModel* treeDoc);

		/**
		* onCategoryListItemSelectionChanged
		*
		* React to itemSelectionChanged event and update GUI
		*/
		void onCategoryListItemSelectionChanged();

	signals:
		/// Signal for changing the behaviour of menu's items and toolbar's buttons
		void pfCategoryViewSelected(TreeNode* treeNode);

		//protected:
		//    virtual void ProcessPlanApprovalStateUpdate(const TA_Base_Core::ApprovalStateUpdate &approvalStateUpdate);
		//    virtual void ProcessModifyNodeUpdate(const TA_Base_Core::NodeSummary &nodeSummary, std::string sessionId);
		//    virtual void ProcessInsertNodeUpdate(const TA_Base_Core::NodeSummary &nodeSummary, std::string sessionId);
		//    virtual void ProcessInsertNodeIntoRootUpdate(const TA_Base_Core::NodeSummary &nodeSummary, std::string sessionId);
		//    virtual void ProcessMoveNodeUpdate(const TA_Base_Core::TransferNodeUpdate &transferNodeUpdate, std::string sessionId);
		//    virtual void ProcessMoveNodeToRootUpdate(const TA_Base_Core::TransferNodeUpdate &transferNodeUpdate, std::string sessionId);
		//    virtual void ProcessMoveNodeFromRootUpdate(const TA_Base_Core::TransferNodeUpdate &transferNodeUpdate, std::string sessionId);
		//    virtual void ProcessDeleteNodeUpdate(const TA_Base_Core::DeleteNodeUpdate &deleteNodeUpdate);
		//    virtual void ProcessDeleteNodeFromRootUpdate(const TA_Base_Core::NodeIds &nodeIds);
		//    virtual void ProcessDeleteNodeFromRootUpdate(TreeNodeId nodeId);

		//protected:
		//    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
		//    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
		//    virtual void PostNcDestroy();

	protected:

		//QTTODO2: find Qt equivalent slots
		//void OnCategoryListItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
		//void OnCategoryListItemDblclk(NMHDR* pNMHDR, LRESULT* pResult);
		//LRESULT OnViewShown(WPARAM wParam, LPARAM lParam);

		//int FindNode(TreeNode *treeNode);  // returns -1 if node is not found
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);

	private:
		/**
		* addChildrenToList
		*
		* Add child nodes to CtlTableWidget's item
		*
		* @param treeNode  Pointer to TreeNode
		*
		* @param tableWidget  Reference to CtlTableWidget
		*/
		void addChildrenToList(TreeNode* treeNode, CtlTableWidget &tableWidget);

		/**
		* addToList
		*
		* Add a child node to CtlTableWidget
		*
		* @param treeNode  Pointer to TreeNode
		*
		* @param tableWidget  Reference to CtlTableWidget
		*/
		void addToList(TreeNode* treeNode, CtlTableWidget &tableWidget);

		/**
		* addRowItem
		*
		* Add a cell to CtlTableWidget's row
		*
		* @param treeNode  Pointer to TreeNode
		*
		* @param tableWidget  Reference to CtlTableWidget
		*
		* @param row  Row index
		*
		* @param column  Column index
		*
		* @param text  Cell label
		*/
		CtlTableWidgetItem* addRowItem(TreeNode* treeNode, CtlTableWidget& tableWidget, int row, int column, std::string text);

		/**
		* getCategoryNode
		*
		* Get PlanTreeFrame PModel for tree node modifications
		*
		* @return PlanTreeModel  Pointer to PlanTreeModel
		*/
		CategoryNode* getCategoryNode();

		//void RemoveNode(TreeNode *treeNode);

	private:
		PlanTreeFrame* m_pTreeFrame; /// Store parent's widget
		QPoint origin; /// Point of the mouse
		QRubberBand *rubberBand; /// Rectangle selection of mouse
	};

	/////////////////////////////////////////////////////////////////////////////
} // namespace TA_Base_App

#endif // !defined(AFX_CATEGORYNODEVIEW_H__F2270A61_6B13_4F81_8449_21F1CCDB26FB__INCLUDED_)
