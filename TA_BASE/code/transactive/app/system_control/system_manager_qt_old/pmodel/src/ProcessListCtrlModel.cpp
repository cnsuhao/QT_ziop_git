#include "ProcessListCtrlModel.h"
#include "AppServiceCenter.h"
#include "AppServiceConsts.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

#include "core/data_access_interface/entity_access/src/IEntityData.h"


namespace TA_Base_App
{

    ProcessListCtrlModel::ProcessListCtrlModel(TA_Base_Bus::IGUIApplicationController * pController, const char * pmodelName)
        : TA_Base_Bus::AbstractPModel(pController, pmodelName)
        , m_isFrozen(false), m_sortedCol(PROCESS_ID_COL), m_sortAscending(true)
    {
    }

    ProcessListCtrlModel::~ProcessListCtrlModel(void)
    {
    }

	std::string ProcessListCtrlModel::getEntityDescription( const std::string& entityName ) const
	{
		return AppServiceCenter::getInstance().getEntityDescription(entityName);
	}

	bool ProcessListCtrlModel::isManagedProcessFromName( const std::string& entityName ) const
	{
		return AppServiceCenter::getInstance().isManagedProcessFromName(entityName);
	}

	void ProcessListCtrlModel::getRuningStatusDesc( TA_Base_Core::EProcessStatus state, std::string &itemStatusColDesc, std::string &itemSinceColDesc, bool isManagedProcess )
	{
		switch (state)
		{
		case TA_Base_Core::Unstarted:
			itemStatusColDesc = AppServiceConsts::NOT_STARTED;
			itemSinceColDesc = "";
			break;

		case TA_Base_Core::Startup:
			itemStatusColDesc = AppServiceConsts::STARTUP;

			break;

		case TA_Base_Core::RunningControl:
			if (isManagedProcess)
			{
				itemStatusColDesc = AppServiceConsts::RUNNING_CONTROL;
			}
			else
			{
				itemStatusColDesc = AppServiceConsts::RUNNING;
			}

			break;

		case TA_Base_Core::RunningMonitor:
			if (isManagedProcess)
			{
				itemStatusColDesc = AppServiceConsts::RUNNING_MONITOR;
			}
			else
			{
				itemStatusColDesc = AppServiceConsts::RUNNING;
			}

			break;

		case TA_Base_Core::GoingToControl:
			if (isManagedProcess)
			{
				itemStatusColDesc = AppServiceConsts::GOING_TO_CONTROL;

			}
			else
			{
				itemStatusColDesc = AppServiceConsts::RUNNING;
			}

			break;

		case TA_Base_Core::GoingToMonitor:
			if (isManagedProcess)
			{
				itemStatusColDesc = AppServiceConsts::GOING_TO_MONITOR;
			}
			else
			{
				itemStatusColDesc = AppServiceConsts::RUNNING;
			}

			break;

		case TA_Base_Core::Terminating:

			itemStatusColDesc = AppServiceConsts::TERMINATING;
			break;

		case TA_Base_Core::NotRunning:
			itemStatusColDesc = AppServiceConsts::NOT_RUNNING;
			itemSinceColDesc = "";
			break;

		case TA_Base_Core::Stopped:
			itemStatusColDesc = AppServiceConsts::STOPPED;
			itemSinceColDesc = "";
			break;
		}
	}

	int ProcessListCtrlModel::compareIntegers( LPARAM lParamSort, const char *item1, const char * item2 )
	{
		SortParams* sp = (SortParams*)lParamSort;

		// Convert them to integers
		int int1, int2;
		int1 = atoi(item1);
		int2 = atoi(item2);

		// Make the comparison
		if (sp->sortAscending)
		{
			return int1 > int2;
		}
		else
		{
			return int2 > int1;
		}
	}

	int ProcessListCtrlModel::compareString2(SortParams* sp, const char * locationName, const char *item1, const char * item2)
	{

		bool isLocalItem1 = false;
		bool isLocalItem2 = false;

		if ( NULL != strstr(item1, locationName) )
		{
			isLocalItem1 = true;
		}

		if ( NULL != strstr(item2, locationName) )
		{
			isLocalItem2 = true;
		}

		if (true == isLocalItem1 && false == isLocalItem2 )
		{
			return -1;
		}
		else if (false == isLocalItem1 && true == isLocalItem2 )
		{
			return 1;
		}
		else
		{
			// Make the comparison
			if (sp->sortAscending)
			{
				return _stricmp(item1, item2);
			}
			else
			{
				return _stricmp(item2, item1);
			}
		}
	}

