using System;
using Dottest.Framework;

namespace OPCSampleGrpConfig.ControllerMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCSampleGrpConfig")]
    [MethodSpecificStubsClassAttribute("OPCSampleGrpConfig.Controller.OPCDataSelectorController")]
    public class OPCDataSelectorControllerMSS
    {
        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Controller.OPCDataSelectorController.GetDataPointList(System.String,System.String)")]
        public static void GetDataPointListMSS(
        OPCSampleGrpConfig.Controller.OPCDataSelectorController target,
        System.String opcServerName, System.String filterString)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, opcServerName, filterString }, out result))
            {
                return;
            }
            else
            {
                target.GetDataPointList(opcServerName, filterString);
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Controller.OPCDataSelectorController.GetDataPointList(System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TesttreeOpcItems_NodeMouseClick01")]
        public static void GetDataPointListTSS(
        OPCSampleGrpConfig.Controller.OPCDataSelectorController target,
        System.String opcServerName, System.String filterString)
        {
            return;
        }

    }
}

