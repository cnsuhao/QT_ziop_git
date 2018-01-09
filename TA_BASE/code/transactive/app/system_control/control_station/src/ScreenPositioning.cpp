/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ScreenPositioning.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Calculates the screen positions of launch pad (if exist), banner and applications
  *
  */
#include "StdAfx.h"
#include "DataCache.h"
#include "app/system_control/control_station/src/ScreenPositioning.h"
#include "app/system_control/control_station/src/ControlStationRootDialog.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "core/data_access_interface/entity_access/src/banner.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/utilities/src/RunParams.h"

using namespace TA_Base_Core;

const RECT ScreenPositioning::NULL_RECT = {0, 0, 0, 0};            // This is used to indicate a rectangle whose dimension is zero
unsigned long ScreenPositioning::LAUNCH_BAR_WIDTH = 171;     // This is the width of the launch bar

ScreenPositioning::ScreenPositioning()
    : m_isLaunchBarShown(false),
      m_numberOfMonitors(0),
      m_eachScreenWidth(0),
      m_displaySpanAllScreens(false)
{
}

void ScreenPositioning::asyncInitialize()
{
    FUNCTION_ENTRY("loadScreenLayout");

    loadSystemScreenInfo();

    TA_Base_Core::ControlStation& controlStationDatabaseEntry = *DataCache::getInstance().getControlStationEntity();
    std::vector<IEntityData*>& banners = DataCache::getInstance().getBanners();
    m_displaySpanAllScreens = (controlStationDatabaseEntry.getGraphWorxScreen() == ControlStation::FULL_SCREENS);

    // Raymond Pau++ TD16471
    // load customize size/pos of each screen
    for (unsigned int screen = 1; screen <= m_numberOfMonitors; ++screen)
    {
        if (true == controlStationDatabaseEntry.isScreenSizeSpecified(screen))
        {
            TA_Base_Core::TA_RECT rect = controlStationDatabaseEntry.getScreenSize(screen);
            m_screenLayout[screen].screenRect = { rect.left, rect.top, rect.right, rect.bottom };
            m_screenLayout[screen].screenRect.left = rect.left;
            m_screenLayout[screen].screenRect.top = rect.top;
            m_screenLayout[screen].screenRect.right = rect.right;
            m_screenLayout[screen].screenRect.bottom = rect.bottom;
        }
    }

    // ++Raymond Pau TD16471
    loadBannerScreenLayout(controlStationDatabaseEntry, banners);
    loadGraphworxScreenLayout(controlStationDatabaseEntry);
    loadApplicationScreenLayout();

    FUNCTION_EXIT;
}

void ScreenPositioning::loadSystemScreenInfo()
{
    RECT area;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &area, 0);
    m_screenBottom = area.bottom;
    m_screenTop = area.top;

    calculateMonitor(m_numberOfMonitors, m_eachScreenWidth);

    // initialise the screen layout values
    for (unsigned int i = 1, left = 0; i <= m_numberOfMonitors; i++)
    {
        RECT tmp = { left, m_screenTop, left = left + m_eachScreenWidth, m_screenBottom };
        m_screenLayout[i].screenRect = tmp;
        m_screenLayout[i].bannerRect = NULL_RECT;
        m_screenLayout[i].launchBarRect = NULL_RECT;
        m_screenLayout[i].applicationRect = NULL_RECT;
        m_screenLayout[i].schematicRect = NULL_RECT;
        m_screenLayout[i].bannerPosition = ControlStation::NEITHER;
        m_screenLayout[i].launchBarPosition = ControlStation::HIDDEN;
        m_screenLayout[i].hasGraphWorx = false;
    }
}

