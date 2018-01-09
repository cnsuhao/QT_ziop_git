using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;
using Entity.Trending;
using System.Collections.Generic;

namespace TrendViewer.ModelMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.Model.TrendViewModel")]
    public class TrendViewModelMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.Model.TrendViewModel.GetHistDPLogList(Entity.Trending.EtyHistDataPoint,System.DateTime,System.DateTime)")]
        public static System.Collections.Generic.List<Entity.Trending.EtyDataLogDPLogTrend> GetHistDPLogListMSS(
            /*TrendViewer.Model.TrendViewModel*/ object target,
        Entity.Trending.EtyHistDataPoint histDP, System.DateTime startTime, System.DateTime endTime)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, histDP, startTime, endTime }, out result))
            {
                return (System.Collections.Generic.List<Entity.Trending.EtyDataLogDPLogTrend>)result;
            }
            else
            {
                return (System.Collections.Generic.List<Entity.Trending.EtyDataLogDPLogTrend>)Helper.CallMethod(target, "GetHistDPLogList", new object[] { histDP, startTime, endTime });
            }
        }

        [TestSpecificStub("TrendViewer.Model.TrendViewModel.GetHistDPLogList(Entity.Trending.EtyHistDataPoint,System.DateTime,System.DateTime)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestExportHistoryData01")]
        public static System.Collections.Generic.List<Entity.Trending.EtyDataLogDPLogTrend> GetHistDPLogListTSS(
            /*TrendViewer.Model.TrendViewModel*/ object target,
        Entity.Trending.EtyHistDataPoint histDP, System.DateTime startTime, System.DateTime endTime)
        {
            List<EtyDataLogDPLogTrend> logList = new List<EtyDataLogDPLogTrend>();
            EtyDataLogDPLogTrend log = new EtyDataLogDPLogTrend();
            log.CreateTime = DateTime.Now;
            log.Value = 2;
            logList.Add(log);
            return logList;
        }

    }
}