    time_t ProcessListCtrlModel::getTimeFromStr(const std::string & strDatetime, bool isKRTCDateFormat)
    {
        TA_ASSERT(strDatetime.length() == 19, "Data time should be correct format");
        struct tm newTime;
        newTime.tm_isdst = 0;

        if (isKRTCDateFormat)
        {
            //CTime timeA(atoi(item1.Mid(0, 4)),   // Year
            //    atoi(item1.Mid(5, 2)),   // Month
            //    atoi(item1.Mid(8, 2)),   // Day
            //    atoi(item1.Mid(11, 2)),  // Hour
            //    atoi(item1.Mid(14, 2)),  // Min
            //    atoi(item1.Mid(17, 2))); // Sec
            newTime.tm_year = atoi(strDatetime.substr(0,4).c_str());
            newTime.tm_mon = atoi(strDatetime.substr(5,2).c_str());
            newTime.tm_mday = atoi(strDatetime.substr(8,2).c_str());
            newTime.tm_hour = atoi(strDatetime.substr(11,2).c_str());
            newTime.tm_min = atoi(strDatetime.substr(14,2).c_str());
            newTime.tm_sec = atoi(strDatetime.substr(17,2).c_str());
        }
        else
        {
            //CTime timeA(atoi(item1.Mid(6, 4)),   // Year
            //    atoi(item1.Mid(3, 2)),   // Month
            //    atoi(item1.Mid(0, 2)),   // Day
            //    atoi(item1.Mid(11, 2)),  // Hour
            //    atoi(item1.Mid(14, 2)),  // Min
            //    atoi(item1.Mid(17, 2))); // Sec

            newTime.tm_year = atoi(strDatetime.substr(6,4).c_str());
            newTime.tm_mon = atoi(strDatetime.substr(3,2).c_str());
            newTime.tm_mday = atoi(strDatetime.substr(0,2).c_str());
            newTime.tm_hour = atoi(strDatetime.substr(11,2).c_str());
            newTime.tm_min = atoi(strDatetime.substr(14,2).c_str());
            newTime.tm_sec = atoi(strDatetime.substr(17,2).c_str());
        }
        time_t retTime = mktime(&newTime);
        return retTime;

    }

	int ProcessListCtrlModel::compareDates( SortParams* sp, const std::string & item1, const std::string & item2)
	{
		// Check if the fields are the right size before converting to a
		// date (they could be blank if the process isn't running).
		if (item1.length() < 19)
		{
			return !(sp->sortAscending);
		}
		else if (item2.length() < 19)
		{
			return sp->sortAscending;
		}

		//TD16179++
		time_t time1;
		time_t time2;
		//struct tm time1, time2;

		if (TA_Base_Core::RunParams::getInstance().isSet(RPRAM_KRTCDATEFORMAT))
		{
			//FORMAT: %Y/%m/%d %H:%M:%S - 2006/07/04 11:42:29
            time1 = getTimeFromStr(item1, true);
            time2 = getTimeFromStr(item2, true);

		}
		else
		{
			//FORMAT: %d/%m/%Y %H:%M:%S - 04/07/2006 11:43:05

            time1 = getTimeFromStr(item1, false);
            time2 = getTimeFromStr(item2, false);
		}
		//++TD16179		

		// Make the comparison
		if (sp->sortAscending)
		{
			return time1 > time2;
		}
		else
		{
			return time2 > time1;
		}
	}

    std::string ProcessListCtrlModel::getProcessNewStateDesc( TA_Base_Core::EProcessStatus state)
    {
        std::string statusColDesc;
        switch (state)
        {

        case TA_Base_Core::Unstarted:
            statusColDesc = AppServiceConsts::NOT_STARTED;
            break;

        case TA_Base_Core::Startup:
            statusColDesc = AppServiceConsts::STARTUP;
            break;

        case TA_Base_Core::RunningControl:
            statusColDesc = AppServiceConsts::RUNNING_CONTROL;
            break;

        case TA_Base_Core::RunningMonitor:
            statusColDesc = AppServiceConsts::RUNNING_MONITOR;
            break;

        case TA_Base_Core::GoingToControl:
            statusColDesc = AppServiceConsts::GOING_TO_CONTROL;
            break;

        case TA_Base_Core::GoingToMonitor:
            statusColDesc = AppServiceConsts::GOING_TO_MONITOR;
            break;

        case TA_Base_Core::Terminating:
            statusColDesc = AppServiceConsts::TERMINATING;
            break;

        case TA_Base_Core::NotRunning:
            statusColDesc = AppServiceConsts::NOT_RUNNING;
            break;

        case TA_Base_Core::Stopped:
            statusColDesc = AppServiceConsts::STOPPED;
            break;
        }	
        return statusColDesc;
    }

}