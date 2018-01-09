/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/alarm/alarm_manager/src/AlarmsGui.cpp $
  * @author:  A. Siow
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * Defines the class behaviors for the application. Launches the Alarm Manager as
  * a managed application using Generic GUI. 
  */
#include "globals.h"
#include "AlarmManagerApp.h"
#include "ace\ACE.h"
#include "core\utilities\src\DebugUtil.h"
#include "app\alarm\alarm_manager\pmodel\src\AlarmManagerAppController.h"
#include "app\alarm\alarm_manager\src\AlarmViewFactory.h"
#include "bus\generic_gui_view\src\TransActiveMessage.h"
#include <QtWidgets\QMessageBox>

namespace TA_Base_App
{
	 
	AlarmManagerApp::AlarmManagerApp(int argc, char *argv[]) 
		:TA_Base_Bus::TransActiveWinApp(argc,argv,TA_Base_Bus::MVT_DIALOG, true)
	{			 
		LOGBEFORECOTS("ACE::init" );
		ACE::init();
		LOGAFTERCOTS("ACE::init");	
		AlarmViewFactory* vf=new AlarmViewFactory(this);		 
		this->getViewManager()->setViewFactory(vf);
	}

	TA_Base_Bus::IApplicationController * AlarmManagerApp::createApplicationController()
	{
		return new AlarmManagerAppController(this, "Alarm Manager");
	}

	bool AlarmManagerApp::runWithoutControlStation()
	{
		return false;
	}

	TA_Base_Bus::ITransActiveView* AlarmManagerApp::createMainView()
	{
		return this->getViewManager()->getView(ViewType_AlarmManager, "main");
	}
}

/////////////////////////////////////////////////////////////////////////////
 
