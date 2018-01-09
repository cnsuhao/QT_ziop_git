/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/system_control/banner/pmodel/src/BannerAppController.h $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: Noel
  *
  */

#ifndef __BANNER_PMODEL_CONST_H_INCLUDED__
#define __BANNER_PMODEL_CONST_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace TA_Base_App
{
	#define PMODEL_START_PAGE_UPDATE "UpdateBannerStartPage"

	#define PMODEL_PROFILE_LOCAL	0
	#define PMODEL_LEFT_BANNER		"LeftBanner"
	#define PMODEL_CENTRE_BANNER	"CentreBanner"
	#define PMODEL_RIGHT_BANNER		"RightBanner"

	#define PMODEL_LEFT_BANNER_DEFAULT_STR		"LeftBannerDefaultPage"
	#define PMODEL_CENTRE_BANNER_DEFAULT_STR	"CentreBannerDefaultPage"
	#define PMODEL_RIGHT_BANNER_DEFAULT_STR		"RightBannerDefaultPage"

	#define PMODEL_BANNER_DLG	"BannerDlg"

	enum Banner_EventArgs
	{
		START_TAB = 1,
		MAX_EVENT_AGRS
	};


} //TA_Base_App

#endif //__BANNER_PMODEL_CONST_H_INCLUDED__
