#include "Utilities.h"
#include "SystemManagerConsts.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include <boost/lexical_cast.hpp>


namespace Utilities
{
    std::vector< std::vector<std::string> > convert(const TA_Base_Core::ProcessDataSeq& dataSeq)
    {
        std::vector< std::vector<std::string> > v(dataSeq.length());

        for (size_t i = 0; i < dataSeq.length(); ++i)
        {
            const TA_Base_Core::ProcessData& d = dataSeq[i];
            v[i].push_back(d.ProcessId.in());
            v[i].push_back(d.host.in());
            v[i].push_back(boost::lexical_cast<std::string>(d.operationMode));
            v[i].push_back(boost::lexical_cast<std::string>(d.state));
            v[i].push_back(boost::lexical_cast<std::string>(d.requestedState));
            v[i].push_back(boost::lexical_cast<std::string>(d.started));
            v[i].push_back(boost::lexical_cast<std::string>(d.restarts));
            v[i].push_back(d.debugLevel.in());
            v[i].push_back("");
        }

        return v;
    }


    std::map<std::pair<size_t, size_t>, std::string> compareProcessData(const TA_Base_Core::ProcessDataSeq& oldd, const TA_Base_Core::ProcessDataSeq& newd)
    {
        std::map<std::pair<size_t, size_t>, std::string> updates;

        if (oldd.length() != newd.length())
        {
            return updates;
        }

        size_t size = oldd.length();
        enum { PROCESS_ID, LOCATION, OPERATION_MODE, STATE, REQUESTED_STATE, STARTED, RESTARTS, DEBUG_LEVEL };

        for (size_t i = 0; i < size; ++i)
        {
            const TA_Base_Core::ProcessData& o = oldd[i];
            const TA_Base_Core::ProcessData& n = newd[i];

            if (o.operationMode != n.operationMode)
            {
                updates[std::make_pair(i, OPERATION_MODE)] = boost::lexical_cast<std::string>(n.operationMode);
            }

            if (o.state != n.state)
            {
                updates[std::make_pair(i, STATE)] = boost::lexical_cast<std::string>(n.state);
            }

            if (o.requestedState != n.requestedState)
            {
                updates[std::make_pair(i, REQUESTED_STATE)] = boost::lexical_cast<std::string>(n.requestedState);
            }

            if (o.started != n.started)
            {
                updates[std::make_pair(i, STARTED)] = boost::lexical_cast<std::string>(n.started);
            }

            if (o.restarts != n.restarts)
            {
                updates[std::make_pair(i, RESTARTS)] = boost::lexical_cast<std::string>(n.restarts);
            }

            if (std::strcmp(o.debugLevel.in(), n.debugLevel.in()))
            {
                updates[std::make_pair(i, DEBUG_LEVEL)] = boost::lexical_cast<std::string>(n.debugLevel.in());
            }
        }

        return updates;
    }
}
