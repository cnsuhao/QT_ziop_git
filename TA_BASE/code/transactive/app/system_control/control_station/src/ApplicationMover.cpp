/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ApplicationMover.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This thread is linked to a single instance of a RunningApplication. After a set period
  * of time it will attempt to move the application into the specified position.
  *
  */

#include "StdAfx.h"
#include "app/system_control/control_station/src/ApplicationMover.h"
#include "bus/generic_gui_pmod/src/GenericGuiConstants.h"
#include "bus/generic_gui_pmod/src/AppLauncher.h"
#include "bus/generic_gui_view/src/AnyRect.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/utilities/src/DebugUtil.h"

using TA_Base_Core::ThreadGuard;
using TA_Base_Core::DebugUtil;
using TA_Base_Bus::AnyRect;

ApplicationMover::ApplicationMover()
    : m_isTerminating(false)
{
    m_moveInformation.processId = 0;
    m_moveInformation.posFlag = 0;
    m_moveInformation.alignFlag = 0;
    m_moveInformation.objectDim = AnyRect::DEFAULT;
    m_moveInformation.boundaryDim = AnyRect::DEFAULT;
    m_moveInformation.appDim = AnyRect::DEFAULT;
}

void ApplicationMover::setProcessDetails(unsigned long processId,
                                         unsigned long posFlag,
                                         unsigned long alignFlag,
                                         const RECT& objectDim,
                                         const RECT& boundaryDim)
{
    m_moveInformation.processId = processId;
    m_moveInformation.posFlag = posFlag;
    m_moveInformation.alignFlag = alignFlag;
    m_moveInformation.objectDim = objectDim;
    m_moveInformation.boundaryDim = boundaryDim;
    m_moveInformation.appDim = AnyRect::DEFAULT;
    m_moveInformation.finishedRepositioning = false;
}

void ApplicationMover::run()
{
    FUNCTION_ENTRY("run");

    try
    {
        if ((m_moveInformation.processId == 0) && (m_moveInformation.posFlag == 0))
        {
            // Nothign has been configured so we have nothing to move.
            return;
        }

        int maxTries = 5;
        int numTries = 0;

        // If we haven't been terminated in the meantime we can now attempt to move the window
        while (!m_moveInformation.finishedRepositioning && !m_isTerminating && (numTries < maxTries))
        {
            LOG_DEBUG("Attempt %d to move application %d", numTries++, static_cast<int>(m_moveInformation.processId));
            ::EnumWindows(WndPositionEnumHandler, reinterpret_cast<long>(&m_moveInformation));
            // Give the window time to appear.
            Thread::sleep(100);
        }
    }
    catch (...)
    {
        // Catch all exceptions as there is nothing outside this method to catch them.
        LOG_EXCEPTION("Unknown", "run() of ApplicationMover threw an unknown exception");
    }

    FUNCTION_EXIT;
}

BOOL CALLBACK ApplicationMover::WndPositionEnumHandler(HWND hWnd, LPARAM lParam)
{
    WindowMoveInformation* info = reinterpret_cast<WindowMoveInformation*>(lParam);

    if (info == NULL)
    {
        LOG_DEBUG("info is NULL");
        return FALSE;
    }

    // find out which process & thread created this window
    DWORD dwThisWindowProcessID;
    DWORD dwThisWindowThreadID;
    dwThisWindowThreadID = ::GetWindowThreadProcessId(hWnd, &dwThisWindowProcessID);

    // if it's the process we are interested in then we want to move it
    if (info->processId == dwThisWindowProcessID)
    {
        // Create a temporary window
        CWnd tempWindow;
        tempWindow.Attach(hWnd);

        // Get some information about it to help determine if it is the correct one
        CWnd* parent = tempWindow.GetParent();
        BOOL isVisible = tempWindow.IsWindowVisible();

        if (parent != NULL || !isVisible)
        {
            LOG_DEBUG("parent != NULL || !isVisible");
            // This child is not visible or it is not the parent window
            // Therefore we are not interested.
            tempWindow.Detach();
            return TRUE;
        }

        RECT  windowRectWin;
        tempWindow.GetWindowRect(&windowRectWin);
        AnyRect windowRect = windowRectWin;
        bool isRepositioned = TA_Base_Bus::AppLauncher::calculateNewCoordinates(windowRect, AnyRect(info->objectDim), AnyRect(info->boundaryDim), info->posFlag, info->alignFlag);
        windowRectWin = windowRect;
        // if its coordinates are changed, then move to the new coordinates, otherwise, centre the
        // application without changing the size of the application.

        if (isRepositioned)
        {
            info->appDim = windowRect;
            tempWindow.MoveWindow(&windowRectWin);
            LOG_DEBUG("Moved to: (%li,%li,%li,%li)", windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);
        }
        else
        {
            tempWindow.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE);
            // we need to get this using the GetWindowRect method as the app is centred by the MFC library
            // and we don't know what the new coordinates are.
            tempWindow.GetWindowRect(&info->appDim);
            LOG_DEBUG("Either no positioning flag is defined or there's some problems with the arguments, centering application instead.");
        }

        // TD638 - Set the window to be the top most so it is given focus and then move it back again.
        tempWindow.SetForegroundWindow();
        tempWindow.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        tempWindow.SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

        // Now detach so that when this variable goes out of scope we don't kill the HWND.
        tempWindow.Detach();

        info->finishedRepositioning = true;

        LOG_DEBUG("Successfully moved application %d", static_cast<int>(info->processId));

        // We have found and repositioned the window so now we can return FALSE to indicate
        // it has been done.
        return FALSE;
    }

    return TRUE;
}

void ApplicationMover::terminate()
{
    FUNCTION_ENTRY("terminate");
    m_isTerminating = true;
    FUNCTION_EXIT;
}

void ApplicationMover::resetThread()
{
    FUNCTION_ENTRY("resetThread");
    m_isTerminating = false;
    FUNCTION_EXIT;
}
