/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/constants.h $
  * @author:  C. DeWolfe
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  * 
  * Just a few constant declarations that the Banner app uses
  */


#if !defined(BANNER_CONST_H__3A7A2_7A1A_4539_AA8B_9F1D15675D56__INCLUDED_)
#define BANNER_CONST_H__3A7A2_7A1A_4539_AA8B_9F1D15675D56__INCLUDED_

#include <string>
#include <QColor>

namespace BANNER_CONST
{
    // command lines to register and deregister the COM server
    static const std::string REGISTER_CMD = "--install";
    static const std::string UNREGISTER_CMD = "--remove";

	//changing this runparam indicates that the alarms associated with this
	//asset should be displayed in the banner
	static const char* RPARAM_ASSETALARM = "AssetAlarm";
	
	//this required run param specifies the tab that will be selected
	//when the Banner starts. It can be set to one of the following tab name values
	//note that we do not listen for changes to this param. It only has effect
	//during init
	static const std::string RPARAM_START_TAB = "StartTab";

	static const QColor CCL_GOLD = QColor::fromRgb(255, 213, 0);

    // Get a unique user defined message id for set window position message.
    static std::string WM_SET_FLASH_STATE = "SetFlashState";

    // Constant required by the ATS COM IDL.
    static const unsigned int ATS = 130; // Dialog ID of the ATS page.

	// 4669Pre-FAT TD#13762
	static const QColor CCL_RED = QColor::fromRgb(255, 0, 0);

	const static int BTN_ICO_ALARM_ON = 1;
	const static int BTN_ICO_ALARM_OFF = 2;
	const static int BTN_ICO_ALARM_ON_DOWN = 3;
	const static int BTN_ICO_ALARM_OFF_DOWN = 4;

	const static int BTN_ICO_CALL_ON = 5;
	const static int BTN_ICO_CALL_OFF = 6;
	const static int BTN_ICO_CALL_ON_DOWN = 7;
	const static int BTN_ICO_CALL_OFF_DOWN = 8;

	const static int BTN_ICO_INCIDENT_ON = 9;
	const static int BTN_ICO_INCIDENT_OFF = 10;
	const static int BTN_ICO_INCIDENT_ON_DOWN = 11;
	const static int BTN_ICO_INCIDENT_OFF_DOWN = 12;

	const static int BTN_ICO_PLAN_ON = 13;
	const static int BTN_ICO_PLAN_OFF = 14;
	const static int BTN_ICO_PLAN_ON_DOWN = 15;
	const static int BTN_ICO_PLAN_OFF_DOWN = 16;

	const static int BTN_ICO_ATS_ON = 17;
	const static int BTN_ICO_ATS_OFF = 18;
	const static int BTN_ICO_ATS_ON_DOWN = 19;
	const static int BTN_ICO_ATS_OFF_DOWN = 20;

	const static int BTN_ICO_ALARMKRTC_ON = 21;
	const static int BTN_ICO_ALARMKRTC_OFF = 22;
	const static int BTN_ICO_ALARMKRTC_ON_DOWN = 23;
	const static int BTN_ICO_ALARMKRTC_OFF_DOWN = 24;
}

#endif //!defined(BANNER_CONST_H__3A7A2_7A1A_4539_AA8B_9F1D15675D56__INCLUDED_)