using DAO.Trending.Common;
using Dottest.Framework;
using NUnit.Framework;
using DAO.Trending.Helper;

namespace DAO.Trending.DotTest.Common
{
    [TestFixture()]
    public class DBConnectionStringsTests
    {
        private DBConnectionStrings dbConnectionStrings;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            dbConnectionStrings = DBConnectionStrings.GetInstance();
            DBConnectionStrings.ReleaseInstance();
            
          //  ((IDisposable)dbConnectionStrings).Dispose(); // to call the desctruction

            dbConnectionStrings = DBConnectionStrings.GetInstance();
            TestDBInit.openConn();
        }


        [Test]
        public void TestGetConnectionStrings()
        {
            dbConnectionStrings.GetConnectionStrings();
        }

        [Test]
        public void TestUpdateDBStatus()
        {
            dbConnectionStrings.UpdateDBStatus(TestDBInit.localConnectionString1, DBStatus.DB_OFFLINE);
            dbConnectionStrings.UpdateDBStatus(TestDBInit.localConnectionString1, DBStatus.DB_ONLINE);
            dbConnectionStrings.UpdateDBStatusWithoutMonitoring(TestDBInit.localConnectionString1, DBStatus.DB_OFFLINE);
            dbConnectionStrings.UpdateDBStatusWithoutMonitoring(TestDBInit.localConnectionString1, DBStatus.DB_ONLINE);
        }

        [Test]
        public void TestTerminateMonitorThread()
        {
            dbConnectionStrings.TerminateMonitorThread();
        }


    }
}
