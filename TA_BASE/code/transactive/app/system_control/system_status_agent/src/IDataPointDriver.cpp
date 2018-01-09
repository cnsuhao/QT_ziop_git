/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution in any form.
  * 
  * Source:    $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/system_status_agent/src/IDataPointDriver.cpp $
  * @author:   Chris DeWolfe
  * @version:  $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last mofified by:  $Author: CM $
  *
  * IDataPointDriver class implimentation. Given that this class is an interface
  * this file just provides static member init. See header file for class descrption
  */

#ifdef _MSC_VER
  #pragma warning(disable: 4786)  // identifier was truncated in the debug information
#endif // _MSC_VER

#include "app/system_control/system_status_agent/src/IDataPointDriver.h"

namespace TA_Base_App
{
	const std::string IDataPointDriver::MONITOR = "monitor";
	const std::string IDataPointDriver::CONTROL = "control";
	const std::string IDataPointDriver::INIT = "init";
}
