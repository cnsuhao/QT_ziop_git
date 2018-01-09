#if defined (STEPTYPE_ACTIVATE_CAMERA_PRESET)

#include "core/utilities/src/TAAssert.h"
#include "core/exceptions/src/TransactiveException.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/CameraPresetStepParameter.h"
//#include "PlanStepParameterEditDlg.h"
#include "app/response_plans/plan_manager/PlanStep/src/PlanStep.h"
#include "app/response_plans/plan_manager/src/PlanNode.h"
#include "app/response_plans/plan_manager/src/VideoEnquirer.h"
//#include "app/response_plans/plan_manager/src/MessageBox.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameterListCtrlModel.h"
#include <boost/lexical_cast.hpp>

using namespace TA_Base_App;

namespace
{
    const unsigned long DEFAULT_KEY = 0;
    const std::string UNKNOWN_NAME = "";
}

/////////////////////////////////////////////////////////////////////////////
// PlanStepParameter

CameraPresetStepParameter::CameraPresetStepParameter(PlanStep& parent, std::string name,
                                                     unsigned long cameraEntityKey,
                                                     unsigned long presetKey) :
    PlanStepParameter(parent, name),
    m_cameraKey(cameraEntityKey),
    m_presetKey(presetKey),
    m_presetName("")
{
    FUNCTION_ENTRY("CameraPresetStepParameter");

    try
    {
        VideoEnquirer enquirer;
        m_presetName = enquirer.getPresetName(m_cameraKey, m_presetKey);
    }
    catch(TA_Base_Core::TransactiveException& e)
    {
        LOG_EXCEPTION_CATCH(SourceInfo, "TransactiveException", e.what());
        m_presetName = UNKNOWN_NAME;
    }

    FUNCTION_EXIT;
}


CameraPresetStepParameter::CameraPresetStepParameter(PlanStep& parent, const CameraPresetStepParameter& parameter) :
    PlanStepParameter(parent, parameter.getName()),
    m_cameraKey(parameter.getCameraKey()),
    m_presetKey(parameter.getPresetKey()),
    m_presetName("")
{
    FUNCTION_ENTRY("CameraPresetStepParameter");

    try
    {
        VideoEnquirer enquirer;
        m_presetName = enquirer.getPresetName(m_cameraKey, m_presetKey);
    }
    catch(TA_Base_Core::TransactiveException& e)
    {
        LOG_EXCEPTION_CATCH(SourceInfo, "TransactiveException", e.what());
        m_presetName = UNKNOWN_NAME;
    }

    FUNCTION_EXIT;
}


CameraPresetStepParameter::CameraPresetStepParameter(const CameraPresetStepParameter& parameter) :
    PlanStepParameter(*parameter.getParentStep(), parameter.getName()),
    m_cameraKey(parameter.getCameraKey()),
    m_presetKey(parameter.getPresetKey()),
    m_presetName("")
{
    FUNCTION_ENTRY("CameraPresetStepParameter");

    try
    {
        VideoEnquirer enquirer;
        m_presetName = enquirer.getPresetName(m_cameraKey, m_presetKey);
    }
    catch(TA_Base_Core::TransactiveException& e)
    {
        LOG_EXCEPTION_CATCH(SourceInfo, "TransactiveException", e.what());
        m_presetName = UNKNOWN_NAME;
    }

    FUNCTION_EXIT;
}


CameraPresetStepParameter::~CameraPresetStepParameter()
{
    FUNCTION_ENTRY("~CameraPresetStepParameter");

    // Do nothing.

    FUNCTION_EXIT;
}


void CameraPresetStepParameter::addToParameterList(CPlanStepParameterListCtrlModel &listCtrlModel)
{
    FUNCTION_ENTRY("addToParameterList");

    listCtrlModel.addParameter(getName().c_str(), m_presetName.c_str(), this);

    FUNCTION_EXIT;
}


void CameraPresetStepParameter::setCameraKey(unsigned long cameraKey)
{
    FUNCTION_ENTRY("setCameraKey");

    //TA_ASSERT(getParentStep()->getParentPlan()->isCopiedPlan(),"Attempt to update the factory plan.");
    m_cameraKey = cameraKey;

    FUNCTION_EXIT;
}


void CameraPresetStepParameter::setPresetKey(unsigned long presetKey)
{
    FUNCTION_ENTRY("setPresetKey");

    //TA_ASSERT(getParentStep()->getParentPlan()->isCopiedPlan(),"Attempt to update the factory plan.");
    m_presetKey = presetKey;

    FUNCTION_EXIT;
}


void CameraPresetStepParameter::setPresetName(const std::string& presetName)
{
    FUNCTION_ENTRY("setPresetName");

    //TA_ASSERT(getParentStep()->getParentPlan()->isCopiedPlan(),"Attempt to update the factory plan.");
    m_presetName = presetName;

    FUNCTION_EXIT;
}


/*
TA_Base_App::CPlanStepParameterEditDlg* CameraPresetStepParameter::getEditDialog(bool canEdit)
{
    FUNCTION_ENTRY( "getEditDialog" );
    FUNCTION_EXIT;
    return new TA_Base_App::CPlanStepParameterEditDlg(*this,canEdit);
}
*/


#endif // defined(STEPTYPE_ACTIVATE_CAMERA_PRESET)
