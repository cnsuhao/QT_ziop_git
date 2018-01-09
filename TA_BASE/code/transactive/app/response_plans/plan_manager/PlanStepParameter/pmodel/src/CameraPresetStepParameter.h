#pragma once

#if defined (STEPTYPE_ACTIVATE_CAMERA_PRESET)

#include <string>
#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameter.h"


namespace TA_Base_App
{
    // class CPlanStepParameterEditDlg;


    /////////////////////////////////////////////////////////////////////////////
    // CameraPresetStepParameter

    class CPlanStepParameterListCtrlModel;

    class CameraPresetStepParameter : public PlanStepParameter
    {
            // Operations
        public:
            CameraPresetStepParameter(PlanStep& parent, std::string name, unsigned long cameraKey, unsigned long presetKey);
            CameraPresetStepParameter(PlanStep& parent, const CameraPresetStepParameter& parameter);
            CameraPresetStepParameter(const CameraPresetStepParameter& parameter);
            virtual ~CameraPresetStepParameter();

            virtual void addToParameterList(CPlanStepParameterListCtrlModel& listCtrl);

            // You have to know the camera key to get the preset name.

            void setCameraKey(unsigned long cameraKey);
            void setPresetKey(unsigned long presetKey);
            void setPresetName(const std::string& presetName);
            unsigned long getCameraKey() const
            {
                return m_cameraKey;
            }
            unsigned long getPresetKey() const
            {
                return m_presetKey;
            }
            std::string getPresetName() const
            {
                return m_presetName;
            }

        protected:
            //virtual TA_Base_App::CPlanStepParameterEditDlg* getEditDialog(bool canEdit);

        private:
            CameraPresetStepParameter& operator=(const CameraPresetStepParameter& parameter);


            // Attributes
        private:
            unsigned long m_cameraKey;
            unsigned long m_presetKey;
            std::string m_presetName;
    };
}

#endif // defined(STEPTYPE_ACTIVATE_CAMERA_PRESET)