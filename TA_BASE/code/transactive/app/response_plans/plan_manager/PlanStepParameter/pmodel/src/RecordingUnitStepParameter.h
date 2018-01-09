/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/RecordingUnitStepParameter.h $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  **/
#if !defined(AFX_RECORDINGUNITSTEPPARAMETER_H__C2F24121_97C5_4248_A8ED_AE3F93792A2C__INCLUDED_)
#define AFX_RECORDINGUNITSTEPPARAMETER_H__C2F24121_97C5_4248_A8ED_AE3F93792A2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "PlanStepParameter.h"


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // RecordingUnitStepParameter

    class CPlanStepParameterListCtrlModel;
    class RecordingUnitStepParameter : public PlanStepParameter
    {
            // Operations
        public:
            RecordingUnitStepParameter(PlanStep& parent, std::string name, std::string recordingUnit);
            RecordingUnitStepParameter(PlanStep& parent, const RecordingUnitStepParameter& parameter);
            RecordingUnitStepParameter(const RecordingUnitStepParameter& parameter);
            virtual ~RecordingUnitStepParameter();

            virtual void addToParameterList(CPlanStepParameterListCtrlModel& listCtrl);

            std::string getRecordingUnit() const
            {
                return m_recordingUnit;
            }

        protected:
            //virtual CPlanStepParameterEditDlg* getEditDialog(bool canEdit);

        private:
            RecordingUnitStepParameter& operator=(const RecordingUnitStepParameter& parameter);


            // Attributes
        private:
            std::string m_recordingUnit;
    };
}

#endif // !defined(AFX_RECORDINGUNITSTEPPARAMETER_H__C2F24121_97C5_4248_A8ED_AE3F93792A2C__INCLUDED_)
