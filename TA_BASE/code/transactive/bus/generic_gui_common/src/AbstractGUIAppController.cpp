/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/AbstractGUIAppController.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #4 $
  *
  * Last modification: $DateTime: 2012/09/14 11:27:59 $
  * Last modified by:  $Author: limin.zhu $
  * 
  * This inherits from IGUIApplicationController and IGUIApplicationConfig and implements all methods that are common
  * between dialogs, property sheets and documents. It also introduces some protected methods to 
  * be implemented by child classes to aid in consistency.
  */

#pragma warning(disable:4786)


 
#include "bus/generic_gui_common/src/AbstractGUIAppController.h"
#include "bus/generic_gui_common/src/AppLauncher.h"
#include "bus/generic_gui_common/src/ITransActiveView.h"
#include "bus/generic_gui_common/src/ITransActivePModel.h"
#include "bus/common_subsystem_facade/src/SecuritySubsystemFacade.h"
#include "bus/user_settings/src/SettingsMgr.h"
#include "bus/generic_gui_entity/src/GenericGUI.h"

#include "core/message/src/CtaAuditing.h"
#include "core/data_access_interface/src/GuiAccessFactory.h"
#include "core/data_access_interface/src/IGui.h"

#include "core/message/src/ConnectionChecker.h"
#include "core/naming/src/Naming.h"
#include "core/message/src/TransactiveMessageSubscription.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/database/src/DbServerLocator.h"
#include "core/synchronisation/src/ThreadGuard.h"

#include "core/utilities/src/Hostname.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/exceptions/src/GenericGUIException.h"
#include "core/exceptions/src/UserSettingsException.h"
#include "core/exceptions/src/ApplicationException.h"
/*
using TA_Base_Core::RunParams;
using TA_Base_Core::DebugUtil;
using TA_Base_Core::TransactiveException;
using TA_Base_Core::GenericGUIException;
using namespace TA_Base_Bus::TA_GenericGui;
*/

#include "ace/ACE.h"

namespace TA_Base_Bus
{

/*
	// Static variable initialisation   
	const std::string AbstractGUIAppController::APPLICATION_NAME = "ApplicationName";
	const std::string AbstractGUIAppController::ERROR_MESSAGE = "ErrorMessage";
	const std::string AbstractGUIAppController::CONSOLE_NAME = "ConsoleName";
*/

