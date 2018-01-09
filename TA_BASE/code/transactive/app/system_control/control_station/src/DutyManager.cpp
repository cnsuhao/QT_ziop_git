/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/DutyManager.cpp $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class manages all duty related tasks such as popping out the
  * duty request and notification dialogs
  */
#include "StdAfx.h"
#include "ControlStationRootDialog.h"
#include "SessionManager.h"
#include "IProcessManager.h"
#include "ProcessManager.h"
#include "DutyManager.h"
#include "ExclusiveDutyDialog.h"
#include "OverRideExclusiveDuty.h"
#include "SessionDetails.h"
#include "DatabaseStatusMonitor.h"
#include "resourceQt.h"
#include "DutySignal.h"
#include "SessionSignal.h"
#include "Signals.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/security/authentication_agent/idl/src/SessionInfoCorbaDef.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/data_access_interface/src/IOperator.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/ISubsystem.h"
#include "core/data_access_interface/src/Profile.h"
#include "core/data_access_interface/src/SessionAccessFactory.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/data_access_interface/src/RegionAccessFactory.h"
#include "core/data_access_interface/src/SubsystemAccessFactory.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/entity_access/src/DutyAgentEntityData.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/message/src/MessageSubscriptionManager.h"
#include "core/message/IDL/src/CommsMessageCorbaDef.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/corba/src/CorbaUtil.h"
//#include <boost/range/algorithm/remove_if.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/algorithm/cxx11/one_of.hpp>

using namespace TA_Base_App;
using namespace TA_Base_Bus;
using namespace TA_Base_Core;
using namespace TA_Base_Core::DutyAgentBroadcastComms;

DutyManager::DutyManager()
{
    setupConnection();
}

void DutyManager::setupConnection()
{
    SessionSignal::logined.connect(boost::bind(&DutyManager::onSessionLogin, this));
    SessionSignal::logouted.connect(boost::bind(&DutyManager::onSessionLogout, this));
    DutySignal::responseDutyAgent.connect(boost::bind(&DutyManager::respond, this, _1, _2));
    DutySignal::loseExclusiveDuty.connect(boost::bind(&DutyManager::handleLoseExclusiveDuty, this, _1));
}

void DutyManager::asyncInitialize()
{
    DatabaseStatusMonitor::getInstance().waitForAnyAvaliable(10000);

    // Get the entity name based on the locationKey
    TA_ASSERT(TA_Base_Core::RunParams::getInstance().promiseIsSet(RPARAM_LOCATIONKEY, 10000), "RPARAM_LOCATIONKEY not set");
    unsigned long locationKey = TA_Base_Core::getRunParamValue(RPARAM_LOCATIONKEY, 0);

    try
    {
        // this will only return a single entry
        CorbaNameList dutyAgentName = EntityAccessFactory::getInstance().getCorbaNamesOfTypeAtLocation(DutyAgentEntityData::getStaticType(), locationKey, true);
        TA_ASSERT(dutyAgentName.size(), "Cannot find duty agent");

        // If more than one name was returned for the duty agent, then that's a bad thing (it's a theoretically
        // impossible situation, but as there is little we can do to resolve it, we'll just use the first one
        // on the list. Need to log the problem though.
        if (dutyAgentName.size() != 1)
        {
            LOG_ERROR("More than one duty agent name resolved for location key %d.", locationKey);
            LOGMORE(SourceInfo, "Will use first name provided: %s", dutyAgentName[0]);
        }

        // Set the agent name - this is the name of the agent to contact.
        m_dutyAgentQuery.setCorbaName(dutyAgentName[0]);
    }
    catch (...)
    {
        // Okay, if the name of the duty agent can't be found in the database, then the processing shouldn't really
        // reach hear (the Auth Agent won't run without a duty agent at the very least), however, if it DOES then
        // just log it (the duty agent object is only used for informational purposes, and so the system can
        // still run without it.
        LOG_EXCEPTION("General Excpetion", "Exception caught while retrieving database information for the Duty Agent.");
    }
}

void DutyManager::onSessionLogin()
{
    waitAsyncInitialize();
    TA_Base_Core::MessageSubscriptionManager::getInstance().subscribeToBroadcastCommsMessage(TA_Base_Core::DutyAgentBroadcastComms::DutyRequest, this, NULL);
    TA_Base_Core::MessageSubscriptionManager::getInstance().subscribeToBroadcastCommsMessage(TA_Base_Core::DutyAgentBroadcastComms::DutyChangedNotification, this, NULL);
}

