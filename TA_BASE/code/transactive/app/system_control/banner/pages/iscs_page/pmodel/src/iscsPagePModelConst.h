/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/3001/transactive/app/incidents/incident_manager/pmodel/src/iscsPagePModelConst.h $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: Noel
  *
  */

#ifndef __ISCS_PAGE_PMODEL_CONST_H_INCLUDED__
#define __ISCS_PAGE_PMODEL_CONST_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace TA_Base_App
{
	#define PROFILE_LOCAL   0
	#define PROFILE_CENTRAL 1

	#define PMODEL_ALARMS_GUI_DLG	"AlarmGuiDialog"

	// Incident Report Manager Data change
	#define PMODEL_SESSION_CHANGE	"SessionChange"
	#define PMODEL_NOTIFY_UNMUTE	"UnMute"
	#define PMODEL_SYSTEM_STATUS	"System Status"

	enum ISCS_EventArgs
	{
		SESSION_ID = 1,
		MAX_EVENT_AGRS
	};

}

#endif //__ISCS_PAGE_PMODEL_CONST_H_INCLUDED__