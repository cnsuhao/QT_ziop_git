/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanManagerFrame.cpp $
* @author:  Bart Golab
* @version: $Revision: #6 $
*
* Last modification: $DateTime: 2013/04/29 17:39:48 $
* Last modified by:  $Author: huangjian $
*
* This class is the main Transactive QMainWindow for Plan Manager.
*/

#include "app/response_plans/plan_manager/src/PlanManagerFrame.h"

#include <QtGui>
#include <qmdisubwindow.h>
#include <QApplication>
#include <QStatusBar>
#include <qprogressbar.h>
#include <QLabel>

#include "core/utilities/src/TAAssert.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"

#include "app/response_plans/plan_manager/pmodel/src/PlanNode.h"
#include "app/response_plans/plan_manager/pmodel/src/CategoryNode.h"
#include "app/response_plans/plan_manager/pmodel/src/PlanManagerFrameModel.h"

#include "app/response_plans/plan_manager/src/PlanTreeFrame.h"
#include "app/response_plans/plan_manager/src/PlanTreeView.h"
#include "app/response_plans/plan_manager/src/ActivePlanListFrame.h"
#include "app/response_plans/plan_manager/src/ActivePlanListView.h"

using namespace TA_Base_App;
using namespace TA_Base_Bus;
using namespace TA_Base_Core;

/////////////////////////////////////////////////////////////////////////////
// PlanManagerFrame construction/destruction

PlanManagerFrame::PlanManagerFrame(TA_Base_Bus::ITransActiveApp * app)
:TA_Base_Bus::AbstractTransActiveDialog(app, VIEWTYPE_PLANMANAGER),
m_realPModel(NULL)
{
	FUNCTION_ENTRY("PlanManagerFrame");

	// UI Setup
	ui.setupUi(this);

	m_toolBarManager.initialise(&ui);
	// Set View Menu
	planGroup = new QActionGroup(this);
	planGroup->addAction(ui.actionViewAllPlans);
	planGroup->addAction(ui.actionViewApprovedPlans);
	planGroup->addAction(ui.actionViewUnapprovedPlans);
	planGroup->addAction(ui.actionViewActivePlans);

	m_toolBarManager.selectToolBar(MainToolBarManager::TBT_CATEGORY);

	resize(1680, 839); // Do we need to put in ViewResizeProperty?

	setWindowTitle(tr("Plan Manager"));

	m_lblCapsLock = initialiseKeyLockSize();
	m_lblNumLock = initialiseKeyLockSize();
	m_lblScrollLock = initialiseKeyLockSize();

	initialiseKeyLockState();

	m_statusBar = statusBar();
	m_statusBar->addPermanentWidget(m_lblCapsLock);
	m_statusBar->addPermanentWidget(m_lblNumLock);
	m_statusBar->addPermanentWidget(m_lblScrollLock);

	// Setup progress bar
	m_progressBar = new QProgressBar(this);
	m_progressBar->setRange(0, 100);
	m_progressBar->setValue(0);
	m_progressBar->setMinimum(1);
	m_progressBar->setMaximum(100);
	m_progressBar->setTextVisible(true);
	m_progressBar->setFormat(tr("Initializing Paramerters..."));
	m_statusBar->addWidget(m_progressBar);

	setUpConnectionPoints();

	// Not used? Obsolete? Discuss with Huirong
	//TA_Base_Bus::ViewResizeProperty properties;
	//properties.canMaximise = true;
	//properties.maxHeight = 839;
	//properties.maxWidth = 1680;
	//properties.minHeight = 0;
	//properties.minWidth = 0;
	//m_resizeViewHelper->setViewResizeProperty(properties);
	/*setResizingProperties(properties);*/

	FUNCTION_EXIT;
}

PlanManagerFrame::~PlanManagerFrame()
{
	FUNCTION_ENTRY("~PlanManagerFrame");

	m_planBrowserFrameMap.clear();

	FUNCTION_EXIT;
}

