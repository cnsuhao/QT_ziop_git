/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/banner/src/IscsBanner.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  *
  */
// IscsBanner.cpp : Implementation of CIscsBanner
#include "app/system_control/banner/src/stdafx.h"
#include "app/system_control/banner/src/BannerInterface.h"
#include "app/system_control/banner/src/IscsBanner.h"
#include "bus/banner_framework/src/constants.h"
#include "app/system_control/banner/src/Banner.h"

/////////////////////////////////////////////////////////////////////////////
// CIscsBanner


STDMETHODIMP CIscsBanner::setAtsButtonState(BOOL flash)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    //PostMessage(HWND_BROADCAST, BANNER_CONST::WM_SET_FLASH_STATE, BANNER_CONST::ATS, flash);
    return S_OK;
}
