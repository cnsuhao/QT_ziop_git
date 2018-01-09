//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanNodeView.h $
// @author:  Bart Golab
// @version: $Revision: #2 $
//
// Last modification: $DateTime: 2017/05/22 15:28:33 $
// Last modified by:  $Author: hoa.le $
//
// <description>

#ifndef PLANNODEVIEW_H
#define PLANNODEVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <qscrollarea.h>
#include <qtabwidget.h>

#include "app/response_plans/plan_manager/src/PlanTreeFrame.h"
#include "app/response_plans/plan_manager/src/PlanGeneralDetailTab.h"
//#include "app/response_plans/plan_manager/src/PlanStepDetailPage.h"
//#include "app/response_plans/plan_manager/src/PlanFlowchartPage.h"

class QScrollArea;
class QTabWidget;

namespace TA_Base_App
{
    //class CPlanBrowserDoc;
    //class PlanNode;


    /////////////////////////////////////////////////////////////////////////////
    // PlanNodeView form view

	class PlanNodeView : public QScrollArea
    {
		Q_OBJECT

	public:
		PlanNodeView(PlanTreeFrame* parent = NULL);           // protected constructor used by dynamic creation

    protected:
        //CPlanBrowserDoc *GetDocument();
        //PlanNode *GetPlanNode();

    //public:
    //    virtual void OnDraw(CDC* pDC);
    //    virtual void OnInitialUpdate();

    protected:
        //virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

    // Implementation
    protected:
       ~PlanNodeView();

        //afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        //afx_msg void OnSize(UINT nType, int cx, int cy);

        //afx_msg LRESULT OnSelectActivePlanInstance(WPARAM wParam, LPARAM lParam);
	   void onResetPlanNodeView(PlanTreeModel* treeModel);
        //afx_msg LRESULT OnRefreshPlanStepList(WPARAM wParam, LPARAM lParam);

	   // Attributes:
	   private:
	       QTabWidget m_planTabWidget;
	       PlanGeneralDetailTab* m_pGeneralDetailTab;
	       //CPlanStepDetailPage m_stepDetailPage;
	       //CPlanFlowchartPage m_flowchartPage;

    };

    /////////////////////////////////////////////////////////////////////////////


} // namespace TA_Base_App

#endif // !defined(PLANNODEVIEW_H)
