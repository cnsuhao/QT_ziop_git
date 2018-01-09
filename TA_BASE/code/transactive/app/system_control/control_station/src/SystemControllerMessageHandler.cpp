#include "StdAfx.h"
#include "SystemControllerMessageHandler.h"
#include "DataCache.h"
#include "Signals.h"
#include "SessionSignal.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/message/types/SystemControllerComms_MessageTypes.h"
#include "core/utilities/src/RunParams.h"

using namespace TA_Base_Core;

SystemControllerMessageHandler::SystemControllerMessageHandler()
    : m_isGroupOnline(true)
{
    setupSignalConnections();
}

void SystemControllerMessageHandler::setupSignalConnections()
{
    SessionSignal::logined.connect(boost::bind(&SystemControllerMessageHandler::subscribMessage, this));
    SessionSignal::logouted.connect(boost::bind(&SystemControllerMessageHandler::unsubscribMessage, this));
}

void SystemControllerMessageHandler::asyncInitialize()
{
    std::string systemControllerName = TA_Base_Core::RunParams::getInstance().promiseGet(RPARAM_SYSTEMCONTROLLERNAME);
    m_systemControllerEntity = DataCache::getInstance().getEntity(systemControllerName);
}

void SystemControllerMessageHandler::subscribMessage()
{
    MessageSubscriptionManager::getInstance().subscribeToCommsMessage(TA_Base_Core::SystemControllerComms::SystemControllerGroupOnline, this, 0, 0, m_systemControllerEntity->getLocation(), NULL);
    MessageSubscriptionManager::getInstance().subscribeToCommsMessage(TA_Base_Core::SystemControllerComms::SystemControllerGroupOffline, this, 0, 0, m_systemControllerEntity->getLocation(), NULL);
}

void SystemControllerMessageHandler::unsubscribMessage()
{
    MessageSubscriptionManager::getInstance().unsubscribeToMessages(this);
}

void SystemControllerMessageHandler::receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message)
{
    try
    {
        //
        // messages from the system controller
        //
        char const* groupName = NULL;
        bool isGroupOnline = false;
        std::string mesgTypeKey = message.messageTypeKey;

        if ((message.messageState >>= groupName) == 0)
        {
            LOG_ERROR("Could not retrieve the group string for SystemControllerGroupOnline message");
            groupName = "Unknown";
        }

        // if we got an online message and we're already know that, then just ignore, otherwise notify
        if (mesgTypeKey == TA_Base_Core::SystemControllerComms::SystemControllerGroupOnline.getTypeKey())
        {
            isGroupOnline = true;
            LOG_INFO("Received a SystemControllerGroupOnline message");
        }

        // if we got an offline message and we're already know that, then just ignore, otherwise notify
        if (mesgTypeKey == TA_Base_Core::SystemControllerComms::SystemControllerGroupOffline.getTypeKey())
        {
            isGroupOnline = false;
            LOG_INFO("Received a SystemControllerGroupOffline message");
        }

        // tell the control station root dialog to display notification
        // update the session id of all
        m_isGroupOnline = isGroupOnline;
        Signals::systemControllerStateChanged(isGroupOnline, groupName);

        //++lizettejl
        // Added for TD 6950: Some GUIs need to know when the system is in degraded mode.
        // Refresh the run params as the group online value would have changed
        // TODO:
        //retrieveRunParamsFromSystemController(m_hostname);
    }
    catch (...)
    {
        LOG_EXCEPTION("Unknown", "Caught some error when processing specialised message");
    }
}
