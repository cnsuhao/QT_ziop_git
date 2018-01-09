#include "ProcessParamsDlgModel.h"
#include "AppServiceCenter.h"
#include "SystemManagerConsts.h"

#include "bus/TA_MessageResource/resource.h"

#include "core/message/src/MessagePublicationManager.h"
#include "core/message/src/AuditMessageSender.h"
#include "core/message/types/OperatorAudit_MessageTypes.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/message/src/NameValuePair.h"
#include "core/exceptions/src/SystemControllerException.h"

namespace TA_Base_App
{

    ProcessParamsDlgModel::ProcessParamsDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
        : TA_Base_Bus::AbstractPModel(appController, pmodelName)
    {
    }

    ProcessParamsDlgModel::~ProcessParamsDlgModel(void)
    {
    }

	void ProcessParamsDlgModel::setProcessRunParam( const std::string& processId, const TA_Base_Core::RunParam& param )
	{
		AppServiceCenter::getInstance().setProcessRunParam(processId, param);
	}

	std::string ProcessParamsDlgModel::getProcessDebugLevel( const std::string& processId )
	{
		return AppServiceCenter::getInstance().getProcessDebugLevel(processId);
	}

	void ProcessParamsDlgModel::changeProcessParams( const char * debugLevel )
	{
		TA_Base_Core::RunParam runParam;

		std::string name = RPARAM_DEBUGLEVEL;
		std::string value = debugLevel; 
		runParam.name  = name.c_str();
		runParam.value = value.c_str();

		// For all selected processes
		std::string process;
		for(std::vector<std::string>::iterator iter = m_processes.begin(); 
			iter != m_processes.end(); iter++)
		{
			process = *iter;

			// Log an event.

			const unsigned long entityKey = m_appController->getEntityKey();
			std::string sessionId = m_appController->getSessionId();
			std::string currentUser = m_appController->getOperatorName();

			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo,
				"Operator %s has requested that the debug level of process '%s' be changed to '%s'",
				currentUser.c_str(), process.c_str(), runParam.value);


			// Only send an audit message if in a valid session.
			if (! sessionId.empty())
			{
				TA_Base_Core::NameValuePairs desc;
				desc["ProcessId"] = process;
				desc["debuglevel"] = debugLevel;

                m_appController->getAuditMessageSender()->submitAuditMessage(TA_Base_Core::OperatorAudit::ApplicationDebugLevelChange,
                    desc);

			}

			try
			{
				setProcessRunParam(process, runParam);
			}
			catch(TA_Base_Core::SystemControllerException&)
			{
				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(TA_Base_Bus::PARAM_ENCODE_STR_1, IDS_UE_020024));
				dataContextPtr->pushParamValue(process);

				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_ERROR_SET_PARAM) ) ;
				m_appController->getWinApp()->requestUserReply(eventArgs);

				//m_appController->getWinApp()->showMsgBox(IDS_UE_020024, process.c_str());
			}
		}
	}

	TA_Base_Core::DebugUtil::EDebugLevel ProcessParamsDlgModel::getCurrentDebugLevel( )
	{
		TA_Base_Core::DebugUtil::EDebugLevel debugLevel;

		std::string entityName = m_processes[0];
		std::string debugLevelString = getProcessDebugLevel(entityName);
		TA_Base_Core::DebugUtil::EDebugLevel firstProcessDebugLevel = 
			TA_Base_Core::DebugUtil::getInstance().getDebugLevelFromString(debugLevelString.c_str());

		std::string process;

		// Now loop through each process, checking if any process doesn't
		// match the first process (in which case use the default).

		for(std::vector<std::string>::iterator iter = m_processes.begin(); 
			iter != m_processes.end(); iter++)
		{
			process = *iter;
			debugLevelString = getProcessDebugLevel(process);
			debugLevel = TA_Base_Core::DebugUtil::getInstance().getDebugLevelFromString(debugLevelString.c_str());

			if (debugLevel != firstProcessDebugLevel)
			{
				debugLevel = TA_Base_Core::DebugUtil::DebugInfo; // The default.
				break;
			}
		}	
		return debugLevel;
	}
}