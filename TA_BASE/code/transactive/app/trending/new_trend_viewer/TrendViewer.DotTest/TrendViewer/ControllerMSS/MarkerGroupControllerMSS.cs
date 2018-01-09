using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ControllerMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.Controller.MarkerGroupController")]
    public class MarkerGroupControllerMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.Controller.MarkerGroupController.SetFormType(TrendViewer.View.FormType)")]
        public static void SetFormTypeMSS(
            /*TrendViewer.Controller.MarkerGroupController*/ object target,
        TrendViewer.View.FormType type)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, type }, out result))
            {
                return;
            }
            else
            {
                Helper.CallMethod(target, "SetFormType", new object[] { type });
            }
        }

        [TestSpecificStub("TrendViewer.Controller.MarkerGroupController.SetFormType(TrendViewer.View.FormType)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(MarkerListControllerTests), "TestSaveMarker02")]
        public static void SetFormTypeTSS(
            /*TrendViewer.Controller.MarkerGroupController*/ object target,
        TrendViewer.View.FormType type)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

