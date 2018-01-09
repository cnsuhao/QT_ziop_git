//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/LabelMaker.h $
// @author:  Bart Golab
// @version: $Revision: #2 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// <description>

#if !defined(AFX_LABELMAKER_H__A6EF7B1D_89CB_4405_905A_98F3C2A307FD__INCLUDED_)
#define AFX_LABELMAKER_H__A6EF7B1D_89CB_4405_905A_98F3C2A307FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // LabelMaker

    class LabelMaker
    {
    // Operations
    public:
        LabelMaker();
        virtual ~LabelMaker();

        static std::string getAllPlanTreeRootNodeLabel();
        static std::string getApprovedPlanTreeRootNodeLabel();
        static std::string getUnapprovedPlanTreeRootNodeLabel();
        static std::string getPlanGeneralDetailTabLabel();
        static std::string getPlanStepDetailTabLabel();
        static std::string getActivePlanListPlanColumnLabel();
        static std::string getActivePlanListCategoryColumnLabel();
        static std::string getActivePlanListInstanceColumnLabel();
        static std::string getActivePlanListOwnerColumnLabel();
        static std::string getActivePlanListLocationColumnLabel();
        static std::string getActivePlanListActivationColumnLabel();
        static std::string getActivePlanListStateColumnLabel();
        static std::string getActivePlanListStepColumnLabel();
        static std::string getActivePlanListRemarksColumnLabel();
        static std::string getStepListStepColumnLabel();
        static std::string getStepListNameColumnLabel();
        static std::string getStepListTypeColumnLabel();
        static std::string getStepListDescriptionColumnLabel();
        static std::string getStepListSkipColumnLabel();
        static std::string getStepListIgnoreFailureColumnLabel();
		static std::string getStepListNoWaitColumnLabel();
        static std::string getStepListDelayColumnLabel();
        static std::string getStepParameterListNameColumnLabel();
        static std::string getStepParameterListValueColumnLabel();
		// Referenced from StringDefn.h instead
        //static std::string getNodeListNameColumnLabel();
        //static std::string getNodeListTypeColumnLabel();
        //static std::string getNodeListStatusColumnLabel();
        static std::string getTisMessageListTypeColumnLabel();
        static std::string getTisMessageListIdColumnLabel();
        static std::string getTisMessageListMessageColumnLabel();
        static std::string getTisMessageListPriorityColumnLabel();
        static std::string getYesNoLabel(bool affirmative);
        static std::string getTrueFalseLabel(bool condition);
        static std::string getNewActivePlanInstanceLabel();
        static std::string getRootNodeTypeLabel();
        static std::string getCategoryNodeTypeLabel();
        static std::string getPlanNodeTypeLabel();
        static std::string getEmptyCategoryLabel();
        static std::string getNonEmptyCategoryLabel();
        static std::string getCategoryHeadingPrefix();
        static std::string getPlanHeadingPrefix();
        static std::string getStepHeadingPrefix();
        static std::string getDecisionStepDialogTitlePrefix();
        static std::string getLogEntryStepDialogTitlePrefix();
        static std::string getPromptStepDialogTitlePrefix();
        static std::string getRunPlanStepDialogTitlePrefix();
        static std::string getLaunchGuiAppStepDialogTitlePrefix();
        static std::string getLaunchSchematicStepDialogTitlePrefix();
        static std::string getFailureSummaryStepDialogTitlePrefix();
    private:
		static std::string getLabel(const char *definedLabel, const char *defaultLabel);
    };

    /////////////////////////////////////////////////////////////////////////////

    // {{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

} // namespace TA_Base_App

#endif // !defined(AFX_LABELMAKER_H__A6EF7B1D_89CB_4405_905A_98F3C2A307FD__INCLUDED_)
