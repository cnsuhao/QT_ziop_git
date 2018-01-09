/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/GraphWorxCommunicator.h $
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

#pragma once
#include "IGraphWorxManager.h"

class IGwx32;
class GraphWorxManager;

class GraphWorxCommunicator
{
public:

    GraphWorxCommunicator(IGraphWorxManager::GraphWorXInstance& instance);
    virtual ~GraphWorxCommunicator();

    void startGraphWorx();
    void stopGraphWorx();
    void startAndMonitorGraphWorx();
    boost::thread m_thread;

    /**
      * isGraphWorxInstanceBesideLaunchBar
      *
      * This will determine if this GraphWorx instance is running beside the launch bar.
      * This is done by checking to see if it is the full width of the screen. If it
      * is not then it is next to the launch bar.
      *
      * @return bool - True if this instance is running on the same screen as GraphWorx.
      */
    bool isGraphWorxInstanceBesideLaunchBar(unsigned long screenWidth);

    /**
      * getDisplay
      *
      * This will return the name and path of the display this GraphWorx instance
      * currently has loaded.
      */
    const std::string& getDisplay();

    /**
      * getChildDisplay
      *
      * This will return the name and path of the popup display this GraphWorx instance
      * currently has loaded.
      */
    const std::string& getChildDisplay();

    /**
      * loadComplete
      *
      * This indicates that the display has been completely loaded
      *
      * @param leftPosition the left position of the display screen
      *
      * @return true if this graphworx screen's left position is as specified
      */
    bool loadComplete(unsigned long leftPosition);

    /**
      * printDisplay
      *
      * Prints the graphworx display held by this instance of graphworx.
      */
    void printDisplay();

protected:

    /**
      * runGraphWorx
      *
      * This will actually kick off an instance of GraphWorX, load the display and position it.
      */
    void runGraphWorx();

    /**
      * loadBaseDisplay
      *
      * This will create the GraphWorX instance and open the base display
      *
      * @exception TransactiveException Will throw this when there is a problem
      *            creating a GraphWorX instance or when any file error occured.
      */
    void loadBaseDisplay(const std::string& baseDisplay);

    /**
      * terminateGraphWorx
      *
      * This will nicely terminate the instance of GraphWorX.
      */
    void terminateGraphWorx();

    void setupSignalConnections();

    static std::string getErrorMessage(COleException* ex);

    // This holds each GraphWorX window, configuration information for it and whether
    // it is running or not.
    IGraphWorxManager::GraphWorXInstance m_graphWorXWindow;

    // This is the path and name of the child popup display of the GraphWorX window.
    std::string m_currentChildDisplay;

    // Indicates when the thread is terminated
    volatile bool m_isRunning;

    // This indicates if GraphWorx loaded correctly
    volatile bool m_isValidLoad;
};

typedef boost::shared_ptr<GraphWorxCommunicator> GraphWorxCommunicatorPtr;
