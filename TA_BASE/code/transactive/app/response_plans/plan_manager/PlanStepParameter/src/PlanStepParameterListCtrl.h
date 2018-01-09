//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanStepParameterListCtrl.h $
// @author:  Bart Golab
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2013/09/03 14:11:12 $
// Last modified by:  $Author: CM $
//
// <description>

#pragma once

#include "bus/generic_gui_view/src/AbstractTransActiveView.h"

namespace TA_Base_App
{
    class PlanStepParameter;
    class CPlanStepParameterListCtrlModel;

    /////////////////////////////////////////////////////////////////////////////
    // CPlanStepParameterListCtrl window

    class CPlanStepParameterListCtrl : public CListCtrl,
        public TA_Base_Bus::AbstractTransActiveView
    {
            // Construction
        public:
            CPlanStepParameterListCtrl();
            virtual ~CPlanStepParameterListCtrl();

            // Operations
        public:
            virtual BOOL SetReadOnly(BOOL readOnly = TRUE);

            virtual void notifyUpdate(const char* updateType, TA_Base_Bus::EventArgs* args = NULL);

            virtual void setupPModel();

            CPlanStepParameterListCtrlModel* getRealModel();

            // Attributes
        private:
            CPlanStepParameterListCtrlModel* m_realPModel;
            virtual void AddParameter(CString parameterName,
                                      CString parameterValue,
                                      PlanStepParameter* planStepParameter);

            // Overrides
            // ClassWizard generated virtual function overrides
            // {{AFX_VIRTUAL(CPlanStepParameterListCtrl)
        protected:
            virtual void PreSubclassWindow();
            virtual void createPModel();
            // }}AFX_VIRTUAL

            // Implementation
            // Generated message map functions
        protected:
            // {{AFX_MSG(CPlanStepParameterListCtrl)
            afx_msg void OnItemChanging(NMHDR* pNMHDR, LRESULT* pResult);
            afx_msg void OnDestroy();
            // }}AFX_MSG

            DECLARE_MESSAGE_MAP()
    };

    /////////////////////////////////////////////////////////////////////////////

    // {{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

} // namespace TA_Base_App