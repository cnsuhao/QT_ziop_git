/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/RealNumberPowerOfExpression.cpp $
  * @author:  Ross Tucker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  */

#include <math.h>

#include "bus/mathematical_evaluation/src/RealNumberPowerOfExpression.h"
#include "core/utilities/src/TAAssert.h"

namespace TA_Base_Bus
{
    RealNumberPowerOfExpression::RealNumberPowerOfExpression(std::auto_ptr< IRealNumberExpression > leftHandSide,
                                                             std::auto_ptr< IRealNumberExpression > rightHandSide)
    :m_leftHandSide(leftHandSide), m_rightHandSide(rightHandSide)
    {
        TA_ASSERT(m_leftHandSide.get()  != NULL, "leftHandSide is null!");
        TA_ASSERT(m_rightHandSide.get() != NULL, "rightHandSide is null!");
    }


    RealNumberPowerOfExpression::~RealNumberPowerOfExpression()
    {
        m_leftHandSide  = std::auto_ptr< IRealNumberExpression >(static_cast< IRealNumberExpression* >(NULL));
        m_rightHandSide = std::auto_ptr< IRealNumberExpression >(static_cast< IRealNumberExpression* >(NULL));
    }


    double RealNumberPowerOfExpression::evaluate() const
    {
        return pow(m_leftHandSide->evaluate(), m_rightHandSide->evaluate());
    }
}
