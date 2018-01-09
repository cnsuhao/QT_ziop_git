#include "ChangeOperationModeDlgModel.h"
#include "AppServiceCenter.h"
#include "AppServiceConsts.h"
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

    ChangeOperationModeDlgModel::ChangeOperationModeDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
        : TA_Base_Bus::AbstractPModel(appController, pmodelName)
        , m_operationMode(TA_Base_Core::NotApplicable)
    {
    }

    ChangeOperationModeDlgModel::~ChangeOperationModeDlgModel(void)
    {
    }

	void ChangeOperationModeDlgModel::changeProcessOperationMode( const std::string& processId, TA_Base_Core::EOperationMode opMode )
	{
		AppServiceCenter::getInstance().changeProcessOperationMode(processId, opMode);
	}

	void ChangeOperationModeDlgModel::sendAuditMessage( const std::string &process, const unsigned long entityKey, const std::string & sessionId )
	{
		// Only send an audit message if in a valid session.
		if (sessionId.empty())
		{
			return;
		}


		TA_Base_Core::NameValuePairs desc;
		desc["ProcessId"] = process;
		desc["mode"] = m_operationMode == TA_Base_Core::Control ? AppServiceConsts::CONTROL : AppServiceConsts::MONITOR;

		m_appController->getAuditMessageSender()->submitAuditMessage(TA_Base_Core::OperatorAudit::ApplicationChangeModeRequest,
			desc);
    }

	void ChangeOperationModeDlgModel::changeOperationMode()
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
				"Operator %s requested that process ''%s'' have its operation mode changed to %d.",
				currentUser.c_str(), process.c_str(), m_operationMode);

			sendAuditMessage(process, entityKey, sessionId);


			//
			// Change the operation mode
			//

			try
			{
				changeProcessOperationMode(process, m_operationMode);
			}
			catch(TA_Base_Core::SystemControllerException& )
			{
				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(TA_Base_Bus::PARAM_ENCODE_STR_1, IDS_UE_020040));
				dataContextPtr->pushParamValue(process);
				
				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_ERROR_CHANGE_OPMODE) ) ;
				m_appController->getWinApp()->requestUserReply(eventArgs);

				//m_appController->getWinApp()->showMsgBox(IDS_UE_020040, process.c_str());
			}

		}
	}

}