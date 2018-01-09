/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/about_box/src/TransActiveAboutBox.cpp $
  * @author:  Ripple
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2015/11/24 11:54:54 $
  * Last modified by:  $Author: limin.zhu $
  *
  */

#include "TransActiveAboutBox.h"
#include "TransActiveAboutBoxDlg.h"


void CTransActiveAboutBox::DisplayAboutBox(const char buildVersion[],
        const char buildDate[],
        const char componentName[],
        const char componentVersion[],
        const int headerArraySize,
        char* headerArray[])
{
    TransactiveAboutBoxDlg* dlg = new TransactiveAboutBoxDlg(buildVersion, buildDate, componentName, componentVersion, headerArraySize, headerArray);
    dlg->show();
}
