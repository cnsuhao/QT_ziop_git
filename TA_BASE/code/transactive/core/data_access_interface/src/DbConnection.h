/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/data_access_interface/src/DbConnection.h $
  * @author:  R.Stagg
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class will return a connection string for a given data type/action
  */
#if !defined(DbConnection_29AA5110_185C_4781_89E1_C0663B2B94FD__INCLUDED_)
#define DbConnection_29AA5110_185C_4781_89E1_C0663B2B94FD__INCLUDED_

#include "core/data_access_interface/src/DbStatus.h"
#include "core/data_access_interface/src/DataTypeEnums.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "core/timers/src/TimedWaitSemaphore.h"
#include "core/utilities/src/RunParamUser.h"
#include <ace/Singleton.h>
#include <ace/Recursive_Thread_Mutex.h>
#include <map>
#include <string>
#include <vector>

namespace TA_Base_Core
{
    class DbConnection : public RunParamUser
    {
        typedef std::pair<EDataTypes, EDataActions> DataKey;
        typedef std::pair<std::string, std::string> DataConnection; // database name, connection string
        typedef std::map<DataKey, std::vector<DataConnection> > DatabaseMap;

    public:

        static DbConnection& getInstance();
        static void removeInstance();

        /**
          * Fills in the conection string with the correct string for the given datatype
          * and action. An exception will be thrown if a string can not be found.
          * @param dataType    The type of data this connection is for.
          * @param action    the action to be preformed on this datatype
          * @param connectionString    The returned connection string goes here.
          *
          */
        void getConnectionString(const EDataTypes dataType, const EDataActions action, std::string& connectionString);

    protected:

        DbConnection();
        virtual ~DbConnection() {}
        friend class ACE_Singleton<DbConnection, ACE_Recursive_Thread_Mutex>;

        virtual void onRunParamChange(const std::string& name, const std::string& value);
        std::set<std::string> getDbNameRunParams(const std::vector<DataConnection>& connections);

    protected:

        DatabaseMap m_dbMap;
        NonReEntrantThreadLockable m_lock;
        TimedWaitSemaphore m_timedWaitSemaphore;
        std::set<std::string> m_waitingDbNameRunParams;
    };
} // namespace
#endif // !defined(DbConnection_29AA5110_185C_4781_89E1_C0663B2B94FD__INCLUDED_)
