#ifndef DEBUGUTILINIT_H_INCLUDED
#define DEBUGUTILINIT_H_INCLUDED

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/utilities/src/DebugUtilInit.h $
  * @author:  Lachlan Partington
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  */

#if defined( WIN32 )
#pragma warning( disable : 4786 )
#endif   // defined( WIN32 ) 

#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

namespace TA_Base_Core
{
    // Uses the following RunParams to set the corresponding DebugUtil settings:
    // RPARAM_DEBUGFILE
    // RPARAM_DEBUGLEVEL
    // RPARAM_DEBUGFILEMAXSIZE
    // RPARAM_DEBUGMAXFILES
    // RPARAM_DEBUGPIDFILENAMES
    // RPARAM_DEBUGLEVELFILE
    // RPARAM_DEBUGLEVELOFF

    void gSetDebugUtilFromRunParams();

} 

#endif // DEBUGUTILINIT_H_INCLUDED

