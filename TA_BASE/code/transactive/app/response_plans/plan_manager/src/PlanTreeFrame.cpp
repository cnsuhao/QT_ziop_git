/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanTreeFrame.cpp $
* @author:  Bart Golab
* @version: $Revision: #3 $
*
* Last modification: $DateTime: 2017/05/22 15:28:33 $
* Last modified by:  $Author: hoa.le $
*
* This class is a container widget for the MDI documents.
*/

#include "app/response_plans/plan_manager/src/PlanTreeFrame.h"

#include "app/response_plans/plan_manager/pmodel/src/PlanTreeModel.h"
#include "app/response_plans/plan_manager/pmodel/src/TreeNode.h"
#include "app/response_plans/plan_manager/pmodel/src/CategoryNode.h"
#include "app/response_plans/plan_manager/pmodel/src/PlanNode.h"

#include "app/response_plans/plan_manager/src/PlanManagerFrame.h"
#include "app/response_plans/plan_manager/src/PlanTreeView.h"
#include "app/response_plans/plan_manager/src/CategoryNodeView.h"
#include "app/response_plans/plan_manager/src/PlanNodeView.h"
#include "app/response_plans/plan_manager/src/MessageBox.h"

using namespace TA_Base_App;
using namespace TA_Base_Bus;

//BEGIN_MESSAGE_MAP(PlanTreeDoc, PlanBrowserService)
//	// {{AFX_MSG_MAP(PlanTreeModel)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_EDIT, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_CONTROL, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_APPROVE, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_UNAPPROVE, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_SCHEDULE, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_NEW, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_DELETE, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_CUT, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_COPY, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_PASTE, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_CATEGORY_NEW, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_CATEGORY_DELETE, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_CATEGORY_RENAME, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_CATEGORY_OR_PLAN_DELETE, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_IMPORT_PLAN, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_EXPORT_PLAN, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_VISIBLE, OnUpdateTreeNodeCmd)
//	ON_UPDATE_COMMAND_UI(ID_PLAN_INVISIBLE, OnUpdateTreeNodeCmd)
//	ON_COMMAND(ID_PLAN_EDIT, OnEditPlan)
//	ON_COMMAND(ID_PLAN_CONTROL, OnControlPlan)
//	ON_COMMAND(ID_PLAN_APPROVE, OnApprovePlan)
//	ON_COMMAND(ID_PLAN_UNAPPROVE, OnUnapprovePlan)
//	ON_COMMAND(ID_PLAN_SCHEDULE, OnSchedulePlan)
//	ON_COMMAND(ID_PLAN_NEW, OnNewPlan)
//	ON_COMMAND(ID_PLAN_DELETE, OnDeletePlan)
//	ON_COMMAND(ID_PLAN_CUT, OnCutPlan)
//	ON_COMMAND(ID_PLAN_COPY, OnCopyPlan)
//	ON_COMMAND(ID_PLAN_PASTE, OnPastePlan)
//	ON_COMMAND(ID_CATEGORY_NEW, OnNewCategory)
//	ON_COMMAND(ID_CATEGORY_DELETE, OnDeleteCategory)
//	ON_COMMAND(ID_CATEGORY_OR_PLAN_DELETE, OnDeleteCategoryOrPlan)
//	ON_COMMAND(ID_STEP_EDIT, OnEditStep)
//	ON_COMMAND(ID_PLAN_VISIBLE, OnMakePlanVisible)
//	ON_COMMAND(ID_PLAN_INVISIBLE, OnMakePlanInvisible)
//	ON_COMMAND(ID_IMPORT_PLAN, OnImportPlan)
//	ON_COMMAND(ID_EXPORT_PLAN, OnExportPlan)
//	// }}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PlanTreeFrame construction/destruction
//        enum {COPY_PLAN,COPY_CATEGORY,CUT_PLAN,CUT_CATEGORY,NO_ACTION} ECopyNodeAction;

