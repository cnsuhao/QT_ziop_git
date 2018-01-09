/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mfc_extensions/src/transactiveHelp/transactiveHelp.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */

#ifndef IMPORT_EXTENSIONS
#define TRANSACTIVEHELP_API __declspec(dllexport)
#else
#define TRANSACTIVEHELP_API __declspec(dllimport)
#endif

#include "bus/mfc_extensions/src/StdAfx.h"
#include "cots/HTMLHelp/inc/htmlhelp.h"

namespace TA_Base_Bus 
{
	extern "C"
	{
		TRANSACTIVEHELP_API int positionTransactiveHelp(
			const TCHAR* szHelp, 
			const TCHAR* szWindow,
			const TCHAR* szCaption,
			const TCHAR* szHome,
			long left, 
			long top, 
			long right, 
			long bottom);
	}
}

