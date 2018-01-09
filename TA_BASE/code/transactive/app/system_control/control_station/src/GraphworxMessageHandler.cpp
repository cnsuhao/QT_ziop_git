#include "StdAfx.h"
#include "GraphworxMessageHandler.h"
#include "DataCache.h"
#include "SessionSignal.h"
#include "GraphworxSignal.h"
#include "TAToGxMessageInformation.h"
#include "SessionDetails.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/message/types/SystemControllerComms_MessageTypes.h"
#include "core/message/types/ControlStationComms_MessageTypes.h"
#include "core/process_management/IDL/src/IControlStationCorbaDef.h"
#include "core/utilities/src/RunParams.h"

using namespace TA_Base_Core;

GraphworxMessageHandler::GraphworxMessageHandler()
{
    setupSignalConnections();
}

void GraphworxMessageHandler::setupSignalConnections()
{
    SessionSignal::logined.connect(boost::bind(&GraphworxMessageHandler::subscribMessage, this));
    SessionSignal::logouted.connect(boost::bind(&GraphworxMessageHandler::unsubscribMessage, this));
}

void GraphworxMessageHandler::asyncInitialize()
{
    //TODO
}

void GraphworxMessageHandler::subscribMessage()
{
    unsigned long locationKey = TA_Base_Core::getRunParamValue(RPARAM_LOCATIONKEY, 0);
    MessageSubscriptionManager::getInstance().subscribeToCommsMessage(ControlStationComms::TAToGXMessageNotification, this, SessionDetails::getInstance().getConsoleId(), 0, locationKey, NULL);
    MessageSubscriptionManager::getInstance().subscribeToCommsMessage(ControlStationComms::GXToTAMessageNotification, this, 0, 0, locationKey, NULL); //lizettejl++ (DP-changes)
}

void GraphworxMessageHandler::unsubscribMessage()
{
    MessageSubscriptionManager::getInstance().unsubscribeToMessages(this);
}

void GraphworxMessageHandler::receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message)
{
    std::string mesgTypeKey = message.messageTypeKey;

    try
    {
        if (0 == mesgTypeKey.compare(TA_Base_Core::ControlStationComms::TAToGXMessageNotification.getTypeKey()))
        {
            processTAToGxMessage(message);
        }

        if (0 == mesgTypeKey.compare(TA_Base_Core::ControlStationComms::GXToTAMessageNotification.getTypeKey()))
        {
            processGxToTAMessage(message);
        }
    }
    catch (...)
    {
        LOG_EXCEPTION("Unknown", "Caught some error when processing specialised message");
    }
}

void GraphworxMessageHandler::processTAToGxMessage(const TA_Base_Core::CommsMessageCorbaDef& message)
{
    TA_Base_Core::IControlStationCorbaDef::TAToGXMessage* data;

    if ((message.messageState >>= data) == 0)
    {
        LOG_DEBUG("Received a TAToGXMessageNotification but couldn't extract TAToGXMessage.  Ignore");
        return;
    }

    LOG_INFO("Received a TAToGXMessageNotification message");
    TAToGxMessageInformation* pGxMsg = new TAToGxMessageInformation;
    pGxMsg->targetSessionID = data->targetSessionID;
    pGxMsg->targetSchematicID = data->targetSchematicID;
    pGxMsg->sourceEntityName = data->sourceEntityName;
    pGxMsg->message = data->message;
    GraphworxSignal::ta2GxMessage(pGxMsg);
}

void GraphworxMessageHandler::processGxToTAMessage(const TA_Base_Core::CommsMessageCorbaDef& message)
{
    try
    {
        LOG_INFO("Received a GXToTAMessageNotification message");
        TA_Base_Core::IManagedApplicationCorbaDef::GXToTAMessage* msgGx2Ta;

        if ((message.messageState >>= msgGx2Ta) == 0)
        {
            LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, "Failed to interpret message");
            return;
        }

        if (msgGx2Ta->entityType == TA_Base_Core::IManagedApplicationCorbaDef::ControlStation)
        {
            GraphworxSignal::gx2TaMessage(new TA_Base_Core::IManagedApplicationCorbaDef::GXToTAMessage(*msgGx2Ta));
        }
    }
    catch (...)
    {
        LOG_EXCEPTION("Unknown", "Caught some error when processing specialized message");
    }
}
