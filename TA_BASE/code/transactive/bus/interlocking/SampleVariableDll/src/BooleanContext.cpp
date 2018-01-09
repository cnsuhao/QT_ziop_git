/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/interlocking/SampleVariableDll/src/BooleanContext.cpp $
  * @author:  Daniel Hoey
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * DatapointContext is a class that provides resolution of datapoint variables.
  */

#include "stdafx.h"
#include "BooleanContext.h"

namespace TA_Base_Bus
{
   /**
	* Constructor
	* 
	*/
    BooleanContext::BooleanContext()
    {
    }

   /**
	* Destructor
	* 
	*/
    BooleanContext::~BooleanContext()
    {
    }

    /**
     * getValue
     *
     * Gets the value of a variable. 
     *
     * @exception MathematicalEvaluationException - if variable cannot be found
     */
    bool BooleanContext::getValue(const std::string& variableName) const
    {
        // TODO add the required code to correctly evaluate this variable
        if ( variableName.length() > 4)
        {
            return true;
        }
        return false;
    }

}
