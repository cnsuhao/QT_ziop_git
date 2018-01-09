#pragma once
#include "Application.h"

namespace TA_Base_Core
{
    class IGui;
    class IEntityData;
}

class SystemManagerApplication : public Application
{
public:

    SystemManagerApplication(TA_Base_Core::IGui* gui);
    virtual ~SystemManagerApplication();
    static SystemManagerApplication* create(TA_Base_Core::IGui* gui);

protected:

    void onLogin();
    void setupSignalConnections();

protected:

    std::vector<TA_Base_Core::IEntityData*> m_bannerEntities;
};