void PlanManagerFrame::addNewDocument(EPlanTreeType ePlanType)
{
	FUNCTION_ENTRY("addNewDocument");
	// QTTODO2: Connect to slots for document?

	QAction* action = NULL;
	const char* planType = "";
	const char* planId = "";
	switch (ePlanType)
	{
	case PT_ALLPLANS:
		action = ui.actionViewAllPlans;
		planType = VIEWTYPE_ALLPLANTREEFRAME;
		planId = VIEWID_ALLPLANTREEFRAME;
		break;
	case PT_APPROVEDPLANS:
		action = ui.actionViewApprovedPlans;
		planType = VIEWTYPE_APPROVEDPLANTREEFRAME;
		planId = VIEWID_APPROVEDPLANTREEFRAME;
		break;
	case PT_UNAPPROVEDPLANS:
		action = ui.actionViewUnapprovedPlans;
		planType = VIEWTYPE_UNAPPROVEDPLANTREEFRAME;
		planId = VIEWID_UNAPPROVEDPLANTREEFRAME;
		break;
	case PT_ACTIVEPLANS:
		action = ui.actionViewActivePlans;
		planType = VIEWTYPE_ACTIVEPLANLISTFRAME;
		planId = VIEWID_ACTIVEPLANLISTFRAME;
	}

	TA_Base_Bus::ITransActiveView* pView = this->m_pWinApp->getViewManager()->getView(planType, planId);

	if (pView)
	{
		if (ePlanType >= PT_ALLPLANS && ePlanType <= PT_UNAPPROVEDPLANS)
		{
			PlanTreeFrame* planTreeFrame = dynamic_cast<PlanTreeFrame*>(pView);

			if (planTreeFrame)
			{
				subWindow = ui.mdiArea->addSubWindow(planTreeFrame);
				subWindow->setWindowFlags(Qt::CustomizeWindowHint);
				subWindow->showMaximized();
				//ui.mdiArea->setActiveSubWindow(subWindow);

				connect(planTreeFrame, SIGNAL(mfCategoryViewSelected(TreeNode*)), this, SLOT(onCategoryViewSelectionChanged(TreeNode*)));

				if (planTreeFrame->getLeftPane())
				{
					connect(planTreeFrame->getLeftPane(), SIGNAL(mfTreeNodeChanged(TreeNode*)), this, SLOT(onTreeNodeSelectionChanged(TreeNode*)));
				}

				addPlanBrowserFrame(action, planTreeFrame);
			}
		}
		else
		{
			ActivePlanListFrame* activeListFrame = dynamic_cast<ActivePlanListFrame*>(pView);

			if (activeListFrame)
			{
				subWindow = ui.mdiArea->addSubWindow(activeListFrame);
				subWindow->setWindowFlags(Qt::CustomizeWindowHint);
				subWindow->showMaximized();
				//ui.mdiArea->setActiveSubWindow(subWindow);
				
				// TODO: Signals and Slots connections

				addPlanBrowserFrame(action, activeListFrame);
			}

			
		}
	}

	FUNCTION_EXIT;
}

///////////////////////////////////////////////////////////////////////////////
// PlanManagerFrame event handlers

