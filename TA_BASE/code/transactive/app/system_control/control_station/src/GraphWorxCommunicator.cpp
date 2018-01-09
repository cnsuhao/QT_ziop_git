/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/GraphWorxCommunicator.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class will communicate with GraphWorX. It will launch a GraphWorX instance and
  * continue to send it messages through its lifetime.
  * This is different to the GraphWorXApplication in that it ensures the GraphWorX instance
  * is always running and only hides it when the user logs out.
  */

#include "StdAfx.h"
#include "GraphWorxCommunicator.h"
#include "IGwx32.h"
#include "SessionSignal.h"
#include "ControlStationConstants.h"
#include "ControlStationRootDialog.h"
#include "core/exceptions/src/GraphWorxException.h"
#include "core/utilities/src/DebugUtil.h"
#include <objbase.h>  // For CoInitializeEx
#include <tlhelp32.h> // For process control

using namespace TA_Base_Core;

GraphWorxCommunicator::GraphWorxCommunicator(IGraphWorxManager::GraphWorXInstance& instance)
    : m_graphWorXWindow(instance),
      m_currentChildDisplay(""),
      m_isRunning(false),
      m_isValidLoad(true)
{
    setupSignalConnections();
}

GraphWorxCommunicator::~GraphWorxCommunicator()
{
    stopGraphWorx();
}

void GraphWorxCommunicator::setupSignalConnections()
{
    SessionSignal::logined.connect(boost::bind(&GraphWorxCommunicator::startGraphWorx, this));
    SessionSignal::logouted.connect(boost::bind(&GraphWorxCommunicator::stopGraphWorx, this));
}

void GraphWorxCommunicator::startGraphWorx()
{
    if (!m_isRunning)
    {
        m_isRunning = true;
        m_thread = boost::thread(&GraphWorxCommunicator::startAndMonitorGraphWorx, this);
    }
}

void GraphWorxCommunicator::stopGraphWorx()
{
    if (m_isRunning)
    {
        m_isRunning = false;
        m_thread.join();
    }
}

void GraphWorxCommunicator::startAndMonitorGraphWorx()
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    runGraphWorx();

    while (m_isRunning && m_isValidLoad)
    {
        try
        {
            m_graphWorXWindow.graphworx->GetFileName();
        }
        catch (COleException* ex)
        {
            LOG_ERROR("Failed to connect GraphWorx, Have to open a new one. error: %s", getErrorMessage(ex).c_str());
            terminateGraphWorx();
            runGraphWorx();
        }

        boost::this_thread::sleep_for(boost::chrono::seconds(1));
    }

    terminateGraphWorx();
    CoUninitialize();
}

void GraphWorxCommunicator::runGraphWorx()
{
    m_isValidLoad = false;

    try
    {
        // If the display is empty then there is no point continuing
        if (m_graphWorXWindow.display.empty())
        {
            TA_THROW(TransactiveException("Startup display passed to be loaded is empty"));
        }

        // If the background display is not specified, then we'll just launch the startup
        // display as it is, otherwise, we have to open the background display first then
        // the startup display (fix for issue #1787).  By doing so the startup display will
        // be displayed with the windows properties set in the background display - so if
        // the background display is set with no title bar and no system menu, the startup
        // display will be also be opened with no title bar and no system menu even if the
        // gdf itself is set with both title bar and system menu.  When the startup display
        // is copied across, it will be displayed with its own properties - with title bar
        // and system menu, so user will be able to close it.
        if (m_graphWorXWindow.background.empty())
        {
            LOG_INFO("Background display is empty, so we're just going to display the startup display, %s", m_graphWorXWindow.display.c_str());
            loadBaseDisplay(m_graphWorXWindow.display);
        }
        else
        {
            LOG_INFO("Background display is %s. Going to open it now", m_graphWorXWindow.background.c_str());
            loadBaseDisplay(m_graphWorXWindow.background);

            // We open up the startup display now
            if (m_graphWorXWindow.graphworx->IsRuntimeMode())
            {
                DisplayInformation* info = new DisplayInformation { m_graphWorXWindow.display.c_str(), "", m_graphWorXWindow.left };
                ControlStationRootDialog::getInstance().postMessage(WM_LAUNCH_DISPLAY, (WPARAM)info);
            }
            else
            {
                // If the background display is somehow not running, then we'll have to just open up
                // the display as it is.
                LOG_INFO("Opening the startup display now", m_graphWorXWindow.display.c_str());

                if (!m_graphWorXWindow.graphworx->FileOpen(m_graphWorXWindow.display.c_str()))
                {
                    TA_THROW(TransactiveException("Failed to perform a FileOpen on the GraphWorX instance"));
                }

                LOG_INFO("Setting the dimension of display to left = %d, top = %d, width = %d, height = %d", m_graphWorXWindow.left, m_graphWorXWindow.top, m_graphWorXWindow.width, m_graphWorXWindow.height);
                m_graphWorXWindow.graphworx->SetWindowDimensionsPixels(m_graphWorXWindow.left, m_graphWorXWindow.top, m_graphWorXWindow.width, m_graphWorXWindow.height);
                m_graphWorXWindow.graphworx->StartRuntime();
            }
        }

        m_graphWorXWindow.graphworx->ShowWindow();
        m_isValidLoad = true;
        LOG_INFO("Successfully started up graphworx");
        return;
    }
    catch (TransactiveException&)
    {
        // Catch because we don't want this to propagate. It has served it's purpose as we have skipped all
        // functionality we didn't want to execute (after an error occurred). So now we can continue the thread.
        LOG_EXCEPTION("TransactiveException", "GraphworX Load Failed");
    }
    catch (COleException* ex)
    {
        LOG_ERROR("Thrown while GraphWorX was attempting to start up: %s", getErrorMessage(ex).c_str());
    }

    delete m_graphWorXWindow.graphworx;
    m_graphWorXWindow.graphworx = NULL;
    m_isValidLoad = false;
}

