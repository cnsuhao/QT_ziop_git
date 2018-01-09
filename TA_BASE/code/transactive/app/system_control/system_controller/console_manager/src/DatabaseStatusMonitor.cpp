#include "StdAfx.h"
#include "DatabaseStatusMonitor.h"
#include "core/data_access_interface/src/DbConnectionStrings.h"
#include "core/exceptions/src/FatalConfigurationException.h"
#include "bus/database_status_monitor/src/SimpleDbStatusChecker.h"
#include "core/utilities/src/TAAssert.h"

using namespace TA_Base_Core;
using namespace TA_Base_Bus;

DatabaseStatusMonitor::DatabaseStatusMonitor()
    : m_pollPeriod(5)
{
}

void DatabaseStatusMonitor::asyncInitialize()
{
    addDbConnectionFile(RunParams::getInstance().promiseGet(RPARAM_DBCONNECTIONFILE));

    if (RunParams::getInstance().isSet(RPARAM_CENTRALDATABASE))
    {
        addDbConnection(RunParams::getInstance().get(RPARAM_CENTRALDATABASE));
    }

    if (RunParams::getInstance().isSet(RPARAM_MONITOREDDATABASE))
    {
        addDbConnection(RunParams::getInstance().get(RPARAM_MONITOREDDATABASE));
    }

    if (RunParams::getInstance().isSet(RPARAM_DBCONNECTION))
    {
        addDbConnection(RunParams::getInstance().get(RPARAM_DBCONNECTION));
    }
}

void DatabaseStatusMonitor::notifyStatusChange(const std::string& DbConnectionInfo, TA_Base_Bus::DatabaseStatusCorbaDef::EState newStatus)
{
    std::string name = DbConnectionStrings::getDatabaseName(DbConnectionInfo);
    const std::string& status = getDatabaseStatusString(newStatus);
    RunParams::getInstance().set(name, status);
    LOG_DEBUG("Notify database %s status is %s", name.c_str(), (RPARAM_DBONLINE == status ? "ONLINE" : "OFFLINE"));
    std::cout << boost::format("\t%s=%s") % name % status << std::endl;
    onDbStatus(name, status);
}

const std::string& DatabaseStatusMonitor::getDatabaseStatusString(DatabaseStatusCorbaDef::EState status)
{
    static std::map<DatabaseStatusCorbaDef::EState, std::string> map
    {
        { DatabaseStatusCorbaDef::statusUnknown, RPARAM_DBUNKNOWN },
        { DatabaseStatusCorbaDef::statusAvailable, RPARAM_DBONLINE },
        { DatabaseStatusCorbaDef::statusUnAvailable, RPARAM_DBOFFLINE }
    };

    return map[status];
}

void DatabaseStatusMonitor::addDbConnectionFile(const std::string& filename)
{
    if (m_dbConnectionFiles.insert(boost::filesystem::system_complete(filename)).second)
    {
        addDbConnections(DbConnectionStrings(filename).getConnectionStringSet());
        LOG_DEBUG("Add db-connection-file %s", filename.c_str());
    }
}

void DatabaseStatusMonitor::addDbConnection(const std::string& connectionString)
{
    if (m_connectionInfoSet.insert(connectionString).second)
    {
        TA_Base_Bus::SimpleDbStatusCheckerPtr db(new TA_Base_Bus::SimpleDbStatusChecker(connectionString, m_pollPeriod, this));
        db->start();
        m_dbStatusCheckers[connectionString] = db;
        m_dbNameSet.insert(DbConnectionStrings::getDatabaseName(connectionString));
        LOG_DEBUG("Add database %s to the monitored database map", connectionString.c_str());
    }
}

void DatabaseStatusMonitor::addDbConnections(const std::set<std::string>& connectionStrings)
{
    std::set<std::string> newConnectionStrings;

    std::set_difference(connectionStrings.begin(),
                        connectionStrings.end(),
                        m_connectionInfoSet.begin(),
                        m_connectionInfoSet.end(),
                        std::inserter(newConnectionStrings, newConnectionStrings.begin()));

    for (const std::string& connectionString : newConnectionStrings)
    {
        addDbConnection(connectionString);
    }
}

void DatabaseStatusMonitor::onRunParamChange(const std::string& name, const std::string& value)
{
    if (RPARAM_DBCONNECTIONFILE == name)
    {
        boost::async(boost::bind(&DatabaseStatusMonitor::addDbConnectionFile, this, value));
    }
}

void DatabaseStatusMonitor::onNewParam(const std::string& name, const std::string& value)
{
    if (boost::starts_with(name, RPARAM_DBPREFIX))
    {
        onDbStatus(name, value);
    }
}

void DatabaseStatusMonitor::waitForAnyAvaliable(int timeoutMs)
{
    LOG_DEBUG("DatabaseStatusMonitor::waitForAvaliable, timeoutMs = %d - begin", timeoutMs);

    if (timeoutMs != -1)
    {
        boost::future_status status = m_promiseAny.wait_for(boost::chrono::milliseconds(timeoutMs));
        TA_Assert(boost::future_status::ready == status);
    }
    else
    {
        m_promiseAny.wait();
    }

    LOG_DEBUG("DatabaseStatusMonitor::waitForAvaliable, timeoutMs = %d - end", timeoutMs);
}

void DatabaseStatusMonitor::waitForAllAvaliable(int timeoutMs)
{
    LOG_DEBUG("DatabaseStatusMonitor::waitForAllAvaliable, timeoutMs = %d - begin", timeoutMs);

    if (timeoutMs != -1)
    {
        boost::future_status status = m_promiseAll.wait_for(boost::chrono::milliseconds(timeoutMs));
        TA_Assert(boost::future_status::ready == status);
    }
    else
    {
        m_promiseAll.wait();
    }

    LOG_DEBUG("DatabaseStatusMonitor::waitForAllAvaliable, timeoutMs = %d - end", timeoutMs);
}

void DatabaseStatusMonitor::onDbStatus(const std::string& name, const std::string& status)
{
    m_dbNameSet.insert(name);

    if (RPARAM_DBONLINE == status)
    {
        m_onlineDbNameSet.insert(name);
        m_offlineDbNameSet.erase(name);
    }
    else if (RPARAM_DBOFFLINE == status)
    {
        m_onlineDbNameSet.erase(name);
        m_offlineDbNameSet.insert(name);
    }
    else if (RPARAM_DBUNKNOWN == status)
    {
        m_onlineDbNameSet.erase(name);
        m_offlineDbNameSet.erase(name);
    }

    checkAvaliable();
}

void DatabaseStatusMonitor::checkAvaliable()
{
    if (m_onlineDbNameSet.size())
    {
        m_promiseAny.set_value();

        if (m_offlineDbNameSet.empty())
        {
            m_promiseAll.set_value();
        }
    }
    else
    {
        if (m_offlineDbNameSet.size())
        {
            m_promiseAll.clear();

            if (m_offlineDbNameSet == m_dbNameSet)
            {
                m_promiseAny.clear();
            }
        }
    }
}
