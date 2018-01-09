using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace TrendViewer.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.View.HistDataPointList")]
    public class HistDataPointListMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.View.HistDataPointList.InitializeDataPointListDataGridView()")]
        public static void InitializeDataPointListDataGridViewMSS(
        TrendViewer.View.HistDataPointList target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                Helper.CallMethod(target, "InitializeDataPointListDataGridView", new object[] { });
            }
        }

        [TestSpecificStub("TrendViewer.View.HistDataPointList.InitializeDataPointListDataGridView()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(HistDataPointListTests), "TestDisplay")]
        public static void InitializeDataPointListDataGridViewTSS(
        TrendViewer.View.HistDataPointList target)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

