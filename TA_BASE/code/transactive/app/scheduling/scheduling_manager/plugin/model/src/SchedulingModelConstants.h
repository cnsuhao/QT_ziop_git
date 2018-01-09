/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scheduling/scheduling_manager/plugin/model/src/SchedulingModelConstants.h $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This file specifies the constants and enumerated required by the Scheduling Manager.
  */

#ifndef SCHEDULING_MODEL_CONSTANTS
#define SCHEDULING_MODEL_CONSTANTS

#include <string>
#include <atlstr.h>
#include "bus/scheduling/schedulingagentcorbadef/IDL/src/SchedulingAgentCorbaDef.h"

#define PMODEL_MAIN_FRAME "MainFrameModel"
#define PMODEL_JOB_DLG "JobDialogModel"

#define SYSTEM_INDEGRADEDMODE "NotifySystemIndegradedMode"
#define SYSTEM_INNORMALMODE "NotifySystemInnormalMode"
namespace TA_Base_App
{
    namespace SchedulingManager
    {
        ///////////////////////////////////////////////////////////////////////
        //
        // Modify the following date time formats to suit a particular locale
        //
        ///////////////////////////////////////////////////////////////////////


        // The time format.  This will be used in the Job Dialog to show the time jobs
        // are scheduled.  e.g. 13:00
        static const CString TIME_FORMAT = "%H:%M";

        ///////////////////////////////////////////////////////////////////////
        //
        // Run params and values
        //
        ///////////////////////////////////////////////////////////////////////

        // The run param name for job info
        static const std::string RPARAM_JOBINFO = "JobInfo";

        // The delimiters that will be used in the JobInfo run param value to 
        // separate the values.  The JobInfo run param value is expected to be 
        // in the form of 
        // <Type>:<ParameterName1>/<ParameterValue1>,<ParameterName2>/<ParameterValue2> ...
        // e.g. --job-info=Plan:PlanID/123
        static const char DELIMITER_JOBTYPE = ':';
        static const char DELIMITER_JOBPARAMNAME = ',';
        static const char DELIMITER_JOBPARAMVALUE = '/';

        // The run param to indicate the server status
        static const std::string RPARAM_SERVERSTATUS = "ServerStatus";

        // The run param value to indicate the server is up
        static const std::string RVALUE_SERVER_UP = "ServerUp";

        // The run param value to indicate the server is down
        static const std::string RVALUE_SERVER_DOWN = "ServerDown";


        ///////////////////////////////////////////////////////////////////////
        //
        // Enumerated Types
        //
        ///////////////////////////////////////////////////////////////////////

        // The flags representing the days of the week
        enum EDayOfWeek 
        {
            Monday = 0x1, 
            Tuesday = 0x2,
            Wednesday = 0x4,
            Thursday = 0x8,
            Friday = 0x10,
            Saturday = 0x20,
            Sunday = 0x40
        };

        // The range of the recurrence
        enum ERecurrenceRange 
        {
            NoEndDate,
            EndAfterNumTimes,
            EndByDate 
        };

        // we just going to make our types the same as the agent's so that GUI classes don't need to know the agent's type
        enum ERecurrenceType
        {
            OnceOff = TA_Base_Bus::SchedulingAgentCorbaDef::OnceOff,
            Daily =   TA_Base_Bus::SchedulingAgentCorbaDef::Daily,
            Weekly =  TA_Base_Bus::SchedulingAgentCorbaDef::Weekly,
            Monthly = TA_Base_Bus::SchedulingAgentCorbaDef::Monthly,
            Yearly =  TA_Base_Bus::SchedulingAgentCorbaDef::Yearly
        };

        // The types of actions that could be performed on the application
        enum EAction
        {
            Add,
            Delete,
            View,
            Filter,
            Edit,
            Enable,
            Disable,
            Print
        };

        
        ///////////////////////////////////////////////////////////////////////
        //
        // Constants
        //
        ///////////////////////////////////////////////////////////////////////

        // The maximum number of jobs to be displayed on the table
        static const int MAX_JOB_TO_DISPLAY = 1500;

        // The maximum number of times the job could recur
        static const int MAX_RECURRENCE = 100;

        // Maximum length of the description
        static const int MAX_DESCRIPTION_LENGTH = 100;

        // If a job is supposed to run on the last day of a month, then use this constant
        static const int LAST_DAY_OF_MONTH = 0;

        // If a job is supposed to run recurringly, use this as the day of month that it will end
        static const int RECUR_INDEFINITELY = 0;

        // Id for a job that has not been scheduled
        static std::string UNSCHEDULED_JOBID = "";

        // Equivalent of February in number
        static const int FEBRUARY = 2;

        // These are the number of pixels in the icons used in the application
		static const unsigned long ICON_SIZE = 16;

        // If the CString.Compare or std::string.compare methods return this value, the the
        // two strings used in the comparisons are equal
        static const int STRINGS_IDENTICAL = 0;

    }; // SchedulingManager
}; // TA_Base_App

#endif // SCHEDULING_MODEL_CONSTANTS
