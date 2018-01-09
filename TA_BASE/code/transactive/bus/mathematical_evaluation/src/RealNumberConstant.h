#ifndef M_Eval_RealNumConstant_H
#define M_Eval_RealNumConstant_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/RealNumberConstant.h $
  * @author:  Ross Tucker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * A constant in a real number expression.
  */

#include "bus/mathematical_evaluation/src/IRealNumberExpression.h"

namespace TA_Base_Bus
{
        class RealNumberConstant : public IRealNumberExpression
        {
        public:
        /**
          * Constructor
          * 
          */
        RealNumberConstant(double value);
     
        /**
          * Destructor
          * 
          */
        virtual ~RealNumberConstant();

        /**
         * evaluate
         *
         * Evaluates the expression and returns the result
         *
         */
        virtual double evaluate() const;

        // Allow RealNumberConstant to be copyable.
        RealNumberConstant( const RealNumberConstant& theRealNumberConstant);
        
        private:

        // Disable assignment operator.
        RealNumberConstant& operator=(const RealNumberConstant&);

        double m_value;
        };
}
#endif // M_Eval_RealNumConstant_H
