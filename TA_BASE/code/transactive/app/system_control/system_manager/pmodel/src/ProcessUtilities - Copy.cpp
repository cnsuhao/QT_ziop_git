#include "ProcessUtilities.h"
#include "ProcessDataInformation.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/case_conv.hpp>


namespace
{
    const char* const PROCESS_STATUS_STR[] = { "Unstarted", "Startup", "GoingToControl", "GoingToMonitor", "RunningControl", "RunningMonitor", "Terminating", "NotRunning", "Stopped" };
    const char* const OPERATION_MODE_STR[] = { "Control", "Monitor", "N/A" };
    TA_Base_App::ProcessDataInformation info;
}


namespace ProcessUtilities
{
    AndPredicate operator && (const IPredicate& lhs, const IPredicate& rhs)
    {
        return AndPredicate(lhs, rhs);
    }


    OrPredicate operator || (const IPredicate& lhs, const IPredicate& rhs)
    {
        return OrPredicate(lhs, rhs);
    }


    NotPredicate operator !(IPredicate& pred)
    {
        return NotPredicate(pred);
    }


    std::vector< std::vector<std::string> > convertToVector(const TA_Base_Core::ProcessDataSeq& dataSeq)
    {
        std::vector< std::vector<std::string> > v(dataSeq.length());

        for (size_t i = 0; i < dataSeq.length(); ++i)
        {
            const TA_Base_Core::ProcessData& d = dataSeq[i];
            v[i].push_back(getDescription(d.ProcessId.in()));
            v[i].push_back(d.host.in());
            v[i].push_back(OPERATION_MODE_STR[d.operationMode]);
            v[i].push_back(PROCESS_STATUS_STR[d.state]);
            v[i].push_back(PROCESS_STATUS_STR[d.requestedState]);
            v[i].push_back(getTimeStr(d.started));
            v[i].push_back(boost::lexical_cast<std::string>(d.restarts));
            v[i].push_back(d.debugLevel.in());
            v[i].push_back("");
        }

        return v;
    }


    std::map<std::pair<size_t, size_t>, std::string> getUpdates(const TA_Base_Core::ProcessDataSeq& oldd, const TA_Base_Core::ProcessDataSeq& newd)
    {
        std::map<std::pair<size_t, size_t>, std::string> updates;
        size_t size = oldd.length();
        enum { PROCESS_ID_COL, LOCATION_COL, OPERATION_MODE_COL, STATE_COL, REQUESTED_STATE_COL, STARTED_COL, RESTARTS_COL, DEBUG_LEVEL_COL };

        for (size_t i = 0; i < size; ++i)
        {
            const TA_Base_Core::ProcessData& o = oldd[i];
            const TA_Base_Core::ProcessData& n = newd[i];

            if (o.operationMode != n.operationMode)
            {
                updates[std::make_pair(i, OPERATION_MODE_COL)] = OPERATION_MODE_STR[n.operationMode];
            }

            if (o.state != n.state)
            {
                updates[std::make_pair(i, STATE_COL)] = PROCESS_STATUS_STR[n.state];
            }

            if (o.requestedState != n.requestedState)
            {
                updates[std::make_pair(i, REQUESTED_STATE_COL)] = PROCESS_STATUS_STR[n.requestedState];
            }

            if (o.started != n.started)
            {
                updates[std::make_pair(i, STARTED_COL)] = getTimeStr(n.started);
            }

            if (o.restarts != n.restarts)
            {
                updates[std::make_pair(i, RESTARTS_COL)] = boost::lexical_cast<std::string>(n.restarts);
            }

            if (std::strcmp(o.debugLevel.in(), n.debugLevel.in()))
            {
                updates[std::make_pair(i, DEBUG_LEVEL_COL)] = n.debugLevel.in();
            }
        }

        return updates;
    }


    bool isAllControl(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection)
    {
        return isAllMode(data, selection, TA_Base_Core::Control);
    }


    bool isAllMonitor(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection)
    {
        return isAllMode(data, selection, TA_Base_Core::Monitor);
    }


    bool isControl(const TA_Base_Core::ProcessDataSeq& data, size_t i)
    {
        return isMode(data, i, TA_Base_Core::Control);
    }


    bool isMonitor(const TA_Base_Core::ProcessDataSeq& data, size_t i)
    {
        return isMode(data, i, TA_Base_Core::Monitor);
    }


    bool isNotApplicable(const TA_Base_Core::ProcessDataSeq& data, size_t i)
    {
        return isMode(data, i, TA_Base_Core::NotApplicable);
    }


    bool isAllMode(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, TA_Base_Core::EOperationMode mode)
    {
        return predicate(data, selection, Predicate(boost::bind(&isMode, _1, _2, mode)));
    }


    bool isMode(const TA_Base_Core::ProcessDataSeq& data, size_t i, TA_Base_Core::EOperationMode mode)
    {
        return mode == data[i].operationMode;
    }


    bool isAnyStartAble(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection)
    {
        return predicate(data, selection, Predicate(&isStartAble), true);
    }


