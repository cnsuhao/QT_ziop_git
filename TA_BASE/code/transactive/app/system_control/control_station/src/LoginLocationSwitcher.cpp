#include "StdAfx.h"
#include "LoginLocationSwitcher.h"
#include "Signals.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"

using namespace TA_Base_Core;

LoginLocationSwitcher::LoginLocationSwitcher()
{
    setupSignalConnections();
}

void LoginLocationSwitcher::setupSignalConnections()
{
    Signals::operatorRequestLoginInfo.connect(boost::bind(&LoginLocationSwitcher::handleOperatorRequestLoginInfoSignal, this, _3));
}

void LoginLocationSwitcher::handleOperatorRequestLoginInfoSignal(ILocation* location)
{
    //TODO: cache

    if (location && location->getKey())
    {
        boost::smatch m;
        static boost::regex e("(?ix) ^(.*?) (\\w+) (ConnectionStrings.csv)$");
        std::string dbConnectionFile = TA_Base_Core::RunParams::getInstance().get(RPARAM_DBCONNECTIONFILE);

        if (boost::regex_match(dbConnectionFile, m, e))
        {
            std::string newDbConnectionFile = m.str(1) + location->getName() + m.str(3);
            TA_Base_Core::RunParams::getInstance().set(RPARAM_DBCONNECTIONFILE, newDbConnectionFile);
        }
    }
}
