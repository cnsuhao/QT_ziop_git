using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace TrendViewer.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.View.FormulaList")]
    public class FormulaListMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.View.FormulaList.PopulateDataPointListDataGridView(System.Int32)")]
        public static void PopulateDataPointListDataGridViewMSS(
        TrendViewer.View.FormulaList target,
        System.Int32 mode)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, mode }, out result))
            {
                return;
            }
            else
            {
                target.PopulateDataPointListDataGridView(mode);
            }
        }

        [TestSpecificStub("TrendViewer.View.FormulaList.PopulateDataPointListDataGridView(System.Int32)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(FormulaListTests), "TestDataPointFormulaViewer_Load02")]
        public static void PopulateDataPointListDataGridViewTSS(
        TrendViewer.View.FormulaList target,
        System.Int32 mode)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

