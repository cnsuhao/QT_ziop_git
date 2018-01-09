/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/IProcessManager.h $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This interface advertises the calls the AutoProxy will need to callback.
  */

#if !defined(IProcessManager_36889BB4_BAF3_4bc1_B189_C5757C976786__INCLUDED_)
#define IProcessManager_36889BB4_BAF3_4bc1_B189_C5757C976786__INCLUDED_

#include "app\system_control\control_station\src\ControlStationConstants.h"
#include <map>
#include <vector>
#include <string>
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"

// Forward declarations
class RunningApplication;
class Application;

class IProcessManager
{
public:
    struct ApplicationProperties
    {
        unsigned long key;
        std::string name;
        bool isAccessible;
    };

    typedef std::map<unsigned long, Application*> ApplicationMap;
    typedef std::map<TA_Base_Core::ProcessId, RunningApplication*> RunningApplicationMap;

    static IProcessManager& getInstance();

    /**
     * Destructor
     */
    virtual ~IProcessManager() { };


    /**
      * launchApplication
      *
      * This will launch an application in a specified position. Any additional arguments
      * to provide to the application can be specified.
      *
      * @param appType - The application to launch
      * @param string additionalCommands - This is a string containing any additional commands to
      *                                    be passed to the application. This should take the form
      *                                    of a command line you would pass to the application if
      *                                    you ran it from the command line. The default is no
      *                                    parameters.
      * @param posFlag - How the application is going to be positioned relative to boundary paramter
      * @param alignFlag - How the application is going to be positioned relative to point parameter
      * @param objectDim - The dimension of the object that this application will be launched relative
      *                    to (if POS_RELATIVE or POS_NOOVERLAP is specified).
      * @param boundaryDim - The dimension of the object that this application will be bounded in
      *                      (if POS_BOUNDED is specified)
      * @param bool isVisible - This indicates if the running application should be displayed
      *                         to the user as running. For example a banner should not be
      *                         displayed as a running application that the user can switch to.
      *                         Therefore for this 'false' should be passed in. The default is
      *                         'true' as most applications will want to be seen.
      *
      * @exception ApplicationException - This indicates that the application could not be launched.
      */
    virtual void launchApplication(unsigned long appType,
                                   const std::string& additionalCommands,
                                   unsigned long posFlag,
                                   unsigned long alignFlag,
                                   const RECT& objectDim,
                                   const RECT& boundaryDim,
                                   bool isVisible = true) = 0;


    /**
      * giveApplicationFocus
      *
      * This will tell the specified application to change it's focus so it is the current
      * active window.
      *
      * @param unsigned long pid - This is the process id of the application to gain the focus.
      *
      */
    virtual void giveApplicationFocus(unsigned long pid) = 0;
};

#endif // !defined(IProcessManager_36889BB4_BAF3_4bc1_B189_C5757C976786__INCLUDED_)