void ScreenPositioning::calculateMonitor(unsigned long& monitorNumber, unsigned long& monitorWidth)
{
    struct Resolution
    {
        size_t width;
        size_t height;
    };

    Resolution resolutions[] =
    {
        {640, 256},   {512, 342},   {800, 240},   {512, 384},   {640, 320},   {640, 350},   {640, 360},   {480, 500},
        {720, 348},   {720, 350},   {640, 400},   {720, 364},   {800, 352},   {600, 480},   {640, 480},   {640, 512},
        {768, 480},   {800, 480},   {848, 480},   {854, 480},   {800, 600},   {960, 540},   {832, 624},   {960, 544},
        {1024, 576},  {1024, 600},  {960, 640},   {1024, 640},  {960, 720},   {1136, 640},  {1024, 768},  {1024, 800},
        {1152, 720},  {1152, 768},  {1280, 720},  {1120, 832},  {1280, 768},  {1152, 864},  {1334, 750},  {1280, 800},
        {1152, 900},  {1024, 1024}, {1366, 768},  {1280, 854},  {1600, 768},  {1280, 960},  {1080, 1200}, {1440, 900},
        {1280, 1024}, {1440, 960},  {1600, 900},  {1400, 1050}, {1440, 1024}, {1440, 1080}, {1600, 1024}, {1680, 1050},
        {1776, 1000}, {1600, 1200}, {1600, 1280}, {1920, 1080}, {1920, 1200}, {1920, 1280}, {2048, 1152}, {1792, 1344},
        {1856, 1392}, {2880, 900},  {1800, 1440}, {2048, 1280}, {1920, 1400}, {2538, 1080}, {2560, 1080}, {1920, 1440},
        {2160, 1440}, {2048, 1536}, {2304, 1440}, {2560, 1440}, {2304, 1728}, {2560, 1600}, {2560, 1700}, {2560, 1800},
        {2560, 1920}, {3440, 1440}, {2736, 1824}, {2880, 1800}, {2560, 2048}, {2732, 2048}, {2800, 2100}, {3200, 1800},
        {3000, 2000}, {3200, 2048}, {3200, 2400}, {3840, 2160}, {3840, 2400}, {4096, 2304}, {5120, 2160}, {4096, 3072},
        {4500, 3000}, {5120, 2880}, {5120, 3200}, {5120, 4096}, {6400, 4096}, {6400, 4800}, {7680, 4320}, {7680, 4800},
        {8192, 4608}, {8192, 8192}
    };

    size_t n = sizeof(resolutions) / sizeof(Resolution);
    size_t totalWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    size_t monitorHeight = GetSystemMetrics(SM_CYSCREEN);
    monitorNumber = GetSystemMetrics(SM_CMONITORS);
    monitorWidth = totalWidth / monitorNumber;

    if (1 == monitorNumber && 2 < totalWidth / monitorHeight)
    {
        for (size_t i = 0; i < n; ++i)
        {
            Resolution& r = resolutions[i];

            if (monitorHeight == r.height)
            {
                if (0 == totalWidth % r.width)
                {
                    monitorWidth = r.width;
                    monitorNumber = totalWidth / monitorWidth;
                    return;
                }
            }
        }

        if (TA_Base_Core::RunParams::getInstance().isSet("MonitorWidth") && TA_Base_Core::RunParams::getInstance().isSet("MonitorNumber"))
        {
            monitorNumber = TA_Base_Core::getRunParamValue("MonitorNumber", 1);
            monitorWidth = TA_Base_Core::getRunParamValue("MonitorWidth", 1920);
        }
        else
        {
            monitorNumber = totalWidth / (monitorHeight * 16 / 9);
            monitorWidth = totalWidth / monitorNumber;
        }
    }
}

