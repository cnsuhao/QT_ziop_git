using System;
using Dottest.Framework;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.Control")]
    public class ControlMSS
    {
        [MethodSpecificStubsMethod("System.Windows.Forms.Control.get_InvokeRequired()")]
        public static System.Boolean get_InvokeRequiredMSS(
        System.Windows.Forms.Control target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.InvokeRequired;
            }
        }

        [TestSpecificStub("System.Windows.Forms.Control.get_InvokeRequired()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataLoggerTests), "TestcloseApplicationToolStripMenuItem_Click02")]
        public static System.Boolean get_InvokeRequiredTSS(
        System.Windows.Forms.Control target)
        {
            return true;
        }

        [MethodSpecificStubsMethod("System.Windows.Forms.Control.BeginInvoke(System.Delegate)")]
        public static System.IAsyncResult BeginInvokeMSS(
        System.Windows.Forms.Control target,
        System.Delegate method)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, method }, out result))
            {
                return (System.IAsyncResult)result;
            }
            else
            {
                return target.BeginInvoke(method);
            }
        }

        [TestSpecificStub("System.Windows.Forms.Control.BeginInvoke(System.Delegate)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataLoggerTests), "TestcloseApplicationToolStripMenuItem_Click02")]
        public static System.IAsyncResult BeginInvokeTSS(
        System.Windows.Forms.Control target,
        System.Delegate method)
        {
            return null;
        }

    }
}

