/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/GenericGUI.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This is the central class for GUI applications. It listens for several callbacks
  * and passes relevant messages onto the GUI application. This class also kicks off
  * ManagedProcess.
  */
#if defined(_MSC_VER)
    #pragma warning(disable:4786)
#endif // defined (_MSC_VER)

#include "core/data_access_interface/entity_access/src/NotificationAgentData.h"
#include "core/data_access_interface/entity_access/src/DefaultEntity.h"

//#include "bus\TA_MessageResource\resource.h"
#include "bus\generic_gui_entity\src\GenericGUI.h"

#include "bus\generic_gui_pmod\src\PModelInterface.h"
#include "bus\response_plans\event_triggered\src\EventTriggeredPlanProcessor.h"
#include "bus\security\access_control\actions\src\AccessControlledActions.h"
#include "bus\security\rights_library\src\RightsLibrary.h"

#include "core/process_management/IDL/src/IControlStationCorbaDef.h"
#include "core/alarm/src/AlarmHelperManager.h"
#include "core/alarm/src/AlarmPublicationManager.h"
#include "core/alarm/src/AlarmSubscriptionManager.h"
#include "core\configuration_updates\src\OnlineUpdateListener.h"
#include "core\data_access_interface\entity_access\src\EntityAccessFactory.h"
#include "core\data_access_interface\entity_access\src\IEntityData.h"
#include "core/data_access_interface/src/IResource.h"
#include "core\exceptions\src\GenericGUIException.h"
#include "core\exceptions\src\DatabaseException.h"
#include "core\exceptions\src\DataException.h"
#include "core/exceptions/src/ApplicationException.h"

#include "core\exceptions\src\EntityTypeException.h"
#include "core\exceptions\src\UtilitiesException.h"
#include "core\exceptions\src\InvalidCommandLineException.h"
#include "core\exceptions\src\ProcessAlreadyRunningException.h"
#include "core\exceptions\src\ManagedProcessException.h"
#include "core\exceptions\src\InvalidCommandLineException.h"
#include "core\exceptions\src\RightsException.h"
#include "core\process_management\src\UtilityInitialiser.h"
#include "core\process_management\src\ManagedProcess.h"
#include "core\naming\src\Naming.h"
#include "core\message\src\TransactiveMessageSubscription.h"
#include "core\message\src\MessagePublicationManager.h"
#include "core\message\types\ConfigUpdate_MessageTypes.h"
#include "core\utilities\src\TAAssert.h"

using TA_Base_Core::GenericGUIException;
using TA_Base_Core::TransactiveException;
using TA_Base_Core::DatabaseException;
using TA_Base_Core::DataException;
using TA_Base_Core::ManagedProcessException;
using TA_Base_Core::InvalidCommandLineException;
using TA_Base_Core::EntityTypeException;
using TA_Base_Core::UtilitiesException;
using TA_Base_Core::ProcessAlreadyRunningException;
using TA_Base_Core::RightsException;

using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using TA_Base_Core::CorbaUtil;
using TA_Base_Core::ConfigUpdateMessageCorbaDef;
using TA_Base_Core::EntityAccessFactory;
using TA_Base_Core::ConfigUpdateDetails;
using TA_Base_Core::OnlineUpdateListener;
using TA_Base_Core::IEntityData;
using TA_Base_Bus::RightsLibrary;

namespace TA_Base_Bus
{
#if 0    //TD18355 bugfix, transferred to ConfigurationEditor.cpp
    // TD15529
    const std::string GenericGUI::CONFIGURATIONEDITOR_APPNAME = "ConfigurationEditor" ;
    const std::string GenericGUI::CONFIGCONNECTIONFILE_NAME = "ConfigConnectionStrings.csv";
#endif

    // TimerCache is used internally by GenericGUI to diagnose
    // startup delays in applications.
    class TimerCache
    {
    public:
        void start(const std::string& name);
        void stop();
        void log(const std::string& context) const;

    private:
        clock_t m_start;
        clock_t m_stop;

        typedef std::list<std::string> NameList;
        typedef std::list<double> ElapsedTimeList;
        NameList m_name;
        ElapsedTimeList m_elapsedTime;
    };

    void TimerCache::start(const std::string& name)
    {
        m_name.push_back(name);
        m_start = ::clock();
    }

