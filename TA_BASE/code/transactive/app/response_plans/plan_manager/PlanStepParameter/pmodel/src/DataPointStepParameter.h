/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/DataPointStepParameter.h $
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

#if defined (STEPTYPE_DATA_POINT_CHECK) || defined (STEPTYPE_DATA_POINT_SET) || defined (STEPTYPE_DATA_POINT_TIME_CHECK)

#include <string>
#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "PlanStepParameter.h"

namespace TA_Base_App
{
    class CPlanStepParameterListCtrlModel;
    /////////////////////////////////////////////////////////////////////////////
    // DataPointStepParameter

    class DataPointStepParameter : public PlanStepParameter
    {
            // Operations
        public:
            DataPointStepParameter(PlanStep& parent, std::string name, std::string dataPoint);
            DataPointStepParameter(PlanStep& parent, DataPointStepParameter& parameter);
            DataPointStepParameter(DataPointStepParameter& parameter);
            virtual ~DataPointStepParameter();

            virtual void addToParameterList(CPlanStepParameterListCtrlModel& listCtrl);

            std::string getDataPoint() const
            {
                return m_dataPoint;
            }
            void setDataPoint(std::string dataPoint);

        protected:
            //virtual CPlanStepParameterEditDlg* getEditDialog(bool canEdit);

        private:
            DataPointStepParameter& operator=(const DataPointStepParameter& parameter);


            // Attributes
        private:
            std::string m_dataPoint;
    };
}

#endif // defined (STEPTYPE_DATA_POINT_CHECK) || defined (STEPTYPE_DATA_POINT_SET) || defined (STEPTYPE_DATA_POINT_TIME_CHECK)
