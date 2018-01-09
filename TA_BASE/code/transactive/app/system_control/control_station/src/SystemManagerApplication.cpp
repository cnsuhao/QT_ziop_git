#include "StdAfx.h"
#include "SystemManagerApplication.h"
#include "Signals.h"
#include "SessionSignal.h"
#include "DataCache.h"
#include "ScreenPositioning.h"
#include "RunningApplicationManager.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/utilities/src/RunParams.h"

using namespace TA_Base_Bus;
using namespace TA_Base_Core;

SystemManagerApplication::SystemManagerApplication(TA_Base_Core::IGui* gui)
    : Application(gui)
{
    setupSignalConnections();
}

SystemManagerApplication::~SystemManagerApplication()
{
}

void SystemManagerApplication::setupSignalConnections()
{
    SessionSignal::logined.connect(boost::bind(&SystemManagerApplication::onLogin, this));
}

SystemManagerApplication* SystemManagerApplication::create(TA_Base_Core::IGui* gui)
{
    return new SystemManagerApplication(gui);
}

void SystemManagerApplication::onLogin()
{
    static RECT defaultRect = { -1, -1, -1, -1 };
    RECT boundaryRect = defaultRect;
    std::string commandline = " --EngityName=SystemManager --DebugFile=C:\\transactive\\logs\\SystemManager.log ";

    RunningApplicationManager::getInstance().createRunningApplication(this,
                                                                      commandline,
                                                                      TA_Base_Bus::TA_GenericGui::POS_BOUNDED,
                                                                      TA_Base_Bus::TA_GenericGui::ALIGN_FIT,
                                                                      defaultRect,
                                                                      boundaryRect,
                                                                      false);
}
