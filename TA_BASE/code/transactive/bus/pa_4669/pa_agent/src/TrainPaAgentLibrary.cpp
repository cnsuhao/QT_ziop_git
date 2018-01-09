#ifdef __WIN32__
#pragma warning(disable:4250)
#endif

#include "bus/pa_4669/pa_agent/src/TrainPaAgentLibrary.h"
#include "bus/pa_4669/pa_agent/src/PaAgentEntityNotFoundException.h"

const static std::string   STATION_PA_AGENT_ENTITY_TYPE_NAME = "StationPAAgent";
const static std::string   MASTER_PA_AGENT_ENTITY_TYPE_NAME = "MasterPAAgent";

namespace TA_Base_Bus
{

    TrainPaAgentLibrary::TrainPaAgentLibrary( bool isMaster /*= true*/ )
    {
        TrainPaAgentLibrary(0, isMaster);
    }

    TrainPaAgentLibrary::TrainPaAgentLibrary( unsigned long locationKey, bool isMaster /*= true*/ ):
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

    TrainPaAgentLibrary::~TrainPaAgentLibrary()
    {

    }

    std::string TrainPaAgentLibrary::broadcastTrainDva( const TA_Base_Bus::IPAAgentCorbaDef::TrainCoverage& trains, CORBA::ULong messageKey, CORBA::ULong libraryVersion, const TA_Base_Bus::IPAAgentCorbaDef::TrainCyclicParams& stCyclicParams, CORBA::Boolean hasOverrideOption, const char* sessionId, CORBA::Boolean isEmergency )
    {
        std::string broadcastId;
        CORBA_CALL_RETURN( broadcastId, m_PaAgent, broadcastTrainDva, ( trains,
            messageKey,
            libraryVersion,
            stCyclicParams,
            hasOverrideOption,
            sessionId,
            isEmergency) );
        return broadcastId;
    }

    std::string TrainPaAgentLibrary::broadcastTrainLive( const TA_Base_Bus::IPAAgentCorbaDef::TrainCoverage& trains, CORBA::Boolean hasOverrideOption, const char* staticGroup, const char* sessionId )
    {
        std::string broadcastId;
        CORBA_CALL_RETURN( broadcastId, m_PaAgent, broadcastTrainLive, ( trains,
            hasOverrideOption,
            staticGroup,
            sessionId ) );
        return broadcastId;
    }

    void TrainPaAgentLibrary::beginTrainLiveAnnouncement( const char* broadcastId )
    {
        CORBA_CALL( m_PaAgent, beginTrainLiveAnnouncement, ( broadcastId ) );
    }

    void TrainPaAgentLibrary::continueTrainLiveAnnouncement( const char* broadcastId )
    {
        CORBA_CALL( m_PaAgent, continueTrainLiveAnnouncement, ( broadcastId ) )
    }

    void TrainPaAgentLibrary::endTrainLiveAnnouncement( const char* broadcastId )
    {
        CORBA_CALL( m_PaAgent, endTrainLiveAnnouncement, ( broadcastId ) );
    }

    void TrainPaAgentLibrary::retryTrainBroadcast( const char* broadcastId, const TA_Base_Bus::IPAAgentCorbaDef::TrainCoverage& trains, CORBA::Boolean hasOverrideOption, const char* sessionId )
    {
        CORBA_CALL( m_PaAgent, retryTrainBroadcast, ( broadcastId, 
            trains,
            hasOverrideOption,
            sessionId ) );
    }

    void TrainPaAgentLibrary::terminateBroadcast( const char* broadcastId, const char* sessionId )
    {
        CORBA_CALL( m_PaAgent, terminateBroadcast, (
            broadcastId,
            sessionId ) );
    }

    void TrainPaAgentLibrary::removeBroadcast( const char* broadcastId, const char* sessionId )
    {
        CORBA_CALL( m_PaAgent, removeBroadcast, ( broadcastId, sessionId ) );
    }

    TA_Base_Bus::IPAAgentCorbaDef::BroadcastIdTypeSeq* TrainPaAgentLibrary::getBroadcastIdTypes()
    {
        TA_Base_Bus::IPAAgentCorbaDef::BroadcastIdTypeSeq* broadcastIds = NULL;
        CORBA_CALL_RETURN( broadcastIds, m_PaAgent, getBroadcastIdTypes, () );//limin
        return broadcastIds;
    }

    void TrainPaAgentLibrary::changeBroadcastId( const char* fromBroadcastId, const char* toBroadcastId, const char* sessionId )
    {
        CORBA_CALL( m_PaAgent, changeBroadcastId, (
            fromBroadcastId, 
            toBroadcastId,
            sessionId ) );
    }

    TA_Base_Bus::IPAAgentCorbaDef::BroadcastConfig* TrainPaAgentLibrary::getBroadcastConfig( const char* broadcastId )
    {
        TA_Base_Bus::IPAAgentCorbaDef::BroadcastConfig* result = NULL;
        CORBA_CALL_RETURN( result, m_PaAgent, getBroadcastConfig, ( broadcastId ) );
        return result;
    }

    TA_Base_Bus::IPAAgentCorbaDef::ProgressReport* TrainPaAgentLibrary::getProgressReport( const char* broadcastId )
    {
        TA_Base_Bus::IPAAgentCorbaDef::BroadcastProgressUpdateData* ret = NULL;
        CORBA_CALL_RETURN( ret, m_PaAgent, getProgressReport, ( broadcastId ) );
        return ret;
    }

    TA_Base_Bus::IPAAgentCorbaDef::TrainDvaMessageRecords* TrainPaAgentLibrary::getTrainDvaMessageRecords()
    {
        TA_Base_Bus::IPAAgentCorbaDef::TrainDvaMessageRecords* ret = NULL;
        CORBA_CALL_RETURN( ret, m_PaAgent, getTrainDvaMessageRecords, () );
        return ret;
    }

    void TrainPaAgentLibrary::broadcastEventTriggeredTrainDva( CORBA::ULong messageKey, CORBA::Short libraryVersion, const TA_Base_Bus::IPAAgentCorbaDef::TrainCyclicParams& announcementPeriod, const char* sessionId, const TA_Base_Bus::IPAAgentCorbaDef::TrainCoverage& trains )
    {

    }

    void TrainPaAgentLibrary::broadcastEventTriggeredTrainDvaForStation( CORBA::ULong messageKey, CORBA::Short libraryVersion, const TA_Base_Bus::IPAAgentCorbaDef::TrainCyclicParams& announcementPeriod, const char* sessionId )
    {

    }

    void TrainPaAgentLibrary::broadcastEventTriggeredTrainDvaForInbound( CORBA::ULong messageKey, CORBA::Short libraryVersion, const TA_Base_Bus::IPAAgentCorbaDef::TrainCyclicParams& announcementPeriod, const char* sessionId )
    {

    }

    void TrainPaAgentLibrary::broadcastEventTriggeredTrainDvaForOutbound( CORBA::ULong messageKey, CORBA::Short libraryVersion, const TA_Base_Bus::IPAAgentCorbaDef::TrainCyclicParams& announcementPeriod, const char* sessionId )
    {

    }

}