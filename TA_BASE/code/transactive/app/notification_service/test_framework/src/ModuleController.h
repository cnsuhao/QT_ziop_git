/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/notification_service/test_framework/src/ModuleController.h $
  * @author:  Kent Yip
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Contains function prototypes for ModuleController executable.
  *
  */

// OBSOLETE FILE - TO BE DELETED

#include <iostream>
#include <string>
#include <map>
#include <sstream>

// STAF includes



bool startAllSimulators( STAFHandlePtr handle, HandleMap& simulatorHandles );

RemoteProcess spawnSimulator(
    STAFHandlePtr handle, 
    const std::string& executableName,
    const std::string& simulatorID, 
    const std::string& host);

void killAll(STAFHandlePtr handle, HandleMap& processHandles);



 
