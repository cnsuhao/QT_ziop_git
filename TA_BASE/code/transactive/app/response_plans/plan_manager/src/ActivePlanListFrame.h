//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/ActivePlanListFrame.h $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// <description>

#if !defined(AFX_ACTIVEPLANFRAME_H__0030AE2D_E475_4236_BFC6_A3B626929C8D__INCLUDED_)
#define AFX_ACTIVEPLANFRAME_H__0030AE2D_E475_4236_BFC6_A3B626929C8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ActivePlanListView.h"
#include "PlanBrowserFrame.h"
#include "bus/generic_gui_view/src/AbstractTransActiveView.h"

#include "app\response_plans\plan_manager\pmodel\src\ActivePlanModel.h"

namespace TA_Base_App
{
	class ActivePlanListView;
	class ActivePlanModel;

    /////////////////////////////////////////////////////////////////////////////
    // ActivePlanListFrame frame

	class ActivePlanListFrame : public PlanBrowserFrame,
								public TA_Base_Bus::AbstractTransActiveFormView
    {
		Q_OBJECT

    public:
		ActivePlanListFrame(TA_Base_Bus::ITransActiveApp* app, const char* viewType);           // protected constructor used by dynamic creation
		virtual ~ActivePlanListFrame();

		virtual std::string getMyPModelName();
		void setupPModel();
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args = NULL);

		/**
		* getTreeModel
		*
		* Get PModel object to for implementing application logic
		*
		* @return PlanTreeModel*  Pointer to PlanTreeModel object
		*/
		ActivePlanModel* getTreeModel();

		/**
		* createMdiSubWindow
		*
		* Setup the MDI's initial document view
		*
		* @param pListView  Pointer to ActivePlanListView
		*
		* @return bool  True if document view is created, false otherwise
		*/
		bool createMdiSubWindow(ActivePlanListView* pListView);

		void closeEvent(QCloseEvent *event);

		virtual void registerSettings();
		void onViewShown();

	private:
		ActivePlanListView* m_pListView;
		ActivePlanModel* m_pPlanModel;

    };

    /////////////////////////////////////////////////////////////////////////////

    // {{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

} // namespace TA_Base_App

#endif // !defined(AFX_ACTIVEPLANFRAME_H__0030AE2D_E475_4236_BFC6_A3B626929C8D__INCLUDED_)
