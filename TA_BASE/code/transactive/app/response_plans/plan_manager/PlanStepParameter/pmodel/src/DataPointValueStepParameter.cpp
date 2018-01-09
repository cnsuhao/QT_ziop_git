/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/DataPointValueStepParameter.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  **/

#if defined (STEPTYPE_DATA_POINT_CHECK) || defined (STEPTYPE_DATA_POINT_SET)

#include "core/utilities/src/TAAssert.h"
#include "app/response_plans/plan_manager/src/PlanManagerPModelConsts.h"
#include "app/response_plans/plan_manager/src/PlanStep.h"
#include "app/response_plans/plan_manager/src/PlanNode.h"
#include "PlanStepParameterListCtrlModel.h"
#include "DataPointValueStepParameter.h"
//#include "PlanStepParameterEditDlg.h"
#include "PModelLabelMaker.h"
#include "core/exceptions/src/PlanStepException.h"
#include <sstream>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// PlanStepParameter

DataPointValueStepParameter::DataPointValueStepParameter(PlanStep& parent, std::string name, TA_Base_Core::DataPointValue value) :
    PlanStepParameter(parent, name),
    m_value(value)
{
    FUNCTION_ENTRY("DataPointValueStepParameter");
    FUNCTION_EXIT;
}


DataPointValueStepParameter::DataPointValueStepParameter(PlanStep& parent, const DataPointValueStepParameter& parameter) :
    PlanStepParameter(parent, parameter.getName()),
    m_value(parameter.getDataPointValue())
{
    FUNCTION_ENTRY("DataPointValueStepParameter");
    FUNCTION_EXIT;
}


DataPointValueStepParameter::DataPointValueStepParameter(const DataPointValueStepParameter& parameter) :
    PlanStepParameter(*parameter.getParentStep(), parameter.getName()),
    m_value(parameter.getDataPointValue())
{
    FUNCTION_ENTRY("DataPointValueStepParameter");
    FUNCTION_EXIT;
}


DataPointValueStepParameter::~DataPointValueStepParameter()
{
    FUNCTION_ENTRY("~DataPointValueStepParameter");
    FUNCTION_EXIT;
}


void DataPointValueStepParameter::addToParameterList(CPlanStepParameterListCtrlModel &listCtrl)
{
    FUNCTION_ENTRY("addToParameterList");

    std::string valueStr = (m_value._d() == TA_Base_Core::UNDEFINED_DATA_POINT ? "" : getDataPointValueAsString());

    listCtrl.addParameter(getName().c_str(), valueStr.c_str(), this);

    FUNCTION_EXIT;
}


std::string DataPointValueStepParameter::getDataPointValueAsString()
{
    FUNCTION_ENTRY("getDataPointValueAsString");

    switch(m_value._d())
    {
        case TA_Base_Core::BOOLEAN_DATA_POINT:
            FUNCTION_EXIT;
            return PModelLabelMaker::getTrueFalseLabel(m_value.booleanValue());

        case TA_Base_Core::NUMERIC_DATA_POINT:
            {
                std::ostringstream ss;
                ss << std::fixed << std::setprecision(m_value.numericValue().precision);
                ss << m_value.numericValue().value;
                FUNCTION_EXIT;
                return ss.str();
            }

        case TA_Base_Core::TEXT_DATA_POINT:
            FUNCTION_EXIT;
            return m_value.textValue();

        default:
            {
                std::ostringstream errorMessage;
                errorMessage << "Invalid data point value type (" << m_value._d() << ")";

                TA_THROW(TA_Base_Core::PlanStepException(errorMessage.str(), TA_Base_Core::DATA_POINT_SET_STEP));
            }
    }

    FUNCTION_EXIT;
    return "Undefined data point value";
}


void DataPointValueStepParameter::setDataPointValue(TA_Base_Core::DataPointValue value)
{
    FUNCTION_ENTRY("setDataPointValue");

    m_value = value;

    FUNCTION_EXIT;
}


void DataPointValueStepParameter::setDataPointValue(double value, unsigned long precision)
{
    FUNCTION_ENTRY("setDataPointValue");

    m_value.numericValue().value = value;
    m_value.numericValue().precision = precision;

    FUNCTION_EXIT;
}


void DataPointValueStepParameter::setDataPointValue(bool value)
{
    FUNCTION_ENTRY("setDataPointValue");

    m_value.booleanValue(value);

    FUNCTION_EXIT;
}


void DataPointValueStepParameter::setDataPointValue(const char * value)
{
    FUNCTION_ENTRY("setDataPointValue");

    m_value.textValue(CORBA::string_dup(value));

    FUNCTION_EXIT;
}


/*
TA_Base_App::CPlanStepParameterEditDlg* DataPointValueStepParameter::getEditDialog(bool canEdit)
{
    FUNCTION_ENTRY( "getEditDialog" );
    FUNCTION_EXIT;
    return new TA_Base_App::CPlanStepParameterEditDlg(*this,canEdit);
}
*/


#endif // defined (STEPTYPE_DATA_POINT_CHECK) || defined (STEPTYPE_DATA_POINT_SET)
