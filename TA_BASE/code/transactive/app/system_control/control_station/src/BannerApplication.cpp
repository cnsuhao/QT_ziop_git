#include "StdAfx.h"
#include "BannerApplication.h"
#include "Signals.h"
#include "SessionSignal.h"
#include "DataCache.h"
#include "ScreenPositioning.h"
#include "RunningApplicationManager.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"

using namespace TA_Base_Bus;

BannerApplication::BannerApplication(TA_Base_Core::IGui* gui)
    : Application(gui)
{
    setupSignalConnections();
}

BannerApplication::~BannerApplication()
{
}

void BannerApplication::setupSignalConnections()
{
    SessionSignal::logined.connect(boost::bind(&BannerApplication::onLogin, this));
}

BannerApplication* BannerApplication::create(TA_Base_Core::IGui* gui)
{
    return new BannerApplication(gui);
}

void BannerApplication::onLogin()
{
    if (m_bannerEntities.empty())
    {
        m_bannerEntities = DataCache::getInstance().getBanners();
    }

    std::vector<bool> visibility = ScreenPositioning::getInstance().getBannerVisibility();
    size_t bannerCount = m_bannerEntities.size();
    size_t monitorCount = ScreenPositioning::getInstance().getNumberOfMonitors();
    size_t count = std::min(bannerCount, monitorCount);

    for (size_t i = 0; i < count; ++i)
    {
        if (!visibility[i])
        {
            continue;
        }

        static RECT defaultRect = { -1, -1, -1, -1 };
        std::string commandline = str(boost::format("--entity-name=%s") % m_bannerEntities[i]->getName());
        RECT boundaryRect = ScreenPositioning::getInstance().getRect(TA_GenericGui::SCREEN_SPECIFIC, TA_GenericGui::AREA_BANNER, i + 1);

        //continue; //TODO

        RunningApplicationManager::getInstance().createRunningApplication(this,
                                                                          commandline,
                                                                          TA_Base_Bus::TA_GenericGui::POS_BOUNDED,
                                                                          TA_Base_Bus::TA_GenericGui::ALIGN_FIT,
                                                                          defaultRect,
                                                                          boundaryRect,
                                                                          false);  // false indicates this application is not available as a running application
    }
}
