using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.Timer")]
    public class TimerMSS
    {
        [MethodSpecificStubsMethod("System.Windows.Forms.Timer.Stop()")]
        public static void StopMSS(
        System.Windows.Forms.Timer target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.Stop();
            }
        }

        [TestSpecificStub("System.Windows.Forms.Timer.Stop()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestStopRealTimeCharting02")]
        public static void StopTSS(
        System.Windows.Forms.Timer target)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

