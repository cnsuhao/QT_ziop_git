using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace DAO.Trending.HelperMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.Helper.LocationKeyHelper")]
    public class LocationKeyHelperMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.Helper.LocationKeyHelper.init()")]
        public static void initMSS(
        DAO.Trending.Helper.LocationKeyHelper target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.init();
            }
        }

        [TestSpecificStub("DAO.Trending.Helper.LocationKeyHelper.init()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
         [TestSpecificStubContext(typeof(TrendViewTests), "TestInitializeTrendViewer02" )]
        public static void initTSS(
        DAO.Trending.Helper.LocationKeyHelper target)
        {
            Exception e= new Exception();
            throw e;
        }

    }
}

