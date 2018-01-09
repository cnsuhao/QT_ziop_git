/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/banner/pages/iscs_page/src/globals.h $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  *
  */
#ifndef GLOBALS_H
#define GLOBALS_H
#include <QtCore/qglobal.h>

#define	ViewType_IscsBannerPange			"IscsBannerPage"

//static const ULONG WM_ON_CHANGE_MSG		= WM_USER + 1;
//static const ULONG WM_UPDATE_ALARMS_MSG = WM_USER + 2;
//static const ULONG WM_ON_SYSTEM_DEGRADED =	WM_USER + 3;
//static const ULONG WM_ON_SYSTEM_RECOVERED =	WM_USER + 4;
//static const ULONG WM_ON_SESSION_CHANGED =	WM_USER + 5;
//static const ULONG WM_ON_NOTIFY_UNMUTE =	WM_USER + 6;

#ifdef ISCSPAGEDLL_LIB
# define ISCSPAGEDLL_EXPORT Q_DECL_EXPORT
#else
# define ISCSPAGEDLL_EXPORT Q_DECL_IMPORT
#endif

#endif
