/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/event/event_viewer/pmodel/src/IATSComWrapper.cpp $
* @author:  
* @version: $Revision:  $
*
* Last modification: $DateTime:  $
* Last modified by:  $Author:  $
*
* 
*
*/

#include  "app/event/event_viewer/pmodel/src/IATSComWrapper.h"

namespace TA_Base_App
{
	IATSComWrapper* IATSComWrapper::m_instance_realTime = NULL;
	IATSComWrapper* IATSComWrapper::m_instance_hist = NULL;

	IATSComWrapper* IATSComWrapper::getInstanceRealTime()
	{		
		return m_instance_realTime;
	}

	void IATSComWrapper::freeInstanceRealTime()
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "ATSComWrapper::freeInstanceRealTime() entry . ");
		if(m_instance_realTime!=NULL)
		{
			delete m_instance_realTime;
			m_instance_realTime = NULL;
			//CoUninitialize();
		}
		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "ATSComWrapper::freeInstanceRealTime() exit . ");
	}

	IATSComWrapper* IATSComWrapper::getInstanceHistorical()
	{
		return m_instance_hist;
	}

	void IATSComWrapper::freeInstanceHistorical()
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "ATSComWrapper::freeInstanceHistorical() entry . ");
		if(m_instance_hist!=NULL)
		{
			delete m_instance_hist;
			m_instance_hist = NULL;
			//CoUninitialize();
		}
		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "ATSComWrapper::freeInstanceHistorical() exit . ");
	}

	void IATSComWrapper::setInstanceRealTime(IATSComWrapper * realtime)
	{
		if(m_instance_realTime == NULL)
		{
			m_instance_realTime = realtime;
			//CoInitialize(0);  //moved to ATSComWrapper class
		}
	}

	void IATSComWrapper::setInstanceHistorical(IATSComWrapper * historical)
	{
		if(m_instance_hist == NULL)
		{
			m_instance_hist = historical;
			//CoInitialize(0);
		}
	}
};