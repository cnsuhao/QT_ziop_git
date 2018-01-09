/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/banner/src/banner.h $
  * @author:  C. DeWolfe
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  * 
  * The class, BannerApp, provides the CWinApp for this MFC project.
  * The app uses the GenericGUI framework to display a top level dialog of type BannerDlg.
  * Refer to the BannerDlg for more information 
  *
  */

#if !defined(AFX_BANNER_H__8457DD48_0374_4DA0_BA32_112A601E1D5E__INCLUDED_)
#define AFX_BANNER_H__8457DD48_0374_4DA0_BA32_112A601E1D5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/system_control/banner/pmodel/src/BannerAppController.h"
#include "app/system_control/banner/src/resource.h"		// main symbols
#include "bus/generic_gui_view/src/TransActiveWinApp.h"

/////////////////////////////////////////////////////////////////////////////
// BannerApp:
// See banner.cpp for the implementation of this class
//

//Forward declaration
class ATSCOMHandler;

class BannerApp : public TA_Base_Bus::TransActiveWinApp
{
public:
	BannerApp();

	/**
	* AlarmManagerApp
	*
	* Constructor for the AlarmManagerApp object.
	*
	* @return
	*
	* @exception
	*/
	BannerApp(int argc, char *argv[]);
	virtual ~BannerApp(){};

	TA_Base_Bus::IApplicationController * createApplicationController();

	virtual TA_Base_Bus::ITransActiveView* createMainView();

private:
	ATSCOMHandler* m_pAtsComHandler;
	//BOOL InitATL();

	//BOOL m_bATLInited;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BANNER_H__8457DD48_0374_4DA0_BA32_112A601E1D5E__INCLUDED_)
