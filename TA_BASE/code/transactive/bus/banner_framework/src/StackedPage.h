/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/StackedPage.h $
  * @author:  Zoran M.Todorovic
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  * 
  * The class TStackedPage provides a "child" dialog in the stacked dialog framework.
  * The three classes TStackedDialog, TStackedPage, and TStackedPageHelper 
  * used togeather, provide a framework to create a dialog similar in behaviour to a CPropertySheet.
  * Basically, the TStackedDialog acts as a frame to display "child" dialogs (TStackedPage).
  * At any given time all,but one TStackedPage objects (child dialogs) are hidden.
  * There is also a TStackedPageHelper class which provides help in manipulating and adding dialogs.
  *
  * Note that the original code was third party, as the original header of STACKDIALOG.H indicates. 
  * The definitions and declarations all originally resided in two files
  */


#if !defined __STACKED_PAGE_H__
#define __STACKED_PAGE_H__

#include "bus/generic_gui_view/src/TransActiveDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace TA_Base_Bus
{
    class TStackedPage :  public TA_Base_Bus::AbstractDialog
    {
		Q_OBJECT
    public:
		TStackedPage(TA_Base_Bus::ITransActiveApp * app, std::string viewType, QWidget* pParent = NULL);
		
	    virtual ~TStackedPage();

	    virtual void OnSetActive(void);
	    virtual void OnKillActive(void);
	    virtual void OnCreatePage(void);
	    virtual void OnDestroyPage(void);

	  
	public slots:
	    virtual void OnOK();
	    virtual void OnCancel();

    };
}

#endif //#if !defined __STACKED_PAGE_H__
