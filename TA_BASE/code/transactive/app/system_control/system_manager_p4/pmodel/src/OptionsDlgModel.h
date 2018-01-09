#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"


namespace TA_Base_App
{
    class OptionsDlgModel :
        public TA_Base_Bus::AbstractPModel
    {
    public:
        OptionsDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
        virtual ~OptionsDlgModel(void);
	public:
		void setRefreshRate(unsigned int newRate);

		/**
		* getRefreshIntervals
		*
		* Retrieves the available refresh intervals (in seconds).
		*
		* @return A vector of ints containing the refresh intervals.
		*/
		std::vector<int> getRefreshIntervals();

		/**
		* getCurrentRefreshRate
		*
		* Returns the current refresh rate.
		*
		* @return The refresh rate.
		*/
		unsigned int getCurrentRefreshRate();
    };

}