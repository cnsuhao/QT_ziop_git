/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanTreeFrame.h $
* @author:  Bart Golab
* @version: $Revision: #3 $
*
* Last modification: $DateTime: 2017/05/22 15:28:33 $
* Last modified by:  $Author: hoa.le $
*
* This class is a container widget for the MDI documents.
*/

#ifndef PLANTREEFRAME_H
#define PLANTREEFRAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <qsplitter.h>

#include "bus/generic_gui_view/src/AbstractTransActiveView.h"

#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"

#include "app/response_plans/plan_manager/pmodel/src/PlanTreeModel.h"

#include "app/response_plans/plan_manager/src/PlanBrowserFrame.h"

namespace TA_Base_App
{
	class PlanTreeView;
	class PlanTreeModel;
	class TreeNode;
	class CategoryNodeView;
	class PlanNodeView;

	/////////////////////////////////////////////////////////////////////////////
	// PlanTreeFrame frame

	class PlanTreeFrame : public PlanBrowserFrame,
		public TA_Base_Bus::AbstractTransActiveFormView
	{
		Q_OBJECT

	public:
		PlanTreeFrame(TA_Base_Bus::ITransActiveApp* app, const char* viewType);
		virtual ~PlanTreeFrame();

		virtual std::string getMyPModelName();
		void setupPModel();
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args = NULL);

		/**
		* getLeftPane
		*
		* Get QTreeidget from left pane of QSplitter
		*
		* @return PlanTreeView*  Pointer to PlanTreeview object
		*/
		PlanTreeView* getLeftPane();

		/**
		* getTreeModel
		*
		* Get PModel object to for implementing application logic
		*
		* @return PlanTreeModel*  Pointer to PlanTreeModel object
		*/
		PlanTreeModel* getTreeModel();

		/**
		* createMdiSubWindow
		*
		* Setup the MDI's initial document view
		*
		* @param ptreeView  Pointer to PlanTreeView  
		*
		* @return bool  True if document view is created, false otherwise
		*/
		bool createMdiSubWindow(PlanTreeView* ptreeView);

		/**
		* registerSettings
		*
		* Register for active plan, plan customisation and plan configuration updates.
		* Register for ExecPlanId, LockCategory and SessionId runtime parameter updates.
		*/
		//virtual void registerSettings();

		void onViewShown();

	public slots:
		/**
		* onTreeNodeSelectionChanged
		*
		* Change GUI according to category or plan node selected
		*
		* @param treeNode  Pointer to TreeNode
		*/
		void onTreeNodeSelectionChanged(TreeNode* treeNode);
		void onCategoryViewSelectionChanged(TreeNode* treeNode);

	signals:
		/// Signal for building category list on the right pane of QSplitter
		void categoryResetTreeNodeView(PlanTreeModel* treeModel);
		/// Signal for changing toggling plan node toolbar buttons in main frame
		void mfCategoryViewSelected(TreeNode* treeNode);
		/// Signal for initialising general plan details tab
		void planResetTreeNodeView(PlanTreeModel* treeModel);

	protected:
		/**
		* getRightPane
		*
		* Retrieve the corresponding right Pane widget
		*
		* @return QWidget  Pointer to QWidget* that require type casting
		*/
		QWidget* getRightPane();

		/// Initialise UI
		virtual void initUIControl();

	//protected slots:
		// Phase 4: Plan operations
		//void OnUpdateTreeNodeCmd(CCmdUI* pCmdUI);
		//void OnEditPlan();
		//void OnControlPlan();
		//void OnApprovePlan();
		//void OnUnapprovePlan();
		//void OnSchedulePlan();
		//void OnNewPlan();
		//void OnDeletePlan();
		//void OnCopyPlan();
		//void OnPastePlan();
		//void OnCutPlan();
		//void OnNewCategory();
		//void OnDeleteCategory();
		//void OnDeleteCategoryOrPlan();
		//void OnEditStep();
		//void OnImportPlan();
		//void OnExportPlan();
		//void OnMakePlanVisible();
		//void OnMakePlanInvisible();

	private:
		QSplitter* m_wndSplitter; /// Pointer to layout object that split two horizontal views
		PlanTreeModel* m_pTreeModel; /// Pointer to PModel
		EPlanTreeType m_ePlanType; /// Enum to store Plan Type
		PlanTreeView* m_pTreeView; /// Pointer to QTreewidget object
		CategoryNodeView* m_categoryView; /// Pointer to right pane QTableWidget
		PlanNodeView* m_planView; /// Pointer to right pane QScrollArea
	};

	/////////////////////////////////////////////////////////////////////////////
} // namespace TA_Base_App

#endif // PLANTREEFRAME_H
