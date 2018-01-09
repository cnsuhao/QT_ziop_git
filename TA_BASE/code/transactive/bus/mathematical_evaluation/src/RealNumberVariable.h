#ifndef M_Eval_RealNumVariable_H
#define M_Eval_RealNumVariable_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/RealNumberVariable.h $
  * @author:  Ross Tucker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * A variable in a RealNumber expression.
  */

#include "bus/mathematical_evaluation/src/IRealNumberExpression.h"
#include "bus/mathematical_evaluation/src/ContextContainer.h"
#include <string>

namespace TA_Base_Bus
{
        class RealNumberVariable : public IRealNumberExpression
        {
        public:
      /**
        * Constructor
        * 
        */
        RealNumberVariable(const std::string& name, const ContextContainer& context);

      /**
        * Destructor
        * 
        */
        virtual ~RealNumberVariable();

      /**
        * evaluate
        *
        * Evaluates the expression and returns the result
        *
        */
        virtual double evaluate() const;


        private:

        // Disable copy constructor and assignment operator.
        RealNumberVariable( const RealNumberVariable& theRealNumberVariable);
        RealNumberVariable& operator=(const RealNumberVariable&);

        const ContextContainer& m_context;
        std::string m_name;
        };
}
#endif // M_Eval_RealNumVariable_H
