using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace TrendViewer.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.View.DataPointList")]
    public class DataPointListMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.View.DataPointList.PopulateDataPointListDataGridView(System.Int32,System.Data.DataTable)")]
        public static void PopulateDataPointListDataGridViewMSS(
        TrendViewer.View.DataPointList target,
        System.Int32 mode, System.Data.DataTable dpList)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, mode, dpList }, out result))
            {
                return;
            }
            else
            {
                target.PopulateDataPointListDataGridView(mode, dpList);
            }
        }

        [TestSpecificStub("TrendViewer.View.DataPointList.PopulateDataPointListDataGridView(System.Int32,System.Data.DataTable)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DataPointListTests), "TestDisplay02")]
        public static void PopulateDataPointListDataGridViewTSS(
        TrendViewer.View.DataPointList target,
        System.Int32 mode, System.Data.DataTable dpList)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