void DutyManager::onSessionLogout()
{
    m_subscriptions.removeAllSubscriptions();
}

void DutyManager::receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message)
{
    //
    // message from the duty agent
    //
    LOG_INFO("Got a Comms message.");

    std::string mesgTypeKey = message.messageTypeKey;

    if (mesgTypeKey == DutyAgentBroadcastComms::DutyChangedNotification.getTypeKey())
    {
        LOG_INFO("Processing duty change message.");
        // need to display notification dialog
        processDutyChange(message);
    }
    else if (mesgTypeKey == DutyAgentBroadcastComms::DutyRequest.getTypeKey())
    {
        LOG_INFO("Processing duty update message.");
        // need to display request duty dialog
        processDutyRequest(message);
    }
}

void DutyManager::respond(const std::string& uniqueId, const TA_Base_App::TA_ControlStation::EDutyResponse response)
{
    ThreadGuard guard(m_callbackLock);

    // see if it is already in the map, if it is, then we've already done the request so just ignore
    if (m_callback.find(uniqueId) == m_callback.end())
    {
        LOG_DEBUG("Unique Id %d not in map.  Ignore.", uniqueId);
        return;
    }

    // match our internal EDutyResponse to TA_Base_Bus::IDutyAgentRespCorbaDef::EDutyResponse rather
    // than typecasting it just in case it's in a different order

    static std::map<TA_ControlStation::EDutyResponse, IDutyAgentRespCorbaDef::EDutyResponse> theMap =
    {
        { TA_ControlStation::ACCEPT, IDutyAgentRespCorbaDef::DR_ACCEPT },
        { TA_ControlStation::DENY, IDutyAgentRespCorbaDef::DR_DENY },
        { TA_ControlStation::TIMEOUT, IDutyAgentRespCorbaDef::DR_TIMEOUT }
    };

    m_callback[uniqueId]->respResponse(uniqueId.c_str(), theMap[response]);
    m_callback.erase(uniqueId);
}

// TD19075
void DutyManager::processDutyChange(const TA_Base_Core::CommsMessageCorbaDef& message)
{
    TA_Base_Bus::DutyAgentTypeCorbaDef::DutyPopup* pDutyPopup = NULL;
    TA_Base_Bus::DutyAgentTypeCorbaDef::DutyPopupSequence* pDutySeq = NULL;
    DutyNotificationDetailPtr detail(new DutyNotificationDetail);
    std::string currentId = SessionDetails::getInstance().getSessionId();

    if (message.messageState >>= pDutyPopup)
    {
        if (currentId != pDutyPopup->targetSession.in())
        {
            LOG_INFO("Received a DutyChangedNotification for DutyPopup, but doesn't affect current session.  Ignore [%s]", pDutyPopup->targetSession.in());
            return;
        }

        repackageDutyData(pDutyPopup->gained, detail->gained);
        repackageDutyData(pDutyPopup->denied, detail->denied);
        repackageDutyData(pDutyPopup->lost, detail->lost);

        if (detail->dutyChanged())
        {
            DutySignal::dutyChanged2(detail, 0);
        }
    }
    else if (message.messageState >>= pDutySeq)
    {
        TA_Base_Bus::DutyAgentTypeCorbaDef::DutyPopupSequence& dutySeq = *pDutySeq;
        std::vector<std::string> targetSessions;

        for (size_t i = 0; i < dutySeq.length(); ++i)
        {
            if (currentId == dutySeq[i].targetSession.in())
            {
                repackageDutyData(dutySeq[i].gained, detail->gained);
                repackageDutyData(dutySeq[i].denied, detail->denied);
                repackageDutyData(dutySeq[i].lost, detail->lost);
            }
            else
            {
                targetSessions.emplace_back(dutySeq[i].targetSession.in());
            }
        }

        if (dutySeq.length() && detail->empty())
        {
            LOG_INFO("Received a DutyChangedNotification for DutySequence, but doesn't affect current session.  Ignore [%s]", boost::join(targetSessions, ", ").c_str());
            return;
        }

        // get duty changed data
        LOG_INFO("[processDutyChange] before getDutyChanged, %s", detail->str().c_str());

        getDutyChanged(*detail);

        LOG_INFO("[processDutyChange] after getDutyChanged, %s", detail->str().c_str());

        if (detail->dutyChanged())
        {
            DutySignal::dutyChanged2(detail, 1);
        }
    }
    else
    {
        LOG_INFO("Received a DutyChangedNotification but couldn't extract DutyPopup or DutySequence. Ignore");
        return;
    }

    // doesn't affects the current session, ignore
    LOG_INFO("Received a DutyChangedNotification message");

    if (detail->dutyChanged())
    {
        // Force the applications to update since we've gained or lost duty
        Signals::sessionIdChanged(currentId);
        //SessionManager::getInstance().getProcessManager()->updateSessionId(currentId);

        // There is now a dedicated call for duty updates, so will remove the above call come IT21
        //SessionManager::getInstance().getProcessManager()->dutyChanged();
        Signals::dutyChanged();
    }
    else
    {
        LOG_INFO("Received Duty Change DutyChangedNotification, but no data exist!");
    }
}
// ++TD19075

