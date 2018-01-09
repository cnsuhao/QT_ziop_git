using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using STEE.ISCS.MVC;
using System.Collections.Generic;
using Entity.Trending;
using System.Windows.Forms;
using System;
using TrendViewer.Model;
using TrendViewer.View;
using TrendViewer.Common;
using DAO.Trending;
using DAO.Trending.Common;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class TrendViewControllerTests
    {
        private TrendViewController trendViewController;
        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.TrendView, TestConst.TEST_VIEW_ID); 
            trendViewController = (TrendViewController)view.getController();
            if (DBConnectionStrings.GetInstance().GetConnectionStrings().Count < 1)
            {
                DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
                SimpleDatabase.GetInstance().OpenConnection();
            }
        }

        [Test]
        public void TestNotifyObserver()
        {
            NotifyObject notifyObj = new NotifyObject();
            notifyObj.Name = "anotherName";
            trendViewController.NotifyObserver(notifyObj);

            notifyObj.Name =TestConst.TEST_VIEW_ID;
            notifyObj.Type = DataNotificaitonConst.DPListClosed;
            List<EtyDataPoint> list1 = new List<EtyDataPoint>();
            KeyValuePair<string, List<EtyDataPoint>> pair1 = new KeyValuePair<string, List<EtyDataPoint>>("Test Group Name", list1);
            EtyDataPoint dp = new EtyDataPoint();
            dp.ConfigName = "Test Group Name";
            list1.Add(dp);
            notifyObj.Body = pair1;
            trendViewController.NotifyObserver(notifyObj);

            notifyObj.Type = DataNotificaitonConst.HistDPListClosed;
            List<EtyHistDataPoint> histList = new List<EtyHistDataPoint>();
            KeyValuePair<string, List<EtyHistDataPoint>> pair2 = new KeyValuePair<string, List<EtyHistDataPoint>>("Hist Test Group Name", histList);
            histList.Add(new EtyHistDataPoint());
            notifyObj.Body = pair2;
            trendViewController.NotifyObserver(notifyObj);

            notifyObj.Type = DataNotificaitonConst.MarkListClosed;
            List<EtyMarker> markerList = new List<EtyMarker>();
            KeyValuePair<string, List<EtyMarker>> pair3 = new KeyValuePair<string, List<EtyMarker>>("Marker Test Group Name",markerList);
            markerList.Add(new EtyMarker());
            notifyObj.Body = pair3;
            trendViewController.NotifyObserver(notifyObj);


            notifyObj.Type = DataNotificaitonConst.FormulaListClosed;
            List<EtyFormula> formulaList = new List<EtyFormula>();
            KeyValuePair<string, List<EtyFormula>> pair4 = new KeyValuePair<string, List<EtyFormula>>("Formula Test Group Name", formulaList);
            formulaList.Add(new EtyFormula());
            notifyObj.Body = pair4;
            trendViewController.NotifyObserver(notifyObj);

            notifyObj.Type = DataNotificaitonConst.ModifyTitles;
            string[] titles = new string[3];
            titles[0]="a";
            titles[1]="b";
            titles[2]="c";
            notifyObj.Body = titles;
            trendViewController.NotifyObserver(notifyObj);
        }

        [Test]
        public void TestDrawTrendView()
        {
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.TrendView);  //viewID is "" 
            Form frm = (Form)view;
            string param = "view-edit-0,"+TestConst.GROUP_NAME+","
                + TestConst.MARK_GROUP_NAME + ","
                + TestConst.HIST_GROUP_NAME + "," 
                + TestConst.FORMULA_GROUP_NAME;
            trendViewController.DrawTrendView(ref frm, param);

            param = "view-edit-1";
            trendViewController.DrawTrendView(ref frm, param);

            param = "view-edit-2";
            trendViewController.DrawTrendView(ref frm, param);

            param = "view-0";
            trendViewController.DrawTrendView(ref frm, param);

            param = "view-1";
            trendViewController.DrawTrendView(ref frm, param);

            param = "other";
            trendViewController.DrawTrendView(ref frm, param);
        }

        [Test]
        public void TestshowAbout()
        {
            trendViewController.showAbout(null, null);
        }

        [Test]
        public void TestShowMarkerList()
        {
            trendViewController.ShowMarkerList(null,null);
        }

        [Test]
        public void TestShowFormulaList()
        {
            trendViewController.ShowFormulaList(null, null);
        }

        [Test]
        public void TestShowDataPointList()
        {
            trendViewController.ShowDataPointList(null, null);
        }

        [Test]
        public void TestShowHistDataPointList()
        {
            trendViewController.ShowHistDataPointList(null, null);
        }

        [Test]
        public void TestChangeTrendTitle()
        {
            trendViewController.ChangeTrendTitle(null, null);
        }

        [Test]
        public void TestCaptureScreenShoot01()
        {
            trendViewController.CaptureScreenShoot(null, null);
        }

        [Test]
        public void TestCaptureScreenShoot02()
        {
            trendViewController.CaptureScreenShoot(null, null);
        }

        [Test]
        public void TestGetHistDPLogMap()
        {
            HistDataPointListModel model = new HistDataPointListModel();
            
            Accessor ctlAccessor = ReflectionAccessor.Wrap(trendViewController);
            TrendCache cache = (TrendCache)(ctlAccessor.GetField("m_trendCache"));
            cache.m_histDataPointList = model.GetHistDListByGrp(TestConst.HIST_GROUP_NAME);
            cache.m_histDatapointGrpName = TestConst.HIST_GROUP_NAME;

            DateTime fromTime = new DateTime(2013, 1, 10);
            DateTime toTime = new DateTime(2013, 1, 12);
            trendViewController.GetHistDPLogMap(fromTime, toTime);
        }

        [Test]
        public void TestRealTimeExecuteCmd_Click01()
        {
            //test  If user click "start"
            DataPointListModel model = new DataPointListModel();

            Accessor ctlAccessor = ReflectionAccessor.Wrap(trendViewController);
            TrendCache cache = (TrendCache)(ctlAccessor.GetField("m_trendCache"));

            cache.m_dataPointList = model.GetDPListByGrp("");
            cache.m_datapointGrpName = "";
            trendViewController.RealTimeExecuteCmd_Click(null, null);
            

            cache.m_dataPointList = model.GetDPListByGrp(TestConst.GROUP_NAME);
            cache.m_datapointGrpName = TestConst.GROUP_NAME;
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.TrendView);  //viewID is "" 
            TrendView frm = (TrendView)view;
            Accessor frmAccessor = ReflectionAccessor.Wrap(frm);
            System.Windows.Forms.CheckBox checkbox = (System.Windows.Forms.CheckBox)frmAccessor.GetField("mixedModeCb");
            checkbox.Checked = true;
            frmAccessor.SetField("mixedModeCb", checkbox);
            trendViewController.RealTimeExecuteCmd_Click(null, null);
            
        }

        [Test]
        public void TestRealTimeExecuteCmd_Click02()
        {
            //test  If user click "stop"
            DataPointListModel model = new DataPointListModel();

            Accessor ctlAccessor = ReflectionAccessor.Wrap(trendViewController);
            TrendCache cache = (TrendCache)(ctlAccessor.GetField("m_trendCache"));

            cache.m_dataPointList = model.GetDPListByGrp("");
            cache.m_datapointGrpName = "";
            trendViewController.RealTimeExecuteCmd_Click(null, null);
            
        }


        [Test]
        public void TestDataLoggerConfigurationToolStripMenuItem_Click()
        {
            trendViewController.DataLoggerConfigurationToolStripMenuItem_Click(null, null);
        }
        [Test]
        public void TestDataLoggerConfigurationToolStripMenuItem_Click02()
        {
            trendViewController.DataLoggerConfigurationToolStripMenuItem_Click(null, null);
        }

        [Test]
        public void TestExportHistoryDataToolStripMenuItem_Click01()
        {
            HistDataPointListModel model = new HistDataPointListModel();

            Accessor ctlAccessor = ReflectionAccessor.Wrap(trendViewController);
            TrendCache cache = (TrendCache)(ctlAccessor.GetField("m_trendCache"));

            cache.m_histDataPointList = model.GetHistDListByGrp("");
            cache.m_histDatapointGrpName = "";
            trendViewController.ExportHistoryDataToolStripMenuItem_Click(null, null);

            cache.m_histDataPointList = model.GetHistDListByGrp(TestConst.HIST_GROUP_NAME);
            cache.m_histDatapointGrpName = TestConst.HIST_GROUP_NAME;
            trendViewController.ExportHistoryDataToolStripMenuItem_Click(null, null);
        }

        [Test]
        public void TestExportHistoryDataToolStripMenuItem_Click02()
        { // catch exception in func ExportHistoryDataToolStripMenuItem_Click02(
            Accessor ctlAccessor = ReflectionAccessor.Wrap(trendViewController);
            TrendCache cache = (TrendCache)(ctlAccessor.GetField("m_trendCache"));
            HistDataPointListModel model = new HistDataPointListModel();
            cache.m_histDataPointList = model.GetHistDListByGrp(TestConst.HIST_GROUP_NAME);
            cache.m_histDatapointGrpName = TestConst.HIST_GROUP_NAME;
            trendViewController.ExportHistoryDataToolStripMenuItem_Click(null, null);
        }

        [Test]
        public void TestExportHistoryDataToolStripMenuItem_Click03()
        {
            // hasData = false in ExportHistoryData
            Accessor ctlAccessor = ReflectionAccessor.Wrap(trendViewController);
            TrendCache cache = (TrendCache)(ctlAccessor.GetField("m_trendCache"));
            HistDataPointListModel model = new HistDataPointListModel();
            cache.m_histDataPointList = model.GetHistDListByGrp(TestConst.HIST_GROUP_NAME_NOLOG);
            cache.m_histDatapointGrpName = TestConst.HIST_GROUP_NAME;
            trendViewController.ExportHistoryDataToolStripMenuItem_Click(null, null);
        }

        [Test]
        public void TestExportHistoryData01()
        {
            // m_Model.GetHistDPLogList has data
            Accessor ctlAccessor = ReflectionAccessor.Wrap(trendViewController);
            TrendCache cache = (TrendCache)(ctlAccessor.GetField("m_trendCache"));
            HistDataPointListModel model = new HistDataPointListModel();
            cache.m_histDataPointList = model.GetHistDListByGrp(TestConst.HIST_GROUP_NAME);
            cache.m_histDataPointList[0].DPName = "NotEmpty";
            cache.m_histDatapointGrpName = TestConst.HIST_GROUP_NAME;

            ctlAccessor.Call("ExportHistoryData", "a.csv");
        }


        [Test]
        public void TestHistoryExecuteCmd_Click01()
        {
            HistDataPointListModel model = new HistDataPointListModel();
            Accessor ctlAccessor = ReflectionAccessor.Wrap(trendViewController);
            TrendCache cache = (TrendCache)(ctlAccessor.GetField("m_trendCache"));

            cache.m_histDataPointList = model.GetHistDListByGrp(TestConst.HIST_GROUP_NAME);
            cache.m_histDatapointGrpName = TestConst.HIST_GROUP_NAME;
            trendViewController.HistoryExecuteCmd_Click(null,null);
        }

        [Test]
        public void TestHistoryExecuteCmd_Click02()
        {
           // stub: m_View.GetRealTimeStartFlag() == true
            trendViewController.HistoryExecuteCmd_Click(null, null);
        }

        [Test]
        public void TestHistoryExecuteCmd_Click03()
        {
            // stub: m_View.GetRealTimeStartFlag() == false;
            //stub: m_trendCache.CountEnabledHistDPList()< 1
            trendViewController.HistoryExecuteCmd_Click(null, null);
        }

        [Test]
        public void TestHistoryExecuteCmd_Click04()
        {
            //stub: (maxValue.ToOADate() < minValue.ToOADate())
            trendViewController.HistoryExecuteCmd_Click(null, null);
        }

        [Test]
        public void TestHistoryExecuteCmd_Click05()
        {
            //stub: m_View.GetStartTime() throw exception
            trendViewController.HistoryExecuteCmd_Click(null, null);
        }
        [Test]
        public void Testtimer1_Tick01()
        {
            //m_View.IsMixMode() = true
            trendViewController.timer1_Tick(null, null);
        }

        [Test]
        public void Testtimer1_Tick02()
        {
            //Exception caught when m_View.IsMixMode()
            trendViewController.timer1_Tick(null, null);
        }

        [Test]
        public void TestPrintorToolStripMenuItem_Click()
        {
            trendViewController.PrintorToolStripMenuItem_Click(null, null);
        }
    }
}
