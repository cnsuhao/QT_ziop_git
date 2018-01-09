/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/RealNumberConstant.cpp $
  * @author:  Ross Tucker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  */

#include "bus/mathematical_evaluation/src/RealNumberConstant.h"

namespace TA_Base_Bus
{
    RealNumberConstant::RealNumberConstant(double value)
    :m_value(value)
    {
    }

    RealNumberConstant::~RealNumberConstant()
    {
    }

    double RealNumberConstant::evaluate() const
    {
        return m_value;
    }
}