void DutyManager::repackageDutyData(TA_Base_Bus::DutyAgentTypeCorbaDef::DutyTreeSequence& source, std::vector<DutyDetail>& destination)
{
    for (unsigned int i = 0; i < source.length(); i++)
    {
        DutyDetail duty;
        duty.operatorName = "";
        duty.profileName = "";
        std::string sessionId(source[i].session);

        if (!sessionId.empty())
        {
            try
            {
                //boost::shared_ptr<TA_Base_Core::ISession> session(SessionAccessFactory::getInstance().getSession(sessionId));
                TA_Base_Bus::SessionInfo sessionInfo = SessionManager::getInstance().getSessionInfo(sessionId);
                TA_Base_Core::IOperatorPtr oper(OperatorAccessFactory::getInstance().getOperator(sessionInfo.OperatorId[0], false));

                duty.operatorName = oper->getName();
                TA_Base_Core::ILocationPtr loc(LocationAccessFactory::getInstance().getLocationByKey(sessionInfo.LocationId, false));
                duty.locationName = loc->getDisplayName();
            }
            catch (...)
            {
                LOG_EXCEPTION("Unknown", "Error retrieving session from database");
            }
        }

        if (source[i].profile != ULONG_MAX)
        {
            try
            {
                boost::shared_ptr<TA_Base_Core::IProfile> profile(ProfileAccessFactory::getInstance().getProfile(source[i].profile));
                duty.profileName = profile->getName();
            }
            catch (...)
            {
                LOG_EXCEPTION("Unknown", "Error retrieving profile from database");
            }
        }

        for (unsigned int j = 0; j < source[i].subsystems.length(); j++)
        {
            try
            {
                SubsystemDetail subsystem;
                boost::shared_ptr<TA_Base_Core::IRegion> region(RegionAccessFactory::getInstance().getRegionByKey(source[i].subsystems[j].region));
                subsystem.regionName = region->getDisplayName();

                for (unsigned int k = 0; k < source[i].subsystems[j].subsystems.length(); k++)
                {
                    TA_Base_Core::ISubsystemPtr subsystemData(SubsystemAccessFactory::getInstance().getSubsystemByKey(source[i].subsystems[j].subsystems[k]));
                    subsystem.subsystemNames.insert(subsystemData->getName());
                }

                duty.subsystemDetails.push_back(subsystem);
            }
            catch (...)
            {
                LOG_EXCEPTION("Unknown", "Error retrieving region/subsystem data from database");
            }
        }

        destination.push_back(duty);
    }
}

