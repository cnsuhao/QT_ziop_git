/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/interlocking/InterlockingConfig/src/VariableTypeFactory.h $
  * @author:  Robert Stagg
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  *
  */

#if !defined(VariableTypeFactory_H_)
#define VariableTypeFactory_H_

#include <string>

namespace TA_Base_Bus
{
    class IVariable;
    class IAddVariable;
    /**
     * Creates concrete IVariable classes
     */
    class VariableTypeFactory
    {

    public:
	    VariableTypeFactory();

    public:
  	    virtual ~VariableTypeFactory();

    public:

        /**
	     * Creates a new IVariable object or returns a current one
         *
	     * @param dllName Name of the dll containing the interface
	     * @param added   Callback interface for the object to return information
	     * 
	     */
        IVariable* createVariableSelector( const std::string& dllName, const IAddVariable* added );

    private:
        // Disable copy constructor and assignment operator
	    VariableTypeFactory(const VariableTypeFactory& theVariableTypeFactory);
        VariableTypeFactory& operator=(const VariableTypeFactory &);

    };
}// namespace
#endif // !defined(VariableTypeFactory_H_)