#pragma warning(disable:4786 4503)
#include "StdAfx.h"
#include "DummyAgent.h"
#include "test/TestNaming.h"
#include "bus/generic_agent/src/IEntity.h"
#include "bus/generic_agent/src/GenericAgent.h"

DummyAgent::DummyAgent(int argc, char* argv[])
    : m_genericAgent(NULL)
{
    m_genericAgent = new TA_Base_Bus::GenericAgent(argc, argv, this);
}

void DummyAgent::agentTerminate()
{
    ::exit(0);
}

bool DummyAgent::createAllEntities(TA_Base_Core::IEntityDataPtr, const TA_Base_Core::SharedIEntityDataList&, TA_Base_Bus::IEntityList&)
{
    return true;
}

void DummyAgent::entityCreationComplete()
{
}

TA_Base_Bus::IEntity* DummyAgent::createEntity(TA_Base_Core::IEntityDataPtr EntityData)
{
    return NULL;
}

void DummyAgent::agentSetMonitor()
{
}

void DummyAgent::agentSetControl()
{
}

void DummyAgent::processOnlineUpdate(const TA_Base_Core::ConfigUpdateDetails& updateEvent)
{
}

void DummyAgent::run()
{
    TestNaming t;
    m_genericAgent->run();
}

void DummyAgent::notifyGroupOffline(const std::string& group)
{
}

void DummyAgent::notifyGroupOnline(const std::string& group)
{
}

void DummyAgent::registerForStateUpdates()
{
}

void DummyAgent::receiveSpecialisedMessage(const TA_Base_Core::StateUpdateMessageCorbaDef& message)
{
}
