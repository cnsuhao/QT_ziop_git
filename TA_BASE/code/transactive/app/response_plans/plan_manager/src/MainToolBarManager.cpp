/*
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/MainToolBarManager.cpp $
* @author:  Bart Golab
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2017/05/09 13:33:55 $
* Last modified by:  $Author: hoa.le $
*
* Main toolbar manager for Plan Manager.
*/

#include "MainToolBarManager.h"

#include "core/utilities/src/DebugUtil.h"

#include "app\response_plans\plan_manager\pmodel\src\Utilities.h"

using namespace TA_Base_App;

/////////////////////////////////////////////////////////////////////////////
// MainToolBarManager

MainToolBarManager::MainToolBarManager()
{
	FUNCTION_ENTRY("MainToolBarManager");
	FUNCTION_EXIT;
}

MainToolBarManager::~MainToolBarManager()
{
	FUNCTION_ENTRY("~MainToolBarManager");
	FUNCTION_EXIT;
}

void MainToolBarManager::selectToolBar(EToolBarType toolBarType)
{
	FUNCTION_ENTRY("selectToolBar");

	AbstractToolBarManager::selectToolBar((int)toolBarType);

	FUNCTION_EXIT;
}

void MainToolBarManager::hideToolBar()
{
	FUNCTION_ENTRY("hideToolBar");

	AbstractToolBarManager::selectToolBar(TBT_NONE);

	FUNCTION_EXIT;
}

void MainToolBarManager::initialiseToolBarList(Ui_PlanManagerFrame* toolBarParent)
{
	FUNCTION_ENTRY("initialiseToolBarList");

	addToolBarToList(toolBarParent->toolBarCategoryNode);  // TBT_CATEGORY
	if (Utilities::isPlanManagerConfigurationMode())
	{
		if (!toolBarParent->toolbarPlanNodeAdmin->isEnabled())
		{
			toolBarParent->toolbarPlanNodeAdmin->setEnabled(true);
		}
		addToolBarToList(toolBarParent->toolbarPlanNodeAdmin);
	}
	else
	{
		QToolBar* toolBar = toolBarParent->toolbarPlanNodeAdmin;
		if (toolBar && !toolBar->isEnabled())
		{
			toolBar->hide();
		}
		addToolBarToList(toolBarParent->toolbarPlanNode);
	}
	FUNCTION_EXIT;
}