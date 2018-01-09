#ifdef __WIN32__
#pragma warning(disable:4250)
#endif

#include "bus/pa_4669/pa_agent/src/PaAgentLibrary.h"
#include "bus/pa_4669/pa_agent/src/PaAgentEntityNotFoundException.h"

const static std::string   STATION_PA_AGENT_ENTITY_TYPE_NAME = "StationPAAgent";
const static std::string   MASTER_PA_AGENT_ENTITY_TYPE_NAME = "MasterPAAgent";

namespace TA_Base_Bus
{

    PaAgentLibrary::PaAgentLibrary(bool isMaster)
    {
        PaAgentLibrary(0, isMaster);
    }

    PaAgentLibrary::PaAgentLibrary( unsigned long locationKey, bool isMaster):
    m_paAgentLocationKey(locationKey)
    {

        std::string entityName;
        if (isMaster)
        {
            entityName = MASTER_PA_AGENT_ENTITY_TYPE_NAME;
        }
        else
        {
            entityName = STATION_PA_AGENT_ENTITY_TYPE_NAME;
        }

        // this will only return a single entry
        TA_Base_Core::CorbaNameList paAgentName = 
            TA_Base_Core::EntityAccessFactory::getInstance().getCorbaNamesOfTypeAtLocation( 
            entityName, m_paAgentLocationKey, true);

        // make sure we've got something
        if (paAgentName.size() == 0)
        {
            std::ostringstream msg;
            msg << "There are no entities of type PlanAgent with location key set to " << m_paAgentLocationKey << ".";

            TA_THROW(PaAgentEntityNotFoundException(msg.str()));
        }

        // Set the agent name - this is the name of the agent to contact. 
        m_PaAgent.setCorbaName( paAgentName[0] );
    }

    PaAgentLibrary::~PaAgentLibrary()
    {
    
    }

    std::string PaAgentLibrary::broadcastStationScheduleDva( const ::TA_Base_Bus::IPAAgentCorbaDef::MessageKeySeq& messages, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones, ::CORBA::UShort dwellInSecs, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, const char* groupName, const ::TA_Base_Bus::IPAAgentCorbaDef::StationCyclicParams& stCyclicParams, const char* sessionId )
    {
        std::string broadcastId;
        CORBA_CALL_RETURN( broadcastId, m_PaAgent, broadcastStationScheduleDva, (   
            messages, zones, unsigned short(dwellInSecs), hasChime, isSynchronisedWithTis,
            groupName, stCyclicParams, sessionId ) );

        return broadcastId;
    }

    std::string PaAgentLibrary::broadcastStationDva( const ::TA_Base_Bus::IPAAgentCorbaDef::MessageKeySeq& messages, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones, ::CORBA::UShort dwellInSecs, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, const char* groupName, const char* sessionId )
    {
        std::string broadcastId;

        CORBA_CALL_RETURN( broadcastId, m_PaAgent, broadcastStationDva, (   
            messages, zones, dwellInSecs, hasChime, isSynchronisedWithTis, 
            groupName, sessionId ) );

        return broadcastId;
    }

    std::string PaAgentLibrary::playbackStationDva( ::CORBA::ULong messageKey, ::CORBA::UShort sourceId, const char* sessionId )
    {
        std::string broadcastId;
        CORBA_CALL_RETURN( broadcastId, m_PaAgent, playbackStationDva, 
                                                                ( messageKey, 
    															  sourceId, 
                                                                  sessionId ) );
        return broadcastId;
    }

    std::string PaAgentLibrary::broadcastStationLive( ::CORBA::Boolean hasChime, ::CORBA::UShort sourceId, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones, const char* groupName, const char* sessionId )
    {
        std::string broadcastId;
        CORBA_CALL_RETURN( broadcastId, m_PaAgent, broadcastStationLive, ( 
            hasChime,
            sourceId, 
            zones,
            groupName,
            sessionId ) );

        return broadcastId;
    }

    void PaAgentLibrary::broadcastStationMusic( ::TA_Base_Bus::IPAAgentCorbaDef::EMusicType musicType, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones, const char* sessionId )
    {
        CORBA_CALL( m_PaAgent, broadcastStationMusic,
            ( musicType, zones, sessionId ) );
    }

    std::string PaAgentLibrary::recordAdhocMessage( ::CORBA::UShort sourceId, ::CORBA::ULong messageKey, ::CORBA::UShort usMsgType, const char* sessionId )
    {
        std::string broadcastId;
        CORBA_CALL_RETURN ( broadcastId, m_PaAgent, recordAdhocMessage,
            ( sourceId, messageKey, usMsgType,
            sessionId ) );
        return broadcastId;
    }

    void PaAgentLibrary::retryStationBroadcast( const char* broadcastId, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones, const char* sessionId )
    {
        CORBA_CALL( m_PaAgent, retryStationBroadcast, ( broadcastId, zones, sessionId ) );
    }

    void PaAgentLibrary::agentLinkTest()
    {
        CORBA_CALL( m_PaAgent, agentLinkTest, () );//limin
    }

    ::CORBA::Boolean PaAgentLibrary::hardwareLinkTest( const char* broadcastId, const char* sessionId )
    {
        CORBA::Boolean ret = false;
        CORBA_CALL_RETURN( ret, m_PaAgent, hardwareLinkTest, ( "", sessionId ) );
        return ret;
    }

