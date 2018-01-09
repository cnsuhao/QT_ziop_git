using System;
using Dottest.Framework;

namespace DAO.TrendingMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.DatalogDPGroupTrendDAO")]
    public class DatalogDPGroupTrendDAOMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.DatalogDPGroupTrendDAO.GetOPCGrpByID(System.Double)")]
        public static Entity.Trending.EtyDataLogDPGroupTrend GetOPCGrpByIDMSS(
        DAO.Trending.DatalogDPGroupTrendDAO target,
        System.Double opcGrpID)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, opcGrpID }, out result))
            {
                return (Entity.Trending.EtyDataLogDPGroupTrend)result;
            }
            else
            {
                return target.GetOPCGrpByID(opcGrpID);
            }
        }

        [TestSpecificStub("DAO.Trending.DatalogDPGroupTrendDAO.GetOPCGrpByID(System.Double)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Model.OPCDataSelectorModelTests), "TestCheckConfiguredByOcc01")]
        public static Entity.Trending.EtyDataLogDPGroupTrend GetOPCGrpByIDTSS(
        DAO.Trending.DatalogDPGroupTrendDAO target,
        System.Double opcGrpID)
        {
            Entity.Trending.EtyDataLogDPGroupTrend etyDP = new Entity.Trending.EtyDataLogDPGroupTrend();
            etyDP.LocationName = "OCC";
            return etyDP;
        }

        [MethodSpecificStubsMethod("DAO.Trending.DatalogDPGroupTrendDAO.DeleteOPCSampleGroupById(System.String)")]
        public static System.Boolean DeleteOPCSampleGroupByIdMSS(
        DAO.Trending.DatalogDPGroupTrendDAO target,
        System.String opcGrpId)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, opcGrpId }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.DeleteOPCSampleGroupById(opcGrpId);
            }
        }

        [TestSpecificStub("DAO.Trending.DatalogDPGroupTrendDAO.DeleteOPCSampleGroupById(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Model.OPCSampleGrpConfigStartModelTests), "TestDeleteOPCSampleGrp02")]
        public static System.Boolean DeleteOPCSampleGroupByIdTSS(
        DAO.Trending.DatalogDPGroupTrendDAO target,
        System.String opcGrpId)
        {
            return false;
        }

        [MethodSpecificStubsMethod("DAO.Trending.DatalogDPGroupTrendDAO.UpdateOPCGrpData(Entity.Trending.EtyDataLogDPGroupTrend)")]
        public static System.Boolean UpdateOPCGrpDataMSS(
        DAO.Trending.DatalogDPGroupTrendDAO target,
        Entity.Trending.EtyDataLogDPGroupTrend etyDPGroupTrend)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, etyDPGroupTrend }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.UpdateOPCGrpData(etyDPGroupTrend);
            }
        }

        [TestSpecificStub("DAO.Trending.DatalogDPGroupTrendDAO.UpdateOPCGrpData(Entity.Trending.EtyDataLogDPGroupTrend)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Model.OPCSampleGrpConfigStartModelTests), "TestUpdateOPCSampleGrp02")]
        public static System.Boolean UpdateOPCGrpDataTSS(
        DAO.Trending.DatalogDPGroupTrendDAO target,
        Entity.Trending.EtyDataLogDPGroupTrend etyDPGroupTrend)
        {
            return false;
        }

    }
}

