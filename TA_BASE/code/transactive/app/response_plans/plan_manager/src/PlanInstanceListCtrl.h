//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanInstanceListCtrl.h $
// @author:  Bart Golab
// @version: $Revision: #2 $
//
// Last modification: $DateTime: 2017/05/22 15:28:33 $
// Last modified by:  $Author: hoa.le $
//
// <description>

#if !defined(AFX_PLANINSTANCELISTCTRL_H__2D9A15A8_FC42_4F1B_86D0_C22830B8F2BA__INCLUDED_)
#define AFX_PLANINSTANCELISTCTRL_H__2D9A15A8_FC42_4F1B_86D0_C22830B8F2BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/response_plans/plan_manager/pmodel/src/PlanManagerConsts.h"
#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "bus/response_plans/active_plans_display/src/PlanActiveInstanceListCtrl.h"


namespace TA_Base_App
{
    class PlanInstance;
    class PlanNode;


    /////////////////////////////////////////////////////////////////////////////
    // PlanInstanceListCtrl

    class PlanInstanceListCtrl : public TA_Base_Bus::PlanActiveInstanceListCtrl
    {
    // Construction
    public:
        PlanInstanceListCtrl(bool planPathVisible = false);
        virtual ~PlanInstanceListCtrl();


    // Operations
    //public:
    //    virtual void SetSelectedInstance(PlanInstanceSharedPtr planInstance);
    //    virtual PlanInstanceSharedPtr GetSelectedInstance();
    //    virtual PlanNode *GetSelectedPlan();

    //protected:
    //    virtual void SelectItem(int item, bool selected = true);

    // Attributes
    private:

        // Generated message map functions
    protected:
        //LRESULT OnActivePlanDetailUpdate(WPARAM wParam, LPARAM lParam);
    };

    /////////////////////////////////////////////////////////////////////////////

    // {{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

} // namespace TA_Base_App

#endif // !defined(AFX_PLANINSTANCELISTCTRL_H__2D9A15A8_FC42_4F1B_86D0_C22830B8F2BA__INCLUDED_)