void GraphWorxCommunicator::loadBaseDisplay(const std::string& baseDisplay)
{
    // set the valid load to false first since we haven't successfully loaded it yet
    m_graphWorXWindow.graphworx = new IGwx32;
    COleException oleException;

    if (!m_graphWorXWindow.graphworx->CreateDispatch("Gwx32.Display", &oleException))
    {
        TA_THROW(TransactiveException(str(boost::format("CreateDispatch failed with error: %s") % getErrorMessage(&oleException))));
    }

    if (!m_graphWorXWindow.graphworx->FileNew())
    {
        TA_THROW(TransactiveException("Failed to perform a FileNew on the GraphWorX instance"));
    }

    // We open up the hidden display first
    if (!m_graphWorXWindow.graphworx->FileOpen(baseDisplay.c_str()))
    {
        TA_THROW(TransactiveException("Failed to perform a FileOpen on the GraphWorX instance"));
    }

    try
    {
        IGraphWorxManager::GraphWorXInstance& gw = m_graphWorXWindow;
        LOG_INFO("Setting the dimension of display to left=%d, top=%d, width=%d, height=%d", gw.left, gw.top, gw.width, gw.height);
        gw.graphworx->SetWindowDimensionsPixels(gw.left, gw.top, gw.width, gw.height);
        gw.graphworx->StartRuntime();
    }
    catch (COleException* ex)
    {
        LOG_ERROR("Caught some exception when SetWindowDimensionsPixels/StartRuntime the display: %s", getErrorMessage(ex).c_str());
    }
}

void GraphWorxCommunicator::printDisplay()
{
    FUNCTION_ENTRY("printDisplay");
    // To print the display, first need to get the correct part of the display...
    unsigned long popupCount = m_graphWorXWindow.graphworx->GetOpenPopupCount();

    // Only useful if the graphworx instance is actually showing something (i.e. has more than one popup)
    if (0 < popupCount)
    {
        // Will always be interested only in the first popup
        IGwx32 displayToPrint(m_graphWorXWindow.graphworx->GetOpenPopupByIndex(0));

        // Set the background to white temporarily for the print
        unsigned long backgroundColor = displayToPrint.GetBackgroundColor();
        displayToPrint.SetBackgroundColor(RGB(255, 255, 255));

        // Tell the graphworx window to print
        LOG_INFO("Requesting print-preview dialog from GraphWorX");
        //postMessage((HWND)displayToPrint.GetFrameWindowHandle(), WM_COMMAND, /*57609*/ID_FILE_PRINT_PREVIEW, 0);

        // And finally reset the background color
        displayToPrint.SetBackgroundColor(backgroundColor);
    }

    FUNCTION_EXIT;
}

void GraphWorxCommunicator::terminateGraphWorx()
{
    if (m_graphWorXWindow.graphworx)
    {
        try
        {
            m_graphWorXWindow.graphworx->StopRuntime();
            m_graphWorXWindow.graphworx->ExitApplication(); //TODO: timeout
        }
        catch (COleException* ex)
        {
            LOG_ERROR("Exception caught while trying to close GraphWorx: %s", getErrorMessage(ex).c_str());
        }

        delete m_graphWorXWindow.graphworx;
        m_graphWorXWindow.graphworx = NULL;
    }
}

bool GraphWorxCommunicator::isGraphWorxInstanceBesideLaunchBar(unsigned long screenWidth)
{
    // If the width of the GraphWorx instance is less than that of the screen then
    // it is running next to the launch bar. We use a buffer of 10 pixels to make
    // sure the size is definitely smaller and not just one or two pixels out.
    return (m_graphWorXWindow.width < (screenWidth - 10));
}

const std::string& GraphWorxCommunicator::getDisplay()
{
    return m_graphWorXWindow.display;
}

const std::string& GraphWorxCommunicator::getChildDisplay()
{
    return m_currentChildDisplay;
}

bool GraphWorxCommunicator::loadComplete(unsigned long leftPosition)
{
    bool complete = (m_graphWorXWindow.left <= leftPosition) &&
                    (leftPosition <= (m_graphWorXWindow.left + m_graphWorXWindow.width));

    if (complete)
    {
        LOG_INFO("Completed loading GraphWorX display at position %d", leftPosition);
    }

    return complete;
}

std::string GraphWorxCommunicator::getErrorMessage(COleException* ex)
{
    char msg[1024];
    ex->GetErrorMessage(msg, 1024);
    return msg;
}
