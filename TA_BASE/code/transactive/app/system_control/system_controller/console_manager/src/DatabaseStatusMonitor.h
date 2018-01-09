#pragma once
#include "StriCmp.h"
#include "Promise.h"
#include "Singleton.h"
#include "core/utilities/src/RunParamUser.h"
#include "bus/database_status_monitor/src/IDatabaseStatusObserver.h"
#include "bus/database_status_monitor/DsmCorbaDef/IDL/src/DatabaseStatusCorbaDef.h"

namespace TA_Base_Bus
{
    class SimpleDbStatusChecker;
    typedef boost::shared_ptr<SimpleDbStatusChecker> SimpleDbStatusCheckerPtr;
}

class DatabaseStatusMonitor : public TA_Base_Bus::IDatabaseStatusObserver, public Singleton<DatabaseStatusMonitor>
{
public:

    void waitForAnyAvaliable(int timeoutMs = -1);
    void waitForAllAvaliable(int timeoutMs = -1);

protected:

    DatabaseStatusMonitor();
    virtual void asyncInitialize() override;

    virtual void notifyStatusChange(const std::string& DbConnectionInfo, TA_Base_Bus::DatabaseStatusCorbaDef::EState status);
    virtual void onRunParamChange(const std::string& name, const std::string& value);
    virtual void onNewParam(const std::string& name, const std::string& value);

    static const std::string& getDatabaseStatusString(TA_Base_Bus::DatabaseStatusCorbaDef::EState status);
    void addDbConnectionFile(const std::string& filename);
    void addDbConnection(const std::string& connectionString);
    void addDbConnections(const std::set<std::string>& connectionStrings);
    void onDbStatus(const std::string& name, const std::string& status);
    void checkAvaliable();

private:

    int m_pollPeriod;
    std::set<std::string, StriCmp> m_connectionInfoSet;
    std::set<std::string, StriCmp> m_dbNameSet;
    std::set<std::string, StriCmp> m_onlineDbNameSet;
    std::set<std::string, StriCmp> m_offlineDbNameSet;
    typedef std::map<std::string, TA_Base_Bus::SimpleDbStatusCheckerPtr, StriCmp> SimpleDbStatusCheckerMap;
    SimpleDbStatusCheckerMap m_dbStatusCheckers;
    std::set<boost::filesystem::path> m_dbConnectionFiles;
    Promise<void> m_promiseAny;
    Promise<void> m_promiseAll;
};
