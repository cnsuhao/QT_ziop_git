/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/BannerFrameworkDialog.h $
  * @author:  C. DeWolfe
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  * 
  * The class, BannerFrameworkDialog, provides the "main window" for this application.
  * The app uses the GenericGUI framework The BannerFrameworkDialog uses a framework 
  * provided by TStackedDialog and related classes to provide functionality similar to
  * a tabbed dialog. 
  * 
  * Only one of the above pages is displayed at a time - like a CPropertyPage
  * To the left of the displayed page there are the selection buttons for the pages ("tabs") 
  *
  * It should be noted that these tabs are required to always be visble. Hence, a seperate window
  * is used to contains the tabs - its parent is set to the desktop so that its Z-order can be 
  * set to always-on-top. Note that this would not work if the tabs were a child of the BannerDlg
  *
  */

#ifndef BANNERFRAMEWORKDIALOG_H
#define BANNERFRAMEWORKDIALOG_H 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <QIcon>
#include "bus/banner_framework/src/StackDialog.h"
#include "bus/banner_framework/src/Tabs.h"

namespace TA_Base_Bus
{
    class BannerPage; // Forward declaration

    // Struct to hold the resource IDs for a banner icon.
    struct BannerIcon
    {
        int onIcon;
		int offIcon;
		int onDownIcon;
		int offDownIcon;
    };

    class BannerFrameworkDialog : public TStackedDialog
    {
    public:
	    
		BannerFrameworkDialog(const std::string& viewType, ITransActiveApp * pWinApp,
                    int xPos, int yPos, int width, int height, int tabWidth, QWidget* pParent = NULL);


		void onTabDoubleClick(const std::string& pageId);

        /**
         * enableButtonFlash
         *
         * Starts flashing a particular button
         * 
         * @param buttonName - the name of the button to flash
         * @param dialogId - the ID of the banner page to flash
         *
         * Pre-condition: buttonName must have been added using AddBannerPage
         */
		void enableButtonFlash(const std::string& dialogId);

        /**
         * disableButtonFlash
         *
         * Stops flashing a particular button
         * 
         * @param buttonName - the name of the button to flash
         * @param dialogId - the ID of the banner page to flash
         *
         * Pre-condition: buttonName must have been added using AddBannerPage
         */
		void disableButtonFlash(const std::string& dialogId);
    
        /**
         * showPage 
         *
         * Displays a particular page.
         *
         * @param pageName - name of the page to display
         */
        void showPageByName(const std::string& pageName);
    
        /**
         * showPage 
         *
         * Displays a particular page.
         *
         * @param pageId - ID of the page to display
         */
		void showPageByID(const std::string& pageId);

        /**
         * AddBannerPage
         *
         * Adds a page to the banner with an associated tab button. The name of the page
         * is the same as the name of the button.
         *
         * @param pageId - the id of the MFC dialog associated with the page
         * @param pageName - the name of the page
         * @param bannerIcon - resource IDs for the icons
         * @param page - a pointer the BannerPage for this page
         *
         * Pre-condition: pageId must be unique
         */
		void AddBannerPage(const std::string& pageId, const std::string& pageName, const BannerIcon& bannerIcon,
                            BannerPage* page);
 
        /**
         * getXPosition
         *
         * Returns the left most position of the dialog in pixels
         */
        int getXPosition();

        /**
         * getYPosition
         *
         * Returns the top most position of the dialog in pixels
         */
        int getYPosition();

        /**
         * getWidth
         *
         * Returns the width of the dialog in pixels
         */
        int getWidth();

        /**
         * getHeight
         *
         * Returns the height of the dialog in pixels
         */
        int getHeight();
   
	    /**
	     * getTabs
	     * 
	     * Returns a pointer to the (private) Tabs object
	     */
        Tabs *getTabs();

	    // TStackedDialog method implementations:
		virtual TStackedPage *CreatePage(const std::string& nId);
		virtual void OnPageChanged(const std::string& nId, BOOL bActivated);
     
	    int		m_PageSelection;
		    // NOTE: the ClassWizard will add data members here
	  
    // Implementation
    protected:

	    // Generated message map functions
	    //{{AFX_MSG(BannerDlg)
	   
	/*    afx_msg void OnMove(int x, int y);
	    afx_msg void OnClose();
	    afx_msg void OnDestroy();*/
 
    private:

	    void setPagePointers();
	    void moveTabs(); // we must move the tab window manually

    private:
        enum{ID_HOT_KEY_ALTF4};

		std::map<std::string, BannerPage*> m_bannerPages;
		std::map<std::string, std::string> m_bannerIDs;

    protected:
	    int m_xPos,	m_yPos,	m_width, m_height;
	    HICON m_hIcon;
		Tabs m_tabs;

    };

} // namespace TA_Base_Bus

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // BANNERFRAMEWORKDIALOG_H