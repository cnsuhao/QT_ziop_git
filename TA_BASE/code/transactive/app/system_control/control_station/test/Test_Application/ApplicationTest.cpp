#include "StdAfx.h"
#include <boost/test/unit_test.hpp>
#include "ControlStationAppController.h"
#include "SessionSignal.h"
#include "DataCache.h"
#include "ApplicationManager.h"
#include "ControlStationConstants.h"
#include "DatabaseStatusMonitor.h"
#include "Application.h"
#include "RunningApplication.h"
#include "RunningApplicationManager.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"

using namespace TA_Base_Core;

BOOST_AUTO_TEST_CASE(Application_1)
{
#if 0
    ControlStationAppController* appController = new ControlStationAppController(NULL, "");
    appController->initializeRunParams("--entity-name=ControlStation");
    appController->archiveLogFiles(RunParams::getInstance().promiseGet(RPARAM_DEBUGFILE));
    appController->initializeDebugUtility();

    boost::async(boost::bind(&ControlStationAppController::initializeCorba, appController));
    boost::async(boost::bind(&ControlStationAppController::locateSystemController, appController));
    boost::async(boost::bind(&ControlStationAppController::retrieveRunParamsFromSystemController, appController));

    DataCache::getInstanceNoInit();
    ApplicationManager::getInstanceNoInit();
    DatabaseStatusMonitor::getInstanceNoInit();
    ApplicationManager::getInstance();
    SessionSignal::logined("", nullptr, nullptr, nullptr, false);

    boost::this_thread::sleep_for(boost::chrono::hours(1));
#endif
}
