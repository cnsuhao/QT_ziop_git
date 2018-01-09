/**
* The source code in this file is the property of 
* MI Consulting Group (Australia) and is not for 
* redistribution in any form.
*
# Source:  $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/versioning/src/Version.h $ 
# Author:  Ripple
# Version: $Revision: #1 $
#
# Last modification: $DateTime: 2015/10/09 15:42:35 $
# Last modified by:  $Author: CM $
* 
* Main source file for MiVersion information
*
* NOTE: The variables declared external in Version.cpp
* are defined in the file VersionGen.cpp which is located in
* all component src directories. The variables are resolved
* at link time.
*/
#ifndef VERSION_H
#define VERSION_H

namespace TA_Base_Core
{
    class Version
    {
        public:

        /**
          * displayVersionInfo()
          *
          * This is a static method that will print out versionInformation to stdout.
          */
         static void displayVersionInfo();
    };
}

#endif //#ifndef VERSION_H