void PlanManagerFrame::togglePlanNodeActions(TreeNode* treeNode, bool isMixed)
{
	bool canEdit = m_realPModel->canEditCategories();
	ui.actionCategoryNew->setEnabled(canEdit);
	ui.actionFileExportPlan->setEnabled(canEdit);

	canEdit = m_realPModel->canEditPlans();
	ui.actionPlanNew->setEnabled(canEdit);
	ui.actionPlanCopy->setEnabled(canEdit);
	ui.actionPlanPaste->setEnabled(treeNode->isPlanPaste());

	bool isApproved = treeNode->isApproved();
	canEdit = canEdit && !isApproved;
	ui.actionPlanEdit->setEnabled(canEdit);
	ui.actionPlanCut->setEnabled(canEdit);
	ui.actionPlanDelete->setEnabled(canEdit);

	canEdit = m_realPModel->canApprovePlans();
	ui.actionPlanApprove->setEnabled(canEdit && !isApproved);
	ui.actionPlanUnapprove->setEnabled(canEdit && isApproved);

	canEdit = isApproved && m_realPModel->canControlPlans();
	ui.actionPlanControl->setEnabled(canEdit);

	canEdit = isApproved && m_realPModel->canSchedulePlans();
	ui.actionPlanSchedule->setEnabled(canEdit);

	canEdit = treeNode->isVisible();
	ui.actionPlanVisible->setEnabled(!canEdit);
	ui.actionPlanInvisible->setEnabled(canEdit);

	ui.actionFileImportPlan->setEnabled(false);

	if (isMixed)
	{
		ui.actionPlanCut->setEnabled(!isMixed);
		ui.actionPlanDelete->setEnabled(!isMixed);
		ui.actionPlanPaste->setEnabled(!isMixed);
		ui.actionPlanEdit->setEnabled(!isMixed);
		ui.actionPlanApprove->setEnabled(!isMixed);
		ui.actionPlanUnapprove->setEnabled(!isMixed);
		ui.actionPlanSchedule->setEnabled(!isMixed);
		ui.actionPlanControl->setEnabled(!isMixed);
	}
}

void PlanManagerFrame::toggleCategoryNodeActions(TreeNode* treeNode)
{
	bool canEdit = m_realPModel->canEditCategories();
	ui.actionCategoryNew->setEnabled(canEdit);
	ui.actionCategoryRename->setEnabled(canEdit);
	ui.actionFileImportPlan->setEnabled(canEdit);

	canEdit = canEdit && !treeNode->getNodeSummary().hasChildren;
	ui.actionCategoryDelete->setEnabled(canEdit);

	canEdit = m_realPModel->canEditPlans();
	ui.actionPlanNew->setEnabled(canEdit);
	ui.actionPlanPaste->setEnabled(treeNode->isPlanPaste());

	canEdit = false;
	ui.actionPlanDelete->setEnabled(canEdit);
	ui.actionPlanCopy->setEnabled(canEdit);
	ui.actionPlanCut->setEnabled(canEdit);
	ui.actionPlanEdit->setEnabled(canEdit);
	ui.actionPlanApprove->setEnabled(canEdit);
	ui.actionPlanUnapprove->setEnabled(canEdit);
	ui.actionPlanSchedule->setEnabled(canEdit);
	ui.actionPlanControl->setEnabled(canEdit);

	ui.actionFileExportPlan->setEnabled(false);
}

void PlanManagerFrame::onTreeNodeSelectionChanged(TreeNode* treeNode)
{
	FUNCTION_ENTRY("onTreeNodeSelectionChanged");

	if (dynamic_cast<PlanNode *>(treeNode))
	{
		m_toolBarManager.selectToolBar(MainToolBarManager::TBT_PLAN);
		togglePlanNodeActions(treeNode);
	}
	else if (dynamic_cast<CategoryNode *>(treeNode))
	{
		m_toolBarManager.selectToolBar(MainToolBarManager::TBT_CATEGORY);
		toggleCategoryNodeActions(treeNode);
	}
	else
	{
		m_toolBarManager.selectToolBar(MainToolBarManager::TBT_NONE);
	}

	FUNCTION_EXIT;
}

