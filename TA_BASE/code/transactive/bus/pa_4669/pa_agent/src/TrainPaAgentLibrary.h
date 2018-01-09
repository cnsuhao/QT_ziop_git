#ifndef PA_AGENT_LIBRARY_HEADER_INCLUDED
#define PA_AGENT_LIBRARY_HEADER_INCLUDED

#include <string>
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/naming/src/NamedObject.h"
#include "bus/pa_4669/pa_agent/IDL/src/IPAAgentCorbaDef.h"
#include "bus/pa_4669/pa_agent/IDL/src/ITrainPaMethod.h"

namespace TA_Base_Bus
{
class TrainPaAgentLibrary
{
public:
    TrainPaAgentLibrary(bool isMaster = true);
    TrainPaAgentLibrary(unsigned long locationKey, bool isMaster = true);
    ~TrainPaAgentLibrary();

    std::string broadcastTrainDva(const TA_Base_Bus::IPAAgentCorbaDef::TrainCoverage& trains, CORBA::ULong messageKey, 
        CORBA::ULong libraryVersion, const TA_Base_Bus::IPAAgentCorbaDef::TrainCyclicParams& stCyclicParams, 
        CORBA::Boolean hasOverrideOption, const char* sessionId, CORBA::Boolean isEmergency);
    std::string broadcastTrainLive(const TA_Base_Bus::IPAAgentCorbaDef::TrainCoverage& trains, 
        CORBA::Boolean hasOverrideOption, const char* staticGroup, const char* sessionId);
    void beginTrainLiveAnnouncement(const char* broadcastId);
    void continueTrainLiveAnnouncement(const char* broadcastId);
    void endTrainLiveAnnouncement(const char* broadcastId);
    void retryTrainBroadcast(const char* broadcastId, const TA_Base_Bus::IPAAgentCorbaDef::TrainCoverage& trains, 
        CORBA::Boolean hasOverrideOption, const char* sessionId);
    void terminateBroadcast(const char* broadcastId, const char* sessionId);
    void removeBroadcast(const char* broadcastId, const char* sessionId);
    TA_Base_Bus::IPAAgentCorbaDef::BroadcastIdTypeSeq* getBroadcastIdTypes();
    void changeBroadcastId(const char* fromBroadcastId, const char* toBroadcastId, const char* sessionId);
    TA_Base_Bus::IPAAgentCorbaDef::BroadcastConfig* getBroadcastConfig(const char* broadcastId);
    TA_Base_Bus::IPAAgentCorbaDef::ProgressReport* getProgressReport(const char* broadcastId);
    TA_Base_Bus::IPAAgentCorbaDef::TrainDvaMessageRecords* getTrainDvaMessageRecords();

    void broadcastEventTriggeredTrainDva(CORBA::ULong messageKey, CORBA::Short libraryVersion, const TA_Base_Bus::IPAAgentCorbaDef::TrainCyclicParams& announcementPeriod, const char* sessionId, const TA_Base_Bus::IPAAgentCorbaDef::TrainCoverage& trains);
    void broadcastEventTriggeredTrainDvaForStation(CORBA::ULong messageKey, CORBA::Short libraryVersion, const TA_Base_Bus::IPAAgentCorbaDef::TrainCyclicParams& announcementPeriod, const char* sessionId);
    void broadcastEventTriggeredTrainDvaForInbound(CORBA::ULong messageKey, CORBA::Short libraryVersion, const TA_Base_Bus::IPAAgentCorbaDef::TrainCyclicParams& announcementPeriod, const char* sessionId);
    void broadcastEventTriggeredTrainDvaForOutbound(CORBA::ULong messageKey, CORBA::Short libraryVersion, const TA_Base_Bus::IPAAgentCorbaDef::TrainCyclicParams& announcementPeriod, const char* sessionId);

private:
    TrainPaAgentLibrary( const TrainPaAgentLibrary& thePaAgentLibrary);
    TrainPaAgentLibrary& operator=(const TrainPaAgentLibrary& rhs);

    TA_Base_Core::NamedObject<TA_Base_Bus::ITrainPaMethod,
        TA_Base_Bus::ITrainPaMethod_ptr,
        TA_Base_Bus::ITrainPaMethod_var> m_PaAgent;

    unsigned long m_paAgentLocationKey;
};
}


#endif