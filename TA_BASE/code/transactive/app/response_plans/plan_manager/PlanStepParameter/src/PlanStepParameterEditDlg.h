//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanStepParameterEditDlg.h $
// @author:  Andy Parker
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2013/09/03 14:11:12 $
// Last modified by:  $Author: CM $
//
// <description>

#pragma once

#include "app/response_plans/plan_manager/src/StdAfx.h"
#include "app/response_plans/plan_manager/src/resource.h"

namespace TA_Base_App
{
    class PlanStepParameter;

    /////////////////////////////////////////////////////////////////////////////
    // CPlanStepParameterEditDlg dialog

    class CPlanStepParameterEditDlg : public CDialog
    {
            // Construction
        public:
            CPlanStepParameterEditDlg(PlanStepParameter& parameter, bool canEdit);  // standard constructor
            CPlanStepParameterEditDlg(PlanStepParameter& parameter, UINT templateID, bool canEdit); // called by derived classes

            PlanStepParameter* GetStepParameter() const
            {
                return &m_parameter;
            };

            // Dialog Data
            // {{AFX_DATA(CPlanStepParameterEditDlg)
            enum { IDD = IDD_PLAN_STEP_PARAMETER_EDIT_DLG };
            CString    m_parameterName;
            // }}AFX_DATA


            // Overrides
            // ClassWizard generated virtual function overrides
            // {{AFX_VIRTUAL(CPlanStepParameterEditDlg)
        protected:
            virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
            virtual BOOL OnInitDialog();
            virtual afx_msg void OnAdjustFont();
            // }}AFX_VIRTUAL

            // Implementation
        protected:

            bool                 m_canEdit;
            PlanStepParameter&   m_parameter;

            // Generated message map functions
            // {{AFX_MSG(CPlanStepParameterEditDlg)
            // }}AFX_MSG
            DECLARE_MESSAGE_MAP()
    };

    // {{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

}  // End namespace
