/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/OrExpression.cpp $
  * @author:  Daniel Hoey
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  */

#include "bus/mathematical_evaluation/src/OrExpression.h"
#include "core/utilities/src/TAAssert.h"

namespace TA_Base_Bus
{
    OrExpression::OrExpression(std::auto_ptr< IBooleanExpression > leftHandSide,
                               std::auto_ptr< IBooleanExpression > rightHandSide)
    :m_leftHandSide(leftHandSide), m_rightHandSide(rightHandSide)
    {
        TA_ASSERT(m_leftHandSide.get()  != NULL, "leftHandSide is null!");
        TA_ASSERT(m_rightHandSide.get() != NULL, "rightHandSide is null!");
    }


    OrExpression::~OrExpression()
    {
        m_leftHandSide  = std::auto_ptr< IBooleanExpression >(static_cast< IBooleanExpression* >(NULL));
        m_rightHandSide = std::auto_ptr< IBooleanExpression >(static_cast< IBooleanExpression* >(NULL));
    }


    bool OrExpression::evaluate() const
    {
        return m_leftHandSide->evaluate() || m_rightHandSide->evaluate();
    }
}
