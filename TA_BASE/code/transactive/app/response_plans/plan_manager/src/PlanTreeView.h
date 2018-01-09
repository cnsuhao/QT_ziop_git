/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanTreeView.h $
* @author:  Bart Golab
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2017/05/09 13:33:55 $
* Last modified by:  $Author: hoa.le $
*
* Subclass of CtlTreeWidget to display and manipulate the QTreeWidget.
*/

#ifndef PLANTREEVIEW_H
#define PLANTREEVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <qtreewidget.h>

#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/common_control/src/itemview/CtlTreeWidget.h"

#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"

namespace TA_Base_App
{
	class PlanTreeFrame;
	class PlanTreeModel;
	class TreeNode;

	/////////////////////////////////////////////////////////////////////////////
	// PlanTreeView tree view

	class PlanTreeView : public CtlTreeWidget
		//, public TA_Base_Bus::AbstractTransActiveView
	{
		Q_OBJECT
		// Operations
	public:
		PlanTreeView(EPlanTreeType ePlanType, PlanTreeFrame* planTreeFrame);
		virtual ~PlanTreeView();

		/**
		* getTreeModel
		*
		* Get PModel for this Plan type
		*
		* @return PlanTreeModel  Pointer to PlanTreeModel
		*/
		PlanTreeModel* getTreeModel();

		//virtual void OnInitialUpdate(); // called first time after construct // set imagelist to AllPlan, Approved, Unapproved
		//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);  //QTTODO: // General drawing/updating **can use QWidget::update()???

	public slots :
		/**
		* onViewShown
		*
		* Initialise tree widget display
		*/
		void onViewShown();

		/**
		* onTreeItemSelected
		*
		* Process tree widget item selection
		*
		* @param current  Pointer to current QTreeWidgetItem
		* @param previous  Pointer to preivious QTreeWidgetItem
		*/
		void onTreeItemSelected(QTreeWidgetItem* current, QTreeWidgetItem* previous);

		/**
		* onTreeItemExpanded
		*
		* Process tree widget item expanded
		*
		* @param item  Pointer to current QTreeWidgetItem
		*/
		void onTreeItemExpanded(QTreeWidgetItem* item);

	signals:
		/// Signal to change menu items and toolbar buttons 
		void mfTreeNodeChanged(TreeNode* treeNode);
		/// Signal to change menu items and toolbar buttons 
		void pfTreeNodeChanged(TreeNode* treeNode);

	protected:

		/**
		* ProcessRparamLockCategoryUpdate
		*
		* This will have the effect of populating the root category and, if necessary,
		* locking the tree at a specific sub-category.
		*/
		virtual void ProcessRparamLockCategoryUpdate();

	private:

		/**
		* addToTree
		*
		* Add a child node to QTreeWidget
		*
		* @param treeNode  Pointer to TreeNode
		*
		* @param treeWidget  Reference to CtlTreeWidget
		*
		* @param CtlTreeWidgetItem  Pointer to CtlTreeWidgetItem
		*
		* @return CtlTreeWidgetItem*  Pointer to added item to tree widget
		*/
		CtlTreeWidgetItem* addToTree(TreeNode* treeNode, CtlTreeWidget &treeWidget, CtlTreeWidgetItem* parent = NULL);

		/**
		* addChildrenToTree
		*
		* Add child nodes to QTreeWidget's item
		*
		* @param treeNode  Pointer to TreeNode
		*
		* @param treeWidget  Reference to CtlTreeWidget
		*
		* @param parent  Pointer to CtlTreeWidgetItem
		*/
		void addChildrenToTree(TreeNode* treeNode, CtlTreeWidget &treeWidget, CtlTreeWidgetItem* parent);

	private:
		PlanTreeFrame* m_planTreeFrame; /// Pointer to access PModel object
		bool m_guardTreeAccess; /// Is tree access restricted to a locked category and its descendants
		EPlanTreeType m_ePlanType; /// Store Plan type for processing
	};

	/////////////////////////////////////////////////////////////////////////////
} // namespace TA_Base_App

#endif // PLANTREEVIEW_H
