/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/StackedPage.cpp $
  * @author:  Zoran M.Todorovic
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  * 
  * TStackedPage class defintion
  */

#include "bus/banner_framework/src/StackedPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace TA_Base_Bus
{

	TStackedPage::TStackedPage(TA_Base_Bus::ITransActiveApp * app,
					std::string viewType, QWidget* pParent /*= NULL*/)
					: AbstractDialog(app, viewType, pParent)
    {
    }

    TStackedPage::~TStackedPage()
    {
    }

    void TStackedPage::OnOK()
    {
    }

    void TStackedPage::OnCancel()
    {
    }

    // Called when a page is displayed
    void TStackedPage::OnSetActive(void)
    {
    }

    // Called before a page becomes invisible
    void TStackedPage::OnKillActive(void)
    {
    }

    // Called after the page is created
    void TStackedPage::OnCreatePage(void)
    {
    }

    // Called before the page is destroyed
    void TStackedPage::OnDestroyPage(void)
    {
    }
}
