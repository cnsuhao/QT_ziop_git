using System;
using Dottest.Framework;

namespace DAO.TrendingMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.DatalogDPTrendDAO")]
    public class DatalogDPTrendDAOMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.DatalogDPTrendDAO.GetGrpIDByDPName(System.String)")]
        public static System.Double GetGrpIDByDPNameMSS(
        DAO.Trending.DatalogDPTrendDAO target,
        System.String dataPoint)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, dataPoint }, out result))
            {
                return (System.Double)result;
            }
            else
            {
                return target.GetGrpIDByDPName(dataPoint);
            }
        }

        [TestSpecificStub("DAO.Trending.DatalogDPTrendDAO.GetGrpIDByDPName(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Model.OPCDataSelectorModelTests), "TestCheckConfiguredByOcc01")]
        public static System.Double GetGrpIDByDPNameTSS(
        DAO.Trending.DatalogDPTrendDAO target,
        System.String dataPoint)
        {
            return 1.0;
        }

        [MethodSpecificStubsMethod("DAO.Trending.DatalogDPTrendDAO.GetDataPointByName(System.String,System.String)")]
        public static System.Collections.Generic.List<Entity.Trending.EtyDataLogDPTrend> GetDataPointByNameMSS(
        DAO.Trending.DatalogDPTrendDAO target,
        System.String opcServerName, System.String DataPointNameSubstr)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, opcServerName, DataPointNameSubstr }, out result))
            {
                return (System.Collections.Generic.List<Entity.Trending.EtyDataLogDPTrend>)result;
            }
            else
            {
                return target.GetDataPointByName(opcServerName, DataPointNameSubstr);
            }
        }

        [TestSpecificStub("DAO.Trending.DatalogDPTrendDAO.GetDataPointByName(System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        //[TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static System.Collections.Generic.List<Entity.Trending.EtyDataLogDPTrend> GetDataPointByNameTSS(
        DAO.Trending.DatalogDPTrendDAO target,
        System.String opcServerName, System.String DataPointNameSubstr)
        {
            return target.GetDataPointByName(opcServerName, DataPointNameSubstr);
        }

    }
}

