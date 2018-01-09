//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/ActivePlanListView.h $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2017/05/22 15:28:33 $
// Last modified by:  $Author: hoa.le $
//
// Used for viewing all currently active plans.
//

#if !defined(AFX_ACTIVEPLANLISTVIEW_H__C8B5597F_6D37_4A26_96C4_26DBCF0DCFD3__INCLUDED_)
#define AFX_ACTIVEPLANLISTVIEW_H__C8B5597F_6D37_4A26_96C4_26DBCF0DCFD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <qdialog.h>

#include "app/response_plans/plan_manager/GeneratedFiles/ui_activePlanView.h"

//#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "app/response_plans/plan_manager/src/PlanInstanceListCtrl.h"
#include "app/response_plans/plan_manager/src/ActivePlanListFrame.h"

namespace TA_Base_App
{
	class ActivePlanListFrame;
	class ActivePlanModel;

    /////////////////////////////////////////////////////////////////////////////
    // ActivePlanListView view

	class ActivePlanListView : public QDialog,
								public Ui::activePlanView
    {
		Q_OBJECT
    public:
		ActivePlanListView(ActivePlanListFrame* planListFrame);
        ~ActivePlanListView();

        //virtual HWND getViewHandle();
        //void setTransActiveWinApp(TA_Base_Bus::ITransActiveApp * pApp){ m_pWinApp = pApp; };

		void onViewShown();

	public slots:
		void onAllOwnersCheck();
		void onAllLocationsCheck();
		void onDblclkActiveInstanceList();

    // Attributes
    private:
        bool m_initialised;        
		ActivePlanListFrame* m_planListFrame; /// Pointer to access PModel object

    private:
        PlanInstanceListCtrl    m_planInstanceListCtrl;

  //   protected:
  //      //void OnSize(UINT nType, int cx, int cy);
		//void onDblclkActiveInstancesList(NMHDR* pNMHDR, LRESULT* pResult);
  //      void onAllOwnersCheck();
  //      void onAllLocationsCheck();        

		/**
		* getTreeModel
		*
		* Get PModel for this Plan type
		*
		* @return PlanTreeModel  Pointer to PlanTreeModel
		*/
		ActivePlanModel* getTreeModel();
    };

    //#ifndef _DEBUG  // debug version in ActivePlanListView.cpp
    //inline ActivePlanListModel* ActivePlanListView::GetDocument()
    //    {
    //    FUNCTION_ENTRY( "GetDocument" );

    //    return (ActivePlanListModel*)m_pDocument; }


    //#endif

} // namespace TA_Base_App

#endif // !defined(AFX_ACTIVEPLANLISTVIEW_H__C8B5597F_6D37_4A26_96C4_26DBCF0DCFD3__INCLUDED_)
