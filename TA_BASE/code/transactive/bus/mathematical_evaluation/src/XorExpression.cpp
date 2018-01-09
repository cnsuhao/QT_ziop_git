/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/XorExpression.cpp $
  * @author:  Daniel Hoey
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  */

#include "bus/mathematical_evaluation/src/XorExpression.h"
#include "core/utilities/src/TAAssert.h"

namespace TA_Base_Bus
{
    XorExpression::XorExpression(std::auto_ptr< IBooleanExpression > leftHandSide,
                                 std::auto_ptr< IBooleanExpression > rightHandSide)
    :m_leftHandSide(leftHandSide), m_rightHandSide(rightHandSide)
    {
        TA_ASSERT(m_leftHandSide.get()  != NULL, "leftHandSide is null!");
        TA_ASSERT(m_rightHandSide.get() != NULL, "rightHandSide is null!");
    }


    XorExpression::~XorExpression()
    {
        m_leftHandSide  = std::auto_ptr< IBooleanExpression >(static_cast< IBooleanExpression* >(NULL));
        m_rightHandSide = std::auto_ptr< IBooleanExpression >(static_cast< IBooleanExpression* >(NULL));
    }


    bool XorExpression::evaluate() const
    {
        return m_leftHandSide->evaluate() && !m_rightHandSide->evaluate() ||
               m_rightHandSide->evaluate() && !m_leftHandSide->evaluate();
    }
}

