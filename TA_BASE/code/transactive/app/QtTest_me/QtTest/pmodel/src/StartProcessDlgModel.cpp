#include "StartProcessDlgModel.h"
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

    StartProcessDlgModel::StartProcessDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
        : TA_Base_Bus::AbstractPModel(appController, pmodelName)
    {
    }

    StartProcessDlgModel::~StartProcessDlgModel(void)
    {
    }

	void StartProcessDlgModel::startProcess( const std::string& processId )
	{
		AppServiceCenter::getInstance().startProcess(processId);
	}

	void StartProcessDlgModel::startProcesses()
	{
		const unsigned long entityKey = m_appController->getEntityKey();
		std::string sessionId = m_appController->getSessionId();
		std::string currentUser = m_appController->getOperatorName();

		std::string process;
		for(std::vector<std::string>::iterator iter = m_processes.begin(); 
			iter != m_processes.end(); iter++)
		{
			process = *iter;

			//
			// Log an event.
			//

			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
				"Operator %s requested that process ''%s'' be started.",
				currentUser.c_str(), process.c_str());

			// Only send an audit message if in a valid session.
			if ( ! sessionId.empty())
			{
				TA_Base_Core::NameValuePairs desc;
				desc["ProcessId"] = process;

				m_appController->getAuditMessageSender()->submitAuditMessage(TA_Base_Core::OperatorAudit::ApplicationStartRequest,
					desc);
			}

			//
			// Start the process.
			//

			try
			{
				startProcess(process);
			}
			catch(TA_Base_Core::SystemControllerException&)
			{
				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(TA_Base_Bus::PARAM_ENCODE_STR_1, IDS_UE_020020));
				dataContextPtr->pushParamValue(process);

				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_ERROR_START_PROCESS) ) ;
				m_appController->getWinApp()->requestUserReply(eventArgs);

				//m_appController->getWinApp()->showMsgBox(IDS_UE_020020, process.c_str());
			}
		}
	}
}