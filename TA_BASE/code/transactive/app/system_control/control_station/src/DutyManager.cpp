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
#include "Signals.h"
#include "SessionSignal.h"
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
    Signals::responseDutyAgent.connect(boost::bind(&DutyManager::respond, this, _1, _2));
    Signals::loseExclusiveDuty.connect(boost::bind(&DutyManager::handleLoseExclusiveDuty, this, _1));
}

void DutyManager::asyncInitialize()
{
    DatabaseStatusMonitor::getInstance().waitForAnyAvaliable(10000);

    // Get the entity name based on the locationKey
    TA_ASSERT(TA_Base_Core::RunParams::getInstance().promiseIsSet(RPARAM_LOCATIONKEY, 10000), "RPARAM_LOCATIONKEY unset");
    unsigned long locationKey = TA_Base_Core::getRunParamValue(RPARAM_LOCATIONKEY, 0);
    std::string agentType = TA_Base_Core::DutyAgentEntityData::getStaticType();

    try
    {
        // this will only return a single entry
        TA_Base_Core::CorbaNameList dutyAgentName = TA_Base_Core::EntityAccessFactory::getInstance().getCorbaNamesOfTypeAtLocation(agentType, locationKey, true);
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
    DutyManager::getInstance();
    TA_Base_Core::MessageSubscriptionManager::getInstance().subscribeToBroadcastCommsMessage(TA_Base_Core::DutyAgentBroadcastComms::DutyRequest, this, NULL);
    TA_Base_Core::MessageSubscriptionManager::getInstance().subscribeToBroadcastCommsMessage(TA_Base_Core::DutyAgentBroadcastComms::DutyChangedNotification, this, NULL);
}

void DutyManager::onSessionLogout()
{
    m_subscriptions.removeAllSubscriptions();
}

void DutyManager::receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message)
{
    FUNCTION_ENTRY("receiveSpecialisedMessage");

    try
    {
        //
        // message from the duty agent
        //
        LOG_INFO("Got a Comms message.");

        std::string mesgTypeKey = message.messageTypeKey;

        if (mesgTypeKey == TA_Base_Core::DutyAgentBroadcastComms::DutyChangedNotification.getTypeKey())
        {
            LOG_INFO("Processing duty change message.");
            // need to display notification dialog
            processDutyChange(message);
        }
        else if (mesgTypeKey == TA_Base_Core::DutyAgentBroadcastComms::DutyRequest.getTypeKey())
        {
            LOG_INFO("Processing duty update message.");
            // need to display request duty dialog
            processDutyRequest(message);
        }
    }
    catch (...)
    {
        LOG_EXCEPTION("Unknown", "Caught some error when processing specialised message");
    }

    FUNCTION_EXIT;
}

