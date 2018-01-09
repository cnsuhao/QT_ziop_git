/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/GraphWorxManager.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class will control the instances of the GraphWorx threads and allow the session manager to
  * control GraphWorx.
  */
#include "StdAfx.h"
#include "GraphWorxManager.h"
#include "ScreenPositioning.h"
#include "DataCache.h"
#include "Signals.h"
#include "SessionSignal.h"
#include "GraphWorxCommunicator.h"
#include "core/exceptions/src/GraphWorxException.h"
#include "core/utilities/src/DebugUtil.h"

using namespace TA_Base_Core;

const int GraphWorxManager::SECONDS_TO_WAIT = 30;

GraphWorxManager::GraphWorxManager()
{
    setupSignalConnections();
}

void GraphWorxManager::setupSignalConnections()
{
    //TODO
}

void GraphWorxManager::asyncInitialize()
{
    // Now retrieve how many GraphWorx instances and their positions.
    // Iterate through and store the details for each GraphWorx instance. As well as this start each
    // instance as we go

    TA_Base_Core::ControlStationPtr controlStation = DataCache::getInstance().getControlStationEntity();
    std::string display = controlStation->getStartupDisplay();
    std::string background = controlStation->getBackgroundDisplay();

    for (const ScreenPositioning::WindowPosition& pos : ScreenPositioning::getInstance().getGraphworxPositions())
    {
        GraphWorXInstance gw = { NULL, pos.topLeft.first, pos.topLeft.second, pos.width, pos.height, background, display };
        m_graphWorxCommunicators.push_back(GraphWorxCommunicatorPtr(new GraphWorxCommunicator(gw)));
    }
}

void GraphWorxManager::waitUntilReady()
{
    //TODO
}

std::string GraphWorxManager::calculateDisplayToLoad()
{
    unsigned long screenWidth = ScreenPositioning::getInstance().getEachScreenWidth();

    for (GraphWorxCommunicatorPtr communictor : m_graphWorxCommunicators)
    {
        if (communictor->isGraphWorxInstanceBesideLaunchBar(screenWidth))
        {
            return communictor->getDisplay();
        }
    }

    // If we get to here then we didn't find a matching GraphWorX instance.
    // Throw an exception
    TA_THROW(TA_Base_Core::GraphWorxException("Could not find a GraphWorx instance running on the same screen as the launch bar.", TA_Base_Core::GraphWorxException::NOT_RUN));
}

std::string GraphWorxManager::getCurrentDisplay(unsigned int screenNumber)
{
    if ((screenNumber < 1) || (m_graphWorxCommunicators.size() < screenNumber))
    {
        return "";  // otherwise, not displaying any graphworx on the specified screen
    }

    // TD8433: Get the child display (the popup) instead of the initial display loaded in the communicator.
    return m_graphWorxCommunicators[screenNumber - 1]->getChildDisplay();
}

void GraphWorxManager::loadComplete(unsigned long leftPosition)
{
    for (GraphWorxCommunicatorPtr communictor : m_graphWorxCommunicators)
    {
        if (communictor->loadComplete(leftPosition))
        {
            return;
        }
    }
}
