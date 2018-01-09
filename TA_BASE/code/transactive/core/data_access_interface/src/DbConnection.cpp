/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/data_access_interface/src/DbConnection.cpp $
  * @author:  R.Stagg
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */

#ifdef __WIN32__
    #pragma warning(disable:4786)  // identifier was truncated to 'number' characters in the debug information
    #pragma warning(disable:4503)  // warning C4503: '  ' : decorated name length exceeded, name was truncated
#endif

#include "core/data_access_interface/src/DbConnection.h"
#include "core/data_access_interface/src/DbConnectionStrings.h"
#include "core/exceptions/src/DbConnectionFailed.h"
#include "core/exceptions/src/DbConnectionNoFile.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"
#include "core/threads/src/Thread.h"
#include <boost/algorithm/cxx11/one_of.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <iostream>

namespace TA_Base_Core
{
    DbConnection& DbConnection::getInstance()
    {
        return *ACE_Singleton<DbConnection, ACE_Recursive_Thread_Mutex>::instance();
    }

    void DbConnection::removeInstance()
    {
        ACE_Singleton<DbConnection, ACE_Recursive_Thread_Mutex>::close();
    }

    DbConnection::DbConnection()
        : m_timedWaitSemaphore(0)
    {
        FUNCTION_ENTRY("DbConnection");

        std::string filename = RunParams::getInstance().promiseGet(RPARAM_DBCONNECTIONFILE, 10000);
        TA_ASSERT(filename.size(), "db-connection-file not set");

        try
        {
            DbConnectionStrings connectStrings(filename);
            m_dbMap = connectStrings.getConnectionMap();
            DatabaseNameSet& dbNames = connectStrings.getDatabaseNameSet();

            BOOST_FOREACH(const std::string& dbName, dbNames)
            {
                RunParams::getInstance().registerRunParamUser(this, RPARAM_DBPREFIX + dbName);
            }
        }
        catch (DbConnectionNoFile e)
        {
            throw DbConnectionFailed(e.what());
        }

        FUNCTION_EXIT;
    }

    void DbConnection::getConnectionString(const EDataTypes dataType, const EDataActions action, std::string& connectionString)
    {
        FUNCTION_ENTRY("getConnectionString");
        TA_THREADGUARD(m_lock);
        LOG_DATABASE("Looking for a database for data type %i, action %i.", dataType, action);

        DatabaseMap::iterator dbIter = m_dbMap.find(std::make_pair(dataType, action));

        if (dbIter == m_dbMap.end())
        {
            // should create a lookup function to change the enums back into strings.
            throw DbConnectionFailed(str(boost::format("No connection string defined for %d/%s") % dataType % action));
        }

        std::vector<DataConnection>& dbList = dbIter->second;
        m_waitingDbNameRunParams = getDbNameRunParams(dbList);
        LOG_DEBUG("Trying to find working database...");

        for (size_t i = 0, MAX_RETRY = 3; i < MAX_RETRY; ++i)
        {
            LOG_DEBUG("Start Iteration %d of %d", i + 1, MAX_RETRY);

            BOOST_FOREACH(const DataConnection& connection, dbList)
            {
                if (DbStatus::getInstance().isAvailable(connection.first))
                {
                    connectionString = connection.second;
                    LOG_DEBUG("Found connection string: %s", connectionString.c_str());
                    FUNCTION_EXIT;
                    return;
                }
            }

            m_timedWaitSemaphore.timedWait(5000);
            LOG_DEBUG("End Iteration %d of %d", i + 1, MAX_RETRY);
        }

        // If we make it here, we failed to find any working db, so
        throw DbConnectionFailed("No working database found");
    }

    std::set<std::string> DbConnection::getDbNameRunParams(const std::vector<DataConnection>& connections)
    {
        std::set<std::string> dbNames;

        BOOST_FOREACH(const DataConnection& connection, connections)
        {
            dbNames.insert(RPARAM_DBPREFIX + connection.first);
        }

        return dbNames;
    }

    void DbConnection::onRunParamChange(const std::string& name, const std::string& value)
    {
        if (RPARAM_DBONLINE == value)
        {
            if (boost::algorithm::one_of_equal(m_waitingDbNameRunParams, name))
            {
                m_timedWaitSemaphore.post();
            }
        }
    }
}
