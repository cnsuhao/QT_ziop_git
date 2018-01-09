/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/AbstractLibraryAppController.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #4 $
  *
  * Last modification: $DateTime: 2012/09/14 11:27:59 $
  * Last modified by:  $Author: limin.zhu $
  * 
  * This inherits from IApplicationController and IGUIApplicationConfig and implements all methods that are common
  * between dialogs, property sheets and documents. It also introduces some protected methods to 
  * be implemented by child classes to aid in consistency.
  */

#pragma warning(disable:4786)
#include "bus/generic_gui_pmod/src/AbstractLibraryAppController.h"
#include "bus/generic_gui_pmod/src/AppLauncher.h"
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
 

namespace TA_Base_Bus
{
    AbstractLibraryAppController::AbstractLibraryAppController(ITransActiveApp * pWinApp, const char * applicatoinName)          
		: m_winApp(pWinApp)                  
        , m_applicationName(applicatoinName)
		, m_consoleName("unset")
		, m_operatorName("")
		, m_auditMessageSender(NULL)         
    {
    }


    AbstractLibraryAppController::~AbstractLibraryAppController()
    {
    }
    
    void AbstractLibraryAppController::cleanup()
    {
        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "AbstractLibraryAppController::cleanup");
        cleanupPModels();
        try
        {
			if (m_auditMessageSender != NULL)
			{
				delete m_auditMessageSender;
				m_auditMessageSender = NULL;
			}

            TA_Base_Bus::SecuritySubsystemAccessor::cleanup();
            releaseAppResource();
            releaseISCSCommonResource(); 
        }
        catch( ... )
        {
            LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "Unknown", "Caught in destructor");
        }
    }

    void AbstractLibraryAppController::initAppController(const std::string& commandLine, bool mustConnectControlStation)
	{	 
		//set the console name
		m_consoleName = TA_Base_Core::Hostname::getHostname();
        // Load any user settings and position the application
        //try
        //{
        //    //SecuritySubsystemAccessor::getInstance();
        //    //m_auditMessageSender = new TA_Base_Core::CtaAuditing(getEntityKey());  		 

        //}
        //catch ( const TA_Base_Core::UserSettingsException& )
        //{
        //    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "UserSettingsException", "Exception caught while trying to load the user settings. Defaults will be used instead");
        //}

        allocAppResource();

        processWorkflowDuringAppInit();

	}

    void AbstractLibraryAppController::runController()
    {
        prepareAppControllerRun();
    }


    std::string AbstractLibraryAppController::getSessionId() 
    { 
        return TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
    }


    void AbstractLibraryAppController::dutyChanged()
    {
        // This method is a stub to prevent applications from having to support a method they don't need. 
        // This stub will be removed as of IT21.
    }


    void AbstractLibraryAppController::terminateFromControlStation()
    {
        LOG0( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "start terminateFromControlStation" );
        this->terminateGUI();
        LOG0( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "end terminateFromControlStation" );
    }


    void AbstractLibraryAppController::terminateFromUser()
    {
		LOG0( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "start terminateFromUser" );
        this->terminateGUI();
        LOG0( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "end terminateFromUser" );
    }


    void AbstractLibraryAppController::terminateGUI()
    {
        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "terminateGUI" );
       
        // Close all transactive popup messages
        this->m_winApp->closeAllTransActiveMessages();
        //close all views
        this->m_winApp->getViewManager()->closeAllViews();
        LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "terminateGUI" );
    }


  
    void AbstractLibraryAppController::setMainViewPosition(  const unsigned long posFlag,
                                               const unsigned long alignFlag,
                                               const TA_Rect& objectDim,
                                               const TA_Rect& boundaryDim)
    {
		FUNCTION_ENTRY("setMainViewPosition");
		FUNCTION_EXIT;
    }
 

    void AbstractLibraryAppController::releaseISCSCommonResource()
    {
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "AbstractLibraryAppController::releaseISCSCommonResource");         

    }

	void AbstractLibraryAppController::notifyFatalErrorWhenLaunch()
	{
		 
	};
	
	void AbstractLibraryAppController::checkEntity( TA_Base_Core::IEntityData* guiEntity )
	{
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "AbstractLibraryAppController check GUI Entity");
		m_guiEntity = guiEntity;
	}

       
	TA_Base_Bus::SessionInfo AbstractLibraryAppController::getMySessionInfo()
	{
		std::string sessionId = getSessionId();
		TA_ASSERT(!sessionId.empty(), "Session ID should not be empty");

		SessionInfo sessionInfo = SecuritySubsystemAccessor::getInstance()->getSessionInfo(sessionId);
		return sessionInfo;
	}

	std::string AbstractLibraryAppController::getOperatorName()
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

	void AbstractLibraryAppController::getWorkAreaFromControlStation(const TA_Rect& rcNormalPosition, TA_Rect & workingArea)
	{
	}

    ITransActivePModel * AbstractLibraryAppController::getPModel( const std::string& pModelName )
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

    void AbstractLibraryAppController::cleanupPModels()
    {
        TA_Base_Core::ThreadGuard guard(m_pmodelsLock);
        std::map<std::string, ITransActivePModel*>::iterator it = m_pmodels.begin();
        for(;it !=m_pmodels.end();++it)
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

