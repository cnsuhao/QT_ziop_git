using System;
using Dottest.Framework;

namespace OPCSampleGrpConfig.ModelMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCSampleGrpConfig")]
    [MethodSpecificStubsClassAttribute("OPCSampleGrpConfig.Model.OPCDataSelectorModel")]
    public class OPCDataSelectorModelMSS
    {
        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Model.OPCDataSelectorModel.CheckConfiguredByOcc(System.String)")]
        public static System.Boolean CheckConfiguredByOccMSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        System.String dataPoint)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, dataPoint }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.CheckConfiguredByOcc(dataPoint);
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Model.OPCDataSelectorModel.CheckConfiguredByOcc(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestopcItemDataGridView_CellDoubleClick01")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestCanAddDataPoint01")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestCanAddDataPoint02")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestCanAddDataPoint03")]
        public static System.Boolean CheckConfiguredByOccTSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        System.String dataPoint)
        {
            string TestName = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (TestName == "TestCanAddDataPoint01")
            {
                return true;
            }
            return false;
        }

        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Model.OPCDataSelectorModel.CheckStationGrp(System.String,System.String)")]
        public static System.Boolean CheckStationGrpMSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        System.String dataPoint, System.String currentGrpLocation)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, dataPoint, currentGrpLocation }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.CheckStationGrp(dataPoint, currentGrpLocation);
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Model.OPCDataSelectorModel.CheckStationGrp(System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.View.OPCDataSelectorTests), "TestAddToSelectedListBox01")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestCanAddDataPoint03")]
        public static System.Boolean CheckStationGrpTSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        System.String dataPoint, System.String currentGrpLocation)
        {
            string testName = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (testName == "TestCanAddDataPoint03")
            {
                return false;
            }
            return true;
        }

        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Model.OPCDataSelectorModel.BeginTransaction()")]
        public static void BeginTransactionMSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.BeginTransaction();
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Model.OPCDataSelectorModel.BeginTransaction()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestokCmd_Click01")]
        public static void BeginTransactionTSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target)
        {
            return;
        }

        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Model.OPCDataSelectorModel.CommitTransaction()")]
        public static void CommitTransactionMSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.CommitTransaction();
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Model.OPCDataSelectorModel.CommitTransaction()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestokCmd_Click01")]
        public static void CommitTransactionTSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target)
        {
            return;
        }

        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Model.OPCDataSelectorModel.UpdateDataPointDB(Entity.Trending.EtyDataLogDPTrend)")]
        public static System.Boolean UpdateDataPointDBMSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        Entity.Trending.EtyDataLogDPTrend etyDataPoint)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, etyDataPoint }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.UpdateDataPointDB(etyDataPoint);
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Model.OPCDataSelectorModel.UpdateDataPointDB(Entity.Trending.EtyDataLogDPTrend)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestokCmd_Click01")]
        public static System.Boolean UpdateDataPointDBTSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        Entity.Trending.EtyDataLogDPTrend etyDataPoint)
        {
            return true;
        }

        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Model.OPCDataSelectorModel.UpdateVersionNum()")]
        public static System.Boolean UpdateVersionNumMSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.UpdateVersionNum();
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Model.OPCDataSelectorModel.UpdateVersionNum()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestokCmd_Click01")]
        public static System.Boolean UpdateVersionNumTSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target)
        {
            return true;
        }

        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Model.OPCDataSelectorModel.GetChildOPCDataNodes(System.String,System.String)")]
        public static System.Collections.Generic.List<Entity.Trending.EtyOPCDataNode> GetChildOPCDataNodesMSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        System.String parentNode, System.String opcServerName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, parentNode, opcServerName }, out result))
            {
                return (System.Collections.Generic.List<Entity.Trending.EtyOPCDataNode>)result;
            }
            else
            {
                return target.GetChildOPCDataNodes(parentNode, opcServerName);
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Model.OPCDataSelectorModel.GetChildOPCDataNodes(System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TesttreeOpcItems_AfterSelect01")]
        public static System.Collections.Generic.List<Entity.Trending.EtyOPCDataNode> GetChildOPCDataNodesTSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        System.String parentNode, System.String opcServerName)
        {
            System.Collections.Generic.List<Entity.Trending.EtyOPCDataNode> etyList = new System.Collections.Generic.List<Entity.Trending.EtyOPCDataNode>();
            Entity.Trending.EtyOPCDataNode datanode = new Entity.Trending.EtyOPCDataNode();
            datanode.OPCDataNodeName = "testing";
            etyList.Add(datanode);
            return etyList;
        }

        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Model.OPCDataSelectorModel.GetAllOPCDataPoints(System.String,System.String)")]
        public static System.Collections.Generic.List<Entity.Trending.EtyDataLogDPTrend> GetAllOPCDataPointsMSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        System.String opcServerName, System.String filterString)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, opcServerName, filterString }, out result))
            {
                return (System.Collections.Generic.List<Entity.Trending.EtyDataLogDPTrend>)result;
            }
            else
            {
                return target.GetAllOPCDataPoints(opcServerName, filterString);
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Model.OPCDataSelectorModel.GetAllOPCDataPoints(System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestGetDataPointList01")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestopcServerIPBox_SelectedIndexChanged01")]
        public static System.Collections.Generic.List<Entity.Trending.EtyDataLogDPTrend> GetAllOPCDataPointsTSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        System.String opcServerName, System.String filterString)
        {
            System.Collections.Generic.List<Entity.Trending.EtyDataLogDPTrend> etylist = new System.Collections.Generic.List<Entity.Trending.EtyDataLogDPTrend>();
            Entity.Trending.EtyDataLogDPTrend datapoint = new Entity.Trending.EtyDataLogDPTrend();
            datapoint.OPCDataPointName = "testing";
            etylist.Add(datapoint);
            return etylist;
        }

        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Model.OPCDataSelectorModel.GetAllOPCDataNodes(System.String,System.String)")]
        public static System.Collections.Generic.List<Entity.Trending.EtyOPCDataNode> GetAllOPCDataNodesMSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        System.String serverRootName, System.String opcServerName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, serverRootName, opcServerName }, out result))
            {
                return (System.Collections.Generic.List<Entity.Trending.EtyOPCDataNode>)result;
            }
            else
            {
                return target.GetAllOPCDataNodes(serverRootName, opcServerName);
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Model.OPCDataSelectorModel.GetAllOPCDataNodes(System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestopcServerIPBox_SelectedIndexChanged01")]
        public static System.Collections.Generic.List<Entity.Trending.EtyOPCDataNode> GetAllOPCDataNodesTSS(
        OPCSampleGrpConfig.Model.OPCDataSelectorModel target,
        System.String serverRootName, System.String opcServerName)
        {
            System.Collections.Generic.List<Entity.Trending.EtyOPCDataNode> etyList = new System.Collections.Generic.List<Entity.Trending.EtyOPCDataNode>();
            Entity.Trending.EtyOPCDataNode datanode = new Entity.Trending.EtyOPCDataNode();
            datanode.OPCDataNodeName = "testing";
            etyList.Add(datanode);
            return etyList;
        }

    }
}