void ScreenPositioning::loadBannerScreenLayout(TA_Base_Core::ControlStation& controlStationDatabaseEntry, std::vector<IEntityData*>& banners)
{
    FUNCTION_ENTRY("loadBannerScreenLayout");

    try
    {
        // Step through each banner and set up the m_screenLayout map with the banner positioning
        for (unsigned int i = 0; i < banners.size(); i++)
        {
            Banner* thisBanner = dynamic_cast<Banner*>(banners[i]);

            // First figure out which screen the banner is on.
            // The leftposition can be negative so if it is change this to 0 so we can compare unsigned longs
            int leftPositionOfBanner = thisBanner->getLeftPosition();

            if (thisBanner->getLeftPosition() < 1)
            {
                leftPositionOfBanner = 1;
            }

            int screen = getWhichScreenCoordinateIsOn(leftPositionOfBanner);

            if (screen < 1)
            {
                // The leftposition of the banner was not within our screen range. Therefore it will not be shown
                LOG(DEBUG_WARN, "The position of banner '%s' does not fall on any of the monitors", thisBanner->getName().c_str());
                m_isBannerVisible.push_back(false);
                continue;
            }

            m_isBannerVisible.push_back(true);

            RECT& bannerRect = m_screenLayout[screen].bannerRect;
            bannerRect.left = leftPositionOfBanner;
            bannerRect.right = m_screenLayout[screen].screenRect.right;

            unsigned long topPositionOfBanner = thisBanner->getTopPosition();

            if (topPositionOfBanner < m_screenTop)
            {
                topPositionOfBanner = m_screenTop;
            }

            bannerRect.top = topPositionOfBanner;

            if (topPositionOfBanner < 10)
            {
                // If the position is less than 10 then we assume we are at the top of the screen
                m_screenLayout[screen].bannerPosition = TA_Base_Core::ControlStation::TOP;
                bannerRect.bottom = topPositionOfBanner + thisBanner->getHeight();
            }
            else if (topPositionOfBanner > (m_screenBottom - thisBanner->getHeight() - 10))
            {
                // This means we are at the bottom of the screen.
                m_screenLayout[screen].bannerPosition = TA_Base_Core::ControlStation::BOTTOM;
                bannerRect.bottom = m_screenBottom;
            }
            else
            {
                // Banner is not at the top or bottom so we just pretend it does not exist
                m_screenLayout[screen].bannerPosition = TA_Base_Core::ControlStation::NEITHER;
                bannerRect.bottom = bannerRect.top = m_screenTop;
                bannerRect.right = bannerRect.left;
            }
        }
    }
    catch (const TA_Base_Core::DataException&)
    {
        LOG_EXCEPTION("DataException", "Exception thrown while trying to retrieve the banner details. Will not be displayed");
    }
    catch (const TA_Base_Core::DatabaseException&)
    {
        LOG_EXCEPTION("DatabaseException", "Exception thrown while trying to retrieve the banner details. Will not be displayed");
    }

    FUNCTION_EXIT;
}

void ScreenPositioning::loadGraphworxScreenLayout(TA_Base_Core::ControlStation& controlStationDatabaseEntry)
{
    FUNCTION_ENTRY("loadGraphworxScreenLayout");

    unsigned long graphWorxScreen = ControlStation::NO_SCREENS;

    try
    {
        graphWorxScreen = controlStationDatabaseEntry.getGraphWorxScreen();
    }
    catch (const TA_Base_Core::DataException&)
    {
        LOG_EXCEPTION("DataException", "Exception thrown while trying to retrieve the screens GraphWorx should display on. Will default to NONE");
    }
    catch (const TA_Base_Core::DatabaseException&)
    {
        LOG_EXCEPTION("DatabaseException", "Exception thrown while trying to retrieve the screens GraphWorx should display on. Will default to NONE");
    }

    if (graphWorxScreen == ControlStation::NO_SCREENS)
    {
        // User doesn't want any GraphWorX instances so leave the default settings
        LOG_INFO("GraphWorX not to be displayed on any screens");
        FUNCTION_EXIT;
        return;
    }

    if (graphWorxScreen == ControlStation::ALL_SCREENS)
    {
        ThreadGuard guard(m_layoutLock);
        LOG_INFO("GraphWorX to be shown on all screens");

        for (unsigned int monitor = 1; monitor <= m_numberOfMonitors; ++monitor)
        {
            m_screenLayout[monitor].hasGraphWorx = true;
        }

        return;
    }

    // launching it on a particular screen
    LOG_INFO("GraphWorX to be shown on screen %lu", graphWorxScreen);
    ThreadGuard guard(m_layoutLock);
    ScreenLayout::iterator iter = m_screenLayout.find(graphWorxScreen);

    if (iter != m_screenLayout.end())
    {
        iter->second.hasGraphWorx = true;
    }

    FUNCTION_EXIT;
}

