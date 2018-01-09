/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/StackDialog.h $
  * @author:  Zoran M.Todorovic
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  * 
  * The class TStackedDialog provides the "parent" dialog in the stacked dialog framework.
  * The three classes TStackedDialog, TStackedPage, and TStackedPageHelper 
  * used togeather, provide a framework to create a dialog similar in behaviour to a CPropertySheet.
  * Basically, the TStackedDialog acts as a frame to display "child" dialogs (TStackedPage).
  * At any given time all,but one TStackedPage objects (child dialogs) are hidden.
  * There is also a TStackedPageHelper class which provides help in manipulating and adding dialogs.
  *
  * TStackedDialog is an abstract class.
  *
  * Note that the original code was third party, as the original header of STACKDIALOG.H (below) indicates. 
  * The definitions and declarations all originally resided STACKDIALOG.H and STACKDIALOG.CPP
  */
/*#############################################################################
# STACKDIALOG.H
#
# SCA Software International S.A.
# http://www.scasoftware.com
# scaadmin@scasoftware.com
#
# Copyright (c) 1999 SCA Software International S.A.
#
# Date: 17.12.1999.
# Author: Zoran M.Todorovic
#
# This software is provided "AS IS", without a warranty of any kind.
# You are free to use/modify this code but leave this header intact.
#
#############################################################################*/

#if !defined __STACKED_DIALOG_H__
#define __STACKED_DIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bus/banner_framework/src/StackedPageHelper.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include <QFrame>
namespace TA_Base_Bus
{
    class TStackedPage;

    class TStackedDialog : public TA_Base_Bus::AbstractTransActiveDialog
    {
    protected:
		QFrame* m_nPlaceholder;
		TStackedPageHelperList m_PageList;

    private:
		
	    void flush(void);
	    void destroyPageDialog(TStackedPageHelper *pPage);
	    TStackedPageHelper* getFirst(void);
	    TStackedPageHelper* getCurrent(void);
	    TStackedPageHelper* getNext(void);

    protected:
	    void activatePage(TStackedPageHelper *pPage);
	    void deactivatePage(TStackedPageHelper *pPage);

    public:
		TStackedDialog(ITransActiveApp * pWinApp, std::string viewType, QWidget* pParent = NULL);
		
	    virtual ~TStackedDialog();

		TStackedPageHelper* AddPage(const std::string& nId, DWORD dwHelpId = 0L,
							      DWORD dwData1 = 0, DWORD dwData2 = 0);
	    void DelPage(TStackedPageHelper* pPage);

	    void SetPage(void);
		void SetPage(const std::string& nId);
	    void SetFirstPage(void);
	    void SetNextPage(void);

	    TStackedPageHelper *GetPage(void);
		TStackedPageHelper *GetPage(const std::string& nId);

		virtual TStackedPage* CreatePage(const std::string& nId) = 0;
	    virtual void OnPageChanged(const std::string& nId, BOOL bActivated) = 0;
	   
		void initPlaceHolder(QFrame* placeholder);
    };
}
#endif //!defined __STACKED_DIALOG_H__
