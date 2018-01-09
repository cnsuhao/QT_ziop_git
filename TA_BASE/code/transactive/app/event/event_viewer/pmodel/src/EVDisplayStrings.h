/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source: $File: //depot/3001_TIP/TA_BASE/transactive/app/event/event_viewer/pmodel/src/EVDisplayStrings.h $
* @author
* @version $Revision: # $
*
* Last modification: $DateTime:  $
* Last modified by: 
* 
* 
*
*/
#if !defined(EVDISPLAYSTRINGS_0123456789_1234567__INCLUDED_)
#define EVDISPLAYSTRINGS_0123456789_1234567__INCLUDED_

#include <string>

namespace TA_Base_App
{	
	class EVDisplayStrRes
	{
		public:

			static EVDisplayStrRes* GetInstance();
			static void ReleaseInstance();

			std::string STATUS_ATS_QUERYING;
			std::string STATUS_ISCS_QUERYING;
			std::string STATUS_ATS_READY;
			std::string STATUS_ISCS_READY;
			std::string STATUS_ATS_UNHEALTHY;
			std::string STATUS_ISCS_UNHEALTHY;
			std::string STATUS_ATS_DISABLED;
			std::string STATUS_ICSC_DISABLED;	

			std::string USER_DEFINED_NAMED_FILTER_NAME;
			std::string ALARM_FILTER_MESSAGE;
			std::string ERROR_LOAD;
			std::string ERROR_LOADED;
			// Static constants for identifying columns.
			std::string COLUMN_DATETIME;
			std::string COLUMN_SEVERITY;
			std::string COLUMN_ASSET;
			std::string COLUMN_ALARM_STATUS; // added
			std::string COLUMN_DESCRIPTION;
			std::string COLUMN_VALUE;
			std::string COLUMN_T;  // added
			std::string COLUMN_MMS;
			std::string COLUMN_DSS;
			std::string COLUMN_AVL;
			std::string COLUMN_OPERATOR;
			std::string COLUMN_COMMENT;
			// Static constants for identifying hidden columns.
			std::string HIDDEN_COLUMN_ALARMID;
			std::string HIDDEN_COLUMN_ACKED;
			std::string HIDDEN_COLUMN_STATUS;
			//static std::string HIDDEN_COLUMN_SELECTED;
			 std::string FILTER_APPLICATION_SUBSYSTEM;
			 std::string FILTER_PHYSICAL_SUBSYSTEM;
			  std::string FILTER_SYSTEM_STR;
			  std::string FILTER_SUBSYSTEM_STR;
			  std::string FILTER_LOCATION;
			  std::string FILTER_OPERATOR;
			  std::string FILTER_OPERATOR_STR;
			  std::string FILTER_LOCATION_STR;
			  std::string FILTER_EVENTTYPE; 
			  std::string FILTER_ISCS_SYSTEM;     // add for 955
			// const std::string FILTER_ATS_SYSTEM;     // add for 955
			  std::string FILTER_TMTYPE;     // add for 955
			  std::string FILTER_ALARMSTATE; // add for 955
			  std::string FILTER_ASSET;
			  std::string FILTER_DESCRIPTION;
			  std::string FILTER_SEVERITY;
			  std::string FILTER_ALARMTYPE;
			  std::string FILTER_ALARMID;
			//  constants for storing  filter names.
			 std::string NAMEDFILTER_DEFAULT;
			 std::string NAMEDFILTER_AVALANCHE;
			 std::string PRINT_HEADER_STR;
	private:
		EVDisplayStrRes(){}
		static EVDisplayStrRes* m_instance;
	};
	


}
#endif  // EVDISPLAYSTRINGS_0123456789_1234567__INCLUDED_