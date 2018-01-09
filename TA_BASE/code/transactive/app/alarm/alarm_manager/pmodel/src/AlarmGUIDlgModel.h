#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

#include "app/alarm/alarm_manager/pmodel/src/AlarmManagerService.h"


namespace TA_Base_App
{
    class AlarmGUIDlgModel :
        public TA_Base_Bus::AbstractPModel
    {
    public:
        AlarmGUIDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
        virtual ~AlarmGUIDlgModel(void);
	
		void init();

		/**
		* notifyUpdate
		*
		* Calls the notifyUpdate() function of the AdminManager to retrieve the
		* up-to-date process data.
		*/
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args = NULL);

		std::string getStatusBarData();
		
		void setStatusBarData(const std::string& value);

		std::list<std::string > getFilterList();
			
	private:
		
    };

}