void PlanManagerFrame::onCategoryViewSelectionChanged(TreeNode* treeNode)
{
	FUNCTION_ENTRY("onCategoryViewSelectionChanged");

	PlanTreeFrame* pTreeFrame = qobject_cast<PlanTreeFrame*>(ui.mdiArea->activeSubWindow());

	if (pTreeFrame->getTreeModel()->ArePlanNodesSelected())
	{
		m_toolBarManager.selectToolBar(MainToolBarManager::TBT_PLAN);

		TreeNodeList selectedNodes = pTreeFrame->getTreeModel()->GetSelectedNodes();

		if (selectedNodes.size() > 1)
		{
			bool isApproved = false;
			bool isUnapproved = false;

			// Iterate over selected nodes and determine whether they are all approved or unapproved plans.
			// Where a mixture of approved and unapproved plans is selected, the double click has no effect.
			for (TreeNodeList::iterator iter = selectedNodes.begin(); iter != selectedNodes.end(); iter++)
			{
				TreeNode *treeNode = *iter;			

				if (treeNode->isApproved())
				{
					isApproved = true;
				}
				else
				{
					isUnapproved = true;
				}

				if (isApproved && isUnapproved)
				{
					togglePlanNodeActions(treeNode, true);
					break;
				}
			}
		}
		else
		{
			togglePlanNodeActions(treeNode);
		}
	}


	FUNCTION_EXIT;
}

void PlanManagerFrame::onAppAbout()
{
	FUNCTION_ENTRY("onAppAbout");

	TA_Base_Bus::HelpLauncher::getInstance().displayAboutBox();

	FUNCTION_EXIT;
}

void PlanManagerFrame::onViewToolbar()
{
	FUNCTION_ENTRY("onViewToolbar");
	disconnect(ui.actionViewToolbar, SIGNAL(toggled()),
		this, SLOT(onViewToolbar()));

	m_toolBarManager.toggleToolBarDisplay();

	FUNCTION_EXIT;
}

void PlanManagerFrame::onViewStatusbar()
{
	FUNCTION_ENTRY("OnViewStatusbar");

	if (statusBar()->isHidden())
	{
		statusBar()->show();
	}
	else
	{
		statusBar()->hide();
	}

	FUNCTION_EXIT;
}

void PlanManagerFrame::onViewPlan()
{
	FUNCTION_ENTRY("onViewPlan");

	QAction* action = qobject_cast<QAction*>(sender());
	//TA_ASSERT(action->actionGroup() == planGroup, "action group is different");
	//action->actionGroup()->
	//
	//CFrameWnd *activePlanBrowserFrame = GetActiveFrame(); // Get the active frame which is the MainFrame CMDIFrame not CMDIChildFrame
	//QMdiSubWindow * activeFrame = ui.mdiArea->activeSubWindow(); // Not equivalent to MFC
	PlanBrowserFrame * requiredPlanBrowserFrame = getPlanBrowserFrame(action);
	//PlanBrowserFrame *requiredPlanBrowserFrame = getPlanBrowserFrame(nCommandID);
	TA_ASSERT(requiredPlanBrowserFrame, "No plan browser frame to be activated");

	if (requiredPlanBrowserFrame) // != dynamic_cast<PlanBrowserFrame*>(activeFrame/*->widget()*/))
	{
		if (action == ui.actionViewActivePlans)
		{
			m_toolBarManager.hideToolBar();
		}

		requiredPlanBrowserFrame->activateFrame(); // requiredPlanBrowserFrame = AllPlanTreeFrame / ApprovedPlanTreeFrame / UnapprovedPlanTreeFrame
		//QTTODO:
		//if (activePlanBrowserFrame && activePlanBrowserFrame->GetSafeHwnd() != GetSafeHwnd()) // don't hide the main frame
		//{
		//	activePlanBrowserFrame->ShowWindow(SW_HIDE);
		//}
		//disconnect(action, SIGNAL(triggered()), this, SLOT(onViewPlan()));
		//connect(action, SIGNAL(triggered()), requiredPlanBrowserFrame, SLOT(show()));
		//connect(action, SIGNAL(triggered()), requiredPlanBrowserFrame, SLOT(setFocus()));
	}

	std::string planType = "";
	if (action == ui.actionViewAllPlans)
	{
		planType = VIEWTITLE_ALLPLAN;
	}
	else if (action == ui.actionViewApprovedPlans)
	{
		planType = VIEWTITLE_APPROVEDPLAN;
	}
	else if (action == ui.actionViewUnapprovedPlans)
	{
		planType = VIEWTITLE_UNAPPROVEDPLAN;
	}
	else if (action == ui.actionViewActivePlans)
	{
		planType = VIEWTITLE_ACTIVEPLAN;
	}

	//TAUnicodeString uniStr = TAUnicodeString::createFromMultitByteString(strNewTitle.c_str());
	setMainTitle(m_realPModel->updateTitle(planType));
	//m_strTitle = uniStr.q_str().toStdString().c_str(); // default title in MFC class

	FUNCTION_EXIT;
}

