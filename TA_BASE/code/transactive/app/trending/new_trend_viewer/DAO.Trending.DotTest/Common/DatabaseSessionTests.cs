using DAO.Trending.Common;
using Dottest.Framework;
using NUnit.Framework;
using System;
using DAO.Trending.Helper;

namespace DAO.Trending.DotTest.Common
{
    [TestFixture()]
    public class DatabaseSessionTests
    {
        private DatabaseSession databaseSessionNormal;
        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            databaseSessionNormal = new DatabaseSession(TestDBInit.localConnectionString1);
            TestDBInit.openConn();

        }

        [Test]
        public void TestCreateCommand()
        {
            SimpleDatabase.GetInstance().BeginTransaction();

            SimpleDatabase.GetInstance().ExecuteNonQuery("select * from entity");
            SimpleDatabase.GetInstance().RollbackTransaction();
        }

        [Test]
        public void TestAll()
        {
            DatabaseSession databaseSession = new DatabaseSession("Invalid Connection String;");
            databaseSession.OpenConnection();
            
            Accessor databaseSessionAccessor = ReflectionAccessor.Wrap(databaseSession);
            databaseSessionAccessor.Call("GetConnection", DBType.Oracle);
            databaseSessionAccessor.Call("GetConnection", DBType.MySql);

            databaseSessionAccessor.SetField("m_dbType", DBType.Oracle);
            databaseSession.GetParameterDelimiter();
            databaseSessionAccessor.SetField("m_dbType", DBType.MySql);
            databaseSession.GetParameterDelimiter();

            databaseSession.CheckDatabaseConnection();

            try
            {
                databaseSession.CommitTransaction(); //throw exception
            }
            catch (System.Exception ex)
            {
            	
            }
            try
            {
                databaseSession.RollBackTransaction(); //throw exception
            }
            catch (System.Exception ex)
            {
            	
            }
            
        }


        [Test]
        public void TestBeginTransaction01()
        {
            //for test exception
            Accessor dbSessionAccessor = ReflectionAccessor.Wrap(databaseSessionNormal);
            Object coonn = dbSessionAccessor.GetField("m_connection");
            dbSessionAccessor.SetField("m_connection", null);
            try
            {
                databaseSessionNormal.BeginTransaction();
            }
            catch (System.Exception ex)
            {
            	
            }
            dbSessionAccessor.SetField("m_connection", coonn);
        }



    }
}
