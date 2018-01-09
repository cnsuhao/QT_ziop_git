#pragma once
#include "Singleton.h"
#include "core/message/IDL/src/CommsMessageCorbaDef.h"
#include "core/message/src/BackwardsCompatibility.h"

namespace TA_Base_Core
{
    class IEntityData;
    typedef boost::shared_ptr<IEntityData> IEntityDataPtr;
}

class SystemControllerMessageHandler
    : public TA_Base_Core::SpecialisedMessageSubscriber<TA_Base_Core::CommsMessageCorbaDef>,
      public Singleton<SystemControllerMessageHandler>
{
public:

    bool isGroupOnline() { return m_isGroupOnline; }

protected:

    SystemControllerMessageHandler();
    virtual void asyncInitialize() override;

    void setupSignalConnections();
    void subscribMessage();
    void unsubscribMessage();
    virtual void receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message);

protected:

    bool m_isGroupOnline;
    TA_Base_Core::IEntityDataPtr m_systemControllerEntity;
};
