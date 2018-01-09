using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.View.FormulaData")]
    public class FormulaDataMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.View.FormulaData.GetFormulaName()")]
        public static System.String GetFormulaNameMSS(
        TrendViewer.View.FormulaData target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.String)result;
            }
            else
            {
                return target.GetFormulaName();
            }
        }

        [TestSpecificStub("TrendViewer.View.FormulaData.GetFormulaName()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
         [TestSpecificStubContext(typeof(FormulaDataControllerTests), "TestAddFormulaData_ModifyFormulaData_02" )]
        [TestSpecificStubContext(typeof(FormulaDataControllerTests), "TestAddFormulaData_ModifyFormulaData_03")]
        public static System.String GetFormulaNameTSS(
        TrendViewer.View.FormulaData target)
        {
            return "Not_Empty_Name";
        }

    }
}

