using System;
using Dottest.Framework;
using System.Collections.Generic;

namespace DAO.TrendingMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.OPCNodeNDataPointDAO")]
    public class OPCNodeNDataPointDAOMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.OPCNodeNDataPointDAO.GetUniqueEnabledOPCDataPoint(System.String,System.String)")]
        public static System.Collections.Generic.List<Entity.Trending.EtyOPCDataPoint> GetUniqueEnabledOPCDataPointMSS(
        DAO.Trending.OPCNodeNDataPointDAO target,
        System.String hostName, System.String serverName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, hostName, serverName }, out result))
            {
                return (System.Collections.Generic.List<Entity.Trending.EtyOPCDataPoint>)result;
            }
            else
            {
                return target.GetUniqueEnabledOPCDataPoint(hostName, serverName);
            }
        }

        [TestSpecificStub("DAO.Trending.OPCNodeNDataPointDAO.GetUniqueEnabledOPCDataPoint(System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataPointManagerTests), "TestLoadDataPointTimeStamp01")]
        public static System.Collections.Generic.List<Entity.Trending.EtyOPCDataPoint> GetUniqueEnabledOPCDataPointTSS(
        DAO.Trending.OPCNodeNDataPointDAO target,
        System.String hostName, System.String serverName)
        {
            List<Entity.Trending.EtyOPCDataPoint> etyList = new List<Entity.Trending.EtyOPCDataPoint>();
            Entity.Trending.EtyOPCDataPoint ety = new Entity.Trending.EtyOPCDataPoint();
            ety.OPCDataPointName = "SampleName";
            ety.OPCSampleGrpId = 1;
            etyList.Add(ety);
            return etyList;
        }

    }
}