void PlanManagerFrame::addPlanBrowserFrame(QAction* planView, PlanBrowserFrame* planBrowserFrame)
{
	FUNCTION_ENTRY("addPlanBrowserFrame");

	m_planBrowserFrameMap.insert(std::make_pair(planView, planBrowserFrame));

	FUNCTION_EXIT;
}

PlanBrowserFrame* PlanManagerFrame::getPlanBrowserFrame(QAction* planView)
{
	FUNCTION_ENTRY("getPlanBrowserFrame");

	PlanBrowserFrameMap::iterator iter = m_planBrowserFrameMap.find(planView);

	if (iter != m_planBrowserFrameMap.end())
	{
		FUNCTION_EXIT;
		return (PlanBrowserFrame*)iter->second;
	}

	FUNCTION_EXIT;
	return NULL;
}

void PlanManagerFrame::initUIControl()
{
	FUNCTION_ENTRY("initUIControl");

	// Setup MDI framework
	addNewDocument(PT_ALLPLANS);
	addNewDocument(PT_APPROVEDPLANS);
	addNewDocument(PT_UNAPPROVEDPLANS);
	addNewDocument(PT_ACTIVEPLANS);

	FUNCTION_EXIT;
}

QLabel * PlanManagerFrame::initialiseKeyLockSize()
{
	QLabel * lblLock = new QLabel(" LOCK ");
	lblLock->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//lblLock->setMinimumSize(lblLock->minimumSizeHint());
	QSize size = lblLock->minimumSizeHint();
	lblLock->setFixedWidth(size.width());
	lblLock->clear();

	return lblLock;
}

void PlanManagerFrame::initialiseKeyLockState()
{
	// platform dependent method of determining if CAPS LOCK is on
#ifdef Q_OS_WIN32 // MS Windows version
	if (GetKeyState(VK_CAPITAL) == 1)
	{
		toggleKeyCapsLock();
	}
	if (GetKeyState(VK_NUMLOCK) == 1)
	{
		toggleKeyNumLock();
	}
	if (GetKeyState(VK_SCROLL) == 1)
	{
		toggleKeyScrollLock();
	}
#else // X11 version (Linux/Unix/Mac OS X/etc...)
	Display * d = XOpenDisplay((char*)0);
	bool caps_state = false;
	if (d)
	{
		unsigned n;
		XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
		caps_state = (n & 0x01) == 1;
	}
	return caps_state;
#endif
}

void PlanManagerFrame::toggleKeyCapsLock()
{
	toggleKeyLock(m_lblCapsLock, tr(ID_INDICATOR_CAPS));
}

void PlanManagerFrame::toggleKeyNumLock()
{
	toggleKeyLock(m_lblNumLock, tr(ID_INDICATOR_NUM));
}

void PlanManagerFrame::toggleKeyScrollLock()
{
	toggleKeyLock(m_lblScrollLock, tr(ID_INDICATOR_SCRL));
}

void PlanManagerFrame::toggleKeyLock(QLabel * lblLock, const QString strLock)
{
	if (lblLock->text().isEmpty())
	{
		lblLock->setText(strLock);
	}
	else
	{
		lblLock->clear();
	}
}

void PlanManagerFrame::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_CapsLock:
		toggleKeyCapsLock();
		break;
	case Qt::Key_NumLock:
		toggleKeyNumLock();
		break;
	case Qt::Key_ScrollLock:
		toggleKeyScrollLock();
		break;
	default:
		QWidget::keyPressEvent(event);
	}
}

