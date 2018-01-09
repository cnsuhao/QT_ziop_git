#pragma once

#include <string>
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include <time.h>


namespace TA_Base_App
{
	struct SortParams
	{
		HWND  hwnd;          // The handle of the list control
		int   colIndex;      // The index of the column clicked
		bool  sortAscending; // True if the sort should be in ascending order,
		// and false otherwise.
	};

        /**
          * The list control's columns, in the order they initially appear.
          */
        enum ColumnNames
        {
            PROCESS_ID_COL = 0,
            HOST_COL,
            OPERATION_MODE_COL,
            STATUS_COL,
            NEW_STATUS_COL,
            RUNNING_SINCE_COL,
            RESTARTS_COL,
			DEBUG_LEVEL_COL
        };

    class ProcessListCtrlModel : public TA_Base_Bus::AbstractPModel
    {
    public:
		ProcessListCtrlModel(TA_Base_Bus::IGUIApplicationController * pController, const char * pmodelName);
        virtual ~ProcessListCtrlModel(void);
		std::string getEntityDescription(const std::string& entityName) const;
		bool isManagedProcessFromName(const std::string& entityName) const;
		void getRuningStatusDesc( TA_Base_Core::EProcessStatus state, std::string &itemStatusColDesc, std::string &itemSinceColDesc, bool isManagedProcess );
		static int compareIntegers( LPARAM lParamSort, const char *item1, const char * item2 );
        static int compareString2(SortParams* sp, const char * locationName, const char *item1, const char * item2);
		static int compareDates( SortParams* sp, const std::string & item1, const std::string & item2);
        static time_t getTimeFromStr(const std::string & strDatetime, bool isKRTCDateFormat);
        std::string getProcessNewStateDesc( TA_Base_Core::EProcessStatus state);
    public:
          /**
          * True if the process list is frozen, and false otherwise.
          */
	    bool m_isFrozen;

        /**
          * The index of the column that is sorted.
          */
        int m_sortedCol; 

        /**
          * True if the column is sorted in ascending order, and false otherwise.
          */
        bool m_sortAscending; 

    };

}