    bool isAnyStopAble(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection)
    {
        return predicate(data, selection, Predicate(&isStopAble), true);
    }


    bool isStartAble(const TA_Base_Core::ProcessDataSeq& data, size_t i)
    {
        TA_Base_Core::EProcessStatus state = data[i].state;
        TA_Base_Core::EProcessStatus requestedState = data[i].requestedState;
        return requestedState != TA_Base_Core::Startup && state == TA_Base_Core::Stopped;
    }


    bool isStopAble(const TA_Base_Core::ProcessDataSeq& data, size_t i)
    {
        TA_Base_Core::EProcessStatus state = data[i].state;
        TA_Base_Core::EProcessStatus requestedState = data[i].requestedState;
        return
            requestedState != TA_Base_Core::Stopped &&
            requestedState != TA_Base_Core::NotRunning &&
            state != TA_Base_Core::Stopped &&
            state != TA_Base_Core::Unstarted &&
            state != TA_Base_Core::NotRunning;
    }


    bool isAllSameDebugLevel(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection)
    {
        std::string debugLevel = data[*selection.begin()].debugLevel.in();
        return predicate(data, selection, Predicate(boost::bind(&isDebugLevel, _1, _2, debugLevel)));
    }


    bool isDebugLevel(const TA_Base_Core::ProcessDataSeq& data, size_t i, const std::string& level)
    {
        return data[i].debugLevel.in() == level;
    }


    bool isService(const TA_Base_Core::ProcessDataSeq& data, size_t i)
    {
        return info.getInfo(data[i].ProcessId.in()).isServiceProcess;
    }


    bool isAutoFailBack(const TA_Base_Core::ProcessDataSeq& data, size_t i)
    {
        return info.getInfo(data[i].ProcessId.in()).isAutoFailBack;
    }


    bool predicate(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, IPredicate& predicate, bool isAny)
    {
        BOOST_FOREACH(size_t i, selection)
        {
            bool result = predicate(data, i);

            if (isAny)
            {
                if (result)
                {
                    return true;
                }
            }
            else
            {
                if (false == result)
                {
                    return false;
                }
            }
        }

        return isAny ? false : true;
    }


    std::vector<std::string> selectStartAble(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, bool description)
    {
        return select(data, selection, Predicate(&isStartAble), description);
    }


    std::vector<std::string> selectStopAble(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, bool description)
    {
        return select(data, selection, Predicate(&isStopAble), description);
    }


    std::vector<std::string> selectMonitor(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, bool description)
    {
        return select(data, selection, Predicate(&isMonitor), description);
    }


    std::vector<std::string> selectControl(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, bool description)
    {
        return select(data, selection, Predicate(&isControl), description);
    }


    std::vector<std::string> selectNotDebugLevel(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, const std::string& debugLevel, bool description)
    {
        return select(data, selection, !Predicate(boost::bind(&isDebugLevel, _1, _2, debugLevel)), description);
    }


    std::vector<std::string> selectNotService(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, bool description)
    {
        return select(data, selection, !Predicate(&isService), description);
    }


    std::vector<std::string> select(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, const IPredicate& predicate, bool description)
    {
        std::vector<std::string> result;
        BOOST_FOREACH(size_t i, selection)
        {
            if (predicate(data, i))
            {
                result.push_back(description ? info.getInfo(data[i].ProcessId.in()).description : data[i].ProcessId.in());
            }
        }
        return result;
    }


    std::set<size_t> selectIndexes(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, const IPredicate& predicate)
    {
        std::set<size_t> result;
        BOOST_FOREACH(size_t i, selection)
        {
            if (predicate(data, i))
            {
                result.insert(i);
            }
        }
        return result;
    }


    std::vector<std::string> getNames(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection)
    {
        std::vector<std::string> ids;
        BOOST_FOREACH(size_t i, selection)
        {
            ids.push_back(data[i].ProcessId.in());
        }
        return ids;
    }


    std::vector<std::string> getNames(const TA_Base_Core::ProcessDataSeq& data)
    {
        std::vector<std::string> ids;

        for (size_t i = 0; i < data.length(); ++i)
        {
            ids.push_back(data[i].ProcessId.in());
        }

        return ids;
    }


    std::vector<std::string> getDescriptions(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection)
    {
        std::vector<std::string> des;
        BOOST_FOREACH(size_t i, selection)
        {
            des.push_back(getDescription(data[i].ProcessId.in()));
        }
        return des;
    }


    std::string getDebugLevel(const TA_Base_Core::ProcessDataSeq& data, size_t i)
    {
        return data[i].debugLevel.in();
    }


    std::string getTimeStr(time_t t)
    {
        char buff[20] = { 0 };
        strftime(buff, 20, "%Y/%m/%d %H:%M:%S", localtime(&t));
        return buff;
    }


    std::string getDescription(const TA_Base_Core::ProcessDataSeq& data, size_t i)
    {
        return info.getInfo(data[i].ProcessId.in()).description;
    }


    std::string getDescription(const std::string& name)
    {
        return info.getInfo(name).description;
    }
}
