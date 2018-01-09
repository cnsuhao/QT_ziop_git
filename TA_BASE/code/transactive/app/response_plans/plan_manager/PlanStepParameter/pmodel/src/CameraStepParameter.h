#pragma once

#if defined (STEPTYPE_ASSIGN_CAMERA_TO_MONITOR) || defined (STEPTYPE_ACTIVATE_CAMERA_PRESET)

#include <string>
#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameter.h"


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // CameraStepParameter
    class CPlanStepParameterListCtrlModel;

    class CameraStepParameter : public PlanStepParameter
    {
            // Operations
        public:
            CameraStepParameter(PlanStep& parent, std::string name, unsigned long cameraKey);
            CameraStepParameter(PlanStep& parent, const CameraStepParameter& parameter);
            CameraStepParameter(const CameraStepParameter& parameter);
            virtual ~CameraStepParameter();

            virtual void addToParameterList(CPlanStepParameterListCtrlModel& listCtrlModel);

            void setCameraKey(unsigned long cameraKey);
            void setCameraName(const std::string& cameraName);
            unsigned long getCameraKey() const
            {
                return m_cameraKey;
            }
            std::string getCameraName() const
            {
                return m_cameraName;
            }

            // this method relies on the exceptions thrown
            // by VideoSwitchAgentFactory to determine availability of
            // any cameras in the system
            bool isAnyCameraAvailable();

        protected:
            //virtual TA_Base_App::CPlanStepParameterEditDlg* getEditDialog(bool canEdit);

        private:
            CameraStepParameter& operator=(const CameraStepParameter& parameter);


            // Attributes
        private:
            unsigned long m_cameraKey;
            std::string m_cameraName;
    };
}

#endif // defined(STEPTYPE_ASSIGN_CAMERA_TO_MONITOR) || defined(STEPTYPE_ACTIVATE_CAMERA_PRESET)