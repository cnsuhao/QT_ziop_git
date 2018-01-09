using System;
using Dottest.Framework;

namespace DAO.TrendingMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.DBConnection")]
    public class DBConnectionMSS
    {
        static int i = 0;
        [MethodSpecificStubsMethod("DAO.Trending.DBConnection.OpenConnection(System.String)")]
        public static System.Boolean OpenConnectionMSS(
        DAO.Trending.DBConnection target,
        System.String localConnectionString)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, localConnectionString }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.OpenConnection(localConnectionString);
            }
        }

        [TestSpecificStub("DAO.Trending.DBConnection.OpenConnection(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataPointManagerTests), "TestInitializeDataLogger03")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerWriteQueneTests), "TestCheckDatabaseConnection01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerWriteQueneTests), "TestRun01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestTestOracleConnection01")]
        public static System.Boolean OpenConnectionTSS(
        DAO.Trending.DBConnection target,
        System.String localConnectionString)
        {
            string currentMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if(currentMethod.Equals("TestRun01"))
            {               
                i++;
                if(i>=3)
                {
                    i = 0;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (currentMethod.Equals("TestTestOracleConnection01"))
            {
                i++;
                if (i >= 2)
                {
                    i = 0;
                    return true;
                }
                else
                {
                    return false;
                }
            }

            return true;
        }

    }
}

