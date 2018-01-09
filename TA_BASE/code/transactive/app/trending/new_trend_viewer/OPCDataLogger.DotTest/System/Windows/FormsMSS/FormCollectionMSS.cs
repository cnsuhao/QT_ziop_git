using System;
using Dottest.Framework;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.FormCollection")]
    public class FormCollectionMSS
    {
        [MethodSpecificStubsMethod("System.Windows.Forms.FormCollection.get_Item(System.Int32)")]
        public static System.Windows.Forms.Form get_ItemMSS(
        System.Windows.Forms.FormCollection target,
        System.Int32 index)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, index }, out result))
            {
                return (System.Windows.Forms.Form)result;
            }
            else
            {
                return target[index];
            }
        }

        [TestSpecificStub("System.Windows.Forms.FormCollection.get_Item(System.Int32)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataLoggerTests), "TestcloseApplicationToolStripMenuItem_Click01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataLoggerTests), "TestcloseApplicationToolStripMenuItem_Click02")]
        public static System.Windows.Forms.Form get_ItemTSS(
        System.Windows.Forms.FormCollection target,
        System.Int32 index)
        {
            //return target[index];
            return new System.Windows.Forms.Form();
        }

    }
}

