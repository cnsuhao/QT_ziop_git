using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ControllerMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.Controller.FormulaDataController")]
    public class FormulaDataControllerMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.Controller.FormulaDataController.FormulaNameValid(System.String)")]
        public static System.Boolean FormulaNameValidMSS(
            /*TrendViewer.Controller.FormulaDataController*/ object target,
        System.String name)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, name }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return (System.Boolean)Helper.CallMethod(target, "FormulaNameValid", new object[] { name });
            }
        }

        [TestSpecificStub("TrendViewer.Controller.FormulaDataController.FormulaNameValid(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
         [TestSpecificStubContext(typeof(FormulaDataControllerTests), "TestAddFormulaData_ModifyFormulaData_02" )]
        [TestSpecificStubContext(typeof(FormulaDataControllerTests), "TestAddFormulaData_ModifyFormulaData_03" )]
        public static System.Boolean FormulaNameValidTSS(
            /*TrendViewer.Controller.FormulaDataController*/ object target,
        System.String name)
        {
            string methordName = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (methordName.Equals("TestAddFormulaData_ModifyFormulaData_02"))
            {
                return false;
            }
            else
            {
                return true;
            }
        }

    }
}

