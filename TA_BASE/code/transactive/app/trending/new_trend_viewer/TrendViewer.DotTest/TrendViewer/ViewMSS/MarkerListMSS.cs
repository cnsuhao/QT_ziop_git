using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace TrendViewer.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.View.MarkerList")]
    public class MarkerListMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.View.MarkerList.PopulateDataMarkerListDataGridView(System.Int32)")]
        public static void PopulateDataMarkerListDataGridViewMSS(
        TrendViewer.View.MarkerList target,
        System.Int32 mode)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, mode }, out result))
            {
                return;
            }
            else
            {
                target.PopulateDataMarkerListDataGridView(mode);
            }
        }

        [TestSpecificStub("TrendViewer.View.MarkerList.PopulateDataMarkerListDataGridView(System.Int32)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(MarkerListTests), "TestDataMarkerViewer_Load02")]
        public static void PopulateDataMarkerListDataGridViewTSS(
        TrendViewer.View.MarkerList target,
        System.Int32 mode)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

