#include "SystemManagerDlgModel.h"
#include "AppServiceCenter.h"
#include "SystemManagerConsts.h"

#include "../../src/resource.h"

#include "bus/common_subsystem_facade/src/SecuritySubsystemFacade.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"

#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/data_access_interface/src/ResourceAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/data_access_interface/src/IResource.h"

#include "core/message/src/MessagePublicationManager.h"
#include "core/message/src/AuditMessageSender.h"
#include "core/message/types/OperatorAudit_MessageTypes.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/message/src/NameValuePair.h"


#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/SystemControllerException.h"


namespace TA_Base_App
{

	SystemManagerDlgModel::SystemManagerDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		:TA_Base_Bus::AbstractPModel(appController, pmodelName)
	{
        m_userLoggedIn           = false;


        m_isStartProcessAllowed = false;
        m_isStopProcessAllowed = false;
        m_isSetProcessParamAllowed = false;
        m_isShutdownAllowed = false;
        m_isSetOperatingModeAllowed = false;
        m_isSetOptionsAllowed = false;
	}

	SystemManagerDlgModel::~SystemManagerDlgModel(void)
	{
	}

	std::string SystemManagerDlgModel::getSessionId()
	{
		return m_appController->getSessionId();
	}

    void SystemManagerDlgModel::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
    {
		if (_stricmp(updateType, DOMAIN_DATA_CHANGE_NEW_PROCESS_DATA) == 0)
		{
			notifyViews(DOMAIN_DATA_CHANGE_NEW_PROCESS_DATA, args);
		}
    }

    void SystemManagerDlgModel::refreshData()
    {
        try
        {
            AppServiceCenter::getInstance().refreshData();
        }
        catch(TA_Base_Core::SystemControllerException&)
        {
            AppServiceCenter::getInstance().unregisterObserver(this);
            throw;
        }
    }

    void SystemManagerDlgModel::registerUpdate()
    {
        AppServiceCenter::getInstance().registerObserver(this);
    }

    void SystemManagerDlgModel::unregisterUpdate()
    {
        AppServiceCenter::getInstance().unregisterObserver(this);
    }

