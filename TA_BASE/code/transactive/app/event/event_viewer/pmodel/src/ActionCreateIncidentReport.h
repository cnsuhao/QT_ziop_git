/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/event/event_viewer/src/ActionCreateIncidentReport.h $
  * @author:  Bradley Cleaver
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class launches the Incident Manager with parameters that specify
  * the alarms to populate the report with.
  */

#if !defined(_ACTION_CREATE_INCIDENT_REPORT_H_)
#define _ACTION_CREATE_INCIDENT_REPORT_H_

#include "app/event/event_viewer/pmodel/src/IEventAction.h"
#include "app/event/event_viewer/pmodel/src/CombinedEventFilter.h"
//#include "app/event/event_viewer/pmodel/src/resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ActionCreateIncidentReport : public IEventAction
{
    // Construction
    public:
	    ActionCreateIncidentReport();   // standard constructor
		virtual ~ActionCreateIncidentReport();

        virtual unsigned int getActionId();
        virtual void executeAction( Filter* filter );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_ACTION_CREATE_INCIDENT_REPORT_H_)
