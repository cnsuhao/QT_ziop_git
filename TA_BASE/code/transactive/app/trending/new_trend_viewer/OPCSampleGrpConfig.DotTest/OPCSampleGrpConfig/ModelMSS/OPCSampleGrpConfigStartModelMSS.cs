using System;
using Dottest.Framework;

namespace OPCSampleGrpConfig.ModelMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCSampleGrpConfig")]
    [MethodSpecificStubsClassAttribute("OPCSampleGrpConfig.Model.OPCSampleGrpConfigStartModel")]
    public class OPCSampleGrpConfigStartModelMSS
    {
        [MethodSpecificStubsMethod("OPCSampleGrpConfig.Model.OPCSampleGrpConfigStartModel.DeleteOPCSampleGrp(System.String)")]
        public static System.Boolean DeleteOPCSampleGrpMSS(
        OPCSampleGrpConfig.Model.OPCSampleGrpConfigStartModel target,
        System.String opcSampelGrpId)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, opcSampelGrpId }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.DeleteOPCSampleGrp(opcSampelGrpId);
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.Model.OPCSampleGrpConfigStartModel.DeleteOPCSampleGrp(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestIntervalConfigDataGridView_CellDoubleClick03")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestIntervalConfigDataGridView_CellDoubleClick04")]
        public static System.Boolean DeleteOPCSampleGrpTSS(
        OPCSampleGrpConfig.Model.OPCSampleGrpConfigStartModel target,
        System.String opcSampelGrpId)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (name.Equals("TestIntervalConfigDataGridView_CellDoubleClick03"))
            {
                return true;
            }
            else
                return false;
        }

    }
}

