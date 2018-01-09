using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;
using TrendViewer.DotTest.View;

namespace TrendingChartControlMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendingChartCtl")]
    [MethodSpecificStubsClassAttribute("TrendingChartControl.TrendingChartCtl")]
    public class TrendingChartCtlMSS
    {
        [MethodSpecificStubsMethod("TrendingChartControl.TrendingChartCtl.SaveAsImage(System.String)")]
        public static void SaveAsImageMSS(
        TrendingChartControl.TrendingChartCtl target,
        System.String fileName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, fileName }, out result))
            {
                return;
            }
            else
            {
                target.SaveAsImage(fileName);
            }
        }

        [TestSpecificStub("TrendingChartControl.TrendingChartCtl.SaveAsImage(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestCaptureScreenShoot02")]
        public static void SaveAsImageTSS(
        TrendingChartControl.TrendingChartCtl target,
        System.String fileName)
        {
            //do nothing
            return;
        }

        [MethodSpecificStubsMethod("TrendingChartControl.TrendingChartCtl.InitChartBeforeRealTime(System.Int32,System.Int32,System.Boolean,System.Int32)")]
        public static void InitChartBeforeRealTimeMSS(
        TrendingChartControl.TrendingChartCtl target,
        System.Int32 frameInterval, System.Int32 timeInterval, System.Boolean enable3D, System.Int32 defaultGapDepth)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, frameInterval, timeInterval, enable3D, defaultGapDepth }, out result))
            {
                return;
            }
            else
            {
                target.InitChartBeforeRealTime(frameInterval, timeInterval, enable3D, defaultGapDepth);
            }
        }

        [TestSpecificStub("TrendingChartControl.TrendingChartCtl.InitChartBeforeRealTime(System.Int32,System.Int32,System.Boolean,System.Int32)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestInitializeChartForRealTime02")]
        public static void InitChartBeforeRealTimeTSS(
        TrendingChartControl.TrendingChartCtl target,
        System.Int32 frameInterval, System.Int32 timeInterval, System.Boolean enable3D, System.Int32 defaultGapDepth)
        {
            Exception e = new Exception();
            throw e;
        }

        [MethodSpecificStubsMethod("TrendingChartControl.TrendingChartCtl.AddMarker(System.String,System.Double,System.Double,System.Drawing.Color,System.Drawing.Color)")]
        public static void AddMarkerMSS(
        TrendingChartControl.TrendingChartCtl target,
        System.String markerName, System.Double markerWidth, System.Double markerValue, System.Drawing.Color bColor, System.Drawing.Color fColor)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, markerName, markerWidth, markerValue, bColor, fColor }, out result))
            {
                return;
            }
            else
            {
                target.AddMarker(markerName, markerWidth, markerValue, bColor, fColor);
            }
        }

        [TestSpecificStub("TrendingChartControl.TrendingChartCtl.AddMarker(System.String,System.Double,System.Double,System.Drawing.Color,System.Drawing.Color)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestDrawDataMarker02")]
        public static void AddMarkerTSS(
        TrendingChartControl.TrendingChartCtl target,
        System.String markerName, System.Double markerWidth, System.Double markerValue, System.Drawing.Color bColor, System.Drawing.Color fColor)
        {
            Exception e = new Exception();
            throw e;
        }

        [MethodSpecificStubsMethod("TrendingChartControl.TrendingChartCtl.DisableZooming()")]
        public static void DisableZoomingMSS(
        TrendingChartControl.TrendingChartCtl target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.DisableZooming();
            }
        }

        [TestSpecificStub("TrendingChartControl.TrendingChartCtl.DisableZooming()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestAddSeriesForRealTimeChart")]
        public static void DisableZoomingTSS(
        TrendingChartControl.TrendingChartCtl target)
        {
            Exception e = new Exception();
            throw e;
        }

        [MethodSpecificStubsMethod("TrendingChartControl.TrendingChartCtl.ChangeStyle(System.Boolean)")]
        public static void ChangeStyleMSS(
        TrendingChartControl.TrendingChartCtl target,
        System.Boolean enable3D)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, enable3D }, out result))
            {
                return;
            }
            else
            {
                target.ChangeStyle(enable3D);
            }
        }

        [TestSpecificStub("TrendingChartControl.TrendingChartCtl.ChangeStyle(System.Boolean)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestHistory3DCb_CheckedChanged02")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestRealTime3DCb_CheckedChanged02")]
        public static void ChangeStyleTSS(
        TrendingChartControl.TrendingChartCtl target,
        System.Boolean enable3D)
        {
            Exception e = new Exception();
            throw e;
        }

        [MethodSpecificStubsMethod("TrendingChartControl.TrendingChartCtl.ClearAllSeriesAndMarkers()")]
        public static void ClearAllSeriesAndMarkersMSS(
        TrendingChartControl.TrendingChartCtl target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.ClearAllSeriesAndMarkers();
            }
        }

        [TestSpecificStub("TrendingChartControl.TrendingChartCtl.ClearAllSeriesAndMarkers()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestStartRealTimeOrMixedModeCharting05")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestStartHistoryModeCharting")]
        public static void ClearAllSeriesAndMarkersTSS(
        TrendingChartControl.TrendingChartCtl target)
        {
            Exception e = new Exception();
            throw e;
        }

        [MethodSpecificStubsMethod("TrendingChartControl.TrendingChartCtl.GetSeriesNames()")]
        public static System.Collections.Generic.List<System.String> GetSeriesNamesMSS(
        TrendingChartControl.TrendingChartCtl target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Collections.Generic.List<System.String>)result;
            }
            else
            {
                return target.GetSeriesNames();
            }
        }

        [TestSpecificStub("TrendingChartControl.TrendingChartCtl.GetSeriesNames()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestUpdateChartForRealTimeOrMixedMode04")]
        public static System.Collections.Generic.List<System.String> GetSeriesNamesTSS(
        TrendingChartControl.TrendingChartCtl target)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

