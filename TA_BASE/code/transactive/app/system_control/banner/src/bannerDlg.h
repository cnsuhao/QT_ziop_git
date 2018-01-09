/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/banner/src/bannerDlg.h $
  * @author:  C. DeWolfe
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  * 
  * The class, BannerDlg, provides the "main window" for this application.
  * The app uses the GenericGUI framework, and  BannerDlg is a AbstractDialogGUI.
  * The BannerDlg uses a framework provided by TStackedDialog and related classes,
  * to provide functionality similar to a tabbed dialog. There are four tabbed pages,
  * which provide a list of:
  * 1) ISCS (Transactive) alarms
  * 2) ATS alarms
  * 3) Communications (to the operator)
  * 4) Plans - all plans currently running in the system
  *
  * Only one of the above pages is displayed at a time - like a CPropertyPage
  * To the left of the displayed page there are four buttons, ISCS, ATS, Plans, and Calls
  * These buttons act as the "tabs"
  * It should be noted that these tabs are required to always be visble. Hence, a seperate window
  * is used to contains the tabs - its parent is set to the desktop so that its Z-order can be 
  * set to always-on-top. Note that this would not work if the tabs were a child of the BannerDlg
  *
  * The BannerDlg provides a mechanism to flash a selected tab(s). This is intended as a notification
  * to the user that there is something in the related page which they should address.
  * Each page provides additional functionality related to what it displays
  *
  */

#if !defined(AFX_BANNERDLG_H__2190A8E3_4F74_44CB_84B5_A70F391EE691__INCLUDED_)
#define AFX_BANNERDLG_H__2190A8E3_4F74_44CB_84B5_A70F391EE691__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app\system_control\banner\src\ComponentLibraryAccessor.h"
#include "app\system_control\banner\pmodel\src\BannerDlgPModel.h"
#include "bus/generic_gui_view/src/TransActiveWinApp.h"
#include "bus\banner_framework\src\BannerFrameworkDialog.h"
#include "app\system_control\banner\GeneratedFiles\ui_bannerdlg.h"
#include <QIcon.h>

#define WMU_NOTIFY_UPDATE_STARTTAB WM_USER + 99

namespace TA_Base_App
{

    class BannerGUI;

    class BannerDlgImpl : public TA_Base_Bus::BannerFrameworkDialog
    {
		Q_OBJECT
    public:
		BannerDlgImpl(TA_Base_Bus::ITransActiveApp * pWinApp, QWidget* parent = NULL);
		void closeEvent(QCloseEvent * event);
	signals:
		void setFlash(std::string dialogID, bool enable);

	public slots:
		void OnUpdateStartTab(std::string startTab);
		void setFlashState(std::string dialogID, bool enable);
	
	protected:
	    QIcon m_hIcon;

		// UI Separation
		virtual std::string getMyPModelName();
		void setupPModel();
		void initUIControl();
		void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);
    private:
	    
        void setBannerPage();

        static const int TAB_WIDTH;
        static const char* LEFT_BANNER;
        static const char* CENTRE_BANNER;
        static const char* RIGHT_BANNER;

        static const char* LEFT_BANNER_DEFAULT_STR;
        static const char* CENTRE_BANNER_DEFAULT_STR;
        static const char* RIGHT_BANNER_DEFAULT_STR;

        // The component that performs DLL related tasks
        ComponentLibraryAccessor m_componentLibraryAccessor;

        int m_pageWidth;
        int m_pageHeight;
        int m_pageXPos;
        int m_pageYPos;

		//UI Separation
		BannerDlgPModel* m_pRealModel;
		Ui_bannerDlg m_bannerUi;

    };


} // namespace TA_Base_App

#endif // !defined(AFX_BANNERDLG_H__2190A8E3_4F74_44CB_84B5_A70F391EE691__INCLUDED_)
