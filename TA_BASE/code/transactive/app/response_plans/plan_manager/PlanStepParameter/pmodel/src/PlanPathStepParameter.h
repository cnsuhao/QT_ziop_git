#pragma once

#include <string>
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameter.h"
#include "app/response_plans/plan_manager/PlanStep/src/PlanStepCommonDef.h"

namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // PlanPathStepParameter

    class CPlanStepParameterListCtrlModel;
    class PlanPathStepParameter : public PlanStepParameter
    {
            // Operations
        public:
            PlanPathStepParameter(PlanStep& parent, std::string name, TreeNodeId planNodeId);
            PlanPathStepParameter(PlanStep& parent, PlanPathStepParameter& parameter);
            PlanPathStepParameter(PlanPathStepParameter& parameter);
            virtual ~PlanPathStepParameter();

            virtual void addToParameterList(CPlanStepParameterListCtrlModel& listCtrl);

            TreeNodeId getPlanNodeId() const
            {
                return m_planNodeId;
            }
            void setPlanNodeId(TreeNodeId planNodeId);
            std::string getPlanPath();

        protected:
            //virtual CPlanStepParameterEditDlg* getEditDialog(bool canEdit);

        private:
            PlanPathStepParameter& operator=(const PlanPathStepParameter& parameter);


            // Attributes
        private:
            TreeNodeId m_planNodeId;
    };
}