PlanTreeFrame::PlanTreeFrame(TA_Base_Bus::ITransActiveApp* app, const char* viewType)
: AbstractTransActiveFormView(app, viewType), m_pTreeModel(NULL)
{
	FUNCTION_ENTRY("PlanTreeFrame");

	// TODO: add member initialization code here
	if (_stricmp(viewType, VIEWTYPE_ALLPLANTREEFRAME) == 0)
	{
		m_ePlanType = PT_ALLPLANS;
	}
	else if (_stricmp(viewType, VIEWTYPE_APPROVEDPLANTREEFRAME) == 0)
	{
		m_ePlanType = PT_APPROVEDPLANS;
	}
	else if (_stricmp(viewType, VIEWTYPE_UNAPPROVEDPLANTREEFRAME) == 0)
	{
		m_ePlanType = PT_UNAPPROVEDPLANS;
	}

	m_pTreeView = new PlanTreeView(m_ePlanType, this);

	if (m_pTreeView)
	{
		this->createMdiSubWindow(m_pTreeView);
		//this->setTreeViewPtr(m_pTreeView);
		//connect(pTreeView, SIGNAL(mfTreeNodeChanged(TreeNode*)), this, SLOT(onTreeNodeSelectionChanged(TreeNode*)));
		connect(m_pTreeView, SIGNAL(pfTreeNodeChanged(TreeNode*)), this, SLOT(onTreeNodeSelectionChanged(TreeNode*)));

		connect(m_pTreeView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), m_pTreeView, SLOT(onTreeItemSelected(QTreeWidgetItem*, QTreeWidgetItem*)));
		connect(m_pTreeView, SIGNAL(itemExpanded(QTreeWidgetItem*)), m_pTreeView, SLOT(onTreeItemExpanded(QTreeWidgetItem*)));
	}

	//m_pTreeModel = new PlanTreeModel(m_ePlanType);
	//QTTODO2: create in model

	FUNCTION_EXIT;
}

PlanTreeFrame::~PlanTreeFrame()
{
	FUNCTION_ENTRY("~PlanTreeFrame");
	FUNCTION_EXIT;
}

std::string PlanTreeFrame::getMyPModelName(void)
{
	const char* retVal = "";

	switch (m_ePlanType)
	{
	case PT_ALLPLANS:
		retVal = PMODEL_ALLPLANTREEFRAME;
		break;
	case PT_APPROVEDPLANS:
		retVal = PMODEL_APPROVEDPLANTREEFRAME;
		break;
	case PT_UNAPPROVEDPLANS:
		retVal = PMODEL_UNAPPROVEDPLANTREEFRAME;
		break;
	}

	return retVal;
}

void PlanTreeFrame::setupPModel(void)
{
	m_pTreeModel = dynamic_cast<PlanTreeModel *> (m_pModel);
	TA_ASSERT(m_pTreeModel != NULL, "PModel should be valid");

	// Register view(s) for receiving notify Views notification
	//m_pTreeModel->attachView(this, PMODEL_DATA_CHANGE_SESSION_ID);
}

void PlanTreeFrame::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
{
	// Receives state changes from business objects, which implement IStateChangeSubject
	//if (_stricmp(updateType, PMODEL_DATA_CHANGE_SESSION_ID) == 0)
	//{
	//	activateWindow();
	//}
	//if (_stricmp(updateType, PMODEL_DATA_INIT_CORBA) == 0)
	//{
	//	activatePManagerServant();
	//}
}

void PlanTreeFrame::initUIControl()
{
	FUNCTION_ENTRY("initUIControl");	
	FUNCTION_EXIT;
}

bool PlanTreeFrame::createMdiSubWindow(PlanTreeView* ptreeView)
{
	FUNCTION_ENTRY("createMdiSubWindow");

	m_wndSplitter = new QSplitter(Qt::Horizontal);

	bool retVal = false;
	// create splitter window
	if (m_wndSplitter != NULL)
	{
		m_wndSplitter->addWidget(ptreeView); // Add QtreeWidget to left pane of QSplitter
		m_wndSplitter->addWidget(new QWidget()); // Add empty widget to right pane of QSplitter
		m_wndSplitter->setMinimumWidth(0);
		m_wndSplitter->setMaximumWidth(300);
		setWidget(m_wndSplitter);
		retVal = true;
	}

	FUNCTION_EXIT;
	return retVal;
}

