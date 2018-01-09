/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/IGraphWorxManager.h $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This interface advertises the calls the AutoProxy will need to callback.
  */

#pragma once
#include <map>
#include <vector>
#include <string>

class IGwx32;

class IGraphWorxManager
{
public:

    struct GraphWorXInstance
    {
        IGwx32* graphworx;      // The instance of graphworx display
        unsigned long left;     // The left position of the display
        unsigned long top;      // The top position of the display
        unsigned long width;    // The width of the display
        unsigned long height;   // The height of the display
        std::string background; // The display that will be launched before the startup display if specified.  Displays that are launched after this will possess the frame properties of this background display
        std::string display;    // The startup display
    };

    /**
     * Destructor
     */
    virtual ~IGraphWorxManager() { };

    /**
      * getCurrentDisplay
      *
      * Returns the name of the current display on the specified screen
      *
      * @param screenNumber The screen number
      *
      * @return the name of the current display on the specified screen
      */
    virtual std::string getCurrentDisplay(unsigned int screenNumber) = 0;

    /**
      * loadComplete
      *
      * This indicates that the display has been completely loaded on the specified screen.
      *
      * @param leftPosition the left position of the display screen
      */
    virtual void loadComplete(unsigned long leftPosition) = 0;

    /**
      * waitUntilReady
      *
      * This waits until all instances of Graphworx are ready before returning. This can be used by
      * the login dialog to ensure that GraphWorx isn't grabbing the screen focus before displaying the login
      * dialog
      */
    virtual void waitUntilReady() = 0;
};
