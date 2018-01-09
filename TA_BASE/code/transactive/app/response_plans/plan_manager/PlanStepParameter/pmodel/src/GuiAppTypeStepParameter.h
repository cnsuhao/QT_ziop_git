/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/GuiAppTypeStepParameter.h $
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

#if defined (STEPTYPE_LAUNCH_GUI_APP)

#include <string>
#include "NumericStepParameter.h"


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // GuiAppTypeStepParameter

    class CPlanStepParameterListCtrlModel;
    class GuiAppTypeStepParameter : public NumericStepParameter
    {
            // Operations
        public:
            GuiAppTypeStepParameter(PlanStep& parent, std::string name, unsigned long appType, bool isHidden = false);
            GuiAppTypeStepParameter(PlanStep& parent, const GuiAppTypeStepParameter& parameter);
            GuiAppTypeStepParameter(const GuiAppTypeStepParameter& parameter);
            virtual ~GuiAppTypeStepParameter();

            virtual void addToParameterList(CPlanStepParameterListCtrlModel& listCtrl);

            std::string getApplicationName();

        private:
            GuiAppTypeStepParameter& operator=(const GuiAppTypeStepParameter& parameter);
    };
}

#endif // defined (STEPTYPE_LAUNCH_GUI_APP)