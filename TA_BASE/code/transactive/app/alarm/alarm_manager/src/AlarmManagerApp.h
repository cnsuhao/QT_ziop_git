/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/alarm/alarm_manager/src/AlarmsGui.h $
  * @author:  A. Siow
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * main header file for the ALARMSGUI application  
  */

#if !defined(AFX_ALARMSGUI_H__F9076B65_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
#define AFX_ALARMSGUI_H__F9076B65_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_

 
#include "bus/generic_gui_view/src/TransActiveWinApp.h"

/////////////////////////////////////////////////////////////////////////////
// AlarmManagerApp:
// See AlarmsGui.cpp for the implementation of this class
//
namespace TA_Base_App
{


	class AlarmManagerApp : public TA_Base_Bus::TransActiveWinApp
	{
	public:

		/**
		  * AlarmManagerApp
		  *
		  * Constructor for the AlarmManagerApp object. 
		  * 
		  * @return 
		  *
		  * @exception 
		  */	
		AlarmManagerApp(int argc, char *argv[]);
		virtual ~AlarmManagerApp(){};

		TA_Base_Bus::IApplicationController * createApplicationController();

		virtual TA_Base_Bus::ITransActiveView* createMainView();
	 
	private:

		/**
		* runWithoutControlStation
		*
		* If the System Manager cannot connect to the Control Station then it must attempt to
		* run via other means. This method attempts to achieve this.
		*
		* @return This returns a boolean indicating whether the application should enter
		* the application message pump:
		*    True for Yes
		*    False for No
		*/
		bool runWithoutControlStation();
	
	};

}
////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMSGUI_H__F9076B65_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)

