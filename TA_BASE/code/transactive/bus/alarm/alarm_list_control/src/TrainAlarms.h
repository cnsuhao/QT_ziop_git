/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/TrainAlarms.h $
  * @author:  Ripple
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2016/11/22 14:12:39 $
  * Last modified by:  $Author: Noel $
  *
  */
#if !defined(TrainAlarms_H)
#define TrainAlarms_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bus/trains/TrainAgentCorba/idl/src/ITrainCctvCorba.h"
#include "bus/cctv/video_switch_agent/IDL/src/SwitchManagerCorbaDef.h"
#include "bus/cctv/tvss_agent/TVSSManager/IDL/src/TVSSManagerCorbaDef.h"
#include "core/naming/src/namedobject.h"
#include "bus\alarm\shared_alarm\src\AlarmDataItem.h"

#include <ACE/Singleton.h>
#include <QApplication>
 

namespace TA_Base_Bus
{
	class TrainAlarms  
	{
		Q_DECLARE_TR_FUNCTIONS(TrainAlarms)

        friend class ACE_Singleton<TrainAlarms, ACE_Recursive_Thread_Mutex>;

	public:

        static TrainAlarms& getInstance();


        /**
         * This is used to activate the triggered cameras on alarm acknowledgement
         * 
         * @param details
         */
		void acknowledgeAlarm(TA_Base_Bus::AlarmDataItem* details);
    
    
    private:

        /**
         * Used by ACE singleton
         */
        TrainAlarms();
        ~TrainAlarms();
 

		typedef TA_Base_Core::NamedObject<TA_Base_Bus::TVSSManagerCorbaDef,
			TA_Base_Bus::TVSSManagerCorbaDef_ptr,
			TA_Base_Bus::TVSSManagerCorbaDef_var>   TVSSManagerNameObj;
		TVSSManagerNameObj m_tvssManagerNameObj;

	};

    /**
     * @author adamr
     * @version 1.0
     * @created 13-Mar-2008 6:38:03 PM
     */
    typedef ACE_Singleton<TrainAlarms, ACE_Recursive_Thread_Mutex> TrainAlarmsSingleton;

};

#endif // !defined(TrainAlarms_H)

