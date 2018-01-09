#pragma once
#include "Singleton.h"
#include "core/message/types/MessageTypes.h"
#include "core/message/src/AuditMessageSender.h"

class ControlStationAuditMessageSender : public Singleton<ControlStationAuditMessageSender>
{
public:

    void sendControlStationAuditMessage(const TA_Base_Core::MessageType& messageType);

protected:

    ControlStationAuditMessageSender();
    void setupSignalConnections();

protected:

    TA_Base_Core::AuditMessageSenderPtr m_auditMessageSender;
};
