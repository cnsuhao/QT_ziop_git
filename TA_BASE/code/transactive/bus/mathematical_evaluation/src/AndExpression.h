#ifndef M_Eval_AndExpression_H
#define M_Eval_AndExpression_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/AndExpression.h $
  * @author:  Daniel Hoey
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * AndExpression is an object the represent a logical AND operation
  */
#include <memory>

#include "bus/mathematical_evaluation/src/IBooleanExpression.h"

namespace TA_Base_Bus
{
        class AndExpression : public IBooleanExpression
		{
        public:
        /**
          * Constructor
          * 
          * Constructs an AndExpression. This object takes ownership of the 
          * leftHandSide and rightHandSide pointers. Client of this class must 
          * *not* be delete them.
          *
          */
            AndExpression(std::auto_ptr< IBooleanExpression > leftHandSide,
                          std::auto_ptr< IBooleanExpression > rightHandSide);

        /**
          * Destructor
          */
        virtual ~AndExpression();  
       
        /**
          * evaluate
          *
          * Evaluates the boolean expression and returns the logical AND of the left and right hand sides.
          *
          */
        virtual bool evaluate() const;

		private:

			// Disable copy constructor and assignment operator.
			AndExpression( const AndExpression& theAndExpression);
			AndExpression& operator=(const AndExpression&);

			std::auto_ptr< IBooleanExpression > m_leftHandSide;
			std::auto_ptr< IBooleanExpression > m_rightHandSide;
		};
}
#endif // M_Eval_AndExpression_H