void DutyManager::respond(const std::string& uniqueId, const TA_Base_App::TA_ControlStation::EDutyResponse response)
{
    ThreadGuard guard(m_callbackLock);

    // see if it is already in the map, if it is, then we've already done the request so just ignore
    if (m_callback.find(uniqueId) == m_callback.end())
    {
        LOG_INFO("Unique Id not in map.  Ignore.");
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
    FUNCTION_ENTRY("processDutyChange");
    std::string currentId = SessionDetails::getInstance().getSessionId();
    LOG_INFO("[processDutyChange]1 Get in ProcessDutyChange, SessionID:%s", currentId.c_str());

    TA_Base_Bus::DutyAgentTypeCorbaDef::DutyPopup* pDutyPopup;
    TA_Base_Bus::DutyAgentTypeCorbaDef::DutyPopupSequence* pDutySeq;
    // process and repackage the data so that the GUI could use it straightaway
    DutyNotificationDetail* det = new struct DutyNotificationDetail;
    bool isDutyUpdated;

    if ((message.messageState >>= pDutyPopup) != 0)   // for DutyPopup
    {
        // MessageBox(NULL, "Get DutyPopup Message!", "processDutyChange", MB_OK);
        // check session ID
        if (currentId.compare(pDutyPopup->targetSession) != 0)
        {
            LOG_INFO("Received a DutyChangedNotification for DutyPopup, but doesn't affect current session.  Ignore[%s]", pDutyPopup->targetSession.in());
            FUNCTION_EXIT;
            return;
        }

        // repackage data
        repackageDutyData(pDutyPopup->gained, det->gained);
        repackageDutyData(pDutyPopup->denied, det->denied);
        repackageDutyData(pDutyPopup->lost, det->lost);
        det->changed.clear();

        isDutyUpdated = ((det->gained.size() > 0) || (det->denied.size() > 0) || (det->lost.size() > 0) || (det->changed.size() > 0));

        if (isDutyUpdated)
        {
            ControlStationRootDialog::getInstance().postMessage(WM_DUTY_CHANGE, reinterpret_cast<WPARAM>(det), 0);
        }
    }
    else if ((message.messageState >>= pDutySeq) != 0)   // for DutySeq
    {
        // MessageBox(NULL, "Get DutySequence Message!", "processDutyChange", MB_OK);
        TA_Base_Bus::DutyAgentTypeCorbaDef::DutyPopupSequence& dutySeq = *pDutySeq;

        int nDutyDetails = 0;
        unsigned int iIndex = 0;

        for (iIndex = 0; iIndex < dutySeq.length(); ++iIndex)
        {
            if (currentId.compare(dutySeq[iIndex].targetSession) == 0)
            {
                ++nDutyDetails;
            }
        }

        if (0 == nDutyDetails)
        {
            LOG_INFO("Received a DutyChangedNotification for DutySequence, but doesn't affect current session.  Ignore[%s]", dutySeq[iIndex].targetSession.in());
            FUNCTION_EXIT;
            return;
        }
        else
        {
            LOG_INFO("[processDutyChange]3 get DutyDetail:%d", nDutyDetails);
        }

        // check session ID and repackage data
        for (unsigned int i = 0; i < dutySeq.length(); ++i)
        {
            if (currentId.compare(dutySeq[i].targetSession) != 0)
            {
                LOG_INFO("One of DutyDetail is on concern with current session, index:%d, sessionID:%s", i, dutySeq[i].targetSession.in());
                continue;
            }

            repackageDutyData(dutySeq[i].gained, det->gained);
            repackageDutyData(dutySeq[i].denied, det->denied);
            repackageDutyData(dutySeq[i].lost, det->lost);
        }

        // get duty changed data
        LOG_INFO("[processDutyChange] before getDutyChanged, Gained:%d   Lost:%d   Deined:%d   Changed:%d",
                 det->gained.size(), det->lost.size(), det->denied.size(), det->changed.size());

        getDutyChanged(det);

        LOG_INFO("[processDutyChange] after getDutyChanged, Gained:%d Lost:%d Deined:%d Changed:%d", det->gained.size(), det->lost.size(), det->denied.size(), det->changed.size());

        isDutyUpdated = ((det->gained.size() > 0) || (det->denied.size() > 0) || (det->lost.size() > 0) || (det->changed.size() > 0));

        if (isDutyUpdated)
        {
            ControlStationRootDialog::getInstance().postMessage(WM_DUTY_CHANGE, reinterpret_cast<WPARAM>(det), 1);
        }
    }
    else
    {
        LOG_INFO("Received a DutyChangedNotification but couldn't extract DutyPopup or DutySequence. Ignore");
        FUNCTION_EXIT;
        return;
    }

    // doesn't affects the current session, ignore
    LOG_INFO("Received a DutyChangedNotification message");

    if (isDutyUpdated)
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

    FUNCTION_EXIT;
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
                    subsystem.subsystemName.push_back(subsystemData->getName());
                }

                duty.subsystem.push_back(subsystem);
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
    FUNCTION_ENTRY("processDutyRequest");

    std::string currentId = SessionDetails::getInstance().getSessionId();
    TA_Base_Bus::DutyAgentTypeCorbaDef::DutyRequest* data;

    // can't extract to the DutyRequest, ignore
    if ((message.messageState >>= data) == 0)
    {
        LOG_DEBUG("Received a DutyRequest but couldn't extract DutyRequest.  Ignore");
        FUNCTION_EXIT;
        return;
    }

    // doesn't affects the current session, ignore
    if (currentId.compare(data->targetSession) != 0)
    {
        LOG_DEBUG("Received a DutyRequest but doesn't affect current session.  Ignore");
        FUNCTION_EXIT;
        return;
    }

    if (data->duty.subsystems.length() == 0)
    {
        LOG_INFO("Received a DutyRequest message but doesn't have subsystem details.  Ignore.");
        FUNCTION_EXIT;
        return;
    }

    // set up the map for the callback
    std::string uniqueId = data->uniqueId;
    {
        ThreadGuard guard(m_callbackLock);

        // see if it is already in the map, if it is, then we've already done the request so just ignore
        if (m_callback.find(uniqueId) != m_callback.end())
        {
            LOG_INFO("Received a DutyRequest message but we've already received a message with the same unique id.  Ignore.");
            return;
        }

        LOG_INFO("Received a DutyRequest message");

        // add it to the map
        m_callback.insert(CallbackMap::value_type(uniqueId, data->dutyAgent));
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
        DutyRequestDetail* det = new struct DutyRequestDetail;
        det->uniqueId = uniqueId;
        det->duty.operatorName = oper->getName();
        det->duty.profileName = profile->getName();
        det->duty.locationName = location->getDisplayName();

        for (unsigned int j = 0; j < data->duty.subsystems.length(); j++)
        {
            SubsystemDetail subsystem;
            TA_Base_Core::IRegionPtr region(RegionAccessFactory::getInstance().getRegionByKey(data->duty.subsystems[j].region));
            subsystem.regionName = region->getDisplayName();

            for (unsigned int k = 0; k < data->duty.subsystems[j].subsystems.length(); k++)
            {
                boost::shared_ptr<TA_Base_Core::ISubsystem> subsystemData(SubsystemAccessFactory::getInstance().getSubsystemByKey(data->duty.subsystems[j].subsystems[k]));
                subsystem.subsystemName.push_back(subsystemData->getName());
            }

            det->duty.subsystem.push_back(subsystem);
        }

        ControlStationRootDialog::getInstance().postMessage(WM_DUTY_REQUEST, reinterpret_cast<WPARAM>(det));
    }
    catch (...)
    {
        LOG_EXCEPTION("Unknown", "Error accessing database");
    }

    FUNCTION_EXIT;
}