void DutyManager::processDutyRequest(const TA_Base_Core::CommsMessageCorbaDef& message)
{
    std::string currentId = SessionDetails::getInstance().getSessionId();
    TA_Base_Bus::DutyAgentTypeCorbaDef::DutyRequest* data;

    // can't extract to the DutyRequest, ignore
    if ((message.messageState >>= data) == 0)
    {
        LOG_DEBUG("Received a DutyRequest but couldn't extract DutyRequest.  Ignore");
        return;
    }

    // doesn't affects the current session, ignore
    if (currentId.compare(data->targetSession) != 0)
    {
        LOG_DEBUG("Received a DutyRequest but doesn't affect current session.  Ignore");
        return;
    }

    if (data->duty.subsystems.length() == 0)
    {
        LOG_INFO("Received a DutyRequest message but doesn't have subsystem details.  Ignore.");
        return;
    }

    // set up the map for the callback
    {
        ThreadGuard guard(m_callbackLock);

        // see if it is already in the map, if it is, then we've already done the request so just ignore
        if (m_callback.find(data->uniqueId.in()) != m_callback.end())
        {
            LOG_INFO("Received a DutyRequest message but we've already received a message with the same unique id %D.  Ignore.", data->uniqueId.in());
            return;
        }

        LOG_INFO("Received a DutyRequest message");
        m_callback.emplace(data->uniqueId.in(), data->dutyAgent);
    }

    try
    {
        // process and repackage the data so that the GUI could use it straightaway

        //boost::shared_ptr<TA_Base_Core::ISession> session(SessionAccessFactory::getInstance().getSession(std::string(data->duty.session)));
        std::string sessionId = data->duty.session.in();
        TA_Base_Bus::SessionInfo sessionInfo = SessionManager::getInstance().getSessionInfo(sessionId);

        TA_Base_Core::IProfilePtr profile(ProfileAccessFactory::getInstance().getProfile(data->duty.profile));
        // TD14986 should not use session location instead using console location
        TA_Base_Core::IEntityDataPtr consoleEntity(EntityAccessFactory::getInstance().getEntity(sessionInfo.WorkstationId));
        TA_Base_Core::ILocationPtr location(LocationAccessFactory::getInstance().getLocationByKey(consoleEntity->getLocation()));
        TA_Base_Core::IOperatorPtr oper(OperatorAccessFactory::getInstance().getOperator(sessionInfo.OperatorId[0], false));
        DutyRequestDetail* detail = new DutyRequestDetail;
        detail->uniqueId = data->uniqueId.in();
        detail->duty.operatorName = oper->getName();
        detail->duty.profileName = profile->getName();
        detail->duty.locationName = location->getDisplayName();

        for (unsigned int j = 0; j < data->duty.subsystems.length(); j++)
        {
            SubsystemDetail subsystem;
            TA_Base_Core::IRegionPtr region(RegionAccessFactory::getInstance().getRegionByKey(data->duty.subsystems[j].region));
            subsystem.regionName = region->getDisplayName();

            for (unsigned int k = 0; k < data->duty.subsystems[j].subsystems.length(); k++)
            {
                boost::shared_ptr<TA_Base_Core::ISubsystem> subsystemData(SubsystemAccessFactory::getInstance().getSubsystemByKey(data->duty.subsystems[j].subsystems[k]));
                subsystem.subsystemNames.insert(subsystemData->getName());
            }

            detail->duty.subsystemDetails.emplace_back(std::move(subsystem));
        }

        ControlStationRootDialog::getInstance().postMessage(WM_DUTY_REQUEST, reinterpret_cast<WPARAM>(detail));
    }
    catch (...)
    {
        LOG_EXCEPTION("Unknown", "Error accessing database");
    }
}

void DutyManager::handleLoseExclusiveDuty(BoolPromisePtr promise)
{
    promise->set_value(loseExclusiveDuty());
}

bool DutyManager::loseExclusiveDuty()
{
    TA_Base_Bus::DutyAgentTypeCorbaDef::SessionId_var sessionId = SessionDetails::getInstance().getSessionId().c_str();
    TA_Base_Bus::DutyAgentTypeCorbaDef::SubsystemTreeSequence_var exclusiveSubsystems;
    TA_Base_Bus::IAuthenticationAgentCorbaDef::SessionInfoCorbaDef corbaSession = SessionDetails::getInstance().toCorbaSessionInfo();
    bool hasDuty = false;

    try
    {
        CORBACALL((hasDuty = m_dutyAgentQuery->queryHasExclusiveDuty(corbaSession, exclusiveSubsystems)), m_dutyAgentQuery);
    }
    catch (...)
    {
        // If there is an error, then the duty agent is unavailable, and the duties cannot be determined,
        // so just log out
        LOG_EXCEPTION("General Exception", "An exception was caught while attempting to retrieve duty information from the Duty Agent.");
        return true;
    }

    if (!hasDuty)
    {
        // There are no exclusive subsystem duties held by this session, so can exit immediately
        LOG_INFO("Session %s has no exclusive duties.", sessionId);
        return true;
    }
    else
    {
        // The tree contains exclusive subsystems, so need to notify the user.
        std::vector<SubsystemDetail> subsystems;

        for (unsigned int j = 0; j < exclusiveSubsystems->length(); j++)
        {
            // Ignore empty entries
            if (exclusiveSubsystems[j].subsystems.length() == 0)
            {
                continue;
            }

            try
            {
                SubsystemDetail subsystemDetails;
                TA_Base_Core::IRegionPtr region(RegionAccessFactory::getInstance().getRegionByKey(exclusiveSubsystems[j].region));
                subsystemDetails.regionName = region->getDisplayName();

                for (unsigned int k = 0; k < exclusiveSubsystems[j].subsystems.length(); k++)
                {
                    TA_Base_Core::ISubsystemPtr subsystem(SubsystemAccessFactory::getInstance().getSubsystemByKey(exclusiveSubsystems[j].subsystems[k]));
                    subsystemDetails.subsystemNames.insert(subsystem->getName());
                }

                subsystems.push_back(subsystemDetails);
            }
            catch (...)
            {
                LOG_EXCEPTION("Unknown", "Error retrieving region/subsystem data from database");
            }
        }

        CString subsystemsAtLocations;
        CString locationStr = IDS_LOCATION;
        CString subsystemStr = IDS_SUBSYSTEM;

        for (unsigned int i = 0; i < subsystems.size(); i++)
        {
            subsystemsAtLocations += _T("\n") + locationStr + _T(":\t");
            subsystemsAtLocations += subsystems[i].regionName.c_str();
            subsystemsAtLocations += _T("\n") + subsystemStr + _T("\t");

            for (const std::string& name : subsystems[i].subsystemNames)
            {
                subsystemsAtLocations += name.c_str();
                subsystemsAtLocations += _T("\n\t\t");
            }

            subsystemsAtLocations += _T("\n");
        }

        if (SessionDetails::getInstance().isOperatorOverridden())
        {
            LOG_INFO("yg0606 isOverride has no exclusive duties.");
            OverRideExclusiveDuty exclusiveDutyDialog((const char*)subsystemsAtLocations);
            return exclusiveDutyDialog.doModal() != QDialog::Rejected;
        }
        else
        {
            return true;
        }
    }

    return false;
}

