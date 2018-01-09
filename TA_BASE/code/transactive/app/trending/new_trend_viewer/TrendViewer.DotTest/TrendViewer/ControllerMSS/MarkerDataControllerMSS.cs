using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace TrendViewer.ControllerMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.Controller.MarkerDataController")]
    public class MarkerDataControllerMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.Controller.MarkerDataController.GetMarker()")]
        public static Entity.Trending.EtyMarker GetMarkerMSS(
            /*TrendViewer.Controller.MarkerDataController*/ object target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (Entity.Trending.EtyMarker)result;
            }
            else
            {
                return (Entity.Trending.EtyMarker)Helper.CallMethod(target, "GetMarker", new object[] { });
            }
        }

        [TestSpecificStub("TrendViewer.Controller.MarkerDataController.GetMarker()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(MarkerDataTests), "TestMarkerData_Load02")]
        public static Entity.Trending.EtyMarker GetMarkerTSS(
            /*TrendViewer.Controller.MarkerDataController*/ object target)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

