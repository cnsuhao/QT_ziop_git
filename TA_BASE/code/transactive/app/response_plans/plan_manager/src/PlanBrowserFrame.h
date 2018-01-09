/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanBrowserFrame.h $
* @author:  Bart Golab
* @version: $Revision: #3 $
*
* Last modification: $DateTime: 2017/05/22 15:28:33 $
* Last modified by:  $Author: hoa.le $
*
* This class share common methods with PlanTreeFrame and ActivePlanListFrame for manipulating the MDI documents.
*/

#ifndef PLANBROWSERFRAME_H
#define PLANBROWSERFRAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <qmdisubwindow.h>

namespace TA_Base_App
{
	class PlanManagerFrame;
	class PlanTreeView;

	/////////////////////////////////////////////////////////////////////////////
	// PlanBrowserFrame frame

	class PlanBrowserFrame : public QMdiSubWindow
	{
		Q_OBJECT
	protected:
		PlanBrowserFrame();           // protected constructor used by dynamic creation

	public:
		//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		virtual void onViewShown() = 0;
		virtual void activateFrame();
		virtual void registerSettings(){};

		//void setTreeViewPtr(PlanTreeView* planTreeView);
		//PlanTreeView* getTreeView();

		// Implementation
	public:
		virtual ~PlanBrowserFrame();

	protected:
		PlanManagerFrame *GetMainFrame();
		//PlanTreeView* m_planTreeView;
	};

	/////////////////////////////////////////////////////////////////////////////
} // namespace TA_Base_App

#endif // !defined(AFX_PLANBROWSERFRAME_H__C1DF2E25_4C4E_4790_AF08_F526BEA7C853__INCLUDED_)
