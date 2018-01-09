using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;
using TrendViewer.DotTest.View;

namespace TrendViewer.ControllerMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.Controller.FormulaListController")]
    public class FormulaListControllerMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.Controller.FormulaListController.DeleteByFormulaLblName(System.String)")]
        public static void DeleteByFormulaLblNameMSS(
            /*TrendViewer.Controller.FormulaListController*/ object target,
        System.String formulaName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, formulaName }, out result))
            {
                return;
            }
            else
            {
                Helper.CallMethod(target, "DeleteByFormulaLblName", new object[] { formulaName });
            }
        }

        [TestSpecificStub("TrendViewer.Controller.FormulaListController.DeleteByFormulaLblName(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(FormulaListTests), "TestdataPointListDataGridView_CellDoubleClick01")]
        public static void DeleteByFormulaLblNameTSS(
            /*TrendViewer.Controller.FormulaListController*/ object target,
        System.String formulaName)
        {
            return;
        }

    }
}