void DutyManager::handleLoseExclusiveDuty(boost::shared_ptr<Promise<bool> > promise)
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
                    subsystemDetails.subsystemName.push_back(subsystem->getName());
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

            for (unsigned int j = 0; j < subsystems[i].subsystemName.size(); j++)
            {
                subsystemsAtLocations += subsystems[i].subsystemName[j].c_str();
                subsystemsAtLocations += _T("\n\t\t");
            }

            subsystemsAtLocations += _T("\n");
        }

        // TD19409 yg++
        if (SessionDetails::getInstance().isOperatorOverridden())
        {
            LOG_INFO("yg0606 isOverride has no exclusive duties.");
            OverRideExclusiveDuty exclusiveDutyDialog((const char*)subsystemsAtLocations);

            if (exclusiveDutyDialog.doModal() == QDialog::Rejected)
            {
                return false;
            }

            return true;
        }
        else // ++yg
        {
            //TD15531++ [2012-06-06]
            //ExclusiveDutyDialog exclusiveDutyDialog((LPCTSTR)subsystemsAtLocations);

            //if (exclusiveDutyDialog.doModal() == QDialog::Rejected)
            //{
            //    return false;
            //}

            return true;
        }

        /*TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << subsystemsAtLocations;
        UINT selectedButton = userMsg.showMsgBox(IDS_UW_020008);

        if ( selectedButton == IDNO )
        {
            return false;
        }
        return true;*/
        //++TD15531
    }

    return false;
}

// TD19075++
inline bool IsSubsystemEmpty(DutyManager::SubsystemDetail sd)
{
    return sd.subsystemName.empty();
}

inline bool IsDutyDetailEmpty(DutyManager::DutyDetail dd)
{
    return dd.subsystem.empty();
}

