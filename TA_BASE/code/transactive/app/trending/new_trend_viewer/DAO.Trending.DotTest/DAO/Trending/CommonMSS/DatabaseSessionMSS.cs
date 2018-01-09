using System;
using Dottest.Framework;
using DAO.Trending.DotTest.Common;

namespace DAO.Trending.CommonMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.Common.DatabaseSession")]
    public class DatabaseSessionMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.Common.DatabaseSession.BeginTransaction()")]
        public static void BeginTransactionMSS(
        DAO.Trending.Common.DatabaseSession target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.BeginTransaction();
            }
        }

        [TestSpecificStub("DAO.Trending.Common.DatabaseSession.BeginTransaction()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(SimpleDatabaseTests), "TestExceptions")]
        public static void BeginTransactionTSS(
        DAO.Trending.Common.DatabaseSession target)
        {
            Exception e = new Exception();
            throw e;
        }

        [MethodSpecificStubsMethod("DAO.Trending.Common.DatabaseSession.CheckDatabaseConnection()")]
        public static System.Boolean CheckDatabaseConnectionMSS(
        DAO.Trending.Common.DatabaseSession target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.CheckDatabaseConnection();
            }
        }

        [TestSpecificStub("DAO.Trending.Common.DatabaseSession.CheckDatabaseConnection()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DatabaseSessionFactoryTests), "TestGetDatabaseSession02")]
        public static System.Boolean CheckDatabaseConnectionTSS(
        DAO.Trending.Common.DatabaseSession target)
        {
            return false;
        }

    }
}

