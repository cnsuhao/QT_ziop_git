/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/StackedPageHelper.h $
  * @author:  Zoran M.Todorovic
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  * 
  * The class TStackedPageHelper is a helper class in the stacked dialog framework - see below.
  * The three classes TStackedDialog, TStackedPage, and TStackedPageHelper 
  * used togeather, provide a framework to create a dialog similar in behaviour to a CPropertySheet.
  * Basically, the TStackedDialog acts as a frame to display "child" dialogs (TStackedPage).
  * At any given time all TStackedPage objects (child dialogs) are hidden, but one.
  * There is also a TStackedPageHelper class which provides help in manipulating and adding dialogs.
  *
  * Note that the original code was third party, as the  header file in STACKDIALOG.H indicates
  * indicates. The definitions and declarations all originally resided STACKDIALOG.H and STACKDIALOG.CPP
  */

#if !defined __STACKED_PAGE_HELPER_H__
#define __STACKED_PAGE_HELPER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

namespace TA_Base_Bus
{
    class TStackedPage;
    class TStackedDialog;

    class TStackedPageHelper
    {
    private:
        bool m_bCreated;            // True if m_pDialog is created
		bool m_bActive;             // True if dialog is active (has focus)
        std::string m_nDialogID;            // Resource ID of this page
        TStackedPage* m_pDialog;    // Child dialog of this page
        TStackedDialog* m_pParent;  // Parent dialog

    public:
		unsigned long m_dwHelpID;           // Help ID for this page
		unsigned long m_dwData1;            // User defined data
        unsigned long m_dwData2;            // User defined data

    public:
        TStackedPageHelper();
        TStackedPageHelper(const TStackedPageHelper& obj);
        ~TStackedPageHelper();
        TStackedPageHelper& operator=(const TStackedPageHelper& obj);

        TStackedPage* GetPage(void);
		bool IsActive(void);
        void Activate(void);
        void Deactivate(void);

        friend class TStackedDialog;
    };

    typedef std::list< TStackedPageHelper > TStackedPageHelperList;
}

#endif //!defined __STACKED_PAGE_HELPER_H__
