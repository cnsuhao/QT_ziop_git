/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/DataUserSettingException.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */
#include "core\exceptions\src\DataUserSettingException.h"

namespace TA_Base_Core
{

    DataUserSettingException::DataUserSettingException(const char* msg) 
        : m_failType(GENERIC), TransactiveException(msg)
    {

    }	

    DataUserSettingException::DataUserSettingException(const char* msg, const Type failType)
        : m_failType(failType), TransactiveException(msg)
    {

    }
    DataUserSettingException::~DataUserSettingException()
    {

    }



} // End TA_Base_Core
