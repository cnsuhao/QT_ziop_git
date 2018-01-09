#include "StdAfx.h"
#include "Utilities.h"
#include "SessionDetails.h"
#include "SessionManager.h"
#include "core/utilities/src/RunParams.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"

namespace Utilities
{
    const std::string& getOperatorNameFromSession()
    {
        static std::map<unsigned long, std::string> s_operatorMap;
        std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
        SessionDetails& details = SessionDetails::getInstance();
        SessionDetails::SessionInfo sessionInfo = details.getSessionInfo();
        unsigned long operatorId = sessionInfo.operatorId;

        if (s_operatorMap.find(operatorId) == s_operatorMap.end())
        {
            TA_Base_Core::IOperatorPtr iOperator(TA_Base_Core::OperatorAccessFactory::getInstance().getOperator(sessionInfo.operatorId, false));
            s_operatorMap.insert(std::make_pair(operatorId, iOperator->getName()));
        }

        return s_operatorMap[operatorId];
    }

    //TA_Base_Core::RunParam makeRunParam(const std::string& n, const std::string& v)
    //{
    //    TA_Base_Core::RunParam param;
    //    param.name = boost::lexical_cast<std::string>(n).c_str();
    //    param.value = boost::lexical_cast<std::string>(v).c_str();
    //    return param;
    //}

    bool isStation(const std::string& stationName)
    {
        static std::set<std::string> noneStations = { "All Locations", "OCC", "TDS", "KCD", "DCC", "KOCC", "TMF" };
        return boost::algorithm::none_of(noneStations, iEqualsTo(stationName));
    }

    bool isDepot(const std::string& stationName)
    {
        return boost::iequals(stationName, "KCD");
    }
}
