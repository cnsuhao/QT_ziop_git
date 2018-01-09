using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ControllerMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.Controller.DataPointGroupController")]
    public class DataPointGroupControllerMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.Controller.DataPointGroupController.SetFormType(TrendViewer.View.FormType)")]
        public static void SetFormTypeMSS(
            /*TrendViewer.Controller.DataPointGroupController*/ object target,
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

        [TestSpecificStub("TrendViewer.Controller.DataPointGroupController.SetFormType(TrendViewer.View.FormType)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DataPointListControllerTests), "TestSaveDataPoint02")]
        public static void SetFormTypeTSS(
            /*TrendViewer.Controller.DataPointGroupController*/ object target,
        TrendViewer.View.FormType type)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

