/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mfc_extensions/src/prnScreen/prnScreen.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */

#ifndef IMPORT_EXTENSIONS
#define PRNSCREEN_API __declspec(dllexport)
#else
#define PRNSCREEN_API __declspec(dllimport)
#endif
//TD14928 bgn
#ifdef  __cplusplus
extern "C"
{
	PRNSCREEN_API void CapturePrintScreen(long lX);
}
#else
	PRNSCREEN_API void CapturePrintScreen(long lX);
#endif
//TD14928 end