void PlanTreeFrame::onViewShown()
{
	m_pTreeView->onViewShown();
}

PlanTreeModel* PlanTreeFrame::getTreeModel()
{
	return m_pTreeModel;
}

PlanTreeView *PlanTreeFrame::getLeftPane()
{
	FUNCTION_ENTRY("GetLeftPane");
	FUNCTION_EXIT;
	return dynamic_cast<PlanTreeView *>(m_wndSplitter->widget(0));
}

QWidget* PlanTreeFrame::getRightPane()
{
	FUNCTION_ENTRY("GetRightPane");
	FUNCTION_EXIT;
	return m_wndSplitter->widget(1);
}

void PlanTreeFrame::onCategoryViewSelectionChanged(TreeNode* treeNode)
{
	FUNCTION_ENTRY("onCategoryViewSelectionChanged");

	emit mfCategoryViewSelected(treeNode);

	FUNCTION_EXIT;
}

void PlanTreeFrame::onTreeNodeSelectionChanged(TreeNode* treeNode)
{
	FUNCTION_ENTRY("onTreeNodeSelectionChanged");

	// If necessary, update the right hand pane with the new view.
	if (treeNode)
	{
		QWidget* qwRightPane = getRightPane();

		if (qwRightPane)
		{
			if (dynamic_cast<CategoryNode*>(treeNode))
			{
				CategoryNodeView* view = qobject_cast<CategoryNodeView*>(qwRightPane);

				if (!view)
				{
					delete qwRightPane;
					view = new CategoryNodeView(this);
					if (view)
					{
						m_wndSplitter->addWidget(view);
						connect(this, SIGNAL(categoryResetTreeNodeView(PlanTreeModel*)), getRightPane(), SLOT(onResetCategoryNodeView(PlanTreeModel*)));
						connect(view, SIGNAL(categoryViewSelected(TreeNode*)), this, SLOT(onCategoryViewSelectionChanged(TreeNode*)));
					}
				}

				//emit categoryViewSelected(treeNode);

				if (view)
					emit categoryResetTreeNodeView(getTreeModel());
			}
			// Phase 3: View Plan
			else if (dynamic_cast<PlanNode*>(treeNode))
			{
				PlanNodeView* view = qobject_cast<PlanNodeView*>(qwRightPane);

				if (!view)
				{
					delete qwRightPane;
					m_wndSplitter->addWidget(new PlanNodeView(this));
					connect(this, SIGNAL(planResetTreeNodeView(PlanTreeModel*)), getRightPane(), SLOT(onResetPlanNodeView(PlanTreeModel*)));
				}

				if (view)
					emit planResetTreeNodeView(getTreeModel());
			}
		}

		m_wndSplitter->setStretchFactor(1, 1);
		setWidget(m_wndSplitter);
	}

	m_wndSplitter->setStretchFactor(1, 1);

	QWidget* treeWidget = getLeftPane();
	treeWidget->setFocus();

	// If necessary, update the right hand pane with the new view.
	//if (strcmp(GetRightPane()->staticMetaObject().className(), GetRightPaneWidget()->staticMetaObject().className()) != 0)
	{
		//m_wndSplitter.widget(1)->deleteLater();
		////m_wndSplitter.CreateView(0, 1, rightPaneRuntimeClass, CSize(0, 0), NULL);
		//m_wndSplitter.addWidget(rightPaneWidget);
		//m_wndSplitter->setStretchFactor(1, 1);
	}

	// Tell the right hand pane view to update its contents.
	// GetRightPane()->PostMessage(WM_RESET_TREE_NODE_VIEW);

	FUNCTION_EXIT;
}