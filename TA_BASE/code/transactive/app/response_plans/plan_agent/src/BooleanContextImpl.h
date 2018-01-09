#ifndef BooleanContextImpl_H
#define BooleanContextImpl_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/BooleanContextImpl.h $
  * @author:  Ross Tucker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * Boolean Context 
  */

#include "bus/mathematical_evaluation/src/IBooleanContext.h"
#include "core/exceptions/src/MathematicalEvaluationException.h"
#include "core/utilities/src/DebugUtil.h"

namespace TA_Base_App
{
	class BooleanContextImpl : public TA_Base_Bus::IBooleanContext
	{
	public:
	  /**
	    * Constructor
	    * 
	    */
        BooleanContextImpl(){}

	  /**
	    * Destructor
	    * 
	    */
        virtual ~BooleanContextImpl(){}

        virtual bool getValue(const std::string& variableName) const
        {
            return false;
        }  
	};
	
}
#endif // BooleanContextImpl_H
