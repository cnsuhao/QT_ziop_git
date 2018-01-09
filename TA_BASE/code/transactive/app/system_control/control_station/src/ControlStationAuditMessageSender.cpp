#include "StdAfx.h"
#include "ControlStationAuditMessageSender.h"
#include "SessionDetails.h"
#include "Signals.h"
#include "SessionSignal.h"
#include "DataCache.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/message/src/AuditMessageSender.h"
#include "core/message/src/NameValuePair.h"
#include "core/message/types/ControlStationAudit_MessageTypes.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/data_access_interface/entity_access/src/ControlStation.h"
#include "core/utilities/src/RunParams.h"

using namespace TA_Base_Core;

const std::string CONSOLE_NAME("ConsoleName");
const std::string PROFILE_NAME("ProfileName");
const std::string USER_NAME("Username");
const std::string GROUP_ONLINE = "GroupOnline";
const std::string LOGIC_LOCATION("LogicLoc");

ControlStationAuditMessageSender::ControlStationAuditMessageSender()
{
    setupSignalConnections();
}

void ControlStationAuditMessageSender::setupSignalConnections()
{
    Signals::loginDialogFirstShowed.connect(boost::bind(&ControlStationAuditMessageSender::sendControlStationAuditMessage, this, boost::ref(ControlStationAudit::ControlStationStart)));
    SessionSignal::logined.connect(boost::bind(&ControlStationAuditMessageSender::sendControlStationAuditMessage, this, boost::ref(ControlStationAudit::ControlStationLogin)));
    SessionSignal::logouted.connect(boost::bind(&ControlStationAuditMessageSender::sendControlStationAuditMessage, this, boost::ref(ControlStationAudit::ControlStationLogout)));
    Signals::profileChanged.connect(boost::bind(&ControlStationAuditMessageSender::sendControlStationAuditMessage, this, boost::ref(ControlStationAudit::ControlStationChangeProfile)));
}

void ControlStationAuditMessageSender::sendControlStationAuditMessage(const TA_Base_Core::MessageType& messageType)
{
    if (TA_Base_Core::RunParams::getInstance().isSet("NoAuditMessage"))
    {
        return;
    }

    if (!m_auditMessageSender)
    {
        m_auditMessageSender.reset(TA_Base_Core::MessagePublicationManager::getInstance().getAuditMessageSender(TA_Base_Core::ControlStationAudit::Context));
    }

    static unsigned long entityKey = DataCache::getInstance().getControlStationEntity()->getKey();

    std::string consoleName = SessionDetails::getInstance().getConsoleName();
    std::string userName = SessionDetails::getInstance().getOperatorName();
    std::string profileName = SessionDetails::getInstance().getProfileName();
    std::string logicLocation = SessionDetails::getInstance().getLocationDisplayName();
    std::string sessionID = SessionDetails::getInstance().getSessionId();
    DescriptionParametersWrap description;

    if (messageType == TA_Base_Core::ControlStationAudit::ControlStationStart ||
            messageType == TA_Base_Core::ControlStationAudit::MFTRestart ||
            messageType == TA_Base_Core::ControlStationAudit::ControlStationExit)
    {
        description = tuple_list_of(CONSOLE_NAME, consoleName);
    }
    else if (messageType == TA_Base_Core::ControlStationAudit::ControlStationLogout ||
             messageType == TA_Base_Core::ControlStationAudit::ControlStationChangeProfile)
    {
        description = tuple_list_of(CONSOLE_NAME, consoleName)(PROFILE_NAME, profileName)(USER_NAME, userName);
    }
    else if (messageType == TA_Base_Core::ControlStationAudit::ControlStationLogin ||
             messageType == TA_Base_Core::ControlStationAudit::ControlStationChangeProfile)
    {
        description = tuple_list_of(CONSOLE_NAME, consoleName)(PROFILE_NAME, profileName)(LOGIC_LOCATION, logicLocation)(USER_NAME, userName);
    }
    else
    {
        description = tuple_list_of(CONSOLE_NAME, consoleName)(PROFILE_NAME, profileName)(LOGIC_LOCATION, logicLocation)(USER_NAME, userName);
    }

    m_auditMessageSender->sendAuditMessage(messageType, entityKey, description, "N/A", sessionID, "", "", "");
}