    void TimerCache::stop()
    {
        m_stop = ::clock();
        m_elapsedTime.push_back(static_cast<double>((m_stop - m_start) / CLOCKS_PER_SEC));
    }

    void TimerCache::log(const std::string& context) const
    {
        NameList::const_iterator nameIt;
        ElapsedTimeList::const_iterator timeIt;

        LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug, "TimerCache for %s", context.c_str());

        for (nameIt = m_name.begin(), timeIt = m_elapsedTime.begin(); nameIt != m_name.end(); nameIt++, timeIt++)
        {
            LOGMORE(SourceInfo, "%s: %.1f", nameIt->c_str(), *timeIt);
        }
    }


    GenericGUI::GenericGUI(IApplicationController& guiApp, const std::string& commandLine, bool mustConnectControlStation)
        : m_guiAppController(guiApp),
          m_managedProcess(NULL),
          m_guiEntity(NULL),
          m_terminateCode(TA_Base_Core::UserExit),
          m_hasConnectedControlStation(false),
          m_mustConnectControlStation(mustConnectControlStation)
    {
        // Note: This log will not appear in the log file as it has not been set yet.
        LOG(SourceInfo, DebugUtil::FunctionEntry, "Constructor");

        if (commandLine.empty())
        {
            TA_THROW(GenericGUIException(GenericGUIException::COMMAND_LINE_INVALID));
        }

        try
        {
            LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "GenericGUI register application to process manager");
            setUpManagedProcess(commandLine);
            activateServant();
        }
        catch (const ProcessAlreadyRunningException&)
        {
            m_terminateCode = TA_Base_Core::RequestedTerminate;

            // In case the servant was activated we need to deactivate it
            if (m_managedProcess != NULL)
            {
                deactivateServant();
            }

            shutdown();
            throw;
        }
        catch (...)
        {
            // If any error has occurred we need to set the termination code and tell
            // ManagedProcess we are terminating before rethrowing the exception
            m_terminateCode = TA_Base_Core::InitError;

            // In case the servant was activated we need to deactivate it
            if (m_managedProcess != NULL)
            {
                deactivateServant();
            }

            shutdown();
            throw;
        }

        LOG(SourceInfo, DebugUtil::FunctionExit, "Constructor");
    }


    bool GenericGUI::init()
    {
        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "GenericGUI check command line");
        checkCommandLine();

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "GenericGUI load entity data");
        loadEntityData();

        if (m_guiEntity == NULL)  // TD1141
        {
            TA_THROW(GenericGUIException(GenericGUIException::INITIALISATION_ERRORS));
        }

        // TD8684: Ensure plans are triggered from events.
        if (! TA_Base_Core::RunParams::getInstance().isSet("LocalTest"))
        {
            EventTriggeredPlanProcessor::registerWithProcessingLibrary();
        }

        return true;
    }

    GenericGUI::~GenericGUI()
    {
        FUNCTION_ENTRY("~GenericGUI");

        try
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug, "deleting m_managedProcess");

            if (m_managedProcess != NULL)
            {
                // In case shutdown() wasn't called then we'll deactivate and delete
                // again here.
                m_managedProcess->deactivateAndDeleteServant();
            }

            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug, "deleting m_guiEntity");
            delete m_guiEntity;
            m_guiEntity = NULL;
            // Not our responsibility to delete the GUI Application
        }
        catch (...)
        {
            // Catch all exceptions so we don't get errors filtering out
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown", "Caught in destructor of GenericGUI");
        }

        FUNCTION_EXIT;
    }

    /////////////////////////////////////////////////////
    // Private Helper Methods
    /////////////////////////////////////////////////////

    bool GenericGUI::canConnectControlStation()
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "canConnectControlStation");

        try
        {
            // This should have been set by ManagedProcess.
            std::string ior = RunParams::getInstance().get("ProcessManagerIor");

            if (ior.empty())
            {
                LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Could not resolve the Control Station to launch as the ProcessManagerIor in RunParams was empty");
                return false;
            }

            // Change (eg) corbaloc::localhost:6666/ProcessManager
            // to          corbaloc::localhost:6666/ControlStation
            // This will give us the part of the Control Station that allows
            // us to launch applications.
            int position = ior.find("ProcessManager");
            ior = ior.substr(0, position);
            ior += "ControlStation";

            CORBA::Object_var obj = TA_Base_Core::CorbaUtil::getInstance().stringToObject(ior);

            if (!CORBA::is_nil(obj))
            {
                TA_Base_Core::IControlStationCorbaDef_var controlStationObj = TA_Base_Core::IControlStationCorbaDef::_narrow(obj);

                if (CORBA::is_nil(controlStationObj))
                {
                    LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Could not narrow the retrieved object to the Control Station. %s", ior.c_str());
                    return false;
                }
            }
            else
            {
                LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "The retrieved CORBA object is nil. %s", ior.c_str());
                return false;
            }
        }
        catch (const CORBA::Exception& ex)
        {
            // We are not expecting to catch anything other than ApplicationExceptions
            // If any unexpected exceptions are raised (eg CORBA::Exception), we re-throw
            // them as ApplicationExceptions.
            LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch,
                TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex).c_str(),
                "Caught a CORBA exception, could not contact ControlStation");
            return false;
        }

        LOG(SourceInfo, DebugUtil::FunctionExit, "canConnectControlStation");
        return true;
    }

    void GenericGUI::setUpManagedProcess(const std::string& commandLine)
    {
        // Note: This log will not appear in the log file as it has not been set yet.
        LOG(SourceInfo, DebugUtil::FunctionEntry, "setUpManagedProcess");

        TimerCache timer;
        timer.start("setUpManagedProcess(begin)->initialiseUtilities");

        try
        {
            // First initialise all utilities ready for managed process.
            // Among other things, the log file is set in here If this is successful then
            // all future LOG statements will be output to a file (if configured to do so).
            timer.stop();
            timer.start("initialiseUtilities->registerManagedApplication");

            LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "GenericGUI initialiseUtilities");

            TA_Base_Core::UtilityInitialiser::initialiseUtilities(commandLine);

            // If the version information is requested, open the about box.
            if (RunParams::getInstance().isSet(RPARAM_VERSION))
            {
                m_guiAppController.getWinApp()->displayAboutBox();

                LOG(SourceInfo, DebugUtil::ExceptionCatch, "InvalidCommandLineException", "Will change this exception into a ProcessAlreadyRunningException shut down quietly after version info");
                TA_THROW(ProcessAlreadyRunningException("Version info only"));
            }

            bool connectedProcessMgr = false;

            try
            {
                m_managedProcess = new TA_Base_Core::ManagedProcess(this);
                connectedProcessMgr = true;
            }
            catch (...)
            {
                TA_ASSERT(m_managedProcess == NULL, "Managed process initialize failed");
                LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Create managed process failed, m_mustConnectControlStation=%d", m_mustConnectControlStation);

                if (m_mustConnectControlStation)
                {
                    throw;
                }
            }

            // Register GenericGUI as a ManagedApplication since it implements the IManagedGUICorbaDef
            // interface.
            timer.stop();
            timer.start("registerManagedApplication->setUpManagedProcess(end)");

            if (connectedProcessMgr)
            {
                TA_ASSERT(m_managedProcess != NULL, "Managed process has been initialized");

                if (! TA_Base_Core::RunParams::getInstance().isSet("LocalTest"))
                {
                    m_managedProcess->registerManagedApplication(this->_this());
                }

                m_hasConnectedControlStation = canConnectControlStation();
            }
        }
        catch (const InvalidCommandLineException&)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "InvalidCommandLineException", "Will change this exception into a GenericGUIException.");
            TA_THROW(GenericGUIException(GenericGUIException::COMMAND_LINE_INVALID));
        }
        catch (const ManagedProcessException&)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "ManagedProcessException", "Will change this exception into a GenericGUIException.");
            TA_THROW(GenericGUIException(GenericGUIException::COMMUNICATION_ERROR_WITH_CONTROL_STATION));
        }
        catch (const UtilitiesException&)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "UtilitiesException", "Will change this exception into a GenericGUIException.");
            TA_THROW(GenericGUIException(GenericGUIException::INITIALISATION_ERRORS));
        }
        catch (const ProcessAlreadyRunningException&)
        {
            // This indicates the application is already running so we must quietly terminate
            throw;
        }
        catch (const GenericGUIException&)
        {
            // Catch and rethrow so this doesn't get caught in the catch block below.
            throw;
        }
        catch (...)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown", "This exception has been thrown because CorbaUtil initialise or activate did not work. Will change this exception into a GenericGUIException.");
            TA_THROW(GenericGUIException(GenericGUIException::INITIALISATION_ERRORS));
        }

        timer.stop();
        timer.log("setUpManagedProcess");
        LOG(SourceInfo, DebugUtil::FunctionExit, "setUpManagedProcess");
    }


    //void GenericGUI::setUpApplicationType()
    //{
    //    LOG( SourceInfo, DebugUtil::FunctionEntry, "setUpApplicationType" );

    //    //
    //    // Retrieve the application type and set it in RunParams.
    //    //
    //    std::ostringstream appType;
    //    appType << m_guiAppController.getApplicationType();
    //    TA_Base_Core::RunParams::getInstance().set( RPARAM_APPTYPE, appType.str().c_str() );
    //    LOG( SourceInfo, DebugUtil::FunctionExit, "setUpApplicationType" );
    //}


    void GenericGUI::checkCommandLine()
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "checkCommandLine");

        //
        // Check the command line passed was correct. We will check entity separately so
        // this is just for any extra params. This may throw an exception. This should be
        // thrown straight up the the calling class as we can't deal with it.
        //
        m_guiAppController.checkCommandLine();

        LOG(SourceInfo, DebugUtil::FunctionExit, "checkCommandLine");
    }


    void GenericGUI::loadEntityData()
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "loadEntityData");

        //
        // Retrieve the entity name from RunParams to ensure it was on the command line.
        //
        std::string entityName = TA_Base_Core::RunParams::getInstance().get(RPARAM_ENTITYNAME);

        if (entityName.empty())
        {
            TA_THROW(GenericGUIException("No entity specified on command line (ie --entity-name not found)", GenericGUIException::COMMAND_LINE_INVALID));
        }

        //
        // Load the entity and check the configuration is correct
        //
        try
        {
            if (TA_Base_Core::RunParams::getInstance().isSet("LocalTest"))
            {
                fillMyAppEntityData();
            }
            else
            {
                m_guiEntity = EntityAccessFactory::getInstance().getEntity(entityName);
            }

            TA_ASSERT(m_guiEntity != NULL, "EntityAccessFactory returned a NULL entity and yet did not throw an exception.");
            m_guiAppController.checkEntity(m_guiEntity);

            TA_ASSERT(m_guiAppController.getEntity() != NULL, "GUI Entity should not be NULL");
        }
        catch (const DatabaseException&)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "DatabaseException", "Will change this exception into a GenericGUIException. We don't care what happened just that we can't connect to the database");
            TA_THROW(GenericGUIException(GenericGUIException::NO_DATABASE_CONNECTION));
        }
        catch (const EntityTypeException&)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "EntityTypeException", "Will change this exception into a GenericGUIException.");
            TA_THROW(GenericGUIException(GenericGUIException::ENTITY_CONFIGURATION_INVALID));
        }
        catch (const DataException& ex)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "DataException", "Will change this exception into a GenericGUIException.");

            switch (ex.getFailType())
            {
            case (DataException::WRONG_TYPE):
                TA_THROW(GenericGUIException(GenericGUIException::ENTITY_CONFIGURATION_INVALID));
                break;

            case (DataException::NO_VALUE):
                TA_THROW(GenericGUIException(GenericGUIException::ENTITY_DOES_NOT_EXIST));
                break;

            case (DataException::NOT_UNIQUE):
                TA_THROW(GenericGUIException(GenericGUIException::ENTITY_NOT_UNIQUE));
                break;

            default:
                TA_ASSERT(false, "Do not recognise the DataException type");
            }
        }

        //
        // Register interest for online updates of the GUI entity
        //
        OnlineUpdateListener::getInstance().registerInterest(TA_Base_Core::ENTITY_OWNER, *this, m_guiEntity->getKey());

        LOG(SourceInfo, DebugUtil::FunctionExit, "loadEntityData");
    }

    /////////////////////////////////////////////////////
    // Public Member Methods
    /////////////////////////////////////////////////////

    void GenericGUI::run()
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "Run");

        if (! TA_Base_Core::RunParams::getInstance().isSet("LocalTest"))
        {
            if (m_hasConnectedControlStation)
            {
                m_managedProcess->startMonitoringThread();
            }
        }

        ////TD18095 jianghp
        ////m_guiAppController.displayApplication();
        //m_guiAppController.setGenericGUIStartCompleted(true);
        //m_guiAppController.onInitGenericGUICompleted();
        ////TD18095 jianghp

        LOG(SourceInfo, DebugUtil::FunctionExit, "Run");
    }


    void GenericGUI::shutdown()
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "shutdown");

        if (m_managedProcess != NULL)
        {
            std::string message;

            switch (m_terminateCode)
            {
            case (TA_Base_Core::RequestedTerminate):
                message = "Control Station told application to terminate";
                break;

            case (TA_Base_Core::InitError):
                message = "Errors encountered while GenericGUI was being constructed";
                break;

            case (TA_Base_Core::UserExit):
                message = "The user closed the application";
                break;

            case (TA_Base_Core::FatalError):
                message = "An unhandled exception was caught by the application";
                break;

            case (TA_Base_Core::AccessDenied):
                message = "User does not have access permission to run application";
                break;

            case (TA_Base_Core::NoActiveSession):
                message = "No active session for the application to run";
                break;

            case (TA_Base_Core::RightsError):
                message = "Unable to determine the rights - either it's not configured properly or rights agent is not running properly";
                break;

            case (TA_Base_Core::CommsError):
            case (TA_Base_Core::DependencyFailure):
            default:
                // This should never happen but just in case . . .
                message = "Unknown reason for closing";
            }

            m_managedProcess->notifyTerminating(m_terminateCode, message.c_str());

            // Deactivate the managed process servant
            m_managedProcess->deactivateAndDeleteServant();
            m_managedProcess = NULL;
        }

        /*// clean up the rights manager
        if (m_rightsMgr != NULL)
        {
            delete m_rightsMgr;
            m_rightsMgr = NULL;
        }*/

        // Deregister from the OnlineUpdateListener
        OnlineUpdateListener::cleanUp();

        // Clean up the alarm subsystem
        TA_Base_Core::AlarmHelperManager::getInstance().cleanUp();

        LOG(SourceInfo, DebugUtil::FunctionExit, "shutdown");
    }


    void GenericGUI::onTerminate()
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "onTerminate");

        m_terminateCode = TA_Base_Core::RequestedTerminate;
        m_guiAppController.terminateFromControlStation();

        LOG(SourceInfo, DebugUtil::FunctionExit, "onTerminate");
    }


    void GenericGUI::serverStateChange(bool isServerUp)
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "serverStateChange");

        if (isServerUp)
        {
            m_guiAppController.serverIsUp();
        }
        else
        {
            m_guiAppController.serverIsDown();
        }

        LOG(SourceInfo, DebugUtil::FunctionExit, "serverStateChange");
    }


    void GenericGUI::changeFocus(TA_Base_Core::EFocusType focusType)
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "changeAppFocus");

        try
        {
            m_guiAppController.getWinApp()->changeAppFocus(focusType);
        }
        catch (...)
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn, "Caught an exception when trying to set focus - most likely because m_guiAppController has not been instantiated yet");
        }

        LOG(SourceInfo, DebugUtil::FunctionExit, "changeAppFocus");
    }


    void GenericGUI::dutyChanged()
    {
        FUNCTION_ENTRY("dutyChanged");

        try
        {
            m_guiAppController.dutyChanged();
        }
        catch (...)
        {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn, "Caught an exception when trying to notify duty change");
        }

        FUNCTION_EXIT;
    }


    void GenericGUI::setWindowPosition(unsigned long posFlag,
                                       unsigned long alignFlag,
                                       const TA_Base_Core::IManagedGUICorbaDef::RECT& objectDim,
                                       const TA_Base_Core::IManagedGUICorbaDef::RECT& boundaryDim)

    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "setWindowPosition");

        LOG2(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "ControlStaion set the GUIApplcation position, posFlag=%d, alognFlag=%d", posFlag, alignFlag);

        // convert from the corba RECT to our normal windows rect
        TA_Rect obj(objectDim.left, objectDim.top, objectDim.right, objectDim.bottom);
        TA_Rect bound(boundaryDim.left, boundaryDim.top, boundaryDim.right, boundaryDim.bottom);

        LOG4(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "ControlStaion set the GUIApplcation position, view left=%d, top=%d, top=%d, right=%d, bottom=%d", objectDim.left, objectDim.top, objectDim.right, objectDim.bottom);
        LOG4(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "ControlStaion set the GUIApplcation position, boundary left=%d, top=%d, right=%d, bottom=%d", boundaryDim.left, boundaryDim.top, boundaryDim.right, boundaryDim.bottom);

        try
        {
            m_guiAppController.setMainViewPosition(posFlag, alignFlag, obj, bound);
        }
        catch (...)
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn, "Caught an exception when trying to setWindowPosition - most likely because m_guiAppController has not been instantiated yet");
        }

        LOG(SourceInfo, DebugUtil::FunctionExit, "setWindowPosition");
    }


    TA_Base_Core::IManagedGUICorbaDef::RECT GenericGUI::getWindowPosition()
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "getWindowPosition");
        ITransActiveView* mainView = m_guiAppController.getWinApp()->getMainView();
        TA_Rect rect =  mainView->getViewPosition();
        TA_Base_Core::IManagedGUICorbaDef::RECT retval = {rect.left, rect.top, rect.right, rect.bottom};

        LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug, "Window position (top, left, bottom, right) = (%d, %d, %d, %d)", rect.top, rect.left, rect.bottom, rect.right);
        LOG(SourceInfo, DebugUtil::FunctionExit, "getWindowPosition");
        return retval;
    }


    void GenericGUI::processUpdate(const ConfigUpdateDetails& updateEvent)
    {
        LOG(SourceInfo, DebugUtil::FunctionEntry, "processUpdate");

        TA_ASSERT(m_guiEntity != NULL,      "Could not update entity as GUI entity is NULL");

        if (updateEvent.getKey() != m_guiEntity->getKey())
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn, "Key of configuration update does not match the update we are interested in. Ignoring");
            LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
            return;
        }

        if (updateEvent.getType() != TA_Base_Core::ENTITY_OWNER)
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn, "Type of configuration update does not match the update we are interested in. Ignoring");
            LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
            return;
        }

        if (updateEvent.getModifications() == TA_Base_Core::Create)
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn, "Received a 'Create' update for a key that should already exist. Ignoring");
            LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
            return;
        }

        if (updateEvent.getModifications() == TA_Base_Core::Delete)
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn, "Received a 'Delete' update for a key that we are currently using. Reporting to the user and then ignoring.");
            LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
            //TODO
            //UINotificationDataContextPtr dataContextPtr(new UINotificationDataContext(PARAM_ENCODE_NONE_0, IDS_UW_020003));
            // UINotificationEventArgsPtr eventArgs(new UINotificationEventArgs(dataContextPtr, NOTIFY_TYPE_CONFIG_ENTITY_DATA_DELETE) ) ;
            //  m_guiAppController.getWinApp()->processUserNotification(eventArgs);
            return;
        }

        if (updateEvent.getChangedParams().empty())
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn, "Received a configuration update with an empty list of changes. Ignoring");
            LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
            return;
        }

        //
        // If we get to here then we have a valid update so invalidate the entity and then
        // pass it on for processing
        //
        m_guiEntity->invalidate();
        m_guiAppController.entityChanged(updateEvent.getChangedParams());

        LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
    }

    unsigned long GenericGUI::getEntityKey()
    {
        FUNCTION_ENTRY("getEntityKey");

        if (m_guiEntity != NULL)
        {
            return m_guiEntity->getKey();
        }
        else
        {
            return 0;
        }

        FUNCTION_EXIT;
    }


    void GenericGUI::fillMyAppEntityData()
    {
        int occLocationNSAAgentKey = 20000033;
        TA_Base_Core::EntityData* entityData = new TA_Base_Core::DefaultEntity(occLocationNSAAgentKey, TA_Base_Core::NotificationAgentData::getStaticType());
        TA_Base_Core::EntityHelper* pHelper = entityData->getHelper();

        pHelper->setIsValidData();

        std::string entityName = TA_Base_Core::RunParams::getInstance().get(RPARAM_ENTITYNAME);
        pHelper->setName(entityName);

        std::string  strPort = RunParams::getInstance().get(RPARAM_NOTIFYPORT);
        pHelper->setAddress(strPort);

        int location = atoi(RunParams::getInstance().get(RPARAM_LOCATIONKEY).c_str());
        pHelper->setLocation(location);

        m_guiEntity = entityData;
    }
}
