/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source : $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/scada/datapoint_library/src/NotifyEventQueue.h $
  * @author  J. Welton
  * @version $Revision: #1 $
  * Last modification : $DateTime: 2015/10/09 15:42:35 $
  * Last modified by : $Author: CM $
  *
  *
  *	The transport mechanism used by DataPoints to pass
  * event requests to the NotifyEventProcessor
  *
  *
  */

#ifndef NOTIFYEVENTQUEUE_H
#define NOTIFYEVENTQUEUE_H

#include "bus/scada/common_library/src/BlockingSingletonQueue.h"
#include "bus/scada/datapoint_library/src/NotifyEventRequest.h"


namespace TA_Base_Bus
{
	typedef BlockingSingletonQueue<NotifyEventRequest> NotifyEventQueue;
}

#endif
