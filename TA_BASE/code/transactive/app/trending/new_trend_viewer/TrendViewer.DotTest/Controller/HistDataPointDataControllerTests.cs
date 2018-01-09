using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using TrendViewer.View;
using STEE.ISCS.MVC;
using Entity.Trending;
using System.Collections.Generic;
using System.Data;
using System;
using DAO.Trending;
using DAO.Trending.Common;
using TrendViewer.Model;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class HistDataPointDataControllerTests
    {
        private HistDataPointDataController histDataPointDataController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.AddHistDataPointData, TestConst.TEST_VIEW_ID);

            histDataPointDataController = (HistDataPointDataController)(view.getController());
            histDataPointDataController.SetFormType(FormType.Add);
            List<string> dataPointNameList = new List<string>();
            dataPointNameList.Add("dpName1");
            dataPointNameList.Add("dpName2");
            histDataPointDataController.Init(dataPointNameList);
            if (DBConnectionStrings.GetInstance().GetConnectionStrings().Count < 1)
            {
                DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
                SimpleDatabase.GetInstance().OpenConnection();
            }
        }

        [Test]
        public void TestSetFormType_GetFormType()
        {
            FormType type = histDataPointDataController.GetFormType();
            histDataPointDataController.SetFormType(type);
        }

        [Test]
        public void TestGetHistDataPoint()
        {
            histDataPointDataController.GetHistDataPoint();
        }

        [Test]
        public void TestNotifyObserver()
        {
            NotifyObject obj = new NotifyObject();
            obj.Name = "AnotherViewID";

            histDataPointDataController.NotifyObserver(obj);

            obj.Name = TestConst.TEST_VIEW_ID;
            obj.Type = DataNotificaitonConst.DPSelected;
            EtyEntity ety = new EtyEntity();
            ety.Name = "dpName";
            obj.Body = ety;
            histDataPointDataController.NotifyObserver(obj);

        }

        [Test]
        public void TestInit01()
        {
            histDataPointDataController.SetFormType(FormType.Edit);
            List<string> dataPointNameList = new List<string>();
            dataPointNameList.Add("dpName1");
            dataPointNameList.Add("dpName2");

            histDataPointDataController.Init(dataPointNameList);

            histDataPointDataController.SetFormType(FormType.Add);
            histDataPointDataController.Init(dataPointNameList);
        }

        [Test]
        public void TestInit02_HistDataPointNameValid()
        {
            List<string> dataPointNameList = new List<string>();
            dataPointNameList.Add("dpName1");
            dataPointNameList.Add("dpName2");

            DataTable table = new DataTable("TRENDVIEWER_HIST_CONFIG");

            table.Columns.Add("DATA_PT_HOST", System.Type.GetType("System.String"));
            table.Columns.Add("DATA_PT_NAME", System.Type.GetType("System.String"));
            table.Columns.Add("DATA_PT_TYPE", System.Type.GetType("System.String"));
            table.Columns.Add("DATA_PT_COLOR", System.Type.GetType("System.String"));
            table.Columns.Add("DATA_PT_SERVER", System.Type.GetType("System.String"));
            table.Columns.Add("DATA_PT_ENABLED", System.Type.GetType("System.String"));
            table.Columns.Add("DATA_PT_LBL_ENABLED", System.Type.GetType("System.String"));
            table.Columns.Add("DATA_PT_LBL_NAME", System.Type.GetType("System.String"));
            table.Columns.Add("DATA_PT_DT", System.Type.GetType("System.DateTime"));
            table.Columns.Add("DATA_PT_DT_END", System.Type.GetType("System.DateTime"));
            table.Columns.Add("ENTITYKEY", System.Type.GetType("System.UInt64"));
            DataRow row = table.NewRow();

            row["DATA_PT_DT"] = new DateTime(2013,1,1);
            row["DATA_PT_DT_END"] = new DateTime(2013, 1, 1);
            row["ENTITYKEY"] = 0;
            histDataPointDataController.Init(row, dataPointNameList);

            histDataPointDataController.HistDataPointNameValid("dpName1");
            histDataPointDataController.HistDataPointNameValid("dpName3");

        }

        [Test]
        public void TestAddHistDataPointData_ModifyHistDataPoint_01()
        {
            //m_View.GetStartDate() > m_View.GetEndDate()
            Accessor ctlAccessor = ReflectionAccessor.Wrap(histDataPointDataController);
            TrendViewer.View.HistDataPointData view = (TrendViewer.View.HistDataPointData)ctlAccessor.GetField("m_View");
            EtyHistDataPoint dp = new EtyHistDataPoint();
            dp.DPName = "TestDPName.Value";
            dp.DPStartDateTime = new DateTime(2013, 1, 2);
            dp.DPEndDateTime = new DateTime(2013, 1, 1);
            view.SetFormType(FormType.Edit);
            view.Display(dp);
            histDataPointDataController.AddHistDataPointData(null, null);
            histDataPointDataController.ModifyHistDataPoint(null, null);
        }

        [Test]
        public void TestAddHistDataPointData_ModifyHistDataPoint_02()
        {
            //! m_View.GetStartDate() > m_View.GetEndDate()
            Accessor ctlAccessor = ReflectionAccessor.Wrap(histDataPointDataController);
            TrendViewer.View.HistDataPointData view = (TrendViewer.View.HistDataPointData)ctlAccessor.GetField("m_View");
            EtyHistDataPoint dp = new EtyHistDataPoint();
            dp.DPName = "";
            dp.DPStartDateTime = new DateTime(2013, 1, 1);
            dp.DPEndDateTime = new DateTime(2013, 1, 2);
            view.SetFormType(FormType.Edit);
            view.Display(dp);
            //m_View.GetHistDataPointName() ==""
            histDataPointDataController.AddHistDataPointData(null, null);
            histDataPointDataController.ModifyHistDataPoint(null, null);
        }

        [Test]
        public void TestAddHistDataPointData_ModifyHistDataPoint_03()
        {
            //! m_View.GetStartDate() > m_View.GetEndDate()
            //m_View.GetHistDataPointName() !=""

            Accessor ctlAccessor = ReflectionAccessor.Wrap(histDataPointDataController);
            TrendViewer.View.HistDataPointData view = (TrendViewer.View.HistDataPointData)ctlAccessor.GetField("m_View");
            EtyHistDataPoint dp = new EtyHistDataPoint();
            dp.DPName = "tempName";
            dp.DPStartDateTime = new DateTime(2013, 1, 1);
            dp.DPEndDateTime = new DateTime(2013, 1, 2);
            view.SetFormType(FormType.Edit);
            view.Display(dp);

            // HistDataPointNameValid(dpName)
            histDataPointDataController.AddHistDataPointData(null, null);
            histDataPointDataController.ModifyHistDataPoint(null, null);

            //!HistDataPointNameValid(dpName)
            dp.DPName = "dpName1";
            view.Display(dp);
            histDataPointDataController.AddHistDataPointData(null, null);
            histDataPointDataController.ModifyHistDataPoint(null, null);
        }

        [Test]
        public void TestBrowseDP()
        {
            histDataPointDataController.BrowseDP(null, null);
        }

        [Test]
        public void TestGetEntityKeyByName()
        {
            HistDataPointDataModel model = new HistDataPointDataModel();
            model.GetEntityKeyByName("name.Value");
        }
    }
}
