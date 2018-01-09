#pragma once
#include "Singleton.h"
#include "core/configuration_updates/src/IOnlineUpdatable.h"
#include "core/data_access_interface/entity_access/src/ControlStation.h"
#include "core/configuration_updates/src/ConfigUpdateDetails.h"

class ControlStationOnlineUpdateHandler : public TA_Base_Core::IOnlineUpdatable, public Singleton<ControlStationOnlineUpdateHandler>
{
protected:

    ControlStationOnlineUpdateHandler();
    void setupSignalConnections();
    virtual void asyncInitialize() override;
    virtual void processUpdate(const TA_Base_Core::ConfigUpdateDetails& updateEvent);

protected:

    TA_Base_Core::ControlStationPtr m_controlStationEntity;
};