void DutyManager::getDutyChanged(DutyNotificationDetail& detail)
{
    DutyDetailList& gained = detail.gained;
    DutyDetailList& lost = detail.lost;
    DutyDetailList& changed = detail.changed;

    for (DutyDetail& iterGain : detail.gained)
    {
        for (DutyDetail& iterLost : detail.lost)
        {
            DutyDetail details = iterGain;

            for (SubsystemDetail& iterGainSub : iterGain.subsystemDetails)
            {
                for (SubsystemDetail& iterLostSub : iterLost.subsystemDetails)
                {
                    // Handle SubsystemDetail(region, subsystems<set>)
                    SubsystemDetail sDetail;
                    sDetail.regionName = iterLostSub.regionName;

                    if (iterGainSub.regionName == iterLostSub.regionName)
                    {
                        for (const std::string& iterGainSubsys : iterGainSub.subsystemNames)
                        {
                            if (iterLostSub.subsystemNames.erase(iterGainSubsys))
                            {
                                LOG_INFO("Duty Change at: %s --> %s", sDetail.regionName.c_str(), iterGainSubsys.c_str()); sDetail.subsystemNames.insert(iterGainSubsys);
                            }
                        }

                        // do delete from Gain
                        LOG_INFO("Delete all Gain SubSytem");

                        for (const std::string& subsystem : sDetail.subsystemNames)
                        {
                            iterGainSub.subsystemNames.erase(subsystem);
                        }
                    }

                    if (sDetail.subsystemNames.size())
                    {
                        LOG_INFO("Add change details subsystem for region:%s   systemcount:%d", sDetail.regionName.c_str(), sDetail.subsystemNames.size());
                        details.subsystemDetails.push_back(sDetail);
                    }
                }
            }

            auto subsystemNamesEmpty = [](const SubsystemDetail & detail) { return detail.subsystemNames.empty(); };

            // delete all empty subsystem
            LOG_INFO("Del all empty gained subsystem");
            boost::remove_erase_if(iterGain.subsystemDetails, subsystemNamesEmpty);

            LOG_INFO("Del all empty lost subsystem");
            boost::remove_erase_if(iterLost.subsystemDetails, subsystemNamesEmpty);

            // add change detail
            if (details.subsystemDetails.size())
            {
                LOG_INFO("Add changed for:Loc:%s\tUser:%s\tProFile:%s", details.locationName.c_str(), details.operatorName.c_str(), details.profileName.c_str());
                changed.push_back(details);
            }
        }
    }

    auto subsystemDetailsEmpty = [](const DutyManager::DutyDetail & detail) { return detail.subsystemDetails.empty(); };

    LOG_INFO("Del all empty gained dutydetail");
    boost::remove_erase_if(gained, subsystemDetailsEmpty);

    LOG_INFO("Del all empty lost dutydetail");
    boost::remove_erase_if(lost, subsystemDetailsEmpty);
}
