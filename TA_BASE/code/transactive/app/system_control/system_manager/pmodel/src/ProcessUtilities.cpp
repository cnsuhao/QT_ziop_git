#include "ProcessUtilities.h"
#include "ProcessDataInformation.h"
#include "Factory.h"
#include "StdUtilities.h"
#include "IDatabaseService.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>


namespace
{
    const char* const DEBUG_LEVEL_STR[] = { "FATAL", "ERROR", "WARNING", "NORMAL", "INFO", "DEBUG", "SQL", "CORBA", "MSG", "MSGPUB", "PERIODIC", "DATABASE", "EXTERNAL", "TRACE" };
    const char* const PROCESS_STATUS_STR[] = { "Unstarted", "Startup", "GoingToControl", "GoingToMonitor", "RunningControl", "RunningMonitor", "Terminating", "NotRunning", "Stopped" };
    const char* const OPERATION_MODE_STR[] = { "Control", "Monitor", "N/A" };
    static StringList DEBUG_LEVELS(DEBUG_LEVEL_STR, DEBUG_LEVEL_STR + sizeof(DEBUG_LEVEL_STR) / sizeof(char*));
}


namespace TA_Base_App
{
    ProcessUtilities::ProcessUtilities()
        : m_data(NULL),
          m_dbservice(*(TA_Base_App::Factory::instance().getDatabaseService()))
    {
        initPredicates();
    }


    ProcessUtilities::ProcessUtilities(const TA_Base_Core::ProcessDataSeq& data)
        : m_data(&data),
          m_dbservice(*(TA_Base_App::Factory::instance().getDatabaseService()))
    {
        initPredicates();
    }


    ProcessUtilities::ProcessUtilities(const TA_Base_Core::ProcessDataSeq& data, const IndexSet& selection)
        : m_data(&data),
          m_selection(selection),
          m_dbservice(*(TA_Base_App::Factory::instance().getDatabaseService()))
    {
        initPredicates();
    }


    void ProcessUtilities::initPredicates()
    {
        control = Predicate(boost::bind(&ProcessUtilities::isControl, this, _1));
        monitor = Predicate(boost::bind(&ProcessUtilities::isMonitor, this, _1));
        service = Predicate(boost::bind(&ProcessUtilities::isService, this, _1));
        startAble = Predicate(boost::bind(&ProcessUtilities::isStartAble, this, _1));
        stopAble = Predicate(boost::bind(&ProcessUtilities::isStopAble, this, _1));
        notApplicable = Predicate(boost::bind(&ProcessUtilities::isNotApplicable, this, _1));
        autoFailBack = Predicate(boost::bind(&ProcessUtilities::isAutoFailBack, this, _1));
        canChangeMode = !service && !notApplicable;
        canChangeToControl = monitor;
        canChangeToMonitor = control && !service;
    }


    ProcessUtilities& ProcessUtilities::set(const TA_Base_Core::ProcessDataSeq& data)
    {
        m_data = &data;
        return *this;
    }


    ProcessUtilities& ProcessUtilities::set(const IndexSet& selection)
    {
        m_selection = selection;
        return *this;
    }


    ProcessUtilities& ProcessUtilities::set(const TA_Base_Core::ProcessDataSeq& data, const IndexSet& selection)
    {
        m_data = &data;
        m_selection = selection;
        return *this;
    }


    ProcessList ProcessUtilities::convertToVector()
    {
        size_t length = m_data->length();
        ProcessList processes(length);

        for (size_t i = 0; i < length; ++i)
        {
            const TA_Base_Core::ProcessData& d = (*m_data)[i];
            processes[i].push_back(getDescription(d.ProcessId.in()));
            processes[i].push_back(d.host.in());
            processes[i].push_back(OPERATION_MODE_STR[d.operationMode]);
            processes[i].push_back(PROCESS_STATUS_STR[d.state]);
            processes[i].push_back(PROCESS_STATUS_STR[d.requestedState]);
            processes[i].push_back(getTimeStr(d.started));
            processes[i].push_back(boost::lexical_cast<std::string>(d.restarts));
            processes[i].push_back(d.debugLevel.in());
            processes[i].push_back("");
        }

        return processes;
    }