void ScreenPositioning::loadApplicationScreenLayout()
{
    FUNCTION_ENTRY("loadApplicationScreenLayout");

    ThreadGuard guard(m_layoutLock);

    for (unsigned int screen = 1; screen <= m_numberOfMonitors; screen++)
    {
        RECT& appRect = m_screenLayout[screen].applicationRect;

        // Calculate the x position and possible width. The appRect width is affected by the launch bar if it
        // is on the same screen so we must consider this.

        // If the launch pad is hidden, then the width of the appRect screen should cover the whole screen
        // horizontally and cover the rest of the space left by the banner.
        // If it has launch bar, then it should have the same height as launch bar and have the remaining horizontal space.

        double screenPercentage = 1.0;
        std::string param = TA_Base_Core::RunParams::getInstance().get(TA_Base_App::TA_ControlStation::SCREENPERCENTAGE.c_str());

        if (!param.empty())
        {
            screenPercentage = atof(param.c_str()) / 100;
        }

        RECT banner = m_screenLayout[screen].bannerRect;

        switch (m_screenLayout[screen].bannerPosition)
        {
        case (ControlStation::TOP):
            LOG_INFO("Launch bar must allow for the banner at the top of the screen");
            appRect.top = banner.bottom;
            appRect.bottom = static_cast<LONG>(m_screenBottom * screenPercentage);
            break;

        case (ControlStation::BOTTOM):
            LOG_INFO("Launch bar must allow for the banner at the bottom of the screen");
            appRect.top = m_screenTop;
            appRect.bottom = banner.top;
            break;

        case (ControlStation::NEITHER):
        default:
            appRect.top = m_screenTop;
            appRect.bottom = static_cast<LONG>(m_screenBottom * screenPercentage);
            break;
        }

        appRect.left = m_screenLayout[screen].screenRect.left;
        appRect.right = m_screenLayout[screen].screenRect.right;

        // initialise the schematic display to having the same dimension as the applicatoin/main navigator area
        m_screenLayout[screen].schematicRect = m_screenLayout[screen].applicationRect;
    }

    FUNCTION_EXIT;
}

int ScreenPositioning::getScreenNumber(TA_Base_Bus::TA_GenericGui::EScreen targetScreen, long val)
{
    FUNCTION_ENTRY("getScreenNumber");
    int retval = -1;

    switch (targetScreen)
    {
    case TA_Base_Bus::TA_GenericGui::SCREEN_CURRENT:         // Area within the current monitor that currentX lies on
        retval = getWhichScreenCoordinateIsOn(val);
        break;

    case TA_Base_Bus::TA_GenericGui::SCREEN_PREVIOUS:        // Area within the monitor on the left of the one that currentX lies on.  If there is no screen on the left of this, it will just use the current monitor.
        retval = getWhichScreenCoordinateIsOn(val) - 1;
        break;

    case TA_Base_Bus::TA_GenericGui::SCREEN_NEXT:            // Area within the monitor on the right of the one that currentX lies on.  If there is no screen on the right of this, it will just use the current monitor.
        retval = getWhichScreenCoordinateIsOn(val) + 1;
        break;

    case TA_Base_Bus::TA_GenericGui::SCREEN_SPECIFIC:        // Area within the specified monitor
        retval = val;
        break;

    case TA_Base_Bus::TA_GenericGui::SCREEN_FIRST:           // Area within the first monitor (ignores val)
        retval = 1;
        break;

    case TA_Base_Bus::TA_GenericGui::SCREEN_LAST:            // Area within the last monitor (ignores val)
        retval = m_numberOfMonitors;
        break;

    case TA_Base_Bus::TA_GenericGui::SCREEN_ALL:             // Area within all screens
    default:
        break;
    }

    if (targetScreen != TA_Base_Bus::TA_GenericGui::SCREEN_ALL)
    {
        // make sure the screen is within range.
        if (retval > static_cast<int>(m_numberOfMonitors))
        {
            retval = m_numberOfMonitors;
        }
        else if (retval < 1)
        {
            retval = 1;
        }
    }

    FUNCTION_EXIT;
    return retval;
}

