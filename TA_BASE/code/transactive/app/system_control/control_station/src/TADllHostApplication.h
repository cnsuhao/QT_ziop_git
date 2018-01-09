#pragma once
#include "TypeDefines.h"
#include "Application.h"

class TADllHostApplication : public Application
{
public:

    TADllHostApplication(TA_Base_Core::IGui* gui);
    virtual ~TADllHostApplication();
    static TADllHostApplication* create(TA_Base_Core::IGui* gui);

protected:

    void setupSignalConnections();
    void onLogin();

protected:

    std::vector<TA_Base_Core::IEntityData*> m_bannerEntities;
    TA_Base_Core::IEntityDataPtr m_dllHostEntity;
};
