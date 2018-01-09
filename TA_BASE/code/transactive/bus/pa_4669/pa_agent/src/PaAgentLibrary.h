#ifndef PA_AGENT_LIBRARY_HEADER_INCLUDED
#define PA_AGENT_LIBRARY_HEADER_INCLUDED

#include <string>
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/naming/src/NamedObject.h"
#include "bus/pa_4669/pa_agent/IDL/src/IPAAgentCorbaDef.h"
#include "bus/pa_4669/pa_agent/IDL/src/IStationPaMethod.h"

namespace TA_Base_Bus
{
class PaAgentLibrary
{
public:
    PaAgentLibrary(bool isMaster = true);
    PaAgentLibrary(unsigned long locationKey, bool isMaster = true);
    ~PaAgentLibrary();

    std::string broadcastStationScheduleDva(const ::TA_Base_Bus::IPAAgentCorbaDef::MessageKeySeq& messages, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones, ::CORBA::UShort dwellInSecs, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, const char* groupName, const ::TA_Base_Bus::IPAAgentCorbaDef::StationCyclicParams& stCyclicParams, const char* sessionId);
    std::string broadcastStationDva(const ::TA_Base_Bus::IPAAgentCorbaDef::MessageKeySeq& messages, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones, ::CORBA::UShort dwellInSecs, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, const char* groupName, const char* sessionId);
    std::string playbackStationDva(::CORBA::ULong messageKey, ::CORBA::UShort sourceId, const char* sessionId);
    std::string broadcastStationLive(::CORBA::Boolean hasChime, ::CORBA::UShort sourceId, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones, const char* groupName, const char* sessionId);
    void broadcastStationMusic(::TA_Base_Bus::IPAAgentCorbaDef::EMusicType musicType, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones, const char* sessionId);
    std::string recordAdhocMessage(::CORBA::UShort sourceId, ::CORBA::ULong messageKey, ::CORBA::UShort usMsgType, const char* sessionId);
    void retryStationBroadcast(const char* broadcastId, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones, const char* sessionId);
    void agentLinkTest();
    ::CORBA::Boolean hardwareLinkTest(const char* broadcastId, const char* sessionId);
    void terminateBroadcast(const char* broadcastId, const char* sessionId);
    void removeBroadcast(const char* broadcastId, const char* sessionId);
    ::CORBA::Boolean setAdhocType(::CORBA::ULong messageKey, ::CORBA::UShort messageType, const char* sessionId);
    ::CORBA::Boolean setAdhocLabel(::CORBA::ULong messageKey, const char* label, const char* sessionId);
    IPAAgentCorbaDef::BroadcastIdTypeSeq* getBroadcastIdTypes();
    void changeBroadcastId(const char* fromBroadcastId, const char* toBroadcastId, const char* sessionId);
    IPAAgentCorbaDef::BroadcastConfig* getBroadcastConfig(const char* broadcastId);
    IPAAgentCorbaDef::ProgressReport* getProgressReport(const char* broadcastId);
    IPAAgentCorbaDef::EMusicType getMusicStatus(const char* sessionId);
    void forceTerminateAllExistPA();
    IPAAgentCorbaDef::StationDvaMessageRecords* getStationDvaMessageRecords();
    IPAAgentCorbaDef::DvaVersionRecords* getDvaVersionRecords();
    void broadcastEventTriggeredStationDva(::CORBA::ULong messageKey, const char* messageVersionNumber, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, ::CORBA::ULong durationInSecs, ::CORBA::UShort periodInSecs, const char* sessionId, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones);
    void broadcastEventTriggeredStationDvaForStation(::CORBA::ULong messageKey, const char* messageVersionNumber, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, ::CORBA::ULong durationInSecs, ::CORBA::UShort periodInSecs, const char* sessionId);
    void broadcastEventTriggeredStationDvaForArea(::CORBA::ULong messageKey, const char* messageVersionNumber, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, ::CORBA::ULong durationInSecs, ::CORBA::UShort periodInSecs, const char* sessionId, const char* zoneArea);
    void broadcastEventTriggeredStationDvaForGroup(::CORBA::ULong messageKey, const char* messageVersionNumber, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, ::CORBA::ULong durationInSecs, ::CORBA::UShort periodInSecs, const char* sessionId, const char* zoneGroupLabel);
    

private:

    PaAgentLibrary( const PaAgentLibrary& thePaAgentLibrary);
    PaAgentLibrary& operator=(const PaAgentLibrary& rhs);

    TA_Base_Core::NamedObject<TA_Base_Bus::IStationPaMethod,
        TA_Base_Bus::IStationPaMethod_ptr,
        TA_Base_Bus::IStationPaMethod_var> m_PaAgent;

    unsigned long m_paAgentLocationKey;
};
}

#endif