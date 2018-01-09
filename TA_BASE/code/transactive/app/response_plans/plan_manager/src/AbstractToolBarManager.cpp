/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/AbstractToolBarManager.cpp $
* @author:  Bart Golab
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2017/05/09 13:33:55 $
* Last modified by:  $Author: hoa.le $
*
* This class implement common functionalities for Toolbars.
*/

#include "app/response_plans/plan_manager/src/AbstractToolBarManager.h"

#include <vector>
#include <qtoolbar.h>

#include "core/utilities/src/TAAssert.h"

#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;

/////////////////////////////////////////////////////////////////////////////
// AbstractToolBarManager

AbstractToolBarManager::AbstractToolBarManager() :
/* m_toolBarParent(NULL), */m_selectedToolBarIdx(-1), m_isToolBarVisible(true)
{
	FUNCTION_ENTRY("AbstractToolBarManager");
	FUNCTION_EXIT;
}

AbstractToolBarManager::~AbstractToolBarManager()
{
	FUNCTION_ENTRY("~AbstractToolBarManager");

	//for (ToolBarList::iterator iter = m_toolBars.begin(); iter != m_toolBars.end(); iter++)
	//{
	//    QToolBar *toolBar = *iter;

	//    toolBar->DestroyWindow();
	//    delete toolBar;
	//}

	//m_toolBars.clear();

	FUNCTION_EXIT;
}

void AbstractToolBarManager::initialise(Ui_PlanManagerFrame* toolBarParent)
{
	FUNCTION_ENTRY("initialise");

	TA_ASSERT(toolBarParent != NULL, "Specified parent frame pointer is null");

	//m_toolBarParent = toolBarParent;

	// Create all managed toolbars
	initialiseToolBarList(toolBarParent);

	FUNCTION_EXIT;
}

void AbstractToolBarManager::toggleToolBarDisplay()
{
	FUNCTION_ENTRY("toggleToolBarDisplay");

	//TA_ASSERT(m_toolBarParent != NULL, "Undefined toolbar parent frame");

	if (isToolBarEnabled())
	{
		m_isToolBarVisible = isToolBarVisible();

		if (m_isToolBarVisible)
		{
			m_toolBars[m_selectedToolBarIdx]->hide();
		}
		else
		{
			m_toolBars[m_selectedToolBarIdx]->show();
		}

		m_isToolBarVisible = !m_isToolBarVisible;

		// QT?
		//m_toolBarParent->RecalcLayout(FALSE);
	}

	FUNCTION_EXIT;
}

bool AbstractToolBarManager::isToolBarEnabled() const
{
	FUNCTION_ENTRY("isToolBarEnabled");
	FUNCTION_EXIT;
	return m_selectedToolBarIdx >= 0 && m_selectedToolBarIdx < static_cast<int>(m_toolBars.size());
}

bool AbstractToolBarManager::isToolBarVisible() const
{
	FUNCTION_ENTRY("isToolBarVisible");

	if (isToolBarEnabled())
	{
		FUNCTION_EXIT;
		return m_toolBars[m_selectedToolBarIdx]->isVisible() == true;
	}

	FUNCTION_EXIT;
	return false;
}

QToolBar *AbstractToolBarManager::addToolBarToList(QToolBar * toolBar)
{
	FUNCTION_ENTRY("addToolBarToList");

	m_toolBars.push_back(toolBar);

	FUNCTION_EXIT;
	return m_toolBars[m_toolBars.size() - 1];
}

void AbstractToolBarManager::selectToolBar(int toolBarIdx)
{
	FUNCTION_ENTRY("selectToolBar");

	m_selectedToolBarIdx = toolBarIdx;

	for (int idx = 0; idx < static_cast<int>(m_toolBars.size()); idx++)
	{
		QToolBar *toolBar = m_toolBars[idx];

		if (idx == m_selectedToolBarIdx && m_isToolBarVisible)
		{
			toolBar->show();
		}
		else
		{
			toolBar->hide();
		}
	}

	FUNCTION_EXIT;
}