/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/YesNoStepParameter.h $
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
#include "PlanStepParameter.h"


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // YesNoStepParameter

    class CPlanStepParameterListCtrlModel;
    class YesNoStepParameter : public PlanStepParameter
    {
        public:
            YesNoStepParameter(PlanStep& parent, std::string name, bool affirmative);
            YesNoStepParameter(PlanStep& parent, const YesNoStepParameter& parameter);
            YesNoStepParameter(const YesNoStepParameter& parameter);
            virtual ~YesNoStepParameter();

            virtual void addToParameterList(CPlanStepParameterListCtrlModel& listCtrl);

            bool getAffirmative() const
            {
                return m_affirmative;
            }
            void setAffirmative(bool affirmative);

        protected:

            //virtual CPlanStepParameterEditDlg* getEditDialog(bool canEdit);

        private:
            YesNoStepParameter& operator=(const YesNoStepParameter& parameter);


            // Attributes
        private:
            bool m_affirmative;
    };
}