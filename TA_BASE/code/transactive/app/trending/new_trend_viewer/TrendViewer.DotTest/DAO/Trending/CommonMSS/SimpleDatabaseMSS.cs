using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace DAO.Trending.CommonMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.Common.SimpleDatabase")]
    public class SimpleDatabaseMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.Common.SimpleDatabase.OpenConnection()")]
        public static System.Boolean OpenConnectionMSS(
        DAO.Trending.Common.SimpleDatabase target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.OpenConnection();
            }
        }

        [TestSpecificStub("DAO.Trending.Common.SimpleDatabase.OpenConnection()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
         [TestSpecificStubContext(typeof(TrendViewTests), "TestInitializeTrendViewer01" )]
        public static System.Boolean OpenConnectionTSS(
        DAO.Trending.Common.SimpleDatabase target)
        {
            return false;
        }

    }
}