    ProcessUpdates ProcessUtilities::getUpdates(const TA_Base_Core::ProcessDataSeq& oldd, const TA_Base_Core::ProcessDataSeq& newd)
    {
        ProcessUpdates updates;
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


    bool ProcessUtilities::isControl(size_t i)
    {
        return isMode(i, TA_Base_Core::Control);
    }


    bool ProcessUtilities::isMonitor(size_t i)
    {
        return isMode(i, TA_Base_Core::Monitor);
    }


    bool ProcessUtilities::isNotApplicable(size_t i)
    {
        return isMode(i, TA_Base_Core::NotApplicable);
    }


    bool ProcessUtilities::isMode(size_t i, TA_Base_Core::EOperationMode mode)
    {
        return mode == (*m_data)[i].operationMode;
    }


    bool ProcessUtilities::isStartAble(size_t i)
    {
        TA_Base_Core::EProcessStatus state = (*m_data)[i].state;
        TA_Base_Core::EProcessStatus requestedState = (*m_data)[i].requestedState;
        return requestedState != TA_Base_Core::Startup && state == TA_Base_Core::Stopped;
    }


    bool ProcessUtilities::isStopAble(size_t i)
    {
        TA_Base_Core::EProcessStatus state = (*m_data)[i].state;
        TA_Base_Core::EProcessStatus requestedState = (*m_data)[i].requestedState;
        return
            requestedState != TA_Base_Core::Stopped &&
            requestedState != TA_Base_Core::NotRunning &&
            state != TA_Base_Core::Stopped &&
            state != TA_Base_Core::Unstarted &&
            state != TA_Base_Core::NotRunning;
    }


    bool ProcessUtilities::isService(size_t i)
    {
        return m_dbservice.getProcessInfo((*m_data)[i].ProcessId.in()).isServiceProcess;
    }


    bool ProcessUtilities::isAutoFailBack(size_t i)
    {
        return m_dbservice.getProcessInfo((*m_data)[i].ProcessId.in()).isAutoFailBack;
    }


    bool ProcessUtilities::isDebugLevel(size_t i, const std::string& level)
    {
        return level == (*m_data)[i].debugLevel.in();
    }


    bool ProcessUtilities::predicate(const Predicate& pred, bool isAny)
    {
        BOOST_FOREACH(size_t i, m_selection)
        {
            bool result = pred(i);

            if (result && isAny)
            {
                return true;
            }

            if (!result && !isAny)
            {
                return false;
            }
        }

        return isAny ? false : true;
    }


    bool ProcessUtilities::isAllMode(TA_Base_Core::EOperationMode mode)
    {
        return predicate(Predicate(boost::bind(&ProcessUtilities::isMode, this, _1, mode)));
    }


    bool ProcessUtilities::isAllControl()
    {
        return isAllMode(TA_Base_Core::Control);
    }


    bool ProcessUtilities::isAllMonitor()
    {
        return isAllMode(TA_Base_Core::Monitor);
    }


    bool ProcessUtilities::isAnyStartAble()
    {
        return predicate(startAble, true);
    }


    bool ProcessUtilities::isAnyStopAble()
    {
        return predicate(stopAble, true);
    }


    bool ProcessUtilities::isAllSameDebugLevel()
    {
        std::string debugLevel = (*m_data)[*m_selection.begin()].debugLevel.in();
        return predicate(Predicate(boost::bind(&ProcessUtilities::isDebugLevel, this, _1, debugLevel)));
    }


    bool ProcessUtilities::isAnyCanChangeMode()
    {
        return predicate(!service && !notApplicable, true); // TODO: !isAutoFailBack
    }


    bool ProcessUtilities::isAllCannotChangeMode()
    {
        return predicate(!canChangeMode);
    }


    bool ProcessUtilities::isAllCannotChangeToControl()
    {
        return predicate(!canChangeToControl);
    }


    bool ProcessUtilities::isAllCannotChangeToMonitor()
    {
        return predicate(!canChangeToMonitor);
    }


    bool ProcessUtilities::isCanChangeToBothMode()
    {
        return predicate(canChangeToControl, true) && predicate(canChangeToMonitor, true);
    }


    StringList ProcessUtilities::select(const Predicate& pred, bool description)
    {
        StringList result;
        BOOST_FOREACH(size_t i, m_selection)
        {
            if (pred(i))
            {
                result.push_back(description ? m_dbservice.getProcessInfo((*m_data)[i].ProcessId.in()).description : (*m_data)[i].ProcessId.in());
            }
        }
        return result;
    }


    StringList ProcessUtilities::selectStartAble(bool description)
    {
        return select(startAble, description);
    }


    StringList ProcessUtilities::selectStopAble(bool description)
    {
        return select(stopAble, description);
    }


    StringList ProcessUtilities::selectNotStartAble(bool description)
    {
        return select(!startAble, description);
    }


    StringList ProcessUtilities::selectNotStopAble(bool description)
    {
        return select(!stopAble, description);
    }


    StringList ProcessUtilities::selectMonitor(bool description)
    {
        return select(monitor, description);
    }


    StringList ProcessUtilities::selectControl(bool description)
    {
        return select(control, description);
    }


    StringList ProcessUtilities::selectNotDebugLevel(const std::string& debugLevel, bool description)
    {
        return select(!Predicate(boost::bind(&ProcessUtilities::isDebugLevel, this, _1, debugLevel)), description);
    }


    StringList ProcessUtilities::selectNotService(bool description)
    {
        return select(!service, description);
    }


    StringList ProcessUtilities::selectCanChangeMode(bool description)
    {
        return select(canChangeMode, description);
    }


    StringList ProcessUtilities::selectCanChangeToControl(bool description)
    {
        return select(canChangeToControl, description);
    }


    StringList ProcessUtilities::selectCanChangeToMonitor(bool description)
    {
        return select(canChangeToMonitor, description);
    }


    StringList ProcessUtilities::selectCannotChangeMode(bool description)
    {
        return select(!canChangeMode, description);
    }


    StringList ProcessUtilities::selectCannotChangeToControl(bool description)
    {
        return select(!canChangeToControl, description);
    }


    StringList ProcessUtilities::selectCannotChangeToMonitor(bool description)
    {
        return select(!canChangeToMonitor, description);
    }


    IndexSet ProcessUtilities::selectIndexes(const Predicate& pred)
    {
        IndexSet result;
        BOOST_FOREACH(size_t i, m_selection)
        {
            if (pred(i))
            {
                result.insert(i);
            }
        }
        return result;
    }


    IndexSet ProcessUtilities::selectStartAbleIndexes()
    {
        return selectIndexes(startAble);
    }


    IndexSet ProcessUtilities::selectStopAbleIndexes()
    {
        return selectIndexes(stopAble);
    }


    IndexSet ProcessUtilities::selectCanChangeModeIndexes()
    {
        return selectIndexes(canChangeMode);
    }


    StringList ProcessUtilities::getNames()
    {
        StringList ids;
        BOOST_FOREACH(size_t i, m_selection)
        {
            ids.push_back((*m_data)[i].ProcessId.in());
        }
        return ids;
    }


    StringList ProcessUtilities::getAllNames()
    {
        StringList ids;

        for (size_t i = 0; i < (*m_data).length(); ++i)
        {
            ids.push_back((*m_data)[i].ProcessId.in());
        }

        return ids;
    }


    StringList ProcessUtilities::getDescriptions()
    {
        StringList des;
        BOOST_FOREACH(size_t i, m_selection)
        {
            des.push_back(getDescription((*m_data)[i].ProcessId.in()));
        }
        return des;
    }


    std::string ProcessUtilities::getDebugLevel(size_t i)
    {
        return (*m_data)[i].debugLevel.in();
    }


    std::string ProcessUtilities::getTimeStr(time_t t)
    {
        char buff[20] = { 0 };
        strftime(buff, 20, "%Y/%m/%d %H:%M:%S", localtime(&t));
        return buff;
    }


    std::string ProcessUtilities::getDescription(size_t i)
    {
        return m_dbservice.getProcessInfo((*m_data)[i].ProcessId.in()).description;
    }


    std::string ProcessUtilities::getDescription(const std::string& name)
    {
        return m_dbservice.getProcessInfo(name).description;
    }


    int ProcessUtilities::getDebugLevelIndex(const std::string& debugLevel)
    {
        return StdUtilities::findIndex(DEBUG_LEVELS, debugLevel);
    }


    const StringList& ProcessUtilities::getDebugLevels()
    {
        return DEBUG_LEVELS;
    }
}
