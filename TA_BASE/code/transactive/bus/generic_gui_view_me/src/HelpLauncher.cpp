/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/HelpLauncher.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This singleton contains the methods used to launch TransActive dialogs.
  * These include About boxes, help and context specific help.
  *
  */

#pragma warning(disable:4786)

#include "bus\generic_gui_view\src\HelpLauncher.h"
#include "bus\generic_gui_view\src\StdAfx.h"

#include "bus\about_box\src\TransActiveAboutBox.h"
#include "core\versioning\src\Version.h"
//#include "cots\HTMLHelp\inc\htmlhelp.h"
#include "core\utilities\src\RunParams.h"
#include "core\utilities\src\DebugUtil.h"

using namespace TA_Base_Core;

namespace TA_Base_Core
{
    // External versioning variables
    // See VersionGen.cpp
    //
    extern char buildVersion[];
    extern char buildDate[];
    extern char componentName[];
    extern char componentVersion[];
    extern int headerArraySize;
    extern char* headerArray[];
}


namespace TA_Base_Bus
{

    HelpLauncher* HelpLauncher::s_instance = 0;


    HelpLauncher& HelpLauncher::getInstance()
    {
        if( s_instance == 0 )
        {
            s_instance = new HelpLauncher();
        }
        return *s_instance;

    }


    void HelpLauncher::displayAboutBox()
    {
        CTransActiveAboutBox::DisplayAboutBox( TA_Base_Core::buildVersion,
                                               TA_Base_Core::buildDate,
                                               TA_Base_Core::componentName,
                                               TA_Base_Core::componentVersion,
                                               TA_Base_Core::headerArraySize,
                                               TA_Base_Core::headerArray);
    }


    void HelpLauncher::displayContextSpecificHelp(unsigned int itemID)
    {
        FUNCTION_ENTRY("displayContextSpecificHelp");

        LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "displayContextSpecificHelp: resource ID = %d", itemID);

        // if the HelpFile runtime parameter is set
        if (TA_Base_Core::RunParams::getInstance().isSet(RPARAM_HELPFILE))
        {
            displayContextSpecificHelp(itemID,
                TA_Base_Core::RunParams::getInstance().get(RPARAM_HELPFILE).c_str());
        }
        else
        {
            // else log a warning
            LOG_GENERIC( SourceInfo, DebugUtil::DebugWarn, "displayContextSpecificHelp: HelpFile parameter not set!");
        }

        FUNCTION_EXIT
    }

    void HelpLauncher::displayContextSpecificHelp(unsigned int itemID, const std::string& filePath)
    {
        //FUNCTION_ENTRY("displayContextSpecificHelp (2)");

        //LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "displayContextSpecificHelp: resource ID = %d, filePath = %s",
        //    itemID, filePath.c_str());

        //// attempt to display help for the given resourceID
        //HWND hHelpWnd = ::HtmlHelpA(0, filePath.c_str(),
        //    HH_HELP_CONTEXT, itemID);
        //
        //if (0 == hHelpWnd)
        //{
        //    // failed to display context specific, just show generic help
        //    displayHelp();
        //}
        //else
        //{
        //    // center the help wnd over the currently active wnd
        //    CWnd *pHelpWnd = CWnd::FromHandle(hHelpWnd);
        //    if (0 != pHelpWnd)
        //    {
        //        pHelpWnd->CenterWindow(CWnd::GetActiveWindow());
        //    }
        //}

        //FUNCTION_EXIT
    }


    void HelpLauncher::displayHelp(const bool topMost/* = false */)
    {
        // If the HelpFile run param has been set, use that chm file...
        if (TA_Base_Core::RunParams::getInstance().isSet(RPARAM_HELPFILE))
        {
            displayHelp(TA_Base_Core::RunParams::getInstance().get(RPARAM_HELPFILE), topMost);
        }
        // Else use a hard-coded file name.
        else
        {
            displayHelp("Transactive.chm", topMost);
        }
    }


    void HelpLauncher::displayHelp(const std::string& filePath, const bool topMost/* = false */)
    {
        //hHelpWnd = HtmlHelpA(0, filePath.c_str(), HH_DISPLAY_TOC, NULL);
        //if (0 != hHelpWnd)
        //{
        //    // center the help wnd over the currently active wnd
        //    CWnd *pHelpWnd = CWnd::FromHandle(hHelpWnd);
        //    if (0 != pHelpWnd)
        //    {
        //        if (topMost)
        //        {
        //            pHelpWnd->SetForegroundWindow();
        //            pHelpWnd->SetWindowPos(&CWnd::wndTopMost, -1, -1, -1, -1, SWP_NOSIZE|SWP_NOMOVE);
        //            hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMessaageFunc, 0, GetWindowThreadProcessId(hHelpWnd,NULL));
        //            LOG_GENERIC( SourceInfo, DebugUtil::DebugInfo, "help hook %d", hHook);
        //        }
        //        //TD14357
        //        WINDOWPLACEMENT winPlace;
        //        memset(&winPlace,0,sizeof(winPlace));
        //        winPlace.length = sizeof(WINDOWPLACEMENT);
        //        pHelpWnd->GetWindowPlacement(&winPlace);
        //        TA_Rect rt;
        //        CWnd::GetActiveWindow()->GetWindowRect(&rt);
        //        rt.top += 100;
        //        rt.left += 100;
        //        rt.bottom -= 100;
        //        rt.right -= 100;
        //        winPlace.rcNormalPosition = rt;
        //        pHelpWnd->SetWindowPlacement(&winPlace);

        //        pHelpWnd->CenterWindow(CWnd::GetActiveWindow());
        //    }
        //}
    }
}

