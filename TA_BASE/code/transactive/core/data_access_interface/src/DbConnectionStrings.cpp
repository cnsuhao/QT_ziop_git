/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/data_access_interface/src/DbConnectionStrings.cpp $
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
    #pragma warning(disable:4819)
    #pragma warning(disable:4996)
#endif

#include "core/data_access_interface/src/DbConnectionStrings.h"
#include "core/data_access_interface/src/DataTypesLookup.h"
#include "core/exceptions/src/DbConnectionNoFile.h"
#include "core/exceptions/src/DbConnectionFailed.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <ace/Singleton.h>
#include <iostream>

namespace fs = boost::filesystem;

namespace
{
    struct StriCmp
    {
        bool operator()(const std::string& lhs, const std::string& rhs) const
        {
            return boost::ilexicographical_compare(lhs, rhs);
        }
    };

    struct DbConnectionStringsImpl : TA_Base_Core::DbConnectionStrings {};
}

namespace TA_Base_Core
{
    DbConnectionStrings::DbConnectionStrings(const std::string& filename)
        : m_filename(filename)
    {
    }

    DbConnectionStrings::DbConnectionStrings()
    {
        TA_Assert(RunParams::getInstance().isSet(RPARAM_DBCONNECTIONFILE));
        m_filename = RunParams::getInstance().get(RPARAM_DBCONNECTIONFILE);
        RunParams::getInstance().registerRunParamUser(this, RPARAM_DBCONNECTIONFILE);
    }

    DbConnectionStrings& DbConnectionStrings::getInstance()
    {
        return *ACE_Unmanaged_Singleton<DbConnectionStringsImpl, ACE_Recursive_Thread_Mutex>::instance();
    }

    DatabaseConnectionMap& DbConnectionStrings::getConnectionMap()
    {
        return DbConnectionStrings::getInstance().getConnectionMap(m_filename);
    }

    ConnectionStringSet& DbConnectionStrings::getConnectionStringSet()
    {
        return DbConnectionStrings::getInstance().getConnectionStringSet(m_filename);
    }

    DatabaseNameSet& DbConnectionStrings::getDatabaseNameSet()
    {
        return DbConnectionStrings::getInstance().getDatabaseNameSet(m_filename);
    }

    DataConnections DbConnectionStrings::getConnectionList(EDataTypes dataType, EDataActions action)
    {
        return DbConnectionStrings::getInstance().getConnectionList(m_filename, dataType, action);
    }

    //
    // below are
    // Singleton Only
    //

    DatabaseConnectionMap& DbConnectionStrings::getConnectionMap(const std::string& filename)
    {
        TA_THREADGUARD(m_lock);

        if (m_fileDbMap[filename].empty())
        {
            parseFile(filename);
        }

        return m_fileDbMap[filename];
    }

    ConnectionStringSet& DbConnectionStrings::getConnectionStringSet(const std::string& filename)
    {
        TA_THREADGUARD(m_lock);

        if (m_fileDbMap[filename].empty())
        {
            parseFile(filename);
        }

        return m_fileDbStrMap[filename];
    }

    ConnectionStringSet& DbConnectionStrings::getDatabaseNameSet(const std::string& filename)
    {
        TA_THREADGUARD(m_lock);

        if (m_fileDbNameMap[filename].empty())
        {
            parseFile(filename);
        }

        return m_fileDbNameMap[filename];
    }

    DataConnections DbConnectionStrings::getConnectionList(const std::string& filename, EDataTypes dataType, EDataActions action)
    {
        FUNCTION_ENTRY("getConnectionList");

        TA_THREADGUARD(m_lock);

        LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug, "Looking for a database for data type %i, action %i.", dataType, action);

        DatabaseConnectionMap& dbMapRaw = m_fileDbMapRaw[filename];

        if (dbMapRaw.empty())
        {
            parseFile(filename);
        }

        DatabaseConnectionMap::iterator findIt = dbMapRaw.find(std::make_pair(dataType, action));

        if (findIt == dbMapRaw.end())
        {
            throw DbConnectionFailed(str(boost::format("No connection string defined for %d/%d") % dataType % action));
        }