int ScreenPositioning::getWhichScreenCoordinateIsOn(long xCoord) const
{
    FUNCTION_ENTRY("getWhichScreenCoordinateIsOn");

    for (unsigned int monitor = 1; monitor <= m_numberOfMonitors; ++monitor)
    {
        if (xCoord < static_cast<long>(m_eachScreenWidth * monitor))
        {
            LOG_INFO("X coordinate %lu can be found on monitor %lu", xCoord, monitor);
            FUNCTION_EXIT;
            return monitor;
        }
    }

    // Not found
    LOG_INFO("X coordinate %lu can not be found on any monitors.", xCoord);
    FUNCTION_EXIT;
    return -1;
}

RECT ScreenPositioning::getRect(const TA_Base_Bus::TA_GenericGui::EScreen targetScreen,
                                const TA_Base_Bus::TA_GenericGui::EArea targetArea,
                                long val /*= -1*/)
{
    FUNCTION_ENTRY("getRect");
    int screenNum = getScreenNumber(targetScreen, val);
    RECT rect;

    if (screenNum > 0) // if it is getting a rect of a particular screen, then just return the rect as it is
    {
        ScreenArrangement& layout = m_screenLayout[screenNum];

        switch (targetArea)
        {
        case TA_Base_Bus::TA_GenericGui::AREA_SCREEN:
            rect =  layout.screenRect;
            LOG_DEBUG("Getting the screen area of monitor %d", screenNum);
            break;

        case TA_Base_Bus::TA_GenericGui::AREA_BANNER:
            rect = layout.bannerRect;
            LOG_DEBUG("Getting the banner area of monitor %d", screenNum);
            break;

        case TA_Base_Bus::TA_GenericGui::AREA_MAINNAVIGATOR:
            rect = layout.applicationRect;
            LOG_DEBUG("Getting the main navigator/application area of monitor %d", screenNum);
            break;

        case TA_Base_Bus::TA_GenericGui::AREA_LAUNCHPAD:
            rect = layout.launchBarRect;
            LOG_DEBUG("Getting the launchbar area of monitor %d", screenNum);
            break;

        case TA_Base_Bus::TA_GenericGui::AREA_SCHEMATIC:
            rect = layout.schematicRect;
            LOG_DEBUG("Getting the schematic display area of monitor %d", screenNum);
            break;

        default:
            break;
        }
    }
    else
    {
        // we're spanning over all 3 screens, then we're getting the left, top, bottom coordinates
        // of the first screen and the right coordinate of the last screen.
        ScreenArrangement& layoutFirst = m_screenLayout[1];
        ScreenArrangement& layoutLast = m_screenLayout[m_numberOfMonitors];

        switch (targetArea)
        {
        case TA_Base_Bus::TA_GenericGui::AREA_SCREEN:
            rect =  layoutFirst.screenRect;
            rect.right = layoutLast.screenRect.right;
            LOG_DEBUG("Getting the combined screen area of all monitors");
            break;

        case TA_Base_Bus::TA_GenericGui::AREA_BANNER:
            rect = layoutFirst.bannerRect;
            rect.right = layoutLast.bannerRect.right;
            LOG_DEBUG("Getting the combined banner area of all monitors");
            break;

        case TA_Base_Bus::TA_GenericGui::AREA_MAINNAVIGATOR:
            rect = layoutFirst.applicationRect;
            rect.right = layoutLast.applicationRect.right;
            LOG_DEBUG("Getting the combined main navigator/application area of all monitors");
            break;

        case TA_Base_Bus::TA_GenericGui::AREA_LAUNCHPAD:
            rect = layoutFirst.launchBarRect;
            rect.right = layoutLast.launchBarRect.right;
            LOG_DEBUG("Getting the combined launchbar area of all monitors");
            break;

        case TA_Base_Bus::TA_GenericGui::AREA_SCHEMATIC:
            rect = layoutFirst.schematicRect;
            rect.right = layoutLast.schematicRect.right;
            LOG_DEBUG("Getting the combined schematic display area of all monitors");
            break;

        default:
            break;
        }
    }

    FUNCTION_EXIT;
    return rect;
}

