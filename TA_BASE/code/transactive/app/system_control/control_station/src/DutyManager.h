/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/DutyManager.h $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class manages all duty related tasks such as popping out the
  * duty request and notification dialogs
  */

#pragma once
#include "Promise.h"
#include "Singleton.h"
#include "app/system_control/control_station/src/ControlStationConstants.h"
#include "bus/security/duty_agent/IDL/src/IDutyAgentCorbaDef.h"
#include "bus/security/duty_agent/IDL/src/IDutyAgentQueryCorbaDef.h"
#include "core/message/types/DutyAgentBroadcastComms_MessageTypes.h"
#include "core/message/src/SpecialisedMessageSubscriber.h"
#include "core/message/IDL/src/CommsMessageCorbaDef.h"
#include "core/naming/src/NamedObject.h"

class DutyManager
    : public TA_Base_Core::SpecialisedMessageSubscriber<TA_Base_Core::CommsMessageCorbaDef>,
      public Singleton<DutyManager>
{
public:

    struct SubsystemDetail
    {
        std::string regionName;
        std::set<std::string> subsystemNames;
    };

    struct DutyDetail
    {
        std::string operatorName;
        std::string profileName;
        std::string locationName;
        std::vector<SubsystemDetail> subsystemDetails;
    };

    struct DutyNotificationDetail
    {
        std::vector<DutyDetail> gained;
        std::vector<DutyDetail> denied;
        std::vector<DutyDetail> lost;
        std::vector<DutyDetail> changed;
        bool empty() const { return gained.empty() && denied.empty() && lost.empty() && changed.empty(); }
        bool dutyChanged() { return !empty(); }
        std::string str()
        {
            return boost::str(boost::format("{gained:%d, lost:%d, deined:%d, changed:%d}") % gained.size() % lost.size() % denied.size() % changed.size());
        }
    };

    typedef boost::shared_ptr<DutyNotificationDetail> DutyNotificationDetailPtr;

    struct DutyRequestDetail
    {
        DutyDetail duty;
        std::string uniqueId;
    };

    typedef boost::shared_ptr<DutyRequestDetail> DutyRequestDetailPtr;

    typedef std::vector<DutyDetail> DutyDetailList;
    typedef std::vector<DutyDetail>::iterator DutyDetailIter;
    typedef std::vector<SubsystemDetail>::iterator SubsystemDetailIter;

    /**
      * receiveSpecialisedMessage
      *
      * This is overriden by the client to receive their specialise message
      *
      * @param  T   The narrowed Message
      */
    virtual void receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message);

    void respond(const std::string& uniqueId, const TA_Base_App::TA_ControlStation::EDutyResponse response);

    /**
      * loseExclusiveDuty
      *
      * Called when the current session is being logged out. Determines if the current session holds any exclusive
      * duties, and warns the user that those sub-systems will be dutiless once they have logged out.
      */
    bool loseExclusiveDuty();
    void handleLoseExclusiveDuty(BoolPromisePtr promise);

    void initialize();
    void onSessionLogin();
    void onSessionLogout();

protected:

    DutyManager();
    virtual void asyncInitialize() override;

    void getDutyChanged(DutyNotificationDetail& det);   // TD19075

    /**
      * processDutyChange
      *
      * Process the Duty Change notification message
      *
      * @param message The duty comms message
      */
    void processDutyChange(const TA_Base_Core::CommsMessageCorbaDef& message);

    /**
      * processDutyRequest
      *
      * Process the Duty Request notification message
      *
      * @param message The duty comms message
      */
    void processDutyRequest(const TA_Base_Core::CommsMessageCorbaDef& message);

    /**
      * repackageDutyData
      *
      * Repackage the duty tree data into a structure that we can display easily.
      *
      * @param source The original data in the form of DutyTreeSequence
      * @param destination The resultant data.
      *
      */
    void repackageDutyData(TA_Base_Bus::DutyAgentTypeCorbaDef::DutyTreeSequence& source, std::vector<DutyDetail>& destination);

    void setupConnection();

private:

    // guards the callback map
    TA_Base_Core::NonReEntrantThreadLockable m_callbackLock;

    // callback map which maps the unique Id to duty agent object that will
    // be handling the request with such Id
    typedef std::map<std::string, TA_Base_Bus::IDutyAgentRespCorbaDef_var> CallbackMap;
    CallbackMap m_callback;

    // Define the named object for contact to the duty agent
    TA_Base_Core::NamedObject<TA_Base_Bus::IDutyAgentQueryCorbaDef> m_dutyAgentQuery;
};
