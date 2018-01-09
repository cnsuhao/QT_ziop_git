using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.CommonMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.Common.TrendCache")]
    public class TrendCacheMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.Common.TrendCache.CountEnabledHistDPList()")]
        public static System.Int32 CountEnabledHistDPListMSS(
            /*TrendViewer.Common.TrendCache*/ object target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Int32)result;
            }
            else
            {
                return (System.Int32)Helper.CallMethod(target, "CountEnabledHistDPList", new object[] { });
            }
        }

        [TestSpecificStub("TrendViewer.Common.TrendCache.CountEnabledHistDPList()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestHistoryExecuteCmd_Click03")]
        public static System.Int32 CountEnabledHistDPListTSS(
            /*TrendViewer.Common.TrendCache*/ object target)
        {
            return 0;
        }

    }
}

