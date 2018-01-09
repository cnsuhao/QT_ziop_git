using System;
using Dottest.Framework;

namespace OPCSampleGrpConfig.ControllerMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCSampleGrpConfig")]
    [MethodSpecificStubsClassAttribute("OPCSampleGrpConfig.Controller.OPCSampleGrpConfigStartController")]
    public class OPCSampleGrpConfigStartControllerMSS
    {
        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Controller.OPCSampleGrpConfigStartController.GetDatabaseConnection(System.String)")]
        public static System.Boolean GetDatabaseConnectionMSS(
        OPCSampleGrpConfig.Controller.OPCSampleGrpConfigStartController target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.GetDatabaseConnection();
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Controller.OPCSampleGrpConfigStartController.GetDatabaseConnection(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.View.OPCSampleGrpConfigStartTests), "TestAttachListener02")]
        public static System.Boolean GetDatabaseConnectionTSS(
        OPCSampleGrpConfig.Controller.OPCSampleGrpConfigStartController target)
        {
            return false;
        }

    }
}

