/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/data_access_interface/interlocking_variable/src/InterlockVariable.cpp $
  * @author:  Robert Stagg
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  */

#ifdef __WIN32__
#pragma warning(disable:4786)
#pragma warning(disable:4284)
#endif

#include "core/data_access_interface/interlocking_variable/src/InterlockVariable.h"
#include "core/data_access_interface/interlocking_variable/src/InterlockVariableHelper.h"

#include "core/utilities/src/TAAssert.h"

namespace TA_Base_Core
{

    InterlockVariable::InterlockVariable(const std::string& entityType)
		: m_interlockVariableHelper(new InterlockVariableHelper(entityType))
    {
        
    }


    InterlockVariable::~InterlockVariable() 
	{
        delete m_interlockVariableHelper;
        m_interlockVariableHelper=NULL;
    }



    std::string InterlockVariable::getDllName()
    {
        TA_ASSERT(m_interlockVariableHelper != NULL,"The InterlockVariableHelper pointer is null.");
        return m_interlockVariableHelper->getDllName();
    }

    std::string InterlockVariable::getEntityType()
    {
        TA_ASSERT(m_interlockVariableHelper != NULL,"The InterlockVariableHelper pointer is null.");
        return m_interlockVariableHelper->getEntityType();
    }

    std::string InterlockVariable::getButtonText()
    {
        TA_ASSERT(m_interlockVariableHelper != NULL,"The InterlockVariableHelper pointer is null.");
        return m_interlockVariableHelper->getButtonText();
    }

} // closes TA_Base_Core
