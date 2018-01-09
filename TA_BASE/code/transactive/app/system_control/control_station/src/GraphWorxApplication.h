/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/GraphWorxApplication.h $
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

#pragma once
#include "IGraphWorxManager.h"

class GraphWorxApplication
{
public:

    GraphWorxApplication(IGraphWorxManager::GraphWorXInstance instance);
    virtual ~GraphWorxApplication();

private:

    // Made private as these are not needed.
    GraphWorxApplication(const GraphWorxApplication& theGraphWorxApplication) = delete;
    GraphWorxApplication& operator=(const GraphWorxApplication& rhs) = delete;

    /**
      * runGraphWorx
      *
      * This will actually kick off an instance of GraphWorX, load the display and position it.
      */
    void runGraphWorx();

    /**
      * terminateGraphWorx
      *
      * This will nicely terminate the instance of GraphWorX.
      */
    void terminateGraphWorx();

    // This holds each GraphWorX window, configuration information for it and whether
    // it is running or not.
    IGraphWorxManager::GraphWorXInstance m_graphWorXWindow;
};
