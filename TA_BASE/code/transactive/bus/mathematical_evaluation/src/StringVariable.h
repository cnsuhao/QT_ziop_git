#ifndef M_Eval_StringVariable_H
#define M_Eval_StringVariable_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/StringVariable.h $
  * @author:  Daniel Hoey
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * A variable in a boolean expression.
  */

#include "bus/mathematical_evaluation/src/IStringExpression.h"
#include "bus/mathematical_evaluation/src/ContextContainer.h"
#include <string>

namespace TA_Base_Bus
{
        class StringVariable : public IStringExpression
        {
        public:
      /**
        * Constructor
        * 
        */
        StringVariable(const std::string& name, const ContextContainer& context);

      /**
        * Destructor
        * 
        */
        virtual ~StringVariable();

      /**
        * evaluate
        *
        * Evaluates the string expression and returns the result
        *
        */
        virtual std::string evaluate() const;

        private:

        // Disable copy constructor and assignment operator.
        StringVariable( const StringVariable& theStringVariable);
        StringVariable& operator=(const StringVariable&);

        const ContextContainer& m_context;
        std::string m_name;
        };
}
#endif // M_Eval_StringVariable_H
