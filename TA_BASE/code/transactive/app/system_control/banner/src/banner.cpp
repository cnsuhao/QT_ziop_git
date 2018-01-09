/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/banner/src/banner.cpp $
  * @author:  Ripple
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  *
  */
// banner.cpp : Defines the class behaviors for the application.
//

#include "app/system_control/banner/src/stdafx.h"
#include "ATSCOMHandler.h"
#include "ace\ACE.h"
#include "app/system_control/banner/src/banner.h"
#include "app/system_control/banner/src/bannerDlg.h"
#include "app/system_control/banner/src/CommonDef.h"
#include "app/system_control/banner/src/BannerInterface.h"
#include "app/system_control/banner/src/BannerViewFactory.h"
//#include "app/system_control/banner/src/IscsBanner.h"
//#include "app/system_control/banner/src/BannerInterface_i.c"
#include "bus/banner_framework/src/constants.h"
#include "bus/language/src/TALanguage.h"
#include <initguid.h>
#include <atlconv.h>


using namespace TA_Base_App;
using TA_Base_Bus::TransActiveWinApp;

/////////////////////////////////////////////////////////////////////////////
// BannerApp construction

BannerApp::BannerApp(int argc, char *argv[])
	:TA_Base_Bus::TransActiveWinApp(argc, argv, TA_Base_Bus::MVT_DIALOG, true)
{
	// Place all significant initialization in InitInstance
	LOGBEFORECOTS("ACE::init");
	ACE::init();
	LOGAFTERCOTS("ACE::init");
	TA_Base_App::BannerViewFactory* vf = new TA_Base_App::BannerViewFactory(this);
	this->getViewManager()->setViewFactory(vf);

	m_pAtsComHandler = new ATSCOMHandler(getCommandLine());
	m_pAtsComHandler->initialize();
}


TA_Base_Bus::IApplicationController * BannerApp::createApplicationController()
{
	return new TA_Base_App::BannerAppController(this, "Banner");
}

TA_Base_Bus::ITransActiveView* BannerApp::createMainView()
{
	return this->getViewManager()->getView(ViewType_MainBanner, "main");
}
