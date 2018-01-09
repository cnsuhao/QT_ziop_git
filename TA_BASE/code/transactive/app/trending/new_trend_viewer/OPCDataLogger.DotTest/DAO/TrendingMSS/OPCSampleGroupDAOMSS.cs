using System;
using Dottest.Framework;
using System.Collections.Generic;

namespace DAO.TrendingMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.OPCSampleGroupDAO")]
    public class OPCSampleGroupDAOMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.OPCSampleGroupDAO.GetAllEnabledSampleGrp()")]
        public static System.Collections.Generic.List<Entity.Trending.EtyOPCSampleGroup> GetAllEnabledSampleGrpMSS(
        DAO.Trending.OPCSampleGroupDAO target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Collections.Generic.List<Entity.Trending.EtyOPCSampleGroup>)result;
            }
            else
            {
                return target.GetAllEnabledSampleGrp();
            }
        }

        [TestSpecificStub("DAO.Trending.OPCSampleGroupDAO.GetAllEnabledSampleGrp()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataPointManagerTests), "TestLoadDataPointTimeStamp01")]
        public static System.Collections.Generic.List<Entity.Trending.EtyOPCSampleGroup> GetAllEnabledSampleGrpTSS(
        DAO.Trending.OPCSampleGroupDAO target)
        {
            List<Entity.Trending.EtyOPCSampleGroup> etyList = new List<Entity.Trending.EtyOPCSampleGroup>();
            Entity.Trending.EtyOPCSampleGroup ety = new Entity.Trending.EtyOPCSampleGroup();
            ety.SampleGrpName = "SampleGrpName";
            ety.SampleGrpID = 1;
            ety.IntervalType = "M";
            ety.Interval = 1;
            ety.DeltaValue = 1;
            ety.StartTime = "10:30";
            ety.Disabled = false;
            etyList.Add(ety);
            return etyList;
        }

    }
}

