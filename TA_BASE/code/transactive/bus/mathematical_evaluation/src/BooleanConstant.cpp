/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/BooleanConstant.cpp $
  * @author:  Daniel Hoey
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  */

#include "bus/mathematical_evaluation/src/BooleanConstant.h"

namespace TA_Base_Bus
{
    BooleanConstant::BooleanConstant(bool value)
        :m_value(value)
    {
    }

    BooleanConstant::~BooleanConstant()
    {
    }

    bool BooleanConstant::evaluate() const
    {
        return m_value;
    }
}
