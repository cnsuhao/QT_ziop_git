using System;
using Dottest.Framework;

namespace DAO.TrendingMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.TrendLogDAO")]
    public class TrendLogDAOMSS
    {
        static int i = 0;
        [MethodSpecificStubsMethod("DAO.Trending.TrendLogDAO.InsertTrendViewerLog(Entity.Trending.EtyTrendLog)")]
        public static System.Boolean InsertTrendViewerLogMSS(
        DAO.Trending.TrendLogDAO target,
        Entity.Trending.EtyTrendLog etyTrendLog)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, etyTrendLog }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                i++;
                if (i >= 3)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        [TestSpecificStub("DAO.Trending.TrendLogDAO.InsertTrendViewerLog(Entity.Trending.EtyTrendLog)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        //[TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerWriteQueneTests), "TestRun01" )]
        public static System.Boolean InsertTrendViewerLogTSS(
        DAO.Trending.TrendLogDAO target,
        Entity.Trending.EtyTrendLog etyTrendLog)
        {
            i++;
            if(i>=3)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    }
}

