#pragma once
#include "TypeDefines.h"
#include "Singleton.h"
#include "core/message/IDL/src/CommsMessageCorbaDef.h"
#include "core/message/src/BackwardsCompatibility.h"

class GraphworxMessageHandler
    : public TA_Base_Core::SpecialisedMessageSubscriber<TA_Base_Core::CommsMessageCorbaDef>,
      public Singleton<GraphworxMessageHandler>
{
protected:

    GraphworxMessageHandler();
    virtual void asyncInitialize() override;
    virtual void receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message) override;
    void setupSignalConnections();
    void subscribMessage();
    void unsubscribMessage();
    void GraphworxMessageHandler::processTAToGxMessage(const TA_Base_Core::CommsMessageCorbaDef& message);
    void GraphworxMessageHandler::processGxToTAMessage(const TA_Base_Core::CommsMessageCorbaDef& message);
};
