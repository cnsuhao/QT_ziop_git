/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanStepParameter.h $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  **/

#pragma once

#include <string>
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"


namespace TA_Base_App
{
    //class CPlanStepParameterEditDlg;
    class PlanStep;
    class CPlanStepParameterListCtrlModel;


    /////////////////////////////////////////////////////////////////////////////
    // PlanStepParameter

    class PlanStepParameter : public TA_Base_Bus::AbstractPModel
    {
        public:
            PlanStepParameter(PlanStep& parent, std::string name);
            virtual ~PlanStepParameter();

            virtual void addToParameterList(CPlanStepParameterListCtrlModel& listCtrlModel) = 0;

            virtual void editParameter();
            virtual void viewParameter();

            PlanStep* getParentStep() const
            {
                return &m_parent;
            }
            const std::string& getName() const;

        protected:
            //virtual TA_Base_App::CPlanStepParameterEditDlg* getEditDialog(bool canEdit) = 0;


        private:
            PlanStepParameter(const PlanStepParameter& other);
            PlanStepParameter& operator=(const PlanStepParameter& other);


            // Attributes
        private:
            std::string  m_name;
            PlanStep&    m_parent;
    };
}
