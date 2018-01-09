/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source: $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/data_access_interface/src/SimpleDbDatabase.cpp $
  * @author Nick Jardine
  * @version $Revision: #1 $
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by: Nick Jardine
  *
  * SimpleDbDatabase is an implementation of IDatabase, and is responsible for executing queries
  * on the database. It does not analyse the returned data - that is the responsiblility of
  * SimpleDbData.
  * SimpleDbDatabase is specific to SimpleDb and will not work for any other database library.
  *
  */


///////////////////////////////////////////////////////////
//
//  SimpleDbDatabase.cpp
//  Implementation of the Class SimpleDbDatabase
//  Generated by Enterprise Architect
//  Created on:      11-Mar-2003 15:25:06
//  Original author: Karen Graham
//
///////////////////////////////////////////////////////////
//  Modification history:
//
//
///////////////////////////////////////////////////////////

#ifdef __WIN32__
    #pragma warning(disable:4786)
    #pragma warning(disable:4503)
#endif

#include <sstream>
#include "core/data_access_interface/src/SimpleDbDatabase.h"
#include "core/data_access_interface/src/SimpleDbData.h"
#include "core/data_access_interface/src/DbStatus.h"
#include "core/database/src/SimpleDb.h"
#include "core/database/src/DBException.h"
#include "core/database/src/OCIConnection.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DbConnectionFailed.h"
#include "core/exceptions/src/DbUniqueConstraintViolated.h"
#include "core/exceptions/src/SQLException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"

using TA_Base_Core::DebugUtil;

using TA_Base_Core::DatabaseException;

namespace TA_Base_Core
{
    void SimpleDbDatabase::connect(const std::string& connectionString)
    {
        FUNCTION_ENTRY("connect");

        if (NULL != m_db && m_db->IsNeedChange(connectionString))
        {
            // release the m_db object and change to the new connection string
            disconnect();
        }

        if (m_db == NULL) // The pointer has not been initialised. We need to set it up.
        {
            // Get the database connection information (i.e. servicename:username:password).
            //
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug,
                "SimpleDbDatabase::connect(): Database connection string is: %s", connectionString.c_str());

            // Parse the Connection String to seperate the database connection information.
            //
            std::vector<std::string> dbConnData;
            SimpleDb::ParseConnectStr(connectionString, dbConnData);

            // 3 is used because dbConnData should consist of servicename:username:password
            // The comparrison operator is "<" to support legacy connection strings that include
            // the hostname on the end. Oracle will ignore all but the first three items anyway, so
            // as long as we have at least three, then there is not a problem.
            if (dbConnData.size() < 5) //The connection string is not valid
            {
                FUNCTION_EXIT;
                TA_THROW(DbConnectionFailed("The database connection string is not valid."));
            }

            // SimpleDb creates the database connection in the constructor, hence we must
            // pass the connection string with a call to "new"
            try
            {
                m_db = new SimpleDb(dbConnData[0].c_str(), dbConnData[1].c_str(), dbConnData[2].c_str(), dbConnData[3].c_str(), dbConnData[4].c_str());
            }
            catch (const DBException& e)
            {
                try
                {
                    // mark this db bad if we can
                    std::string db(RPARAM_DBPREFIX /*+ dbConnData[1]*/);

                    if (!dbConnData[4].empty())
                        db += (dbConnData[1] + "@" + dbConnData[4]);
                    else
                        db += dbConnData[1];

                    DbStatus::getInstance().onRunParamChange(db, std::string(RPARAM_DBOFFLINE));
                }
                catch (...)
                {
                }

                LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", e.getSummary().c_str());
                FUNCTION_EXIT;
                TA_THROW(DatabaseException(e.getReason()));
            }
        }

        try
        {
            tryToOpenDb();
        }
        catch (const DBException& e)
        {
            // this db has gone bad, caller should try another one
            disconnect();

            try
            {
                // Parse the Connection String to seperate the database connection information.
                //
                std::vector<std::string> dbConnData;
                SimpleDb::ParseConnectStr(connectionString, dbConnData);
                // mark this db bad if we can
                std::string db(RPARAM_DBPREFIX /*+ dbConnData[1]*/);

                if (!dbConnData[4].empty())
                    db += (dbConnData[1] + "@" + dbConnData[4]);
                else
                    db += dbConnData[1];

                DbStatus::getInstance().onRunParamChange(db, std::string(RPARAM_DBOFFLINE));
            }
            catch (...) // don't care about errors - nothing we can do about them anyway
            {
            }

            LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", e.getSummary().c_str());
            FUNCTION_EXIT;
            TA_THROW(DatabaseException(e.getReason()));
        }

