using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.View.TrendView")]
    public class TrendViewMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.View.TrendView.IsMixMode()")]
        public static System.Boolean IsMixModeMSS(
        TrendViewer.View.TrendView target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.IsMixMode();
            }
        }

        [TestSpecificStub("TrendViewer.View.TrendView.IsMixMode()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestRealTimeExecuteCmd_Click01")]
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "Testtimer1_Tick02")]
        public static System.Boolean IsMixModeTSS(
        TrendViewer.View.TrendView target)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (name.Equals("TestRealTimeExecuteCmd_Click01"))
            {
                return true;
            }
            else
            {
                Exception e = new Exception();
                throw e;
            }

        }

        [MethodSpecificStubsMethod("TrendViewer.View.TrendView.GetRealTimeStartFlag()")]
        public static System.Boolean GetRealTimeStartFlagMSS(
        TrendViewer.View.TrendView target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.GetRealTimeStartFlag();
            }
        }

        [TestSpecificStub("TrendViewer.View.TrendView.GetRealTimeStartFlag()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestRealTimeExecuteCmd_Click02")]
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestHistoryExecuteCmd_Click02")]
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestHistoryExecuteCmd_Click03")]
        public static System.Boolean GetRealTimeStartFlagTSS(
        TrendViewer.View.TrendView target)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (name.Equals("TestHistoryExecuteCmd_Click03"))
            {
                return false;
            }
            else
            {
                return true;
            }

        }

        [MethodSpecificStubsMethod("TrendViewer.View.TrendView.GetStartTime()")]
        public static System.DateTime GetStartTimeMSS(
        TrendViewer.View.TrendView target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.DateTime)result;
            }
            else
            {
                return target.GetStartTime();
            }
        }

        [TestSpecificStub("TrendViewer.View.TrendView.GetStartTime()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestHistoryExecuteCmd_Click05")]
        public static System.DateTime GetStartTimeTSS(
        TrendViewer.View.TrendView target)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

