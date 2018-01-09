#include "StdAfx.h"
#include "ControlStationOnlineUpdateHandler.h"
#include "Signals.h"
#include "DataCache.h"
#include "ControlStationConstants.h"
#include "bus/TA_MessageResource/Resource.h"
#include "core/configuration_updates/src/OnlineUpdateListener.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "core/data_access_interface/alarm_rule/src/AlarmRuleAccessFactory.h"
#include "core/utilities/src/RunParams.h"

using namespace TA_Base_Core;
using namespace TA_Base_App;

ControlStationOnlineUpdateHandler::ControlStationOnlineUpdateHandler()
{
    setupSignalConnections();
}

void ControlStationOnlineUpdateHandler::setupSignalConnections()
{
    //TODO
}

void ControlStationOnlineUpdateHandler::asyncInitialize()
{
    m_controlStationEntity = DataCache::getInstance().getControlStationEntity();

    RunParams::getInstance().set(TA_ControlStation::DEBUG_LOG_DIR, m_controlStationEntity->getDebugLogDir());

    if (RunParams::getInstance().get(RPARAM_DEBUGFILE).empty())
    {
        boost::filesystem::path debugPath = boost::filesystem::system_complete(RunParams::getInstance().get(TA_ControlStation::DEBUG_LOG_DIR));

        if (debugPath.size())
        {
            debugPath /= TA_ControlStation::LOG_PRE_STRING + m_controlStationEntity->getName() + TA_ControlStation::LOG_POST_STRING;
            DebugUtil::getInstance().setFile(debugPath.string());
        }
    }

    if (m_controlStationEntity->getUserPreferences())
    {
        RunParams::getInstance().set(RPARAM_USERPREFERENCES, "on");
    }

    RunParams::getInstance().set(TA_ControlStation::ACCESS_CONTROL_POPUP_TIMEOUT, m_controlStationEntity->getAccessControlPopupTimeout());

    //TD16984 - the runparam string should not be sent if value is empty
    if (m_controlStationEntity->getMmsSubmitterType().size())
    {
        RunParams::getInstance().set(AlarmRuleAccessFactory::RPARAM_MMS_SUBMITTER_TYPE, m_controlStationEntity->getMmsSubmitterType());
    }

    // TD6972 if the group offline run param is set send an update message. The group is online we
    // don't need to send anything
    std::string param = TA_Base_Core::RunParams::getInstance().get(RPARAM_GROUPSOFFLINE);

    if (!param.size() && (param != "GroupOnline"))
    {
        Signals::systemControllerStateChanged(false, "");
    }

    OnlineUpdateListener::getInstance().registerInterest(TA_Base_Core::ENTITY_OWNER, *this, m_controlStationEntity->getKey());
}

void ControlStationOnlineUpdateHandler::processUpdate(const ConfigUpdateDetails& updateEvent)
{
    FUNCTION_ENTRY("processUpdate");

    switch (updateEvent.getType())
    {
    case TA_Base_Core::ENTITY_OWNER:
    {
        if (updateEvent.getKey() != m_controlStationEntity->getKey())
        {
            LOG_INFO("This is not control station update, ignore");
            FUNCTION_EXIT;
            return;
        }

        if (updateEvent.getModifications() == TA_Base_Core::Create)
        {
            LOG_WARNING("Got 'Create' update, ignore");
            FUNCTION_EXIT;
            return;
        }

        if (updateEvent.getModifications() == TA_Base_Core::Delete)
        {
            LOG_WARNING("Received a 'Delete' update the control station");
            UINT selectedButton = showMsgBox(IDS_UW_020003);
            FUNCTION_EXIT;
            return;
        }

        const std::vector< std::string> changes = updateEvent.getChangedParams();

        if (changes.empty())
        {
            LOG_INFO("Got no update changes, ignore");
            FUNCTION_EXIT;
            return;
        }

        // update our control station database entity
        m_controlStationEntity->invalidate();

        for (unsigned int i = 0; i < changes.size(); i++)
        {
            if (TA_ControlStation::DEBUG_LOG_DIR == changes[i])
            {
                RunParams::getInstance().set(TA_ControlStation::DEBUG_LOG_DIR.c_str(), m_controlStationEntity->getDebugLogDir().c_str());
                LOG_INFO("DEBUG_LOG_DIR has been changed to %s", m_controlStationEntity->getDebugLogDir().c_str());
            }
            else if (TA_ControlStation::USER_PREFERENCES == changes[i])
            {
                if (true == m_controlStationEntity->getUserPreferences())
                {
                    LOG_INFO("USER_PREFERENCES has been changed to on");
                    RunParams::getInstance().set(RPARAM_USERPREFERENCES, "on");
                }
                else
                {
                    LOG_INFO("RPARAM_USERPREFERENCES has been changed to off");
                    RunParams::getInstance().set(RPARAM_USERPREFERENCES, "");
                }
            }
            else if (TA_ControlStation::ACCESS_CONTROL_POPUP_TIMEOUT == changes[i])
            {
                RunParams::getInstance().set(TA_ControlStation::ACCESS_CONTROL_POPUP_TIMEOUT.c_str(), boost::lexical_cast<std::string>(m_controlStationEntity->getAccessControlPopupTimeout()).c_str());
                LOG_INFO("AccessControlPopupTimeout has been changed to %d", m_controlStationEntity->getAccessControlPopupTimeout());
            }
        }
    }
    break;
    }
}
