//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanDetailTab.h $
// @author:  Bart Golab
// @version: $Revision: #2 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// <description>

#if !defined(AFX_PLANDETAILPAGE_H__89B1A01E_7AD6_4BDA_83A6_3CF9DF0B2578__INCLUDED_)
#define AFX_PLANDETAILPAGE_H__89B1A01E_7AD6_4BDA_83A6_3CF9DF0B2578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QWidget>
#include <qtabwidget.h>

#include "bus/common_control/src/button/CtlCheckBox.h"

namespace TA_Base_App
{
    class PlanTreeFrame;
    class PlanNode;
    class PlanInstanceFilter;
	class PlanTreeModel;

    /////////////////////////////////////////////////////////////////////////////
    // PlanDetailTab dialog

    class PlanDetailTab : public QWidget
    {
		Q_OBJECT
    // Construction
    public:
		PlanDetailTab(QWidget* parent);  // standard constructor
        virtual ~PlanDetailTab();

		void onResetPlanNodeView(PlanTreeModel* treeModel, QTabWidget* pPlanTab);
  //  // Operations
    protected:
  //      //virtual CPropertySheet *GetPropertySheet();
		PlanTreeModel* getTreeModel();
        virtual PlanNode* getPlanNode();
        PlanInstanceFilter* getPlanInstanceFilter();

        virtual void updatePlanDetail();
  //      virtual void AdjustControlPositions();
        virtual void setPlanFilterButtonStates();
		virtual CtlCheckBox* getAllOwnersCheck();
		virtual CtlCheckBox* getAllLocationsCheck();
		virtual void showRelevantControls() = 0;
        virtual void refreshControlContents() = 0;
  //      virtual void RefreshActiveInstances() = 0;
  //      virtual void PurgeActiveInstances() = 0;

  //  protected:
  //      virtual BOOL OnSetActive();
  //      virtual BOOL OnInitDialog();
  //      virtual BOOL PreTranslateMessage(MSG* pMsg);

  //  protected:
  //      void OnAllOwnersCheck();
  //      void OnAllLocationsCheck();
		

	private:
		bool m_isRefreshPending;

    //protected:
    //    CButton    m_allOwnersCheck;
    //    CButton    m_allLocationsCheck;
    };

    // {{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

} // namespace TA_Base_App

#endif // !defined(AFX_PLANDETAILPAGE_H__89B1A01E_7AD6_4BDA_83A6_3CF9DF0B2578__INCLUDED_)
