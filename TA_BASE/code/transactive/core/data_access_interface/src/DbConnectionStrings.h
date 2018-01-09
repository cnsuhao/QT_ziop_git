/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/data_access_interface/src/DbConnectionStrings.h $
  * @author:  R.Stagg
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class reads and parses a csv file to get the connection
  * strings for all known data types
  */
#if !defined(DbConnectionStrings_29AA5110_185C_4781_89E1_C0663B2B94FD__INCLUDED_)
#define DbConnectionStrings_29AA5110_185C_4781_89E1_C0663B2B94FD__INCLUDED_

#include "core/data_access_interface/src/DbStatus.h"
#include "core/data_access_interface/src/DataTypeEnums.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "core/utilities/src/RunParams.h"
#include <map>
#include <string>
#include <vector>
#include <set>

namespace TA_Base_Core
{
    typedef std::set<std::string> ConnectionStringSet;
    typedef std::set<std::string> DatabaseNameSet;
    typedef std::pair<EDataTypes, EDataActions> DataKey;
    typedef std::pair<std::string, std::string> DataConnection; // database name, connection string
    typedef std::vector<DataConnection> DataConnections;
    typedef std::map<DataKey, DataConnections > DatabaseConnectionMap;
    typedef std::map<std::string, DatabaseConnectionMap> FileDatabaseConnectionMap;
    typedef std::map<std::string, ConnectionStringSet> FileDatabaseConnectionStringSetMap;
    typedef std::map<std::string, DatabaseNameSet> FileDatabaseNameSetMap;

    class DbConnectionStrings : public RunParamUser
    {
    public:

        DbConnectionStrings(const std::string& filename);
        virtual ~DbConnectionStrings() {}
        static DbConnectionStrings& getInstance();
        DatabaseConnectionMap& getConnectionMap();
        ConnectionStringSet& getConnectionStringSet();
        DatabaseNameSet& getDatabaseNameSet();

        /**
          * Fills in the conection string with the correct string for the given datatype
          * and action. An exception will be thrown if a string can not be found.
          * @param dataType    The type of data this connection is for.
          * @param action    the action to be preformed on this datatype
          * @param connectionString    The returned connection string goes here.
          *
          */
        DataConnections getConnectionList(EDataTypes, EDataActions);

        static const std::string& getDatabaseName(const std::string& connectionString, const std::string& prefix = RPARAM_DBPREFIX);

    protected:

        DbConnectionStrings();
        DbConnectionStrings(const DbConnectionStrings& theDbConnection);
        void parseFile(const std::string& filename);
        DatabaseConnectionMap& getConnectionMap(const std::string& filename);
        ConnectionStringSet& getConnectionStringSet(const std::string& filename);
        ConnectionStringSet& getDatabaseNameSet(const std::string& filename);
        DataConnections getConnectionList(const std::string& filename, EDataTypes, EDataActions);
        virtual void onRunParamChange(const std::string& name, const std::string& value);

    protected:

        std::string m_filename;
        ReEntrantThreadLockable m_lock;
        FileDatabaseConnectionMap m_fileDbMap;
        FileDatabaseConnectionMap m_fileDbMapRaw;
        FileDatabaseConnectionStringSetMap m_fileDbStrMap;
        FileDatabaseNameSetMap m_fileDbNameMap;
    };
} // namespace
#endif // !defined(DbConnectionStrings_29AA5110_185C_4781_89E1_C0663B2B94FD__INCLUDED_)