void PlanManagerFrame::setUpConnectionPoints()
{
	// Most slots are from PlanTreeModel
	// Menu actions
	connect(ui.actionFileAppExit, SIGNAL(triggered()), this, SLOT(close()));
	//connect(ui.actionFileImportPlan, SIGNAL(triggered()), this, SLOT(OnImportPlan())); //QTTODO:
	//connect(ui.actionFileExportPlan, SIGNAL(triggered()), this, SLOT(OnExportPlan())); //QTTODO:
	connect(ui.actionViewToolbar, SIGNAL(triggered()), this, SLOT(onViewToolbar()));
	connect(ui.actionViewToolbar, SIGNAL(toggled()), this, SLOT(onViewToolbar()));
	connect(ui.actionViewStatusBar, SIGNAL(triggered()), this, SLOT(onViewStatusbar()));
	connect(ui.actionViewAllPlans, SIGNAL(triggered()), this, SLOT(onViewPlan())); //QTTODO:
	//connect(ui.menuView, SIGNAL(triggered(QAction*)), this, SLOT(OnViewPlan(QAction*)));

	// grouped
	connect(ui.actionViewApprovedPlans, SIGNAL(triggered()), this, SLOT(onViewPlan()));
	connect(ui.actionViewUnapprovedPlans, SIGNAL(triggered()), this, SLOT(onViewPlan()));
	connect(ui.actionViewActivePlans, SIGNAL(triggered()), this, SLOT(onViewPlan()));
	//connect(ui.actionCategoryNew, SIGNAL(triggered()), this, SLOT(OnNewCategory())); //QTTODO:
	//connect(ui.actionCategoryDelete, SIGNAL(triggered()), this, SLOT(OnDeleteCategory())); //QTTODO:
	//connect(ui.actionCategoryRename, SIGNAL(triggered()), this, SLOT(OnRenameCategory())); //QTTODO: from PlanTreeView
	//connect(ui.actionPlanNew, SIGNAL(triggered()), this, SLOT(OnNewPlan())); //QTTODO:
	//connect(ui.actionPlanDelete, SIGNAL(triggered()), this, SLOT(OnDeletePlan())); //QTTODO:
	//connect(ui.actionPlanCut, SIGNAL(triggered()), this, SLOT(OnCutPlan())); //QTTODO:
	//connect(ui.actionPlanCopy, SIGNAL(triggered()), this, SLOT(OnCopyPlan())); //QTTODO:
	//connect(ui.actionPlanPaste, SIGNAL(triggered()), this, SLOT(OnPastePlan())); //QTTODO:
	//connect(ui.actionPlanEdit, SIGNAL(triggered()), this, SLOT(OnEditPlan())); //QTTODO:
	//connect(ui.actionPlanApprove, SIGNAL(triggered()), this, SLOT(OnApprovePlan())); //QTTODO:
	//connect(ui.actionPlanUnapprove, SIGNAL(triggered()), this, SLOT(OnUnapprovePlan())); //QTTODO:
	//connect(ui.actionPlanSchedule, SIGNAL(triggered()), this, SLOT(OnSchedulePlan())); //QTTODO:
	//connect(ui.actionPlanControl, SIGNAL(triggered()), this, SLOT(OnControlPlan())); //QTTODO:
	////connect(ui.actionHelpIndex, SIGNAL(triggered()), this, SLOT(OnHelp())); //QTTODO:
	connect(ui.actionHelpAppAbout, SIGNAL(triggered()), this, SLOT(onAppAbout()));
}

void PlanManagerFrame::setMainTitle(std::string strTitle)
{
	setWindowTitle(QString(strTitle.c_str()));
}

