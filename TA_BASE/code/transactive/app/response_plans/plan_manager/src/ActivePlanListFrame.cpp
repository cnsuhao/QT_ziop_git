//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/ActivePlanListFrame.cpp $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// <description>


#include <qevent.h>

#include "core/utilities/src/DebugUtil.h"
//#include "app/response_plans/plan_manager/src/resource.h"
#include "app/response_plans/plan_manager/src/PlanManagerFrame.h"
#include "app/response_plans/plan_manager/src/ActivePlanListFrame.h"

#include "app/response_plans/plan_manager/pmodel/src/ActivePlanModel.h"

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// ActivePlanListFrame

ActivePlanListFrame::ActivePlanListFrame(TA_Base_Bus::ITransActiveApp* app, const char* viewType)
: AbstractTransActiveFormView(app, viewType), m_pPlanModel(NULL)
{
    FUNCTION_ENTRY( "ActivePlanListFrame" );	

	m_pListView = new ActivePlanListView(this);

	if (m_pListView)
	{
		this->createMdiSubWindow(m_pListView);
		//this->setTreeViewPtr(m_pListView);
	}
    FUNCTION_EXIT;
}


ActivePlanListFrame::~ActivePlanListFrame()
{
    FUNCTION_ENTRY( "~ActivePlanListFrame" );
    FUNCTION_EXIT;
}


std::string ActivePlanListFrame::getMyPModelName(void)
{
	return PMODEL_ACTIVEPLANLISTFRAME;
}


void ActivePlanListFrame::setupPModel(void)
{
	m_pPlanModel = dynamic_cast<ActivePlanModel *> (m_pModel);
	TA_ASSERT(m_pPlanModel != NULL, "PModel should be valid");

	m_pPlanModel->attachView(this);
}


void ActivePlanListFrame::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
{
	//if (_stricmp(updateType, PMODEL_DATA_CHANGE_SESSION_ID) == 0)
	//{
	//	activateWindow();
	//}
	//if (_stricmp(updateType, PMODEL_DATA_INIT_CORBA) == 0)
	//{
	//	activatePManagerServant();
	//}
}

bool ActivePlanListFrame::createMdiSubWindow(ActivePlanListView* pListView)
{
	FUNCTION_ENTRY("createMdiSubWindow");

	int retVal = false;

	if (pListView)
	{
		QSplitter* pwndSplitter = new QSplitter(Qt::Vertical);
		// create splitter window
		if (pwndSplitter != NULL)
		{
			pwndSplitter->addWidget(pListView);
			setWidget(pwndSplitter);
			retVal = true;
		}
	}

	FUNCTION_EXIT;
	return retVal;
}

ActivePlanModel* ActivePlanListFrame::getTreeModel()
{
	return m_pPlanModel;
}

void ActivePlanListFrame::registerSettings()
{
	m_pPlanModel->registerSettings();
}

void ActivePlanListFrame::closeEvent(QCloseEvent *event)
{
	m_pPlanModel->deregisterSettings();
	event->accept();
}

void ActivePlanListFrame::onViewShown()
{
	m_pListView->onViewShown();
}

//void ActivePlanListFrame::createView(TA_Base_Bus::ITransActiveApp * pApp)
//{
//
//
//    // First, build the view context structure
//    CCreateContext ccx;
//
//    // Designate the class from which to build the view
//    ccx.m_pNewViewClass = RUNTIME_CLASS(ActivePlanListView);
//    m_pWndView = DYNAMIC_DOWNCAST( ActivePlanListView, this->CreateView(&ccx) );
//
//    // Did we succeed ?
//    if ( !m_pWndView )
//    {
//        TRACE0("Creation of view failed");
//    }
//
//    // Do layout recalc
//    RecalcLayout();
//
//    // Set this view active
//    //SetActiveView(m_formView);
//
//    // Order it to resize the parent window to fit
//    m_pWndView->ResizeParentToFit(FALSE);
//
//    /*m_pWndView = new ActivePlanListView(pApp);*/
//
//    // create a view to occupy the client area of the frame TODO
//    /*if (!m_pWndView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
//        CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
//    {
//        TRACE0("Failed to create view window\n");
//        return -1;
//    }*/
//}

//TA_Base_Bus::AbstractTransActiveFormView* TA_Base_App::ActivePlanListFrame::getActiveView()
//{
//    return dynamic_cast<TA_Base_Bus::AbstractTransActiveFormView*>(m_pWndView);
//}


