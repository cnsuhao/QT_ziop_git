/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/event/event_viewer/pmodel/src/IATSComWrapper.h $
* @author:  
* @version: $Revision:  $
*
* Last modification: $DateTime:  $
* Last modified by:  $Author:  $
*
* 
*
*/

#ifndef IATSCOMWRAPPER_INTERFACE_1234567890_H
#define IATSCOMWRAPPER_INTERFACE_1234567890_H		

#include <string>
#include <vector>
#include "FilterCriteria.h"
#include "ActionCommand.h"
#include "core/data_access_interface/src/ICombinedEventData.h"
#include "core/utilities/src/DebugUtil.h"

using TA_Base_Core::DebugUtil;


/*//#import "../../../../../../bin/win32_nd/ATSBridgeUtil.dll" named_guids
#ifdef _DEBUG
#import "../../../../../../../sdk/win32_nd/ATSBridgeUtil.tlb" named_guids
#else
#import "../../../../../../sdk/win32_nr/ATSBridgeUtil.tlb" named_guids
#endif



using namespace ATSBridgeUtilLib;*/

namespace TA_Base_App
{

	struct ATSSystemItem
	{
		ATSSystemItem(int k, std::string s) : pkey(k), systemName(s) {}


		bool operator<(const ATSSystemItem& rhs)
		{
			return systemName.compare(rhs.systemName) < 0;
		}

		int	pkey;
		std::string		systemName;
		std::vector<std::pair<int , std::string >>  subsystemVector;

	};

	class IATSComWrapper
	{	
	public:
		virtual ActionCommand startATSSubscription(
			FilterCriteria filterCriteria,
			const unsigned int backloglength,				
			std::vector<TA_Base_Core::ICombinedEventData*>& events) = 0;

		virtual ActionCommand stopATSSubscription(void) = 0;

		virtual ActionCommand doPreviousPageQuery(std::vector<TA_Base_Core::ICombinedEventData*> & events, 
			FilterCriteria filterCriteria, unsigned long atsMaxLength) = 0;

		virtual ActionCommand doNextPageQuery(std::vector<TA_Base_Core::ICombinedEventData*> & events, 
			FilterCriteria filterCriteria, unsigned long atsMaxLength) = 0;

		/*virtual void transformToISCSEvents (IATSEventList * pATSEvents, 
		std::vector<TA_Base_Core::ICombinedEventData*>& events , unsigned long lastKey = 0) = 0;*/
		virtual int loadATSSystemsInfo( ) = 0;
		virtual vector<ATSSystemItem> getATSSystemStructureVector() = 0;

		virtual void cleanUp() = 0;
		virtual void doComInitialize()=0;

		static IATSComWrapper* getInstanceRealTime();
		static IATSComWrapper* getInstanceHistorical();

		static void freeInstanceRealTime();
		static void freeInstanceHistorical();

		static void setInstanceRealTime(IATSComWrapper * realtime);
		static void setInstanceHistorical(IATSComWrapper * historical);

	private:
		static IATSComWrapper * m_instance_realTime;
		static IATSComWrapper * m_instance_hist;
	};

};

#endif //IATSCOMWRAPPER_INTERFACE_1234567890_H