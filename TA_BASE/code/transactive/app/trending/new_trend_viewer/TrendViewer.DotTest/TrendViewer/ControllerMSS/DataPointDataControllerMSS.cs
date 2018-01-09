using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ControllerMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.Controller.DataPointDataController")]
    public class DataPointDataControllerMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.Controller.DataPointDataController.DataPointNameValid(System.String)")]
        public static System.Boolean DataPointNameValidMSS(
            /*TrendViewer.Controller.DataPointDataController*/ object target,
        System.String name)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, name }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return (System.Boolean)Helper.CallMethod(target, "DataPointNameValid", new object[] { name });
            }
        }

        [TestSpecificStub("TrendViewer.Controller.DataPointDataController.DataPointNameValid(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DataPointDataControllerTests), "TestAddDataPointData_ModifyDataPoint_02")]
        public static System.Boolean DataPointNameValidTSS(
            /*TrendViewer.Controller.DataPointDataController*/ object target,
        System.String name)
        {
            return false;
        }

    }
}

