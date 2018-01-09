using System;
using Dottest.Framework;
using DAO.Trending.DotTest.Common;

namespace DAO.Trending.CommonMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.Common.SessionStore")]
    public class SessionStoreMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.Common.SessionStore.GetSession(System.String)")]
        public static DAO.Trending.Common.DatabaseSession GetSessionMSS(
        DAO.Trending.Common.SessionStore target,
        System.String connectionString)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, connectionString }, out result))
            {
                return (DAO.Trending.Common.DatabaseSession)result;
            }
            else
            {
                return target.GetSession(connectionString);
            }
        }

        [TestSpecificStub("DAO.Trending.Common.SessionStore.GetSession(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DatabaseSessionFactoryTests), "TestGetDatabaseSession03" )]
        public static DAO.Trending.Common.DatabaseSession GetSessionTSS(
        DAO.Trending.Common.SessionStore target,
        System.String connectionString)
        {
            return null;
        }

    }
}