RECT ScreenPositioning::getLaunchBarRect()
{
    RECT rect = NULL_RECT;

    for (ScreenLayout::iterator layout = m_screenLayout.begin(); layout != m_screenLayout.end(); layout++)
    {
        if (layout->second.launchBarPosition != ControlStation::HIDDEN)
        {
            rect = layout->second.launchBarRect;
        }
    }

    return rect;
}

RECT ScreenPositioning::getAppRectNextToLaunchBar()
{
    FUNCTION_ENTRY("getAppRectNextToLaunchBar");

    for (ScreenLayout::iterator layout = m_screenLayout.begin(); layout != m_screenLayout.end(); layout++)
    {
        if (layout->second.launchBarPosition != ControlStation::HIDDEN)
        {
            return layout->second.applicationRect;
        }
    }

    FUNCTION_EXIT;
    // just make it launch on the first screen
    return m_screenLayout[1].applicationRect;
}

ScreenPositioning::WindowPositionVector ScreenPositioning::getGraphworxPositions()
{
    FUNCTION_ENTRY("getGraphworxPositions");

    WindowPositionVector windowPosition;

    if (m_displaySpanAllScreens)
    {
        RECT graphworx = getRect(TA_Base_Bus::TA_GenericGui::SCREEN_ALL, TA_Base_Bus::TA_GenericGui::AREA_SCREEN);
        Coordinate topLeft;
        topLeft.first = graphworx.left;
        topLeft.second = graphworx.top;
        WindowPosition pos;
        pos.topLeft = topLeft;
        pos.width = graphworx.right - graphworx.left;
        pos.height = graphworx.bottom - graphworx.top;
        windowPosition.push_back(pos);
        FUNCTION_EXIT;
        return windowPosition;
    }

    for (unsigned int screen = 1; screen <= m_numberOfMonitors; screen++)
    {
        // if the right and bottom coordinates are 0 (no need to check left and top
        // as they would undoubtedly be 0 too, then it means that we dont' want to
        // display graphworx, otherwise we do.
        if (m_screenLayout[screen].hasGraphWorx)
        {
            RECT& graphworx = m_screenLayout[screen].applicationRect;
            Coordinate topLeft;
            topLeft.first = graphworx.left;
            topLeft.second = graphworx.top;
            WindowPosition pos;
            pos.topLeft = topLeft;
            pos.width = graphworx.right - graphworx.left;
            pos.height = graphworx.bottom - graphworx.top;
            windowPosition.push_back(pos);
        }
    }

    FUNCTION_EXIT;
    return windowPosition;
}

void ScreenPositioning::setDisplayDimension(const RECT dim)
{
    FUNCTION_ENTRY("setDisplayDimension");
    // see which screen we're setting
    int screenNum = getWhichScreenCoordinateIsOn(dim.left);

    if (screenNum > 0)
    {
        m_screenLayout[screenNum].schematicRect = dim;
        LOG_INFO("Setting the schematic dimension for screen %d as %d, %d, %d, %d", screenNum, dim.left, dim.top, dim.right, dim.bottom);
    }

    FUNCTION_EXIT;
}
