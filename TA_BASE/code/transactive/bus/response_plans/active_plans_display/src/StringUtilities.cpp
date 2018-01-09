/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/response_plans/active_plans_display/src/StringUtilities.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * Collection of Static String related functions related to the display of Active
  * Plan Instances.
  */

#include "stdafx.h"
#include "core/utilities/src/TAAssert.h"
#include "StringUtilities.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

std::map<std::string, int> TA_Base_Bus::StringUtilities::s_executionStateMap;

const char PLAN_PATH_SEPARATOR = ('/');
const char * RUNNING_PLAN_EXEC_STATE = ("Running");
const char * OPERATOR_PAUSED_EXEC_STATE = ("Running");  // Still show plan as running when it's paused for interaction
const char * STOPPED_PLAN_EXEC_STATE = ("Stopped");
const char * PAUSED_PLAN_EXEC_STATE = ("Paused");
const char * DELAYED_PLAN_EXEC_STATE = ("Running");  // Still show plan as running when it's in a delay
const char * COMPLETED_PLAN_EXEC_STATE = ("Completed");
const char * ABORTED_PLAN_EXEC_STATE = ("Aborted");
const char * FAILED_PLAN_EXEC_STATE = ("Failed");
const char * PENDING_PLAN_EXEC_STATE = ("Pending");
const char * UNDEFINED_PLAN_EXEC_STATE = ("Undefined");

using namespace TA_Base_Bus;


/////////////////////////////////////////////////////////////////////////////
// StringUtilities


TAUnicodeString StringUtilities::extractPlanName(const TAUnicodeString &planPath)
{
    int separatorPos = planPath.c_str().ReverseFind(PLAN_PATH_SEPARATOR);
    int planNameStartPos = separatorPos < 0? 0 : separatorPos + 1;

    return planPath.Mid(planNameStartPos);
}


TAUnicodeString StringUtilities::extractCategoryPath(const TAUnicodeString &planPath)
{
    int separatorPos = planPath.c_str().ReverseFind(PLAN_PATH_SEPARATOR);
    int categoryPathLen = separatorPos < 0? 0 : (separatorPos == 0? 1 : separatorPos);

    return planPath.Left(categoryPathLen);    
}


TAUnicodeString StringUtilities::convertToString(const int value)
{
    TAUnicodeString valueString;

    valueString.str().Format(_T("%d"), value);

    return valueString;
}


TAUnicodeString StringUtilities::convertToString(const unsigned long value)
{
    TAUnicodeString valueString;

    valueString.str().Format(_T("%lu"), value);

    return valueString;
}


TAUnicodeString StringUtilities::convertToString(const double value, const unsigned long precision)
{
    TAUnicodeString valueString;
    
    valueString.str().Format(_T("%.*lf"), precision, value);

    return valueString;
}

TAUnicodeString StringUtilities::convertToString(const TA_Base_Core::EActivePlanState execState)
{
    switch (execState)
    {
        case TA_Base_Core::OPERATOR_PAUSED_ACTIVE_PLAN_STATE:
            return TAUnicodeString::CreateFromMultitByteString(OPERATOR_PAUSED_EXEC_STATE);

		case TA_Base_Core::EXECUTING_ACTIVE_PLAN_STATE:
			return TAUnicodeString::CreateFromMultitByteString(RUNNING_PLAN_EXEC_STATE);

		case TA_Base_Core::STOPPED_ACTIVE_PLAN_STATE:
            return TAUnicodeString::CreateFromMultitByteString(STOPPED_PLAN_EXEC_STATE);

        case TA_Base_Core::PAUSED_ACTIVE_PLAN_STATE:
            return TAUnicodeString::CreateFromMultitByteString(PAUSED_PLAN_EXEC_STATE);

        case TA_Base_Core::DELAYED_ACTIVE_PLAN_STATE:
            return TAUnicodeString::CreateFromMultitByteString(DELAYED_PLAN_EXEC_STATE);

        case TA_Base_Core::COMPLETED_ACTIVE_PLAN_STATE:
            return TAUnicodeString::CreateFromMultitByteString(COMPLETED_PLAN_EXEC_STATE);

        case TA_Base_Core::ABORTED_ACTIVE_PLAN_STATE:
            return TAUnicodeString::CreateFromMultitByteString(ABORTED_PLAN_EXEC_STATE);

        case TA_Base_Core::FAILED_ACTIVE_PLAN_STATE:
            return TAUnicodeString::CreateFromMultitByteString(FAILED_PLAN_EXEC_STATE);

        case TA_Base_Core::PENDING_ACTIVE_PLAN_STATE:
            return TAUnicodeString::CreateFromMultitByteString(PENDING_PLAN_EXEC_STATE);
    }

    return TAUnicodeString::CreateFromMultitByteString(UNDEFINED_PLAN_EXEC_STATE);
}


int StringUtilities::compareNumbers(const TAUnicodeString &str1, const TAUnicodeString &str2)
{
    TCHAR *end = NULL;
    double num1 = ::_tcstod(str1.GetString(), &end);
    double num2 = ::_tcstod(str2.GetString(), &end);

    if (num1 < num2)
    {
        return -1;
    }

    if (num1 > num2)
    {
        return 1;
    }
 
    return 0;
}

COleDateTime StringUtilities::getDateTime(const char * str)
{
    COleDateTime time1;
    TAUnicodeString timeString;
    timeString.fromMultiByteString(str);

    time1.ParseDateTime(timeString.GetString());
    return time1;
}

int StringUtilities::compareTimestamps(const char* str1, const char * str2)
{
    COleDateTime time1, time2;

    time1 = getDateTime(str1);
    time2 = getDateTime(str2);

    if (time1 < time2)
    {
        return -1;
    }

    if (time1 > time2)
    {
        return 1;
    }
 
    return 0;
}


int StringUtilities::compareExecutionStates(const TAUnicodeString &str1, const TAUnicodeString &str2)
{
    if (s_executionStateMap.empty())
    {
        int priority = 0; // 0 = highest

        s_executionStateMap.insert(std::make_pair(RUNNING_PLAN_EXEC_STATE, priority++));
        s_executionStateMap.insert(std::make_pair(PAUSED_PLAN_EXEC_STATE, priority++));
        s_executionStateMap.insert(std::make_pair(PENDING_PLAN_EXEC_STATE, priority++));
        s_executionStateMap.insert(std::make_pair(COMPLETED_PLAN_EXEC_STATE, priority++));
        s_executionStateMap.insert(std::make_pair(STOPPED_PLAN_EXEC_STATE, priority++));
        s_executionStateMap.insert(std::make_pair(ABORTED_PLAN_EXEC_STATE, priority++));
        s_executionStateMap.insert(std::make_pair(FAILED_PLAN_EXEC_STATE, priority++));
        s_executionStateMap.insert(std::make_pair(UNDEFINED_PLAN_EXEC_STATE, priority++));
    }

    std::map<std::string, int>::const_iterator stateIter1 = s_executionStateMap.find(str1.toMultiByteStdString());
    std::map<std::string, int>::const_iterator stateIter2 = s_executionStateMap.find(str2.toMultiByteStdString());

    TA_ASSERT(stateIter1 != s_executionStateMap.end(), "Invalid execution state string");
    TA_ASSERT(stateIter2 != s_executionStateMap.end(), "Invalid execution state string");

    int state1Priority = stateIter1->second;
    int state2Priority = stateIter2->second;

    if (state1Priority < state2Priority)
    {
        return -1;
    }

    if (state1Priority > state2Priority)
    {
        return 1;
    }
 
    return 0;    
}
