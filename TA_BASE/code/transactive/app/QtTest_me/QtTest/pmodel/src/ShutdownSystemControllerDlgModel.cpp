#include "ShutdownSystemControllerDlgModel.h"
#include "AppServiceCenter.h"

#include "bus/TA_MessageResource/resource.h"

#include "core/message/src/MessagePublicationManager.h"
#include "core/message/src/AuditMessageSender.h"
#include "core/message/types/OperatorAudit_MessageTypes.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"



namespace TA_Base_App
{

    ShutdownSystemControllerDlgModel::ShutdownSystemControllerDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
        : TA_Base_Bus::AbstractPModel(appController, pmodelName)
    {
    }

    ShutdownSystemControllerDlgModel::~ShutdownSystemControllerDlgModel(void)
    {
    }

	void ShutdownSystemControllerDlgModel::shutdownSystemController()
	{
		const unsigned long entityKey = m_appController->getEntityKey();
		std::string sessionId = m_appController->getSessionId();
		std::string currentUser = m_appController->getOperatorName();

		//
		// Log an event
		//

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
			"Operator %s has requested that the System Controller shut down",
			currentUser.c_str());

		// Only send an audit message if in a valid session.
		if (! sessionId.empty())
		{
			TA_Base_Core::NameValuePairs desc;
			desc["hostname"] = m_hostname;

			m_appController->getAuditMessageSender()->submitAuditMessage(TA_Base_Core::OperatorAudit::SystemControllerShutdown,
				desc);
		}
	}

}