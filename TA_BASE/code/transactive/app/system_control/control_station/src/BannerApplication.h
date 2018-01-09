#pragma once
#include "Application.h"

namespace TA_Base_Core
{
    class IGui;
    class IEntityData;
}

class BannerApplication : public Application
{
public:

    BannerApplication(TA_Base_Core::IGui* gui);
    virtual ~BannerApplication();
    static BannerApplication* create(TA_Base_Core::IGui* gui);

protected:

    void onLogin();
    void setupSignalConnections();

protected:

    std::vector<TA_Base_Core::IEntityData*> m_bannerEntities;
};
