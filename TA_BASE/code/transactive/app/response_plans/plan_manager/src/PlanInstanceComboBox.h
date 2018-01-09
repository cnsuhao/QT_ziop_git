//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanInstanceComboBox.h $
// @author:  Bart Golab
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2015/10/09 15:42:35 $
// Last modified by:  $Author: CM $
//
// <description>

#if !defined(AFX_PLANINSTANCECOMBOBOX_H__0CFA9854_1D87_4531_862F_AF94DB27FDB9__INCLUDED_)
#define AFX_PLANINSTANCECOMBOBOX_H__0CFA9854_1D87_4531_862F_AF94DB27FDB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "PlanManagerCommonDefs.h"


namespace TA_Base_App
{
    class PlanInstance;
    class PlanInstanceFilter;


    /////////////////////////////////////////////////////////////////////////////
    // CPlanInstanceComboBox window

    class CPlanInstanceComboBox : public CComboBox
    {
    // Construction
    public:
        CPlanInstanceComboBox();
        virtual ~CPlanInstanceComboBox();

    // Operations
    public:
        virtual void SetPlanInstanceFilter(PlanInstanceFilter *planInstanceFilter);
        virtual bool IsNewInstance(PlanInstance *planInstance);
        virtual void AddInstance(PlanInstance *planInstance);
        virtual void RemoveInstance(const PlanInstanceId &planInstanceId);
        virtual void RepositionInstance(PlanInstance *planInstance);
        virtual void SetSelectedInstance(PlanInstance *planInstance);
        virtual PlanInstance *GetSelectedInstance();
        virtual void Purge();

    protected:
        virtual CString GetPlanInstanceLabel(PlanInstance *planInstance);
        virtual CString GetPlanInstanceLabel(const PlanInstanceId &planInstanceId);
        virtual void SelectItem(int item);


    // Attributes
    private:
        PlanInstance *m_currentSelection;
        PlanInstanceFilter *m_planInstanceFilter;

    // Overrides
    protected:
        // ClassWizard generated virtual function overrides
        // {{AFX_VIRTUAL(CPlanInstanceComboBox)
        // }}AFX_VIRTUAL

    // Implementation
        // Generated message map functions
    protected:
        // {{AFX_MSG(CPlanInstanceComboBox)
        afx_msg BOOL OnItemSelected();
        // }}AFX_MSG
        DECLARE_MESSAGE_MAP()
    };

    /////////////////////////////////////////////////////////////////////////////

    // {{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

} // namespace TA_Base_App

#endif // !defined(AFX_PLANINSTANCECOMBOBOX_H__0CFA9854_1D87_4531_862F_AF94DB27FDB9__INCLUDED_)
