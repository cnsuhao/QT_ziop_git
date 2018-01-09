/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/SessionManager.h $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class manages all session related tasks such as logging in, logging out
  * and communicating with the System Controller.
  */
#pragma once
#include "MyFuture.h"
#include "Promise.h"
#include "TypeDefines.h"
#include "Singleton.h"
#include "bus/security/authentication_library/src/SessionInfo.h"

namespace TA_Base_Bus
{
    class AuthenticationLibrary;
}

class SessionManager : public Singleton<SessionManager>
{
public:

    TA_Base_Bus::SessionInfo getSessionInfo(const std::string& sessionId);

protected:

    SessionManager();
    virtual ~SessionManager();
    virtual void asyncInitialize() override;

    void handleRequestSessionIdSignal(StringPromisePtr sessionIdPromise, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation* location, const std::string&, bool);
    void handleRequestChangeProfileSignal(StringPromisePtr sessionIdPromise, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation* location, const std::string&, bool);
    void handleRequestChangeOperatorSignal(StringPromisePtr sessionIdPromise, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation* location, const std::string&, bool);
    void handleRequestOperatorOverrideSignal(PromisePtr sessionIdPromise, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation* location, const std::string&, bool);
    void handleRequestRemoveOperatorOverrideSignal(PromisePtr promise);
    void handleRequestEndSessionSignal(PromisePtr);
    void handleServerStateChangedSignal(bool isServerUp);
    void handleSystemControllerStateChangedSignal(bool isGroupOnline, const std::string& groupName);

protected:

    struct Keys
    {
        Keys(unsigned long o, unsigned long p, unsigned long l, unsigned long c)
            : operatorId(o), profileId(p), locationKey(l), consoleId(c)
        {
        }

        unsigned long operatorId;
        unsigned long profileId;
        unsigned long locationKey;
        unsigned long consoleId;
    };

    Keys getKeys(TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation* location);
    void setupSignalConnections();
    void renewSession();
    unsigned int getLocationKey(TA_Base_Core::ILocation* theLocation);
    std::string getLocationName(TA_Base_Core::ILocation* theLocation);

protected:

    bool m_sCGroupOnline;
    TA_Base_Core::IConsolePtr m_console;
    TA_Base_Core::ControlStationPtr m_controlStationEntity;
    MyFuture<TA_Base_Bus::AuthenticationLibrary> m_authenticationLibrary;
    static const std::string CONSOLE_NAME;
    static const std::string PROFILE_NAME;
    static const std::string USER_NAME;
    static const std::string LOGIC_LOCATION;
    static const std::string GROUP_ONLINE;
};