    void PaAgentLibrary::terminateBroadcast( const char* broadcastId, const char* sessionId )
    {
        CORBA_CALL( m_PaAgent, terminateBroadcast, (
            broadcastId,
            sessionId ) );
    }

    void PaAgentLibrary::removeBroadcast( const char* broadcastId, const char* sessionId )
    {
        CORBA_CALL( m_PaAgent, removeBroadcast, ( broadcastId, sessionId ) );
    }

    ::CORBA::Boolean PaAgentLibrary::setAdhocType( ::CORBA::ULong messageKey, ::CORBA::UShort messageType, const char* sessionId )
    {
        CORBA::Boolean ret;
        CORBA_CALL_RETURN( ret, m_PaAgent, setAdhocType, (
            messageKey, messageType, sessionId ) );
        return ret;
    }

    ::CORBA::Boolean PaAgentLibrary::setAdhocLabel( ::CORBA::ULong messageKey, const char* label, const char* sessionId )
    {
        CORBA::Boolean ret = static_cast<CORBA::Boolean>(false);
        CORBA_CALL_RETURN( ret, m_PaAgent, setAdhocLabel, (
            messageKey, label, sessionId ) );

        return ret;
    }

    IPAAgentCorbaDef::BroadcastIdTypeSeq* PaAgentLibrary::getBroadcastIdTypes()
    {
        TA_Base_Bus::IPAAgentCorbaDef::BroadcastIdTypeSeq* broadcastIds = NULL;
        CORBA_CALL_RETURN( broadcastIds, m_PaAgent, getBroadcastIdTypes, () );
        return broadcastIds;
    }

    void PaAgentLibrary::changeBroadcastId( const char* fromBroadcastId, const char* toBroadcastId, const char* sessionId )
    {
        CORBA_CALL( m_PaAgent, changeBroadcastId, (
            fromBroadcastId, 
            toBroadcastId,
            sessionId ) );
    }

    IPAAgentCorbaDef::ProgressReport* PaAgentLibrary::getProgressReport( const char* broadcastId )
    {
        TA_Base_Bus::IPAAgentCorbaDef::BroadcastProgressUpdateData* ret = NULL;
        CORBA_CALL_RETURN( ret, m_PaAgent, getProgressReport, (broadcastId) );
        return ret;
    }

    IPAAgentCorbaDef::BroadcastConfig* PaAgentLibrary::getBroadcastConfig( const char* broadcastId )
    {
        TA_Base_Bus::IPAAgentCorbaDef::BroadcastConfig* result = NULL;
        CORBA_CALL_RETURN( result, m_PaAgent, getBroadcastConfig, (broadcastId) );
        return result;
    }

    IPAAgentCorbaDef::EMusicType PaAgentLibrary::getMusicStatus( const char* sessionId )
    {
        TA_Base_Bus::IPAAgentCorbaDef::EMusicType musicType = TA_Base_Bus::IPAAgentCorbaDef::NoMusic;
        CORBA_CALL_RETURN( musicType, m_PaAgent, 
            getMusicStatus, (sessionId) );

        return musicType;
    }

    void PaAgentLibrary::forceTerminateAllExistPA()
    {
        CORBA_CALL( m_PaAgent, forceTerminateAllExistPA, ());
    }

    IPAAgentCorbaDef::StationDvaMessageRecords* PaAgentLibrary::getStationDvaMessageRecords()
    {
        TA_Base_Bus::IPAAgentCorbaDef::DvaMessagesUpdateData* ret = NULL;
        CORBA_CALL_RETURN( ret, m_PaAgent, getStationDvaMessageRecords, () );
        return ret;
    }

    IPAAgentCorbaDef::DvaVersionRecords* PaAgentLibrary::getDvaVersionRecords()
    {
        TA_Base_Bus::IPAAgentCorbaDef::DvaVersionRecords* ret = NULL;
        CORBA_CALL_RETURN( ret, m_PaAgent, getDvaVersionRecords, () );
        return ret;
    }

    void PaAgentLibrary::broadcastEventTriggeredStationDva( ::CORBA::ULong messageKey, const char* messageVersionNumber, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, ::CORBA::ULong durationInSecs, ::CORBA::UShort periodInSecs, const char* sessionId, const ::TA_Base_Bus::IPAAgentCorbaDef::ZoneCoverage& zones )
    {

    }

    void PaAgentLibrary::broadcastEventTriggeredStationDvaForStation( ::CORBA::ULong messageKey, const char* messageVersionNumber, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, ::CORBA::ULong durationInSecs, ::CORBA::UShort periodInSecs, const char* sessionId )
    {

    }

    void PaAgentLibrary::broadcastEventTriggeredStationDvaForArea( ::CORBA::ULong messageKey, const char* messageVersionNumber, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, ::CORBA::ULong durationInSecs, ::CORBA::UShort periodInSecs, const char* sessionId, const char* zoneArea )
    {

    }

    void PaAgentLibrary::broadcastEventTriggeredStationDvaForGroup( ::CORBA::ULong messageKey, const char* messageVersionNumber, ::CORBA::Boolean hasChime, ::CORBA::Boolean isSynchronisedWithTis, ::CORBA::ULong durationInSecs, ::CORBA::UShort periodInSecs, const char* sessionId, const char* zoneGroupLabel )
    {

    }

}