    AbstractGUIAppController::AbstractGUIAppController(ITransActiveWinApp * pWinApp, const char * applicatoinName) 
        : m_guiEntity(NULL)        // set in checkEntity()
        , m_winApp(pWinApp)           
        , m_userSettings(NULL)      // set in first settings method
        , m_applicationName(applicatoinName)
		, m_consoleName("unset")
		, m_operatorName("")
		, m_auditMessageSender(NULL)
        , m_genericGUI(NULL)
    {

        if (!SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, static_cast<LPVOID>(0), SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE))
        {
            LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, "Could not set the system parametesr so that windows are launched to the top");
        }
        m_positionInfo.posFlag = TA_GenericGui::POS_NONE;
        m_positionInfo.alignFlag = TA_GenericGui::ALIGN_NONE;
        m_positionInfo.objectDim = TA_GenericGui::DEFAULT_RECT;
        m_positionInfo.boundaryDim = TA_GenericGui::DEFAULT_RECT;
        m_positionInfo.appDim = TA_GenericGui::DEFAULT_RECT;

        m_isFirstCalculatePosition = true;

    }


    AbstractGUIAppController::~AbstractGUIAppController()
    {
    }
    
    void AbstractGUIAppController::cleanup()
    {

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "AbstractGUIAppController::cleanup");

        cleanupPModels();

        try
        {
            if (m_userSettings != NULL)
            {
                delete m_userSettings;
                m_userSettings = NULL;
            }

			if (m_auditMessageSender != NULL)
			{
				delete m_auditMessageSender;
				m_auditMessageSender = NULL;
			}

            TA_Base_Bus::SecuritySubsystemAccessor::cleanup();

            releaseAppResource();

            releaseISCSCommonResource();

			ACE::fini();
        }
        catch( ... )
        {
            LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "Unknown", "Caught in destructor");
        }
    }

	void AbstractGUIAppController::initController(const char * cmdLine, bool mustConnectControlStation)
	{
		ACE::init();

        //set application type
        {
            std::ostringstream appType;
            appType << getApplicationType();
            TA_Base_Core::RunParams::getInstance().set( RPARAM_APPTYPE, appType.str().c_str() );
        }

		m_genericGUI = new GenericGUI(*this, cmdLine, mustConnectControlStation);

		//set the console name
		m_consoleName = TA_Base_Core::Hostname::getHostname();


        // Load any user settings and position the application
        try
        {
			m_genericGUI->init();

			SecuritySubsystemAccessor::getInstance();
			m_auditMessageSender = new TA_Base_Core::CtaAuditing(getEntityKey());
            
            processApplicationName();

			configureHelpFile();

			registerRunParamChange();

			this->loadAllUserSettings();

		}
        catch (const TA_Base_Core::UserSettingsException&)
        {
            LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "UserSettingsException", "Exception caught while trying to load the user settings. Defaults will be used instead");
        }

        allocAppResource();

        processWorkflowDuringAppInit();

	}

    void AbstractGUIAppController::processApplicationName()
    {
        std::string appName = TA_Base_Core::RunParams::getInstance().get(RPARAM_APPNAME);		
        // See if the application name is already set in RunParams
        if (appName.empty())
        {
            // not yet set, so just set it as the one that was passed in as the
            // argument
            if (!m_applicationName.empty())
            {
                TA_Base_Core::RunParams::getInstance().set(RPARAM_APPNAME, m_applicationName.c_str());

                LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
                    "Setting the RunParam RPARAM_APPNAME as %s", m_applicationName.c_str());
            }
        }
        else
        {
            // use the one that is already in RunParams
            m_applicationName = appName;
        }

    }

    void AbstractGUIAppController::runController()
    {
        prepareAppControllerRun();

		moveMainViewWhenStartedBasedOnUserSetting();
        
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin Run Generic Gui");
        m_genericGUI->run();

    }


    std::string AbstractGUIAppController::getSessionId() 
    { 
        return TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
    }


    void AbstractGUIAppController::dutyChanged()
    {
        // This method is a stub to prevent applications from having to support a method they don't need. 
        // This stub will be removed as of IT21.
    }


    void AbstractGUIAppController::terminateFromControlStation()
    {
        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "terminateFromControlStation" );

        this->terminateGUI();

        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "terminateFromControlStation" );
    }


    void AbstractGUIAppController::terminateFromUser()
    {
        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "terminateFromUser" );

        prepareForClose();

        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "terminateFromUser" );
    }


    void AbstractGUIAppController::terminateGUI()
    {
        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "terminateGUI" );

        prepareForClose();

        // Close all transactive popup messages
        this->m_winApp->closeAllTransActiveMessages();

        // Closing the main window may not be enough if there are some other
        // modal (or modeless) dialogs floating around...
        // So we enumerate all top level windows. The enumeration handler
        // will determine if they were created by our process, and if so
        // it will destroy them.
        ::EnumWindows(closeAllWindowOfOneProcess, ::GetCurrentProcessId());

        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "terminateGUI" );
    }


    BOOL CALLBACK AbstractGUIAppController::closeAllWindowOfOneProcess(HWND hWnd, LPARAM lParam)
    {
        // lparam contains the current process' ID
        // this saves us having to query it for every call...
        DWORD dwOurProcessID = static_cast<DWORD>(lParam);

        // find out which process & thread created this window
        DWORD dwThisWindowProcessID;
        DWORD dwThisWindowThreadID;
        dwThisWindowThreadID = ::GetWindowThreadProcessId(hWnd, &dwThisWindowProcessID);

        // if it's our process
        if (dwOurProcessID == dwThisWindowProcessID)
        {
	        // Close the window...
	        // The method to use depends on the type of window:
	        // modal dialogs should be ended with ::EndDialog(),
	        // other windows with ::DestroyWindow().
	        // I haven't found a fireproof way of determining
	        // if a window is a modal dialog!
	        // Calling ::EndDialog() on a modeless window is OK, as
	        // long as we also call ::DestroyWindow() after.
	        // ::EndDialog(hWnd,0); // cause hang sometimes
	        ::DestroyWindow(hWnd);

	        // Our GUIs windows should PostQuitMessage() when they
	        // get a WM_DESTROY but I don't trust them to do so,
	        // so let's do it for them...
	        ::PostThreadMessage(dwThisWindowThreadID,WM_QUIT,0,0);
        }
        
        return TRUE;
    }


    void AbstractGUIAppController::setMainViewPosition(  const unsigned long posFlag,
                                               const unsigned long alignFlag,
                                               const RECT& objectDim,
                                               const RECT& boundaryDim)
    {
		FUNCTION_ENTRY("setMainViewPosition");

		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "AbstractGUIAppController::setMainViewPosition");

		// The GUI should just be given focus.
		if (posFlag == TA_GenericGui::POS_NONE)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Will give focus to GUI, rather than moving it");
			getWinApp()->changeAppFocus(TA_Base_Core::GetFocus);

			FUNCTION_EXIT;
			return;
		}

        // If this is the first time we're displaying the rect or if any of the flags/rects
        // are different to the current ones, then we'll have to reset everything and 
        // recalculate the position.
        if (((posFlag != TA_GenericGui::POS_NONE) && (m_positionInfo.posFlag != posFlag)) ||
           (m_positionInfo.alignFlag != alignFlag) ||
           (! TA_EqualRECT(&(m_positionInfo.objectDim), &objectDim)) ||
           (! TA_EqualRECT(&(m_positionInfo.boundaryDim), &boundaryDim)))
        {
            m_positionInfo.posFlag = posFlag;
            m_positionInfo.alignFlag = alignFlag;
            m_positionInfo.objectDim = objectDim;
            m_positionInfo.boundaryDim = boundaryDim;
            // reset the application dimension
            m_positionInfo.appDim = TA_GenericGui::DEFAULT_RECT;
            setMainViewPosition();

			FUNCTION_EXIT;
            return;
        }

        // if the new position is exactly the same as the old one, then we don't have
        // to recalculate the position again.  We'll only move the window if it's equal
        // to the default rect (which is what we set the appDim to when the application is
        // not meant to be repositioned)
        m_winApp->moveMainView(m_positionInfo, m_isFirstCalculatePosition);

		FUNCTION_EXIT;
    }


    void AbstractGUIAppController::setMainViewPosition()
    {
        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "setMainViewPosition" );

		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "AbstractGUIAppController::setMainViewPosition");

        RECT windowRect = getMainViewRect();

        bool isRepositioned = TA_Base_Bus::AppLauncher::getInstance().calculateNewCoordinates(windowRect, 
                                                            m_positionInfo.objectDim, 
                                                            m_positionInfo.boundaryDim, 
                                                            m_positionInfo.posFlag, 
                                                            m_positionInfo.alignFlag);

        // stores the new window dimension
        m_positionInfo.appDim = isRepositioned ? windowRect : TA_GenericGui::DEFAULT_RECT;

        m_winApp->moveMainView(m_positionInfo, m_isFirstCalculatePosition);
        
		m_isFirstCalculatePosition = false;

        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "setMainViewPosition" );
    }


    void AbstractGUIAppController::saveMainViewPositionSettings()
    {
        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "savePositionSettings" );

        if ( m_userSettings == NULL )
        {
            m_userSettings = new TA_Base_Bus::SettingsMgr(getSessionId(),getApplicationType());
            if (m_userSettings == NULL)
            {
                LOG( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugWarn, "new on SettingsMgr returned NULL");
                return;
            }
        }

        RECT r = m_winApp->getMainView()->getViewPosition();
        //TODO: save last windows position into database
		//TA_ASSERT(wnd != NULL, "main view should be valid");
        //WINDOWPLACEMENT place;
        //GetWindowPlacement(wnd, &place);
        //m_userSettings->saveWindowPlacementToDB(place);

        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "savePositionSettings" );
    }


    void AbstractGUIAppController::setUserSetting(TA_Base_Bus::SettingsMgr& userSettings)
    {
        // prevent memory leak - if the new object is not the same as the
        // old one, we need to delete the old one 'cos we're responsible for
        // deleting m_userSettings.
        if ((m_userSettings != NULL) && (m_userSettings != &userSettings))
        {
            delete m_userSettings;
            m_userSettings = NULL;
        }
        m_userSettings = &userSettings;
    }

    void AbstractGUIAppController::releaseISCSCommonResource()
    {
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "AbstractGUIAppController::releaseISCSCommonResource");
        try
        {
            if(m_genericGUI != NULL)
            {
                m_genericGUI->shutdown();
                m_genericGUI->deactivateAndDeleteServant();
            }

			LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "AbstractGUIAppController release singleton resource");

			
			// Deregister from RunParams
			TA_Base_Core::RunParams::getInstance().deregisterRunParamUser(this);

            // Don't need to delete m_genericGUI as the call above does this.

            TA_Base_Core::Naming::cleanUp();
            
            // Clean up the Event Consumers
            TA_Base_Core::gDestroySubscriptionManager();
            TA_Base_Core::gTerminateAndDeleteStructuredEventSuppliers();
            TA_Base_Core::ChannelLocatorConnectionMgr::removeInstance();

            TA_Base_Core::DbServerLocator::removeInstance();
            TA_Base_Core::CorbaUtil::getInstance().cleanup();
        }
        catch( ... )
        {
            LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Catch unknown exeption during Application Controller unitialization");
        }

    }

	void AbstractGUIAppController::notifyFatalErrorWhenLaunch()
	{
		if (m_genericGUI != NULL)
		{
			m_genericGUI->occurredFatalError();
		}
	};

	RECT AbstractGUIAppController::getMainViewRect()
	{
		return m_winApp->getMainView()->getViewPosition();
	}

	void AbstractGUIAppController::prepareForClose()
	{
		try
		{
			saveAllUserSettings();
		}
		catch ( TA_Base_Core::UserSettingsException&)
		{
			//TO DO: Handle here with appropriate error message.
		}

	}

	void AbstractGUIAppController::saveAllUserSettings()
	{
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "saveAllUserSettings");

		// First check if we should be saving the user settings.
		if ( TA_Base_Core::RunParams::getInstance().get(RPARAM_USERPREFERENCES).empty() )
		{
			return;
		}

		if (getUserSetting() == NULL)
		{
			TA_Base_Bus::SettingsMgr* userSettings = 
				new TA_Base_Bus::SettingsMgr(getSessionId(), getApplicationType());
			if (userSettings == NULL)
			{
				LOG( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugWarn, 
					"new on SettingsMgr returned NULL. Cannot load user settings");
				return;
			}
			setUserSetting(*userSettings);
		}

		saveMainViewPositionSettings();
	}

	void AbstractGUIAppController::moveMainViewWhenStartedBasedOnUserSetting()
	{
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Application controller move window according to setting");
		if ( TA_Base_Core::RunParams::getInstance().get(RPARAM_USERPREFERENCES).empty() )
		{
			//setMainViewPosition();
			LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "MoveWindowAccordingToSetting" );            
			return;
		}

		TA_ASSERT( getUserSetting() != NULL,"user setting should not be NULL"); 

		//TODO:replace HWND
		//HWND wnd = m_winApp->getMainView()->getViewHandle();
		//TA_ASSERT(wnd != NULL, "main view should be valid");
		//WINDOWPLACEMENT place = m_userSettings->getWindowPlacementFromDB();
		//SetWindowPlacement(wnd, &place);

		LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "moveMainViewWhenStartedBasedOnSetting" );

	}

	void AbstractGUIAppController::loadAllUserSettings()
	{
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "loadAllUserSettings");
		
		// First check if we should be loading the user settings.
		if ( TA_Base_Core::RunParams::getInstance().get(RPARAM_USERPREFERENCES).empty() )
		{
			return;
		}

		if (getUserSetting() == NULL)
		{
			TA_Base_Bus::SettingsMgr* userSettings = 
				new TA_Base_Bus::SettingsMgr(getSessionId(), getApplicationType());
			if (userSettings == NULL)
			{
				LOG0( SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, 
					"new on SettingsMgr returned NULL. Cannot load user settings");
				return;
			}
			setUserSetting(*userSettings);
		}
	}

	void AbstractGUIAppController::checkEntity( TA_Base_Core::IEntityData* guiEntity )
	{
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "AbstractGUIAppController check GUI Entity");
		m_guiEntity = guiEntity;
	}

    bool AbstractGUIAppController::isConnectedControlStation()
    {
        bool fRet = false;
        if (m_genericGUI != NULL)
        {
            return m_genericGUI->isConnectedControlStation();
        }
        return fRet;
    }

	void AbstractGUIAppController::configureHelpFile()
	{
		FUNCTION_ENTRY("configureHelpFile");

		if (! TA_Base_Core::RunParams::getInstance().get(RPARAM_HELPFILE).empty())
		{
			// Already set
			FUNCTION_EXIT;
			return;
		}

		try
		{
			TA_Base_Core::IGui * rawGui = TA_Base_Core::GuiAccessFactory::getInstance().getGui( getApplicationType() );
			std::auto_ptr<TA_Base_Core::IGui> gui(rawGui);
			std::string helpFile = gui->getHelpFilePath();
			if (!helpFile.empty())
			{
				TA_Base_Core::RunParams::getInstance().set(RPARAM_HELPFILE, helpFile.c_str());
			}
		}
		catch( ... )
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "Database Exception", "Could not set the help file for this application. No help will be available");
		}


		FUNCTION_EXIT;
	}


	void AbstractGUIAppController::onRunParamChange(const std::string& name, const std::string& value)
	{
		LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "onRunParamChange" );

		// New debug file name
		if ( 0 == name.compare(RPARAM_DEBUGFILE) )
		{
			TA_Base_Core::DebugUtil::getInstance().setFile(value.c_str());
		}
		// New debug level
		else if ( 0 == name.compare(RPARAM_DEBUGLEVEL) )
		{
			TA_Base_Core::DebugUtil::getInstance().setLevel( TA_Base_Core::DebugUtil::getDebugLevelFromString(value.c_str()));
		}
		// New max file size
		else if ( 0 == name.compare(RPARAM_DEBUGFILEMAXSIZE) )
		{
			std::istringstream strValue(value);
			int maxSize;
			strValue >> maxSize;
			TA_Base_Core::DebugUtil::getInstance().setMaxSize(maxSize);
		}
		// New max number of debug files
		else if ( 0 == name.compare(RPARAM_DEBUGMAXFILES) )
		{
			std::istringstream strValue(value);
			int maxFiles;
			strValue >> maxFiles;
			TA_Base_Core::DebugUtil::getInstance().setMaxFiles(maxFiles);
		}
		// Use pid in filenames
		else if ( 0 == name.compare(RPARAM_DEBUGPIDFILENAMES) )
		{
			TA_Base_Core::DebugUtil::getInstance().encryptPidInFilenames(value);
		}
		// Turn off logging for these debug levels
		else if ( 0 == name.compare(RPARAM_DEBUGLEVELOFF) )
		{
			TA_Base_Core::DebugUtil::getInstance().decodeDisabledLevels(value);
		}
		// Filenames for debug levels
		else if ( 0 == name.compare(RPARAM_DEBUGLEVELFILE) )
		{
			TA_Base_Core::DebugUtil::getInstance().decodeLevelFilenames(value);
		}
		else if (name.compare(RPARAM_SESSIONID) == 0)
		{
			TA_Base_Bus::SecuritySubsystemAccessor::getInstance()->resetCache();
			m_operatorName.clear();
			sessionIdChanged();
		}


		LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "onRunParamChange" );
	}


	void AbstractGUIAppController::registerRunParamChange()
	{
		//
		// Register to be notified of changes to the Debug parameters.
		// It is possible these could change when Managed Process is set up.
		//
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_DEBUGFILE);
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_DEBUGLEVEL);
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_DEBUGFILEMAXSIZE);
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_DEBUGMAXFILES);
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_DEBUGPIDFILENAMES);
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_DEBUGLEVELOFF);
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_DEBUGLEVELFILE);

		// Register for the SessionId run param
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_SESSIONID);

		// We must set the OperationMode to be Control so that the rebinding of names works.
		// All GUI's are always in Control Mode. Only agents will have different modes.
		TA_Base_Core::RunParams::getInstance().set(RPARAM_OPERATIONMODE, RPARAM_CONTROL);
	}

	TA_Base_Bus::SessionInfo AbstractGUIAppController::getMySessionInfo()
	{
		std::string sessionId = getSessionId();
		TA_ASSERT(!sessionId.empty(), "Session ID should not be empty");

		SessionInfo sessionInfo = SecuritySubsystemAccessor::getInstance()->getSessionInfo(sessionId);
		return sessionInfo;
	}

	std::string AbstractGUIAppController::getOperatorName()
	{
		if (! m_operatorName.empty())
		{
			return m_operatorName;
		}

		try
		{
			TA_Base_Bus::SessionInfo sessionInfo = getMySessionInfo();
			std::auto_ptr<TA_Base_Core::IOperator> iOperator(TA_Base_Core::OperatorAccessFactory::getInstance().getOperator(sessionInfo.UserId, false));
			m_operatorName = iOperator->getName();
			return m_operatorName;
		}
		catch (...)
		{
			return "nobody";
		}
	}

	void AbstractGUIAppController::getWorkAreaFromControlStation(const RECT& rcNormalPosition, RECT & workingArea)
	{
		if (! isConnectedControlStation())
		{
			TA_SetRectEmpty(&workingArea);
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "could not connect control station, temporary return empty area");
			return; 
		}

		try
		{
			workingArea = AppLauncher::getInstance().getRect(TA_GenericGui::SCREEN_CURRENT, TA_GenericGui::AREA_SCHEMATIC, rcNormalPosition.left);
			int width = workingArea.right - workingArea.left;
			int height = workingArea.bottom -workingArea.top;

			LOG4(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Worker Area Info: ----------X=%d,Y=%d,CX=%d,CY=%d",workingArea.left,workingArea.top, width, height);
		}
		catch(TA_Base_Core::ApplicationException& ex)
		{
			// Log the catch, and set the workingArea rect to 0.
			std::stringstream errorMessage;
			errorMessage << "An exception was caught while attempting to retrieve the schematic area of the current"
				<< "screen. X coord = " << rcNormalPosition.left << ". Error Message:";
			LOG_EXCEPTION_CATCH(SourceInfo,"ApplicationException",errorMessage.str().c_str());
			LOGMORE(SourceInfo,ex.what());

			// Set the workingArea rect to 0
			TA_SetRectEmpty(&workingArea);
		}

	}

    ITransActivePModel * AbstractGUIAppController::getPModel( const std::string & pModelName )
    {
        TA_Base_Core::ThreadGuard guard(m_pmodelsLock);
        if ( m_pmodels.find(pModelName) == m_pmodels.end() )
        {
            ITransActivePModel * pModel = createPModel(pModelName);
            pModel->allocViewResource();
            m_pmodels[pModelName] = pModel;

            TA_ASSERT(pModelName == pModel->getName(), "presentation model name is not correct!");
            return pModel;
        }
        return m_pmodels[pModelName];
    }

    void AbstractGUIAppController::cleanupPModels()
    {
        TA_Base_Core::ThreadGuard guard(m_pmodelsLock);
        std::map<std::string, ITransActivePModel*>::iterator it = m_pmodels.begin();
        for(;it !=m_pmodels.end();it++)
        {
            ITransActivePModel * pModel = it->second;
            if (pModel != NULL)
            {
                pModel->cleanup();
                delete pModel;
            }
        }
        m_pmodels.clear();
    }

}

