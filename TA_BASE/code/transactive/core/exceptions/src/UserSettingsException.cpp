/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/UserSettingsException.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */
#include "core\exceptions\src\UserSettingsException.h"

namespace TA_Base_Core
{

    UserSettingsException::UserSettingsException(const char* msg) 
        : TransactiveException(msg)
    {

    }	

    UserSettingsException::~UserSettingsException()
    {

    }



} // End TA_Base_Core
