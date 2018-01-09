using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;
using Entity.Trending;
using TrendViewer.DotTest;
using System.Windows.Forms;
using TrendViewer.DotTest.View;

namespace TrendViewer.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.View.OPCDataSelector")]
    public class OPCDataSelectorMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.View.OPCDataSelector.getDPInGridView(System.Int32)")]
        public static Entity.Trending.EtyEntity getDPInGridViewMSS(
        TrendViewer.View.OPCDataSelector target,
        System.Int32 p)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, p }, out result))
            {
                return (Entity.Trending.EtyEntity)result;
            }
            else
            {
                return target.getDPInGridView(p);
            }
        }

        [TestSpecificStub("TrendViewer.View.OPCDataSelector.getDPInGridView(System.Int32)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataSelectorControllerTests), "TeseopcItemDataGridView_CellDoubleClick02")]
        public static Entity.Trending.EtyEntity getDPInGridViewTSS(
        TrendViewer.View.OPCDataSelector target,
        System.Int32 p)
        {
            EtyEntity ety = new EtyEntity();
            return ety;
        }

        [MethodSpecificStubsMethod("TrendViewer.View.OPCDataSelector.getSelectedNode()")]
        public static System.Windows.Forms.TreeNode getSelectedNodeMSS(
        TrendViewer.View.OPCDataSelector target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Windows.Forms.TreeNode)result;
            }
            else
            {
                return target.getSelectedNode();
            }
        }

        [TestSpecificStub("TrendViewer.View.OPCDataSelector.getSelectedNode()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataSelectorControllerTests), "TestAfterSelect01")]
        [TestSpecificStubContext(typeof(OPCDataSelectorControllerTests), "TestAfterSelect02")]
        [TestSpecificStubContext(typeof(OPCDataSelectorControllerTests), "TestAfterSelect03")]
        public static System.Windows.Forms.TreeNode getSelectedNodeTSS(
        TrendViewer.View.OPCDataSelector target)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;

            if (name.Equals("TestAfterSelect01"))
            {
                //nodes.count>=1
                TreeNode node = new TreeNode();
                node.Tag = TestConst.DATANODE_KEY;
                node.Nodes.Add("1");
                node.Nodes[0].Tag = TestConst.DATANODE_KEY;
                return node;
            }
            else if (name.Equals("TestAfterSelect02"))
            {
                //nodes.count<1
                TreeNode node = new TreeNode();
                node.Tag = TestConst.DATANODE_KEY;
                return node;
            }
            else //(name.Equals("TestAfterSelect03"))
            {
                return null;
            }
        }

        [MethodSpecificStubsMethod("TrendViewer.View.OPCDataSelector.BuildDataNodeStructure(System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity>)")]
        public static void BuildDataNodeStructureMSS(
        TrendViewer.View.OPCDataSelector target,
        System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity> entityMap)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, entityMap }, out result))
            {
                return;
            }
            else
            {
                Helper.CallMethod(target, "BuildDataNodeStructure", new object[] { entityMap });
            }
        }

        [TestSpecificStub("TrendViewer.View.OPCDataSelector.BuildDataNodeStructure(System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity>)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataSelectorTests), "TestLoadDataNodeStructure")]
        public static void BuildDataNodeStructureTSS(
        TrendViewer.View.OPCDataSelector target,
        System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity> entityMap)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