void PlanManagerFrame::updateView()
{
	FUNCTION_ENTRY("updateView");
	// Moved from onInitGenericGUICompleted
	try
	{
		//PlanBrowserFrameMap::iterator iter = m_planBrowserFrameMap.begin();

		//for (; iter != m_planBrowserFrameMap.end(); ++iter)
		//{
		//	TA_ASSERT(iter->second != NULL, "the frame pointer is NULL");
		//	((PlanBrowserFrame*)iter->second)->registerSettings();
		//}

		// Include in the title the location of the Plan Agent used for accessing plan data,
		// as well as editing and running plans.
		std::string strTitle = "Plan Manager";

		strTitle += " - ";
		strTitle += m_realPModel->getDefaultPlanAgentLocationName();

		// Update the child frames and their views.
		//this->SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE); QTTODO: called CPlanTreeView::OnInitialUpdate() x3 on AllPlan, Approved, Unapproved
		// Replace with direct call to the destine class objects

		m_realPModel->setBaseTitle(strTitle);
		setMainTitle(m_realPModel->getTitle());

		// Show the approriate plan tree, depending on operator privileges.
		selectStartupView();

		// Forward to pmodel
		m_realPModel->registerRunParam();
	}
	catch (...)
	{
		LOG(SourceInfo, DebugUtil::ExceptionCatch, "Exception thrown by PlanManagerFrame", "something wrong in initializing the corba objects");
	}

	FUNCTION_EXIT;
}

void PlanManagerFrame::selectStartupView()
{
	FUNCTION_ENTRY("selectStartupView");

	TA_ASSERT(m_pWinApp, "Main frame not initialised");

	// The presence of the --exec-plan-id parameter overrides the access rights based rules
	// used to determine the initial startup screen. In this case the Approved Plans view
	// is always activated.

	if (!m_realPModel->getRunParam(RPARAM_EXECPLANID).empty())
	{
		emit ui.actionViewApprovedPlans->triggered();

		// Set RPARAM_FWDEXECPLANID to the value of RPARAM_EXECPLANID.
		// This will trigger a notification which is handled by the Approved Plans view.
		m_realPModel->setRunRaram(RPARAM_FWDEXECPLANID, m_realPModel->getRunParam(RPARAM_EXECPLANID).c_str());
	}
	if (!m_realPModel->getRunParam(RPARAM_VIEWPLANID).empty())
	{
		emit ui.actionViewApprovedPlans->triggered();

		// Set RPARAM_FWDVIEWPLANID to the value of RPARAM_VIEWPLANID.
		// This will trigger a notification which is handled by the Approved Plans view.
		m_realPModel->setRunRaram(RPARAM_FWDVIEWPLANID, m_realPModel->getRunParam(RPARAM_VIEWPLANID).c_str());
	}
	if (!m_realPModel->getRunParam(RPARAM_ASSOC_ALARMID).empty())
	{
		emit ui.actionViewApprovedPlans->triggered();

		// Set RPARAM_FWDEXECPLANID to the value of RPARAM_EXECPLANID.
		// This will trigger a notification which is handled by the Approved Plans view.
		m_realPModel->setRunRaram(RPARAM_FWDASSOCALARMID, m_realPModel->getRunParam(RPARAM_ASSOC_ALARMID).c_str());
	}
	else
	{
		// The rules used to determine the initial startup screen are based
		// on operator access rights and are as follows:
		//
		// Execution | Editing | View
		// ----------+---------+-----------------
		// No    |   No    | All Plans
		// Yes   |   No    | Approved Plans
		// No    |   Yes   | Unapproved Plans
		// Yes   |   Yes   | All Plans

		//QTTODO:
		bool haveExecutionRights = m_realPModel->canControlPlans();
		bool haveEditingRights = m_realPModel->canEditCategories() || m_realPModel->canEditPlans();

		QAction * startupView = ui.actionViewAllPlans; // default startup view

		if (haveExecutionRights && !haveEditingRights)
		{
			startupView = ui.actionViewApprovedPlans;
		}
		else if (!haveExecutionRights && haveEditingRights)
		{
			startupView = ui.actionViewUnapprovedPlans;
		}

		// Activate the appropriate child frame window
		if (startupView)
		{
			emit startupView->triggered();
		}
	}

	FUNCTION_EXIT;
}

