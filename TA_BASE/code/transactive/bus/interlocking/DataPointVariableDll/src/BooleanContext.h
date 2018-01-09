#ifndef BooleanContext_H
#define BooleanContext_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/interlocking/DataPointVariableDll/src/BooleanContext.h $
  * @author:  Daniel Hoey
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * DatapointContext is a class that provides resolution of datapoint variables.
  */

#include "bus\mathematical_evaluation\src\IBooleanContext.h"
#include "bus/scada/DataPointCorbaDef/src/DataPointCorbaDef.h"
#include "core/naming/src/INamedObject.h"
#include "core/naming/src/NamedObject.h"
#include "core/corba/src/CorbaUtil.h"

namespace TA_Base_Bus
{
	class BooleanContext : public IBooleanContext
	{
	public:
	   /**
	    * Constructor
	    * 
	    */
	    BooleanContext();

	   /**
	    * Destructor
	    * 
	    */
	    virtual ~BooleanContext();

        /**
         * getValue
         *
         * Gets the value of a variable. The variable will be resolved if it has been
         * already.
         *
         * @exception MathematicalEvaluationException - if variable cannot be found
         */
        virtual bool getValue(const std::string& variableName) const;

    private:        

			mutable TA_Base_Core::NamedObject<TA_Base_Bus::DataPointCorbaDef,
							             TA_Base_Bus::DataPointCorbaDef_ptr,
							             TA_Base_Bus::DataPointCorbaDef_var> m_dataPoint;

	};
}
#endif // DatapointContext_H
