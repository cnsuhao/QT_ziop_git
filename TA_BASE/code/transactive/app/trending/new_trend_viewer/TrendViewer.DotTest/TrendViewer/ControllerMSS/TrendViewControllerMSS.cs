using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ControllerMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.Controller.TrendViewController")]
    public class TrendViewControllerMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.Controller.TrendViewController.ExportHistoryData(System.String)")]
        public static void ExportHistoryDataMSS(
            /*TrendViewer.Controller.TrendViewController*/ object target,
        System.String filename)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, filename }, out result))
            {
                return;
            }
            else
            {
                Helper.CallMethod(target, "ExportHistoryData", new object[] { filename });
            }
        }

        [TestSpecificStub("TrendViewer.Controller.TrendViewController.ExportHistoryData(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestExportHistoryDataToolStripMenuItem_Click02")]
        public static void ExportHistoryDataTSS(
            /*TrendViewer.Controller.TrendViewController*/ object target,
        System.String filename)
        {
            Exception e = new Exception();
            throw e;
        }

        [MethodSpecificStubsMethod("TrendViewer.Controller.TrendViewController.RealTimeExecuteCmd_Click(System.Object,System.EventArgs)")]
        public static void RealTimeExecuteCmd_ClickMSS(
            /*TrendViewer.Controller.TrendViewController*/ object target,
        System.Object sender, System.EventArgs e)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, sender, e }, out result))
            {
                return;
            }
            else
            {
                Helper.CallMethod(target, "RealTimeExecuteCmd_Click", new object[] { sender, e });
            }
        }

        [TestSpecificStub("TrendViewer.Controller.TrendViewController.RealTimeExecuteCmd_Click(System.Object,System.EventArgs)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestDrawTrendView")]
        public static void RealTimeExecuteCmd_ClickTSS(
            /*TrendViewer.Controller.TrendViewController*/ object target,
        System.Object sender, System.EventArgs e)
        {
            return;
        }

        [MethodSpecificStubsMethod("TrendViewer.Controller.TrendViewController.GetMinTimeForHistLog()")]
        public static System.DateTime GetMinTimeForHistLogMSS(
            /*TrendViewer.Controller.TrendViewController*/ object target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.DateTime)result;
            }
            else
            {
                return (System.DateTime)Helper.CallMethod(target, "GetMinTimeForHistLog", new object[] { });
            }
        }

        [TestSpecificStub("TrendViewer.Controller.TrendViewController.GetMinTimeForHistLog()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestHistoryExecuteCmd_Click04")]
        public static System.DateTime GetMinTimeForHistLogTSS(
            /*TrendViewer.Controller.TrendViewController*/ object target)
        {
            return new DateTime(2013, 1, 2);
        }

        [MethodSpecificStubsMethod("TrendViewer.Controller.TrendViewController.GetMaxTimeForHistLog()")]
        public static System.DateTime GetMaxTimeForHistLogMSS(
            /*TrendViewer.Controller.TrendViewController*/ object target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.DateTime)result;
            }
            else
            {
                return (System.DateTime)Helper.CallMethod(target, "GetMaxTimeForHistLog", new object[] { });
            }
        }

        [TestSpecificStub("TrendViewer.Controller.TrendViewController.GetMaxTimeForHistLog()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestHistoryExecuteCmd_Click04")]
        public static System.DateTime GetMaxTimeForHistLogTSS(
            /*TrendViewer.Controller.TrendViewController*/ object target)
        {
            return new DateTime(2013, 1, 1);
        }

    }
}