std::string PlanManagerFrame::getMyPModelName()
{
	return PMODEL_PLAN_MANAGER_MAINFRAME;
}

void PlanManagerFrame::setupPModel()
{
	m_realPModel = dynamic_cast<PlanManagerFrameModel*> (m_pModel);
	TA_ASSERT(m_realPModel != NULL, "PModel should be valid");

	// Register view(s) for receiving notify Views notification
	m_pModel->attachView(this, PMODEL_DATA_CHANGE_SESSION_ID);
}

void PlanManagerFrame::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
{
	// Receives state changes from business objects, which implement IStateChangeSubject
	if (_stricmp(updateType, PMODEL_DATA_CHANGE_SESSION_ID) == 0)
	{
		sessionIdChanged();
	}
	//if (_stricmp(updateType, PMODEL_DATA_INIT_CORBA) == 0)
	//{
	//	activatePManagerServant();
	//}
}

void PlanManagerFrame::sessionIdChanged()
{
	FUNCTION_ENTRY("activateWindow");
	// TD10849 Re-register for interactive steps
	// Bring the main window (this) to the front because there may be
	// messageboxes being displayed when PlanManagerCorbaDefImpl is
	// constructed, and the messageboxes are children of the main window.
	show();
	raise();
	activateWindow();

	// Check locations from PModel
	TAUtfString strError(m_realPModel->checkLocations());
	strError.trimmed();
	if (!strError.isEmpty())
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "can not get the following plan agents: [%s]", strError.toMultiByteStdString().c_str());
	}
	// TD11365: Make sure the old window still exists before bringing it
	// to the foreground again.
	if (!isActiveWindow())
	{
		show();
		raise();
		activateWindow();
	}

	FUNCTION_EXIT;
}

void PlanManagerFrame::doNotifyMessage(unsigned long messageID, TRA_WPARAM wParam, TRA_LPARAM lParam)
{
	FUNCTION_ENTRY("doNotifyMessage");

	switch (messageID)
	{
	case TA_MSG_INIT_CORBA:
		onRegisterPlanManagerProgress(wParam, lParam);
		break;
	}

	FUNCTION_EXIT;
}

void PlanManagerFrame::onRegisterPlanManagerProgress(TRA_WPARAM wParam, TRA_LPARAM lParam)
{
	FUNCTION_ENTRY("onRegisterPlanManagerProgress");

	QApplication::setOverrideCursor(Qt::WaitCursor);

	// Activate the interactive step servant
	if (!m_realPModel->isStepServantActive())
	{
		int iSize = m_realPModel->activatePManagerServant();
		int iPercent = 5;
		m_progressBar->setValue(iPercent);
		int interval = 95 / iSize;
		QString strError;
		for (int i = 0; i < iSize; i++)
		{
			// Register with each Station's Agent
			std::string strLocation = m_realPModel->registerStationAgent(i);
			strError += strLocation.c_str();
			strError += " ";
			m_progressBar->setValue(iPercent + interval*i);
			m_progressBar->setFormat(tr("Initializing Plans... ") + QString::number(iPercent + interval*i) + "%");
			QApplication::processEvents();
		}
		m_progressBar->setValue(99);
		m_progressBar->setFormat(tr("Initializing Plans... ") + QString::number(99) + "%");
		strError.trimmed();
		if (!strError.isEmpty())
		{
			m_progressBar->setValue(100);
			m_progressBar->setFormat(tr("Done"));
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "can not get the following plan agents: [%s]", strError.toStdString());
		}
	}

	QApplication::restoreOverrideCursor();

	m_statusBar->removeWidget(m_progressBar);
	m_lblReady = new QLabel(tr("Ready"));
	m_statusBar->addWidget(m_lblReady, 1);

	FUNCTION_EXIT;
}

void PlanManagerFrame::allocAppResource()
{
}

void PlanManagerFrame::cleanup()
{
}