void DutyManager::getDutyChanged(DutyNotificationDetail* det)
{
    DutyDetailList& gained = det->gained;
    DutyDetailList& lost = det->lost;
    DutyDetailList& changed = det->changed;

    for (DutyDetailIter iterGain = gained.begin(); iterGain != gained.end(); ++iterGain)
    {
        for (DutyDetailIter iterLost = lost.begin(); iterLost != lost.end(); ++iterLost)
        {
            DutyDetail details;
            details.locationName = iterGain->locationName;
            details.operatorName = iterGain->operatorName;
            details.profileName = iterGain->profileName;
            //if (iterGain->operatorName == iterLost->operatorName)
            {
                for (SubsystemDetailIter iterGainSub = iterGain->subsystem.begin(); iterGainSub != iterGain->subsystem.end(); ++iterGainSub)
                {
                    for (SubsystemDetailIter iterLostSub = iterLost->subsystem.begin(); iterLostSub != iterLost->subsystem.end(); ++iterLostSub)
                    {
                        // Handle SubsystemDetail(region, subsystems<set>)
                        SubsystemDetail sDetail;
                        sDetail.regionName = iterLostSub->regionName;

                        if (iterGainSub->regionName == iterLostSub->regionName)
                        {
                            std::vector<std::string>::iterator iterLostSubsysEnd = iterLostSub->subsystemName.end();
                            std::vector<std::string>::iterator iterLostSubsysEndOld = iterLostSub->subsystemName.end();

                            for (std::vector<std::string>::iterator iterGainSubsys = iterGainSub->subsystemName.begin(); iterGainSubsys != iterGainSub->subsystemName.end(); ++iterGainSubsys)
                            {
                                iterLostSubsysEnd = std::remove(iterLostSub->subsystemName.begin(), iterLostSubsysEnd, *iterGainSubsys);

                                if (iterLostSubsysEnd != iterLostSubsysEndOld)
                                {
                                    LOG_INFO("Duty Change at:%s-->%s",
                                             sDetail.regionName.c_str(),
                                             iterGainSubsys->c_str());
                                    sDetail.subsystemName.push_back(*iterGainSubsys);
                                }
                            }

                            // do delete from Lost and Gain
                            LOG_INFO("Delete all Lost SubSytem");
                            iterLostSub->subsystemName.erase(iterLostSubsysEnd, iterLostSub->subsystemName.end());
                            // do delete from Gain
                            LOG_INFO("Delete all Gain SubSytem");

                            for (std::vector<std::string>::iterator iterChangeds = sDetail.subsystemName.begin();
                                    iterChangeds != sDetail.subsystemName.end(); ++iterChangeds)
                            {
                                // LOG(DEBUG_INFO,// "Delete gaind SubSytem %s", iterChangeds->c_str());
                                iterGainSub->subsystemName.erase(
                                    std::remove(iterGainSub->subsystemName.begin(),
                                                iterGainSub->subsystemName.end(), *iterChangeds),
                                    iterGainSub->subsystemName.end());
                            }
                        }

                        if (sDetail.subsystemName.size() > 0)
                        {
                            LOG_INFO("Add change details subsystem for region:%s   systemcount:%d",
                                     sDetail.regionName.c_str(), sDetail.subsystemName.size());
                            details.subsystem.push_back(sDetail);
                        }
                    }
                }
            }// end if
            // delete all empty subsystem
            LOG_INFO("Del all empty gained subsystem");
            SubsystemDetailIter gainSubDetailEnd = std::remove_if(
                                                       iterGain->subsystem.begin(), iterGain->subsystem.end(),
                                                       IsSubsystemEmpty);
            iterGain->subsystem.erase(gainSubDetailEnd, iterGain->subsystem.end());

            LOG_INFO("Del all empty lost subsystem");
            SubsystemDetailIter lostSubDetailEnd = std::remove_if(
                                                       iterLost->subsystem.begin(), iterLost->subsystem.end(),
                                                       IsSubsystemEmpty);
            iterLost->subsystem.erase(lostSubDetailEnd, iterLost->subsystem.end());

            // add change detail
            if (details.subsystem.size() > 0)
            {
                LOG_INFO("Add changed for:Loc:%s\tUser:%s\tProFile:%s", details.locationName.c_str(), details.operatorName.c_str(), details.profileName.c_str());
                changed.push_back(details);
            }
        }
    }

    LOG_INFO("Del all empty gained dutydetail");
    DutyDetailIter GainDutyDetail = std::remove_if(gained.begin(), gained.end(), IsDutyDetailEmpty);
    gained.erase(GainDutyDetail, gained.end());

    LOG_INFO("Del all empty lost dutydetail");
    DutyDetailIter LostDutyDetail = std::remove_if(lost.begin(), lost.end(), IsDutyDetailEmpty);
    lost.erase(LostDutyDetail, lost.end());
}
// ++TD19075
