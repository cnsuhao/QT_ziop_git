/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scheduling/scheduling_manager/src/SchedulingManagerConstants.h $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * This file specifies the constants and enumerated required by the Scheduling Manager.
  */

#ifndef SCHEDULING_MANAGER_CONSTANTS
#define SCHEDULING_MANAGER_CONSTANTS

#include "app/scheduling/scheduling_manager/src/resource.h"
#include <string>
#include "bus/scheduling/schedulingagentcorbadef/IDL/src/SchedulingAgentCorbaDef.h"

// Custom window messages
#define WM_JOBSELECTION_CHANGE (WM_USER + 200)
#define WM_JOB_CREATE (WM_USER + 201)
#define WM_JOBLIST_UPDATE (WM_USER + 202)
#define WM_STATUSTEXT_CHANGE (WM_USER + 203)
#define WM_CLEAN_UP (WM_USER + 204)
#define WM_SHOW_EXCEED_MAX_JOB_DLG (WM_USER + 205)
#define WM_JOB_ADDED (WM_USER + 206)
#define WM_JOB_UPDATED (WM_USER + 207)
#define WM_JOB_DELETED (WM_USER + 208)
#define WM_RIGHTS_AGENT_DIED (WM_USER + 209)
//TD16358++
#define WM_SYSTEM_INNORMALMODE (WM_USER + 210)
#define WM_SYSTEM_INDEGRADEDMODE (WM_USER + 211)
//++TD16358

namespace TA_Base_App
{
    namespace SchedulingManager
    {
        ///////////////////////////////////////////////////////////////////////
        //
        // Enumerated Types
        //
        ///////////////////////////////////////////////////////////////////////

        // list all the day strings here just in case the strings get reordered.  
        // Monday is always the first day.
        static const unsigned int DAY[] = 
        {
            IDS_MONDAY,
            IDS_TUESDAY,
            IDS_WEDNESDAY,
            IDS_THURSDAY,
            IDS_FRIDAY,
            IDS_SATURDAY,
            IDS_SUNDAY
        };

    }; // SchedulingManager
}; // TA_Base_App

#endif // SCHEDULING_MANAGER_CONSTANTS
