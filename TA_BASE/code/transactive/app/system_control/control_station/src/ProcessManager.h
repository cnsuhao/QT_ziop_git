/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ProcessManager.h $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This receives callbacks from the ManagedProcesses and will also maintain the running
  * of applications. This implements the ProcessManager corba definition which means this
  * object is what the ManagedProcess's get when they resolve the initial process monitor.
  */

#pragma once
#include "Singleton.h"
#include "core/process_management/IDL/src/IProcessManagerCorbaDef.h"
#include "core/corba/src/ServantBase.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"

class ProcessManager
    : public virtual POA_TA_Base_Core::IProcessManagerCorbaDef,
      public virtual TA_Base_Core::ServantBase,
      public Singleton<ProcessManager>
{
public:

    void setupSignalConnections();
    void activate();
    void deactivate();

    /**
      * Constructor
      *
      * @exception DatabaseException A DatabaseException will be thrown if there is a
      *            problem establishing a connection with the database, or if an
      *            error is encountered while retrieving data.
      * @exception DataException A DataException will be thrown if the data cannot be
      *            converted to the required format (e.g. ACCESSLEVEL should be an
      *            unsigned long), or if the wrong ammount of data is retrieved.
      */
    ProcessManager();
    virtual ~ProcessManager() {}

public:

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
                                   bool isVisible = true);

    /**
      * giveApplicationFocus
      *
      * This will tell the specified application to change it's focus so it is the current
      * active window.
      *
      * @param unsigned long pid - This is the process id of the application to gain the focus.
      *
      */
    virtual void giveApplicationFocus(unsigned long pid);

    /**
      * terminating
      *
      * This is called by a ManagedProcess when it is terminating. This will remove the
      * application from the map of running applications.
      *
      * @param ProcessId - This indicates which application has terminated so it can be accurately
      *                    identified in the map of running applications.
      * @param EManagedProcessTerminateCode - This indicates how the application terminated so
      *                                       we can record why it terminated.
      *
      */
    virtual void terminating(TA_Base_Core::ProcessId p_processID,
                             TA_Base_Core::EManagedProcessTerminateCode p_code);

    /**
      * registerManagedProcess
      *
      * This is called by a ManagedProcess when it wishes to register. The method stored the
      * pointer to the ManagedProcess so it can perform calls on it. It also provides the
      * process id so we can link the managed process to an application that is already running.
      *
      * @param IManagedProcessCorbaDef_ptr - This is the ManagedProcess Corba object.
      * @param ProcessId - This indicates which application is registering so it can be accurately
      *                    identified in the map of running applications (or added if we do not
      *                    yet know about it).
      * @param unsigned long - This indicates the type of application that is registering. If we
      *                        do not yet know about this application we can ensure that we get
      *                        the correct details for it.
      * @param char* entity - This is the entity that the application is using for its configuration
      *                        information. This is used to determine a log file name to be passed
      *                        to the application.
      *
      * @exception IProcessManagerCorbaDef::ProcessAlreadyRunningException
      *            This indicates that there is already a process running of this type and there should not
      *            be another.
      *            IProcessManagerCorbaDef::FailedToRegisterException
      *            This indicates that an error occurred when trying to register the application. This
      *            means the calling application should terminate with an error.
      */
    virtual void registerManagedProcess(TA_Base_Core::IManagedProcessCorbaDef_ptr managedProcess,
                                        TA_Base_Core::ProcessId processId,
                                        CORBA::ULong applicationType,
                                        const char* entity);

    /**
      * registerManagedApplication
      *
      * This is called by a ManagedProcess when it wishes to register as a ManagedApplication.
      * This means that it is a GUI that wants special callbacks. It receives a
      * pointer to the ManagedApplication so it can perform calls on it. It also provides the
      * process id so we can link the managed application to an application that is already running.
      *
      * @param IManagedApplicationCorbaDef_ptr - This is the ManagedApplication Corba object.
      * @param ProcessId - This indicates which application is registering so it can be accurately
      *                    identified in the map of running applications
      *
      */
    virtual void registerManagedApplication(TA_Base_Core::IManagedApplicationCorbaDef_ptr managedApp,
                                            TA_Base_Core::ProcessId processId);

    /**
      * poll
      *
      * This is called to ensure that the Control Station is still alive. The function actually
      * does nothing.
      */
    virtual void poll();

    /**
      * statusChanged
      *
      * This is for agents only and does nothing in this scope.
      */
    virtual void statusChanged(TA_Base_Core::EProcessStatus status, TA_Base_Core::ProcessId processId)
    {
    }

    /**
      * getParams
      *
      * This is called by a ManagedProcess to retrieve the RunParams. This will return all the
      * standard runparams being passed to all applications as well as some application specific
      * ones. (ie DebugFile)
      *
      * @return RunParamSeq - The list of RunParams
      *
      * @param ProcessId - The process id of the application so we can ensure that the application
      *                    requesting the parameters is registered with us and return it
      *                    appropriate parameters.
      * @param char* - The host name. This should always be an empty string.
      */
    virtual TA_Base_Core::RunParamSeq* getParams(TA_Base_Core::ProcessId processId, const char* hostname);

private:

    // Hidden as they are not needed
    ProcessManager(const ProcessManager& theProcessManager);
    ProcessManager& operator=(const ProcessManager& pm);

    //
    // This is used to lock the use of the maps below.
    //
    TA_Base_Core::NonReEntrantThreadLockable m_classLock;
};
