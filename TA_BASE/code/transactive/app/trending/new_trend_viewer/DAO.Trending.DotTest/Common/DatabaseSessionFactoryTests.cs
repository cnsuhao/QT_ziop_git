using DAO.Trending.Common;
using Dottest.Framework;
using NUnit.Framework;

namespace DAO.Trending.DotTest.Common
{
    [TestFixture()]
    public class DatabaseSessionFactoryTests
    {
        private DatabaseSessionFactory databaseSessionFactory;
        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            databaseSessionFactory = new DatabaseSessionFactory();
        }

        [Test]
        public void TestGetDatabaseSession02()
        {
            // stub: m_sessionStore.GetSession(connectionString) => session.CheckDatabaseConnection() = false:
            try
            {
                databaseSessionFactory.GetDatabaseSession();
            }
            catch (System.Exception ex)
            {
            	
            }
        }
        [Test]
        public void TestGetDatabaseSession03()
        {
            databaseSessionFactory.GetDatabaseSession(null);
            //stub: session = m_sessionStore.GetSession(connectionString) == null
            databaseSessionFactory.GetDatabaseSession(TestDBInit.localConnectionString1);
        }

        [Test]
        public void TestGetDatabaseSession01()
        {
            //stub: m_sessionStore.GetSession(connectionString) =>if (session == null)
            databaseSessionFactory.GetDatabaseSession();
        }



    }
}