        FUNCTION_EXIT;
    }


    void SimpleDbDatabase::disconnect()
    {
        FUNCTION_ENTRY("disconnect");

        if (NULL != m_db)
        {
            try
            {
                m_db->Close();
            }
            catch (const DBException& e)
            {
                LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", e.getSummary().c_str());
            }

            delete m_db;
            m_db = NULL;
        }

        FUNCTION_EXIT;
    }

    SimpleDbDatabase::~SimpleDbDatabase()
    {
        FUNCTION_ENTRY("~SimpleDbDatabase");
        disconnect();
        FUNCTION_EXIT;
    }

    void SimpleDbDatabase::cleanQuery()
    {
        FUNCTION_ENTRY("cleanQuery");

        if (m_db != NULL)
        {
            try
            {
                m_db->cleanQuery();
            }
            catch (...)
            {
                LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", "database cleanQuery failure");
            }
        }
        else
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", "database is NULL");
        }

        FUNCTION_EXIT;
    }

    IData* SimpleDbDatabase::executeQuery(const SQLStatement& rSqlObj, const std::vector<std::string>& columnNames)
    {
        return (executeQuery(rSqlObj, columnNames, SimpleDb::MAX_ROWS));
    }

    IData* SimpleDbDatabase::executeQuery(const SQLStatement& rSqlObj, const std::vector<std::string>& columnNames,
                                          const unsigned long numRows)
    {
        FUNCTION_ENTRY("executeQuery");

        //    LOG ( SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugSQL, "SimpleDbDatabase::executeQuery(): SQL statement to execute is: %s", sql.c_str());

        // Assert the pre-conditions on this function
        //TA_ASSERT(0 < sql.size(),"SQL statement passed in is zero size");
        TA_ASSERT(0 < numRows, "numRows is 0 - this is not a sensible number");
        TA_ASSERT(static_cast<unsigned int>(SimpleDb::MAX_ROWS) >= numRows, "numRows is greater than SimpleDb::MAX_ROWS - this is not permissable by SimpleDb");
        TA_ASSERT(columnNames.size() <= static_cast<unsigned int>(m_db->getMaxColumns()), "number of columns is greater than OCIAccess::MAX_COLS - not permissable by SimpleDb");
        TA_ASSERT(0 < columnNames.size(), "columnNames is zero size");
        TA_ASSERT(NULL != m_db, "Database pointer is null."); // connect has not been called

        // Create a buffer in which to store the retrieved data
        Buffer data;

        try
        {
            tryToOpenDb();

            m_db->Exec(rSqlObj, columnNames.size(), data, numRows);
        }
        catch (const DBException& e)
        {
            disconnect();

            try
            {
                DatabaseFactory::getInstance().getDatabase(m_dataType, m_dataAction, this);
                m_db->Exec(rSqlObj, columnNames.size(), data, numRows);
            }
            catch (const DBException& e)
            {
                disconnect();

                try
                {
                    DatabaseFactory::getInstance().getDatabase(m_dataType, m_dataAction, this);
                    m_db->Exec(rSqlObj, columnNames.size(), data, numRows);
                }
                catch (...)
                {
                    // if we catch anything it means there are no dbs available
                    // so just throw original error
                    LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", e.getSummary().c_str());
                    m_db->Close();
                    TA_THROW(DbConnectionFailed(e.getReason()));
                }
            }
        }
        catch (const DatabaseException& e)
        {
            // if we catch anything in here, it means the db in used no long available
            // so just record the log, next time the application getDatabase will try another db
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DatabaseException", e.what());
        }

        // Set the member variables to match the passed-in variables
        m_columnNames = columnNames;


        // Place the data we just loaded into an instance of TA_Base_Core::SimpleDbData (which IS A IData)
        TA_Base_Core::SimpleDbData* idata = new TA_Base_Core::SimpleDbData(columnNames, data);
        m_numRows = idata->getNumRows();

        FUNCTION_EXIT;
        return idata; // Return a pointer to the new IData object
    }


    void SimpleDbDatabase::executeModification(const SQLStatement& rSqlObj)
    {
        FUNCTION_ENTRY("executeModification");

        /*LOG ( SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug,
            "The SQL modification to execute is: %s", sql.c_str());*/

        // Assert the pre-conditions on this function
        //TA_ASSERT(0 < sql.size(),"SQL statement passed in is zero size");
        TA_ASSERT(NULL != m_db, "Database pointer is null."); // connect has not been called

        try
        {
            tryToOpenDb();

            m_db->Exec(rSqlObj);

//#ifdef TDS_DEVELOP
            // If running in TDS station, log update SQL which trigged from CE, add by hongzhi
            if ((RunParams::getInstance().get(RPARAM_ENTITYNAME).compare("ConfigurationEditor") == 0) &&
                    (TA_Base_Core::RunParams::getInstance().isSet(RPARAM_TDSCE)))
            {
                std::string str = TA_Base_Core::RunParams::getInstance().get("ComponentType");
                LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "ComponentType=%s", str.c_str());

                if ((TA_Base_Core::RunParams::getInstance().get("ComponentType").compare("DataPoints") == 0) ||
                        (TA_Base_Core::RunParams::getInstance().get("ComponentType").compare("Displays") == 0) ||
                        (TA_Base_Core::RunParams::getInstance().get("ComponentType").compare("Station System") == 0) ||
                        (TA_Base_Core::RunParams::getInstance().get("ComponentType").compare("MMS Scheduling") == 0))
                {
                    std::string strOraPara, strMyPara;
                    _GetBothSQLFromObj(rSqlObj, strOraPara, strMyPara);

                    if (strOraPara.find("update config_editor_timestamp set last_timestamp = current_timestamp") == std::string::npos &&
                            strOraPara.find("insert into event ") == std::string::npos &&
                            strOraPara.find("update tds_sqllog set log_content ='") == std::string::npos &&
                            strOraPara.find("insert into user_settings (OP_KEY,AP_TYPE_KEY,SETTING_NAME,SETTING_VALUE) values") == std::string::npos)
                    {
                        /*std::string strSql = sql;
                        if (  (strSql.find("audit_data_queue_pkg.enqueue_audit_data") != std::string::npos)
                            ||(strSql.find("AUDIT_DATA_QUEUE_PKG.ENQUEUE_AUDIT_DATA") != std::string::npos)  )
                        {   // there are aq statement inside, if we filter aq, just call ParserCallPkgString() instead of escapeInsertString()
                            ParserCallPkgString(strSql);
                            //strSql = escapeInsertString(strSql);
                        }
                        else
                        {   //it is not a aq statement, it is a normal statement
                            strSql = escapeInsertString(strSql);            //call this always no matter if it is aq
                        }*/

                        ParserSQLString(strOraPara);        //add ";" and "/n" to the end of sql
                        ParserSQLString(strMyPara);         //add ";" and "/n" to the end of sql

                        SQLStatement sqlStatement;
                        prepareSQLStatement(sqlStatement, TDS_SQLLOG_UPDATE_81001, strOraPara, strMyPara);
                        m_db->Exec(sqlStatement);

                        /*std::string sqlTDS;
                        sqlTDS = "update tds_sqllog set log_content = concat((select log_content from tds_sqllog) , '" + strSql + "')";
                        m_db->Exec( sqlTDS.c_str() );*/
                        //                      TdsSQLLoggerWritter::getInstance().WriteTDSLog(sqlTDS.c_str());
                    }
                }
            }

//#endif
        }
        catch (const DBException& e)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", e.getSummary().c_str());

            switch (e.getNativeCode())
            {
            case OCIConnection::UNIQUE_CONSTRAINT_VIOLATED_ERROR:
                TA_THROW(DbUniqueConstraintViolated(e.getReason()));
                break;

            default:
                // this db has gone bad, try another one
                disconnect();

                try
                {
                    DatabaseFactory::getInstance().getDatabase(m_dataType, m_dataAction, this);
                    m_db->Exec(rSqlObj);
                }
                catch (const DBException& e)
                {
                    disconnect();

                    try
                    {
                        DatabaseFactory::getInstance().getDatabase(m_dataType, m_dataAction, this);
                        m_db->Exec(rSqlObj);
                    }
                    catch (...)
                    {
                        // if we catch anything it means there are no dbs available
                        // so just throw original error
                        LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", e.getSummary().c_str());
                        m_db->Close();
                        TA_THROW(DbConnectionFailed(e.getReason()));
                    }
                }

                break;
            }
        }
        catch (const DatabaseException& e)
        {
            // if we catch anything in here, it means the db in used no long available
            // so just record the log, next time the application getDatabase will try another db
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DatabaseException", e.what());
        }

        FUNCTION_EXIT;
    }

    IData* SimpleDbDatabase::executeProcedure(const SQLStatement& rSqlObj, const std::vector<std::string>& columnNames,
                                              const unsigned long numRows /*=1000*/)
    {
        // Due to restrictions in SimpleDb, we are not able to return data from a procedure.
        // As such, the use of this method is a programming error.
        TA_ASSERT(false, "This method is not valid for SimpleDbDatabase");

        // We have to return here to allow the program to compile
        return NULL;
    }

    void SimpleDbDatabase::executeProcedure(const SQLStatement& rSqlObj)
    {
        // Oracle procedures are executed in the same manner as standard queries - therefore we use
        // the same code
        executeModification(rSqlObj);
    }


    bool SimpleDbDatabase::moreData(IData*& returnData)
    {
        FUNCTION_ENTRY("moreData");

        // The IData pointer MUST be null when passed to this method. If it is not, a memory leak
        // will result. This is tested by the assert.
        TA_ASSERT(NULL != m_db, "Database pointer is null");

        //TA_ASSERT( false != m_db->IsOpen(), "Database must be open to request more data" );
        if (! m_db->IsOpen())
        {
            try
            {
                tryToOpenDb();
                LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn, "Database should be opened while requesting more data,something wrong may occurred");
            }
            catch (DatabaseException&)
            {
                LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugError, "moreData: Database exception or crash down");
                return false;
            }
            catch (...)
            {
                LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugError, "moreData: Unknown Database exception");
                return false;
            }
        }

        if (true == m_db->HasMore()) // There is data to read
        {
            Buffer data;

            try
            {
                LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDatabase, "SimpleDbDatabase::moreData calling FetchMore...");
                m_db->FetchMore(m_columnNames.size(), data, SimpleDb::MAX_ROWS);
            }
            catch (const DBException& e)
            {
                LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", e.getSummary().c_str());
                TA_THROW(DatabaseException(e.getReason()));
            }

            // Check if there was actually more data to retrieve
            if (data.size() == 0)
            {
                FUNCTION_EXIT;
                return false;
            }

            returnData = new TA_Base_Core::SimpleDbData(m_columnNames, data);
            FUNCTION_EXIT;
            return true;
        }
        else
        {
            m_db->cleanQuery();
        }

        FUNCTION_EXIT;
        return false; // There was no more data to read
    }


    void SimpleDbDatabase::readBLOB(const std::string& fieldName, const std::string& tableName,
                                    const std::string& whereClause, std::vector<unsigned char>& data)
    {
        TA_ASSERT(m_db != NULL, "Database pointer has not been initialised");
        TA_ASSERT(fieldName.length() > 0, "The BLOB field name has not been specified");
        TA_ASSERT(tableName.length() > 0, "The BLOB table name has not been specified");

        tryToOpenDb();

        try
        {
            m_db->readBLOB(fieldName, tableName, whereClause, data);
        }
        catch (const DBException& e)
        {
            // this db has gone bad, try another one
            disconnect();

            try
            {
                DatabaseFactory::getInstance().getDatabase(m_dataType, m_dataAction, this);
                m_db->readBLOB(fieldName, tableName, whereClause, data);
            }
            catch (...)
            {
                // if we catch anything it means there are no dbs available
                // so just throw original error
                LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", e.getSummary().c_str());

                LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugError, "Failed to read BLOB filed \
				%s from table %s with clause %s", fieldName.c_str(), tableName.c_str(), whereClause.c_str());

                TA_THROW(DatabaseException(e.getReason()));
            }
        }
    }


    void SimpleDbDatabase::writeBLOB(const std::string& fieldName, const std::string& tableName,
                                     const std::string& whereClause, std::vector<unsigned char>& data)
    {
        TA_ASSERT(m_db != NULL, "Database pointer has not been initialised");
        TA_ASSERT(fieldName.length() > 0, "The BLOB field name has not been specified");
        TA_ASSERT(tableName.length() > 0, "The BLOB table name has not been specified");

        tryToOpenDb();


        try
        {
            m_db->writeBLOB(fieldName, tableName, whereClause, data);
        }
        catch (const DBException& e)
        {
            // this db has gone bad, try another one
            disconnect();

            try
            {
                DatabaseFactory::getInstance().getDatabase(m_dataType, m_dataAction, this);
                m_db->writeBLOB(fieldName, tableName, whereClause, data);
            }
            catch (...)
            {
                // if we catch anything it means there are no dbs available
                // so just throw original error
                LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", e.getSummary().c_str());

                LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugError, "Failed to write BLOB filed \
				%s to table %s with clause %s", fieldName.c_str(), tableName.c_str(), whereClause.c_str());

                TA_THROW(DatabaseException(e.getReason()));
            }
        }
    }


    //Mintao++: use for Distributed DB
    std::string SimpleDbDatabase::escapeAQSQLString(const std::string& theString)
    {
        return (escapeInsertString(escapeInsertString(theString)));
    }
    //Mintao++: use for Distributed DB

    int SimpleDbDatabase::getDbServerType()
    {
        return _GetDbServerType();
    }

    void SimpleDbDatabase::setDataTypeAction(EDataTypes dataType, EDataActions dataAction)
    {
        m_dataType   = dataType;
        m_dataAction = dataAction;
    }

    int SimpleDbDatabase::_GetDbServerType()
    {
        return m_db->GetDbServerType();
    }

    void SimpleDbDatabase::prepareSQLStatement(SQLStatement& rSqlStatement, const SQLTypeAdapter& arg0)
    {
        return prepareSQLStatement(rSqlStatement, SQLVarParms() << arg0);
    }

    void SimpleDbDatabase::prepareSQLStatement(SQLStatement& rSqlStatement, SQLVarParms& varParms)
    {
        FUNCTION_ENTRY("prepareSQLStatement");

        try
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugSQL, "Begin prepare SQL statement...");
            //std::string strSQL;
            //SQLVarParms  sqlVarParam(varParms);
            rSqlStatement.strCommon.clear();
            rSqlStatement.strMySQL.clear();
            rSqlStatement.strOracle.clear();
            SQLCode::getInstance().buildSQLStatement(varParms, rSqlStatement);

            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugSQL, "End prepare SQL statement.");
            //return strSQL;
        }
        catch (BadParamCount* e)
        {
            SQLCodeException SQLException(e->what());
            throw SQLException;
        }
        catch (BadIndex* e)
        {
            SQLCodeException SQLException(e->what());
            throw SQLException;
        }
        catch (...)
        {
            SQLCodeException SQLException("Unknown SQLCode exception");
            throw SQLException;
        }

        FUNCTION_EXIT;
    }

    std::string SimpleDbDatabase::escapeInsertString(const std::string& theString)
    {
        // create a new string to hold the parsed string
        std::string escapedString = theString;
        int pos = escapedString.find_first_of("'");

        while (-1 != pos)
        {
            char charToInsert = escapedString.at(pos);
            escapedString.insert(pos, &charToInsert, 1);
            pos = escapedString.find_first_of("'", pos + 2);
        }

        return escapedString;
    }

    std::string SimpleDbDatabase::escapeQueryString(const std::string& theString)
    {
        return escapeInsertString(theString);
    }


    void SimpleDbDatabase::beginTransaction()
    {
        TA_ASSERT(false, "beginTransaction() is not supported by SimpleDb");
    }


    void SimpleDbDatabase::commitTransaction()
    {
        TA_ASSERT(false, "commitTransaction() is not supported by SimpleDb");
    }

    void SimpleDbDatabase::rollbackTransaction()
    {
        TA_ASSERT(false, "rollbackTransaction() is not supported by SimpleDb");
    }

    void SimpleDbDatabase::tryToOpenDb()
    {
        if (false == m_db->IsOpen())
        {
            try
            {
                m_db->Open();
            }
            catch (const DBException& e)
            {
                LOG(SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", e.getSummary().c_str());

                TA_THROW(e);
            }
        }
    }

//#ifdef TDS_DEVELOP
    //hongzhi, for TDS
    void SimpleDbDatabase::ParserCallPkgString(std::string& str)
    {
        std::string::size_type len = 0,  pos_begin, pos_end;
        std::string::size_type pos_begin1, pos_begin2, pos_begin3;
        std::string::size_type pos_begin4, pos_begin5, pos_begin6;
        len = str.length();

        pos_begin1 = str.find("insert");
        pos_begin2 = str.find("INSERT");
        pos_begin3 = str.find("update");
        pos_begin4 = str.find("UPDATE");
        pos_begin5 = str.find("delete");
        pos_begin6 = str.find("DELETE");

        if (pos_begin1 != std::string::npos)
        {
            pos_begin = pos_begin1;
        }
        else
            pos_begin = std::string::npos;


        if ((pos_begin2 != std::string::npos) && (pos_begin2 < pos_begin))
        {
            pos_begin = pos_begin2;
        }

        if ((pos_begin3 != std::string::npos) && (pos_begin3 < pos_begin))
        {
            pos_begin = pos_begin3;
        }

        if ((pos_begin4  != std::string::npos) && (pos_begin4 < pos_begin))
        {
            pos_begin = pos_begin4;
        }

        if ((pos_begin5 != std::string::npos) && (pos_begin5 < pos_begin))
        {
            pos_begin = pos_begin5;
        }

        if ((pos_begin6  != std::string::npos) && (pos_begin6 < pos_begin))
        {
            pos_begin = pos_begin6;
        }

        str.erase(0, pos_begin);

        pos_end = str.rfind("'");
        len = str.length();
        str.erase(pos_end, len - pos_end);

        //str = std::string(" begin  execute immediate  ' ") + str + std::string(" ' ; end;");
        str = str + ";";
        LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL, "ParserCallPkgString returned: %s", str.c_str());
    }
    //hongzhi, for TDS
    void SimpleDbDatabase::ParserSQLString(std::string& str)
    {
        std::string::size_type len = 0, dindx = std::string::npos;
        len = str.length();
        dindx = str.rfind(";");

        if ((dindx == std::string::npos) || ((dindx != std::string::npos) && (dindx < (len - 3))))
        {
            str += ';';
        }

        str += '\n';

        dindx = std::string::npos;
        std::string::size_type indx1 = std::string::npos, indx2 = std::string::npos, indx3 = std::string::npos;

        indx1 = str.find("begin");
        indx2 = str.find("BEGIN");
        indx3 = str.find("Begin");

        if ((indx1 != std::string::npos) && (indx1 < 3))
            dindx = indx1;

        if ((indx2 != std::string::npos) && (indx2 < 3))
            dindx = indx2;

        if ((indx3 != std::string::npos) && (indx3 < 3))
            dindx = indx3;

        if (dindx != std::string::npos)
        {
            //need to add '/' and '\n'
            str += '/';
            str += '\n';
        }

        LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL, "ParserSQLString returned: %s", str.c_str());
    }

    void SimpleDbDatabase::_GetBothSQLFromObj(const SQLStatement& rSqlStatement, std::string& strOra, std::string& strMy)
    {
        if (rSqlStatement.bIsTdsAQ)
        {
            // it's a AQ statement
            strOra = rSqlStatement.strTDSOracle;
            strMy  = rSqlStatement.strTDSMySQL;
        }
        else
        {
            if (!rSqlStatement.strCommon.empty())
            {
                strOra = escapeInsertString(rSqlStatement.strCommon);
                strMy  = strOra;
            }
            else
            {
                strOra = escapeInsertString(rSqlStatement.strOracle);
                strMy  = escapeInsertString(rSqlStatement.strMySQL);
            }
        }
    }

//#endif
} // closes TA_Base_Core
