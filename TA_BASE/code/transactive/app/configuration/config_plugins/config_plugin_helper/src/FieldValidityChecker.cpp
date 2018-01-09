/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/configuration/config_plugins/config_plugin_helper/src/FieldValidityChecker.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * This class is used to check the validity of certain fields.
  */

#include "app/configuration/config_plugins/config_plugin_helper/src/StdAfx.h"
#include "app/configuration/config_plugins/config_plugin_helper/src/FieldValidityChecker.h"

#include "core/utilities/src/DebugUtil.h"

using TA_Base_Core::DebugUtil;

namespace TA_Base_App
{
    bool FieldValidityChecker::isValidRealNumber(const std::string& numberString) const
    {
        FUNCTION_ENTRY("isValidNumber");

		// For reals need to check that the number is formatted correctly -- that is, there
		// is a maximum of 1 ".", 1 "-", and that at least one numerical character trails either
		// of the other two

        LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "String to check if it is a valid real is: %s",numberString.c_str());

		if (numberString.size() == 0) // the data string is empty	
		{
            LOGMORE( SourceInfo, "Number String is empty so it is valid");
			FUNCTION_EXIT;
            return true;
		}


        // check for any invalid characters in this string
        int validCharacters = numberString.find_first_not_of("0123456789-.");
        if (-1 != validCharacters)
		{
            LOGMORE( SourceInfo, "Number contained an invalid character");
			FUNCTION_EXIT;
            return false;
		}

        // Check the negative sign is correct
		int firstDash = numberString.find_first_of("-");                         // finds the location of the first "-"
		int otherDash = numberString.find_first_of("-",firstDash+1);             // determines if there are any subsequent "-"s
		int trailingDash = numberString.find_first_of("0123456789",firstDash+1); // finds the location of the first character that is a number following the dash - this should be firstDash+1

		if (firstDash != otherDash && -1 != otherDash)
		{
            LOGMORE( SourceInfo, "There is more than one dash in the number");
			FUNCTION_EXIT;
            return false;
		}

		if (-1 != firstDash && 0 != firstDash)
		{
            LOGMORE( SourceInfo, "The only dash in the number is not the first character");
			FUNCTION_EXIT;
            return false;
		}

		if (-1 != firstDash && (firstDash+1) != trailingDash) // 
		{
            LOGMORE( SourceInfo, "There is not a numerical character immediately trailing the dash");
			FUNCTION_EXIT;
            return false;
		}

        int firstPoint = numberString.find_first_of(".");                          // finds the location of the first "."
		int otherPoint = numberString.find_first_of(".",firstPoint+1);             // determines if there are any subsequent "."s
		int trailingPoint = numberString.find_first_of("0123456789",firstPoint+1); // finds the locatino of the first -numerical character after the first point - this should return firstPoint+1

		if (firstPoint != otherPoint && -1 != otherPoint)
		{
            LOGMORE( SourceInfo, "There is more than one decimal point in the number");
			FUNCTION_EXIT;
            return false;
		}

		if (-1 != firstPoint && (firstPoint+1) != trailingPoint)
		{
            LOGMORE( SourceInfo, "There is not a numerical character immediately trailing the decimal point");
			FUNCTION_EXIT;
            return false;
		}

		if (-1 != firstPoint && ((0 == firstPoint) || (-1 != firstDash && 1 == firstPoint))) 
		{
            LOGMORE( SourceInfo, "The decimal point is in an invalid location");
			FUNCTION_EXIT;
            return false;
		}	

        LOGMORE( SourceInfo, "The number string is valid");
        FUNCTION_EXIT;
		return true;
    }


    bool FieldValidityChecker::isValidInteger(const std::string& numberString) const
    {
        FUNCTION_ENTRY("isValidInteger");

        LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "String to check if it is a valid integer is: %s",numberString.c_str());

        // Integers must contain all numbers except for the - sign. The - sign must be at the beginning

		int validCharacters = numberString.find_first_not_of("0123456789-");
		int dashLocation = numberString.find_last_of("-");

		// The first test ensures there are no invalid characters
		// The second test ensures the dash is in either the first or second location
		// The third test ensures the returned number is not null
		if ((-1 != validCharacters) || !(dashLocation == 0 || dashLocation == -1) || (numberString.size() == 0))
		{
            LOGMORE( SourceInfo, "The number contains invalid characters or dashes in the wrong position");
			FUNCTION_EXIT;
            return false;
		}

        LOGMORE( SourceInfo, "The number string is valid");
        FUNCTION_EXIT;
        return true;
    }

}
