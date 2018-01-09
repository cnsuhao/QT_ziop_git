/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/data_management/archive_manager/src/ArchiveGuiObserver.cpp $
  * @author:  
  * @version: $Revision: #1  $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:  $
  *
  * This Class is used by for formatting the user message box strings.
  *
  */

#include "app/data_management/archive_manager/src/stdafx.h"
#include "app/data_management/archive_manager/src/ArchiveGuiObserver.h"

namespace TA_Base_App
{
	ArchiveGuiObserver::ArchiveGuiObserver()
	{

	}

	ArchiveGuiObserver::~ArchiveGuiObserver()
	{

	}

	std::string ArchiveGuiObserver::getStringResourceFormated(UINT resourceId, std::string argument1)
	{
		TAUnicodeString result;
		CString argument = TAUnicodeString::CreateFromMultitByteString(argument1.c_str()).str();
		result = TAAfxFormatString(resourceId,argument);
		return result.toMultiByteStdString();
	}

	std::string ArchiveGuiObserver::getStringResourceFormated(UINT resourceId, std::string argument1, std::string argument2)
	{
		TAUnicodeString result;
		result = TAAfxFormatString( resourceId, TAUnicodeString::CreateFromMultitByteString(argument1.c_str()).str() ,TAUnicodeString::CreateFromMultitByteString(argument2.c_str()).str());
		return result.toMultiByteStdString();
	}
	
	std::string ArchiveGuiObserver::getStringResource(UINT resourceId)
	{
		TAUnicodeString result;
		result = TALoadString(resourceId);
		return result.toMultiByteStdString();
	}

	std::string ArchiveGuiObserver::getStringResourceFormated(UINT resourceId, UINT argument1)
	{
		TAUnicodeString argumentStr;
		argumentStr = TALoadString(argument1);
		TAUnicodeString result;
		result = TAAfxFormatString( resourceId, argumentStr.str() );
		return result.toMultiByteStdString();
	}

	std::string ArchiveGuiObserver::getStringResourceFormated(UINT resourceId, std::string argument1,UINT argument2)
	{
		TAUnicodeString argumentStr;
		argumentStr =  TALoadString(argument2);
		TAUnicodeString result;
		result = TAAfxFormatString( resourceId, TAUnicodeString::CreateFromMultitByteString(argument1.c_str()).str(),argumentStr.str() );
		return result.toMultiByteStdString();
	}
	std::string ArchiveGuiObserver::getStringResourceFormated(UINT resourceId, std::string argument1,double argument2,std::string argument3)
	{
		TAUnicodeString result;
		TAUnicodeString argument2Str;
		argument2Str.str().Format(_T("%d"),argument2);
		result = TAAfxFormatString( resourceId, TAUnicodeString::CreateFromMultitByteString(argument1.c_str()).str(),argument2Str.str(),TAUnicodeString::CreateFromMultitByteString(argument3.c_str()).str() );
		return result.toMultiByteStdString();
	}
	
	std::string ArchiveGuiObserver::getStringResourceFormated(UINT resourceId, std::string argument1,std::string argument2,std::string argument3)
	{
		TAUnicodeString result;
		result = TAAfxFormatString( resourceId, TAUnicodeString::CreateFromMultitByteString(argument1.c_str()).str(),TAUnicodeString::CreateFromMultitByteString(argument2.c_str()).str(),TAUnicodeString::CreateFromMultitByteString(argument3.c_str()).str() );
		return result.toMultiByteStdString();
	}
}