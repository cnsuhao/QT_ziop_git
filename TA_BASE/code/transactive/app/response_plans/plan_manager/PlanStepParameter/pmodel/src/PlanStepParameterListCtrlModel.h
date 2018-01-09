#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

namespace TA_Base_App
{
    class PlanStepParameter;
    class CPlanStepParameterListCtrlModel : public TA_Base_Bus::AbstractPModel
    {
        public:
            struct AddedParameterInfo
            {
                std::string name;
                std::string value;
                PlanStepParameter* planStep;

                AddedParameterInfo()
                    : planStep(NULL)
                {
                }

                AddedParameterInfo(const std::string& parameterName,
                                   const std::string& parameterValue,
                                   PlanStepParameter* planStepParameter)
                    : name(parameterName),
                      value(parameterValue),
                      planStep(planStepParameter)
                {
                }
            };

            void addParameter(const std::string& parameterName,
                              const std::string& parameterValue,
                              PlanStepParameter* planStepParameter);

            void notifyErrorOccur(const char* errorType);

            void setCurrentOperationPlanStepParameter(PlanStepParameter* stepParameter);
            PlanStepParameter* getCurrentOperationPlanStepParameter();

            AddedParameterInfo getCurrentAddedParameterInfo();
            CPlanStepParameterListCtrlModel(void);
            virtual ~CPlanStepParameterListCtrlModel(void);
        private:
            AddedParameterInfo m_currentAddedParameter;
            PlanStepParameter* m_currentOperationStepParameter;
    };
}