    bool SystemManagerDlgModel::checkPermissions(std::vector<UINT> & vtErrorIdList) 
    {
        UINT errorId;
        bool errorOccurred = false;

        vtErrorIdList.clear();

        // Get the entity key of the System Controller.
        unsigned long entityKey;

        try
        {
            TA_Base_Core::IEntityData* entityData = TA_Base_Core::EntityAccessFactory::getInstance().
                getEntity(TA_Base_Core::RunParams::getInstance().get(RPARAM_HOSTNAME));
            entityKey = entityData->getKey();
            delete entityData;
        }
        catch(TA_Base_Core::DataException&)
        {
            LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DataException",
                "Errors occurred while trying to retrieve System Controller's entity key.");
            errorOccurred = true;
        }
        catch(TA_Base_Core::DatabaseException&)
        {
            LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DatabaseException",
                "Errors occurred while trying to retrieve System Controller's entity key.");
            errorOccurred = true;
        }


        // Get the resource key from the entity key
        unsigned long resourceKey;
        try
        {
            TA_Base_Core::IResource* resource = TA_Base_Core::ResourceAccessFactory::getInstance().getResourceFromEntity(entityKey);
            resourceKey = resource->getKey();
            delete resource;
        }
        catch(TA_Base_Core::DataException&)
        {
            LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DataException", 
                "Errors occurred while trying to retrieve a resource from the database.");
            errorOccurred = true;
        }
        catch(TA_Base_Core::DatabaseException&)
        {
            LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DatabaseException", 
                "Errors occurred while trying to retrieve the resource from the database.");
            errorOccurred = true;
        }

        if (!errorOccurred)
        {
            // Now check the permissions for each rights-controlled function.
            try
            {
                TA_Base_Bus::EDecisionModule decisionModule = TA_Base_Bus::UNDEFINED_DM;
                std::string reason;
                std::string sessionId = getSessionId();;

                try
                {
                    m_isStartProcessAllowed = TA_Base_Bus::SecuritySubsystemAccessor::getInstance()->isActionPermittedOnResource(sessionId, resourceKey, TA_Base_Bus::aca_START_PROCESS, reason, decisionModule);
                }
                catch(TA_Base_Core::RightsException&)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "RightsException", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;
                    errorId = IDS_START_PROCESS;
                    vtErrorIdList.push_back(errorId);
                }
                catch(...)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "Unknown", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;
                    errorId = IDS_START_PROCESS;
                    vtErrorIdList.push_back(errorId);
                }
                try
                {
                    m_isStopProcessAllowed = TA_Base_Bus::SecuritySubsystemAccessor::getInstance()->isActionPermittedOnResource(sessionId, resourceKey, TA_Base_Bus::aca_STOP_PROCESS, reason, decisionModule);
                }
                catch(TA_Base_Core::RightsException&)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "RightsException", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;
                    errorId = IDS_STOP_PROCESS;
                    vtErrorIdList.push_back(errorId);
                }
                catch(...)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "Unknown", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;
                    errorId = IDS_STOP_PROCESS;
                    vtErrorIdList.push_back(errorId);
                }
                try
                {
                    m_isSetProcessParamAllowed = TA_Base_Bus::SecuritySubsystemAccessor::getInstance()->isActionPermittedOnResource(sessionId, resourceKey, TA_Base_Bus::aca_SET_PROCESS_RUNTIME_PARAMETER, reason, decisionModule);
                }
                catch(TA_Base_Core::RightsException&)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "RightsException", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;

                    errorId = IDS_PROCESS_PARAM_ALLOWED;
                    vtErrorIdList.push_back(errorId);
                }
                catch(...)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "Unknown", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;
                    errorId = IDS_PROCESS_PARAM_ALLOWED;
                    vtErrorIdList.push_back(errorId);
                }
                try
                {
                    m_isShutdownAllowed = TA_Base_Bus::SecuritySubsystemAccessor::getInstance()->isActionPermittedOnResource(sessionId, resourceKey, TA_Base_Bus::aca_SHUTDOWN_SYSTEM_CONTROLLER, reason, decisionModule);
                }
                catch(TA_Base_Core::RightsException&)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "RightsException", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;
                    errorId = IDS_SHUT_DOWN;
                    vtErrorIdList.push_back(errorId);
                }
                catch(...)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "Unknown", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;
                    errorId = IDS_SHUT_DOWN;
                    vtErrorIdList.push_back(errorId);
                }
                try
                {
                    m_isSetOperatingModeAllowed = TA_Base_Bus::SecuritySubsystemAccessor::getInstance()->isActionPermittedOnResource(sessionId, resourceKey, TA_Base_Bus::aca_SET_PROCESS_OPERATING_MODE, reason, decisionModule);
                }
                catch(TA_Base_Core::RightsException&)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "RightsException", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;
                    errorId = IDS_SET_OPERATING_MODE;
                    vtErrorIdList.push_back(errorId);
                }
                catch(...)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "Unknown", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;
                    errorId = IDS_SET_OPERATING_MODE;
                    vtErrorIdList.push_back(errorId);
                }
                try
                {
                    m_isSetOptionsAllowed = TA_Base_Bus::SecuritySubsystemAccessor::getInstance()->isActionPermittedOnResource(sessionId, resourceKey, TA_Base_Bus::aca_SYSTEM_MANAGER_OPTIONS, reason, decisionModule);
                }
                catch(TA_Base_Core::RightsException&)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "RightsException", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;
                    errorId = IDS_SET_OPTIONS;
                    vtErrorIdList.push_back(errorId);
                }
                catch(...)
                {
                    LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "Unknown", 
                        "Errors occurred while trying to determine which actions are permitted.");
                    errorOccurred = true;
                    errorId = IDS_SET_OPTIONS;
                    vtErrorIdList.push_back(errorId);
                }
            }
            catch(TA_Base_Core::RightsException&)
            {
                LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "RightsException", 
                    "Errors occurred while trying to determine which actions are permitted.");
                errorOccurred = true;
            }
            catch(...)
            {
                LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "Unknown", 
                    "Errors occurred while trying to determine which actions are permitted.");
                errorOccurred = true;
            }
        }
        
        if (errorOccurred)
        {
            m_isStartProcessAllowed = true;
            m_isStopProcessAllowed = true;
            m_isSetProcessParamAllowed = true;
            m_isShutdownAllowed = true;
            m_isSetOperatingModeAllowed = true;
            m_isSetOptionsAllowed = true;
        }
        return errorOccurred;

    }

	void SystemManagerDlgModel::sessionIdChanged()
	{
		notifyViews(PMODEL_DATA_CHANGE_SESSION_ID, NULL);
	}

	std::string SystemManagerDlgModel::getSystemControllerHostName()
	{
		return AppServiceCenter::getInstance().getSystemControllerHostName();
	}

	int SystemManagerDlgModel::login( const std::string& hostName, const std::string& portNumber, const std::string& sessionId, const std::string& operatorName )
	{
		return AppServiceCenter::getInstance().login(hostName, portNumber, sessionId, operatorName);
	}

	void SystemManagerDlgModel::logout()
	{
		AppServiceCenter::getInstance().logout();
	}

	std::vector<TA_Base_Core::ProcessData> SystemManagerDlgModel::getProcesses()
	{
		return AppServiceCenter::getInstance().getProcesses();
	}

	bool SystemManagerDlgModel::isServiceProcessFromName( const std::string& entityName ) const
	{
		return AppServiceCenter::getInstance().isManagedProcessFromName(entityName);
	}

	bool SystemManagerDlgModel::isServiceProcessFromDescription( const std::string& entityDescription ) const
	{
		return AppServiceCenter::getInstance().isServiceProcessFromDescription(entityDescription);
	}

	bool SystemManagerDlgModel::isAutoFailBackFromDescription( const std::string& entityDescription ) const
	{
		return AppServiceCenter::getInstance().isAutoFailBackFromDescription(entityDescription);
	}

	bool SystemManagerDlgModel::isManagedProcessFromDescription( const std::string& entityDescription ) const
	{
		return AppServiceCenter::getInstance().isManagedProcessFromDescription(entityDescription);
	}

	void SystemManagerDlgModel::shutdownSystemController()
	{
		AppServiceCenter::getInstance().shutdownSystemController();
	}

	int SystemManagerDlgModel::doLogin(int & errorId, const std::string & sessionId, const std::string & operatorName)
	{
		errorId = 0;

		// Login
		int res = login(
			TA_Base_Core::RunParams::getInstance().get(RPARAM_HOSTNAME), 
			TA_Base_Core::RunParams::getInstance().get(RPARAM_PORTNUMBER),
			sessionId, operatorName);

		switch (res)
		{
		case ERR_AUTH_NO_ERROR:
			break;
		case ERR_AUTH_SOCKET_ERROR:
			errorId = IDS_UNIDENTIFIED_CONSOLE;
			break;
		case ERR_AUTH_SERVICE_ERROR:
			errorId = IDS_AUTHENTICATION_SERVICE_ERROR;
			break;
		case ERR_GET_ADMIN_ERROR:
			errorId = IDS_CANNOT_ACCESS_SYSTEM_CONTROLLER;
			break;
		case ERR_NO_SYSTEM_CONTROLLER_ERROR:
			errorId = IDS_NO_SYSTEM_CONTROLLER;
			break;
		case ERR_RETRIEVING_PROCESS_DATA:
			errorId = IDS_CANNOT_RETRIEVE_INFO;
			break;
		default:
			errorId = IDS_LOGIN;
			break;
		}

		return res;
	}

	void SystemManagerDlgModel::sendAuditMessage(const std::string & sessionId, const TA_Base_Core::MessageType & msgType)
	{
		// Only send an audit message if in a valid session.
		if (!sessionId.empty())
		{
			TA_Base_Core::NameValuePairs desc;
			desc["hostname"] = TA_Base_Core::RunParams::getInstance().get(RPARAM_HOSTNAME);

			m_appController->getAuditMessageSender()->submitAuditMessage(msgType,
				desc);
		}
	}
}