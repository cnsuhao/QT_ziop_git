#ifndef M_Eval_IStringExpression_H
#define M_Eval_IStringExpression_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mathematical_evaluation/src/IStringExpression.h $
  * @author:  Daniel Hoey 
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * IStringExpression is an interface that allows evaluation of string
  * expressions
  */
#include <string>

namespace TA_Base_Bus
{
    class IStringExpression
    {
    public:
       /**
        * Destructor
        * 
        */
        virtual ~IStringExpression()
		{
		}


        /**
         * evaluate
         *
         * Evaluates the string expression and returns the result
         *
         */
        virtual std::string evaluate() const = 0;
    };
}
#endif // M_Eval_IStringExpression_H
