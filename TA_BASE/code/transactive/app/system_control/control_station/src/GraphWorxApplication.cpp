/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/GraphWorxApplication.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class will communicate with GraphWorX. It will launch a single instance of GraphWorx which
  * will be terminated when the user logs out. This is different to the GraphWorXCommunicator in that
  * it does not ensure the GraphWorX instance continues to run and it immediately launches and
  * displays it.
  */

#include "StdAfx.h"
#include "GraphWorxApplication.h"
#include "IGwx32.h"
#include "core/exceptions/src/GraphWorxException.h"
#include "core/utilities/src/DebugUtil.h"

using TA_Base_Core::DebugUtil;
using TA_Base_Core::GraphWorxException;

GraphWorxApplication::GraphWorxApplication(IGraphWorxManager::GraphWorXInstance instance)
    : m_graphWorXWindow(instance)
{
    runGraphWorx();
}

GraphWorxApplication::~GraphWorxApplication()
{
    try
    {
        terminateGraphWorx();
    }
    catch (...)
    {
        // Catch all exceptions so we don't get errors filtering out
        LOG_EXCEPTION("Unknown", "Caught in destructor of GraphWorxApplication");
    }

    CoUninitialize();
}

void GraphWorxApplication::runGraphWorx()
{
    FUNCTION_ENTRY("runGraphWorx");

    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    try
    {
        // If the display is empty then there is no point continuing
        if (m_graphWorXWindow.display.empty())
        {
            TA_THROW(GraphWorxException("Display passed to be loaded is empty", GraphWorxException::INVALID_FILE));
        }

        m_graphWorXWindow.graphworx = new IGwx32;

        COleException oleException;

        if (!m_graphWorXWindow.graphworx->CreateDispatch("Gwx32.Display", &oleException))
        {
            char msg[256] = { 0 };
            oleException.GetErrorMessage(msg, 256);
            TA_THROW(GraphWorxException(str(boost::format("CreateDispatch failed with error: %s") % msg), GraphWorxException::NOT_RUN));
        }

        if (!m_graphWorXWindow.graphworx->FileNew())
        {
            TA_THROW(GraphWorxException("Failed to perform a FileNew on the GraphWorX instance", GraphWorxException::FILE_OPEN_ERROR));
        }

        if (!m_graphWorXWindow.graphworx->FileOpen(m_graphWorXWindow.display.c_str()))
        {
            TA_THROW(GraphWorxException("Failed to perform a FileOpen on the GraphWorX instance", GraphWorxException::FILE_OPEN_ERROR));
        }

        m_graphWorXWindow.graphworx->SetWindowDimensionsPixels(m_graphWorXWindow.left, m_graphWorXWindow.top, m_graphWorXWindow.width, m_graphWorXWindow.height);
        m_graphWorXWindow.graphworx->StartRuntime();
        m_graphWorXWindow.graphworx->ShowWindow();
    }
    catch (GraphWorxException&)
    {
        if (m_graphWorXWindow.graphworx != NULL)
        {
            delete m_graphWorXWindow.graphworx;
            m_graphWorXWindow.graphworx = NULL;
        }

        throw;
    }
    catch (...)
    {
        LOG_EXCEPTION("Unknown", "Thrown while GraphWorX was attempting to start up.");

        if (m_graphWorXWindow.graphworx != NULL)
        {
            delete m_graphWorXWindow.graphworx;
            m_graphWorXWindow.graphworx = NULL;
        }

        TA_THROW(GraphWorxException("Unknown exception caught. Pass this out as a known exception", GraphWorxException::NOT_RUN));
    }

    FUNCTION_EXIT;
}

void GraphWorxApplication::terminateGraphWorx()
{
    try
    {
        if (m_graphWorXWindow.graphworx != NULL)
        {
            m_graphWorXWindow.graphworx->HideWindow();
            m_graphWorXWindow.graphworx->StopRuntime();
            m_graphWorXWindow.graphworx->CloseWindow();
            delete m_graphWorXWindow.graphworx;
            m_graphWorXWindow.graphworx = NULL;
        }
    }
    catch (...)
    {
        LOG_EXCEPTION("Unknown", "Thrown while GraphWorX was attempting to terminate.");
    }
}
