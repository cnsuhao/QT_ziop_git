#include "StdAfx.h"
#include "DatabaseStatusMonitor.h"
#include "core/utilities/src/RunParams.h"
#include <boost/test/unit_test.hpp>

using namespace TA_Base_Core;

BOOST_AUTO_TEST_CASE(DatabaseStatusMonitor_1)
{
    RunParams::getInstance().set(RPARAM_DBCONNECTIONFILE, "OCCConnectionStrings.csv");
    DatabaseStatusMonitor::getInstance().waitForAnyAvaliable();
    std::cout << "DB avaliable" << std::endl;
    boost::this_thread::sleep_for(boost::chrono::seconds(5));
}
