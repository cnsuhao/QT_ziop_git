/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/GraphWorxManager.h $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class will control the instances of the GraphWorx threads and allow the session manager to
  * control GraphWorx.
  */
#pragma once
#include "Singleton.h"
#include "IGraphWorxManager.h"

class GraphWorxCommunicator;
typedef boost::shared_ptr<GraphWorxCommunicator> GraphWorxCommunicatorPtr;
class ScreenPositioning;

class GraphWorxManager : public IGraphWorxManager, public Singleton<GraphWorxManager>
{
public:

    /**
      * launchGraphWorx
      *
      * This launches three instances of GraphWorX on the left, centre and right monitor.
      *
      * @exception GraphWorxException This is thrown if GraphWorx cannot be loaded or a schematic
      *            cannot be displayed. If GraphWorX could not be run then the exception will have
      *            a type NOT_RUN in it and the caller needs to take action. If the type is
      *            FILE_OPEN_ERROR then GraphWorx was launched but errors occurred when trying to
      *            load a schematic.
      */
    void launchGraphWorx();

    /**
      * waitUntilReady
      *
      * This waits until all instances of Graphworx are ready before returning. This can be used by
      * the login dialog to ensure that GraphWorx isn't grabbing the screen focus before displaying the login
      * dialog
      */
    void waitUntilReady();

    /**
      * getCurrentDisplay
      *
      * Returns the name of the current display on the specified screen
      *
      * @param screenNumber The screen number
      *
      * @return the name of the current display on the specified screen
      */
    std::string getCurrentDisplay(unsigned int screenNumber);

    /**
      * loadComplete
      *
      * This indicates that the display has been completely loaded on the specified screen.
      *
      * @param leftPosition the left position of the display screen
      */
    void loadComplete(unsigned long leftPosition);

protected:

    GraphWorxManager();
    virtual ~GraphWorxManager() {}
    virtual void asyncInitialize() override;
    void setupSignalConnections();

    /**
      * calculateDisplayToLoad
      *
      * This method determines which GraphWorX instance is running on the screen with
      * the launch bar and retrieves the display it is currently showing.
      *
      * @return std::string - The name and path of the display for the new GraphWorx
      *                       instance to load.
      */
    std::string calculateDisplayToLoad();

    // The number of seconds to wait for the GraphWorX instances to come back ready.
    static const int SECONDS_TO_WAIT;

    // Each instance of graphworx that is to be launched and must continue to run
    std::vector<GraphWorxCommunicatorPtr> m_graphWorxCommunicators;
};
