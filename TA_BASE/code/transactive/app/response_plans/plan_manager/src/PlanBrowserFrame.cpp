/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanBrowserFrame.cpp $
* @author:  Bart Golab
* @version: $Revision: #3 $
*
* Last modification: $DateTime: 2017/05/22 15:28:33 $
* Last modified by:  $Author: hoa.le $
*
* This class share common methods with PlanTreeFrame and ActivePlanListFrame for manipulating the MDI documents.
*/

#include "app/response_plans/plan_manager/src/PlanBrowserFrame.h"

#include "core/utilities/src/DebugUtil.h"

#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"

#include "app/response_plans/plan_manager/src/PlanManagerFrame.h"
#include "app/response_plans/plan_manager/src/PlanTreeView.h"

using namespace TA_Base_App;

/////////////////////////////////////////////////////////////////////////////
// PlanBrowserFrame

PlanBrowserFrame::PlanBrowserFrame()
{
	FUNCTION_ENTRY("PlanBrowserFrame");
	FUNCTION_EXIT;
}

PlanBrowserFrame::~PlanBrowserFrame()
{
	FUNCTION_ENTRY("~PlanBrowserFrame");
	FUNCTION_EXIT;
}

/////////////////////////////////////////////////////////////////////////////
// PlanBrowserFrame message handlers

PlanManagerFrame *PlanBrowserFrame::GetMainFrame()
{
	FUNCTION_ENTRY("GetMainFrame");

	QWidgetList widgets = qApp->topLevelWidgets();
	for (QWidgetList::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		if ((*i)->objectName() == "PlanManagerFrame")
		{
			PlanManagerFrame *mainFrame = dynamic_cast<PlanManagerFrame *>((QMainWindow*)(*i));
			TA_ASSERT(mainFrame, "main frame is NULL");
			return mainFrame;
		}
	}

	FUNCTION_EXIT;

	return NULL;
}

void PlanBrowserFrame::activateFrame()
{
	FUNCTION_ENTRY("activateFrame");

	// Implement how the frame is activated.
	this->onViewShown();

	mdiArea()->setActiveSubWindow(this);

	this->show();
	this->setFocus();

	FUNCTION_EXIT;
}

//void PlanBrowserFrame::setTreeViewPtr(PlanTreeView* planTreeView)
//{
//	m_planTreeView = planTreeView;
//}
//
//PlanTreeView* PlanBrowserFrame::getTreeView()
//{
//	return m_planTreeView;
//}