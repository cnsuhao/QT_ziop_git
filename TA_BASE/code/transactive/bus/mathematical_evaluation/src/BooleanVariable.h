#ifndef M_Eval_BooleanVariable_H
#define M_Eval_BooleanVariable_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/BooleanVariable.h $
  * @author:  Daniel Hoey
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * A variable in a boolean expression.
  */

#include "bus/mathematical_evaluation/src/IBooleanExpression.h"
#include "bus/mathematical_evaluation/src/ContextContainer.h"
#include <string>

namespace TA_Base_Bus
{
        class BooleanVariable : public IBooleanExpression
        {
        public:
          /**
            * Constructor
            * 
            */
            BooleanVariable(const std::string& name, const ContextContainer& context);

          /**
            * Destructor
            * 
            */
        virtual ~BooleanVariable();

        /**
         * evaluate
         *
         * Evaluates the boolean expression and returns the result
         *
         */
        virtual bool evaluate() const;

        private:

        // Disable copy constructor and assignment operator.
        BooleanVariable( const BooleanVariable& theBooleanVariable);
        BooleanVariable& operator=(const BooleanVariable&);

        const ContextContainer& m_context;
        std::string m_name;
        };

}
#endif // M_Eval_BooleanVariable_H
