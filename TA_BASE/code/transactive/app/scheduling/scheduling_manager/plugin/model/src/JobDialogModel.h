#ifndef JOB_DIALOG_MODEL_INCLUDE_H
#define JOB_DIALOG_MODEL_INCLUDE_H

#include <atlcomtime.h>
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

namespace TA_Base_Bus
{
	class RightsLibrary;
}

namespace TA_Base_App
{
    class JobDialogModel: public TA_Base_Bus::AbstractPModel
    {
    public:

        JobDialogModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
        virtual ~JobDialogModel();

        virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);

        void initRightLib();

	    /** 
          * isJobDetailsButtonEnabled
          *
		  * Checks that a plan has been selected for this job, and that the user has the
		  * rights to both run the Plan Manager and control the selected plan. If all this 
		  * is so, then the Details button can be enabled.
          * 
          * @return true if the 'Details...' button for the selected plan should be enabled, false otherwise.
          */
		bool isJobDetailsButtonEnabled();

		/** 
          * getTimeString
          *
          * Returns the specified time in the form of a string (HH24:MI)
          *
          * @param theTime The time to be displayed
          *
          * @return The formatted time string
          */
        TAUnicodeString getTimeString(COleDateTime& theTime) const;


        /** 
          * getDateString
          *
          * Returns the date string in the format of Thursday, November 1, 2004 if
          * it is a non yearly date, and display it in the format of November 1 if
          * if it yearly.
          *
          * @param theDate The date to be displayed
          * @param showYear Will display day and year if this is true.
          *
          * @return The formatted date string
          *
          */
        TAUnicodeString getDateString(COleDateTime& theDate, const bool showYear = true) const;

    private:
		// the rights library for checking if user can control the selected plan for this job
		TA_Base_Bus::RightsLibrary *m_rightsLibrary;
		unsigned long m_planManagerKey;
        
    };
}

#endif //JOB_DIALOG_MODEL_INCLUDE_H