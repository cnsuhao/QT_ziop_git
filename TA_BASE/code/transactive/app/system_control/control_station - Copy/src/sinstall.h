/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/sinstall.h $
  * @author:  Jiang Haipeng
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description> to install the screen saver and select the transactive screensaver as the default one.
  *
  */

#ifndef SINSTALL_H
#define SINSTALL_H

//-----------------------------------------------------------------------------
class ScreenSaverInstall
{
public:
	ScreenSaverInstall();
	virtual ~ScreenSaverInstall(){};

	BOOL InitInstance();
	
private:
	BOOL InstallIt() const;
	
	BOOL SelectAsDefault(LPCTSTR lpszScr) const;
	
};
//-----------------------------------------------------------------------------

#endif
// End of SINSTALL.H
