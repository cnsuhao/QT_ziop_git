/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/RealNumberVariable.cpp $
  * @author:  Ross Tucker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  */

#include "bus/mathematical_evaluation/src/RealNumberVariable.h"

namespace TA_Base_Bus
{
    RealNumberVariable::RealNumberVariable(const std::string& name, const ContextContainer& context)
    :m_context(context), m_name(name)
    {
    }


    RealNumberVariable::~RealNumberVariable()
    {
    }


    double RealNumberVariable::evaluate() const
    {
        return m_context.getRealNumberValue(m_name);
    }
}