        FUNCTION_EXIT;
        return findIt->second;
    }

    void DbConnectionStrings::parseFile(const std::string& filename)
    {
        // open file and slurp data into map
        TA_ASSERT(!filename.empty(), "Filename can not be null");

        TA_THREADGUARD(m_lock);

        if (!m_fileDbMap[filename].empty())
        {
            return;
        }

        fs::path filepath = fs::system_complete(fs::path(filename, fs::native));
        fs::ifstream dbConnectStringFile(filepath);

        if (!dbConnectStringFile)
        {
            std::string msg = "Unable to open file:" + filepath.native_file_string();
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug, msg.c_str());
            throw DbConnectionNoFile(msg.c_str());
        }

        std::set<std::string, StriCmp> dbStrSet2;
        std::set<std::string, StriCmp> dbNameSet2;
        DatabaseConnectionMap& dbMap = m_fileDbMap[filename];
        DatabaseConnectionMap& dbMapRaw = m_fileDbMapRaw[filename];

        for (std::string line; std::getline(dbConnectStringFile, line);)
        {
            if (line.empty())
            {
                continue;
            }

            std::vector<std::string> values;
            boost::split(values, line, boost::is_any_of(","), boost::token_compress_off);

            // run some checks to ensure we have the right number
            if (values.size() && values[0] == "Data Type")
            {
                continue; // Skip header row
            }

            if (values.size() < 7)   // min required is datatype,dataaction,dbtype,dbname,dbuser,dbpass,dbhostname
            {
                //throw; //something OR just log it ??
                LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug,
                    "Not enough parameters in file: %s ", filepath.native_file_string().c_str());
                LOGMORE(SourceInfo, "line is: %s :SKIPPING", line);
                continue;
            }

            if ((values.size() - 7) % 5 != 0)    // any remaing values must be connection triples
            {
                //throw; // something OR just log it ??
                LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug,
                    "Incorrect number of parameters in file: %s ", filepath.native_file_string().c_str());
                LOGMORE(SourceInfo, "line is: %s :SKIPPING", line);
                continue;
            }

            // we have the correct number of parameters, store them
            DataKey key(std::make_pair(getDataTypesEnum(values[0]), getDataActionsEnum(values[1])));
            values.erase(values.begin(), values.begin() + 2);

            std::vector<DataConnection> connections;
            std::vector<DataConnection> connectionsRaw;
            std::string strFirst;
            std::string strFirstRaw;
            std::string strSecond;

            while (values.size())
            {
                if (!values[0].empty())
                {
                    strFirst = (values[4].empty() ? values[1] : values[1] + "@" + values[4]); //Database Name
                    strFirstRaw = values[1];
                    strSecond = values[0] + ":" + values[1] + ":" + values[2] + ":" + values[3] + ":" + values[4]; //Connection String
                    connections.push_back(std::make_pair(strFirst, strSecond));
                    connectionsRaw.push_back(std::make_pair(strFirstRaw, strSecond));
                    dbStrSet2.insert(strSecond);
                    dbNameSet2.insert(strFirst);
                }

                values.erase(values.begin(), values.begin() + 5);
            }

            dbMap.insert(std::make_pair(key, connections));
            dbMapRaw.insert(std::make_pair(key, connectionsRaw));
        }

        ConnectionStringSet& dbStrSet = m_fileDbStrMap[filename];
        DatabaseNameSet& dbNameSet = m_fileDbNameMap[filename];
        dbStrSet.insert(dbStrSet2.begin(), dbStrSet2.end());
        dbNameSet.insert(dbNameSet2.begin(), dbNameSet2.end());
    }

    void DbConnectionStrings::onRunParamChange(const std::string& name, const std::string& value)
    {
        if (name == RPARAM_DBCONNECTIONFILE)
        {
            m_filename = value;
        }
    }

    const std::string& DbConnectionStrings::getDatabaseName(const std::string& connectionString, const std::string& prefix)
    {
        static std::map<std::string, std::string, StriCmp> cache;
        std::string& name = cache[connectionString];

        if (name.size())
        {
            return name;
        }

        std::vector<std::string> vs;
        boost::split(vs, connectionString, boost::is_any_of(",:/;|"), boost::token_compress_off);
        TA_ASSERT(5 == vs.size(), str(boost::format("Bad db connection string format :%s") % connectionString).c_str());
        return name = prefix + (vs[4].empty() ? vs[1] : (vs[1] + "@" + vs[4]));
    }
} // namespace
