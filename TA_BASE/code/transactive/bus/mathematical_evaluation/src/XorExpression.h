#ifndef M_Eval_XorExpression_H
#define M_Eval_XorExpression_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/XorExpression.h $
  * @author:  Daniel Hoey
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * XorExpression represents a logical XOR operation
  */
#include <memory>

#include "bus/mathematical_evaluation/src/IBooleanExpression.h"

namespace TA_Base_Bus
{
        class XorExpression : public IBooleanExpression
        {
        public:
      /**
        * Constructor
        *
        * Constructs an XorExpression. This object takes ownership of the 
        * leftHandSide and rightHandSide pointers. Client of this class must 
        * *not* be delete them.
        * 
        */
        XorExpression(std::auto_ptr< IBooleanExpression > leftHandSide,
                      std::auto_ptr< IBooleanExpression > rightHandSide);

      /**
        * Destructor
        */
        virtual ~XorExpression();

      /**
        * evaluate
        *
        * Evaluates the boolean expression and returns 
        *  "leftHandSide XOR rightHandSide"
        *
        */
        virtual bool evaluate() const;

        private:

        // Disable copy constructor and assignment operator.
        XorExpression( const XorExpression& theXorExpression);
        XorExpression& operator=(const XorExpression&);

        std::auto_ptr< IBooleanExpression > m_leftHandSide;
        std::auto_ptr< IBooleanExpression > m_rightHandSide;
        };
}
#endif // M_Eval_XorExpression_H
