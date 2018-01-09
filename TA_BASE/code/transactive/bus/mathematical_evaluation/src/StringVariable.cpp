/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/StringVariable.cpp $
  * @author:  Daniel Hoey
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  */

#include "bus/mathematical_evaluation/src/StringVariable.h"

namespace TA_Base_Bus
{
    StringVariable::StringVariable(const std::string& name, const ContextContainer& context)
    :m_context(context), m_name(name)
    {
    }


    StringVariable::~StringVariable()
    {
    }


    std::string StringVariable::evaluate() const
    {
        return m_context.getStringValue(m_name);
    }
}
