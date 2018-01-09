#ifndef M_Eval_NotExpression_H
#define M_Eval_NotExpression_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/NotExpression.h $
  * @author:  Robert Stagg
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * NotExpression represents a boolean NOT operation
  */
#include <memory>

#include "bus/mathematical_evaluation/src/IBooleanExpression.h"

namespace TA_Base_Bus
{
        class NotExpression : public IBooleanExpression
        {
        public:
      /**
        * Constructor
        *
        * Constructs an NotExpression. This object takes ownership of the 
        * rightHandSide pointers. Client of this class must 
        * *not* be delete them.
        * 
        */
        NotExpression(std::auto_ptr< IBooleanExpression > rightHandSide);

      /**
        * Destructor
        */
        virtual ~NotExpression();

      /**
        * evaluate
        *
        * Evaluates the boolean expression and returns 
        *  "NOT rightHandSide"
        *
        */
        virtual bool evaluate() const;

        private:

        // Disable copy constructor and assignment operator.
        NotExpression( const NotExpression& theNotExpression);
        NotExpression& operator=(const NotExpression&);

        std::auto_ptr< IBooleanExpression > m_rightHandSide;
        };
}
#endif // M_Eval_NotExpression_H
