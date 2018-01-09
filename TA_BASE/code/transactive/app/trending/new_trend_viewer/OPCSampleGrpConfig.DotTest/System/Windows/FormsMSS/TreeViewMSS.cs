using System;
using Dottest.Framework;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.TreeView")]
    public class TreeViewMSS
    {
        [MethodSpecificStubsMethod("System.Windows.Forms.TreeView.get_SelectedNode()")]
        public static System.Windows.Forms.TreeNode get_SelectedNodeMSS(
        System.Windows.Forms.TreeView target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Windows.Forms.TreeNode)result;
            }
            else
            {
                return target.SelectedNode;
            }
        }

        [TestSpecificStub("System.Windows.Forms.TreeView.get_SelectedNode()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TesttreeOpcItems_AfterSelect01")]
        public static System.Windows.Forms.TreeNode get_SelectedNodeTSS(
        System.Windows.Forms.TreeView target)
        {
            System.Windows.Forms.TreeNode selectNode = new System.Windows.Forms.TreeNode("testing");
            selectNode.Tag = "12345678";
            return selectNode;
        }

    }
}

