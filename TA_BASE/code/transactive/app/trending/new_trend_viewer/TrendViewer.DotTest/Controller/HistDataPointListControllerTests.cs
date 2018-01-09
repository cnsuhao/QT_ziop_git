using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using Entity.Trending;
using System.Collections.Generic;
using STEE.ISCS.MVC;
using System.Data;
using TrendViewer.View;
using TrendViewer.Model;
using DAO.Trending;
using System;
using System.Windows.Forms;
using DAO.Trending.Common;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class HistDataPointListControllerTests
    {
        private HistDataPointListController histDataPointListController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            if (DBConnectionStrings.GetInstance().GetConnectionStrings().Count < 1)
            {
                DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
                SimpleDatabase.GetInstance().OpenConnection();
            }
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.HistDataPointList, TestConst.TEST_VIEW_ID);
            histDataPointListController = (HistDataPointListController)(view.getController());
            histDataPointListController.InitHistDataPointTableColumns();

            HistDataPointListModel model = new HistDataPointListModel();
            List<EtyHistDataPoint> list  = model.GetHistDListByGrp(TestConst.HIST_GROUP_NAME);
            histDataPointListController.FillHistDataPointTable(list);
            histDataPointListController.SetGrpName(TestConst.HIST_GROUP_NAME);

        }

        [Test]
        public void TestNotifyObserver()
        {
            NotifyObject obj = new NotifyObject();
            obj.Name = "AnotherViewID";
            histDataPointListController.NotifyObserver(obj);

            obj.Name = TestConst.TEST_VIEW_ID;
            obj.Type = DataNotificaitonConst.SelectHistDPGroupToLoad;
            obj.Body = TestConst.HIST_GROUP_NAME;
            histDataPointListController.NotifyObserver(obj);

            obj.Type = DataNotificaitonConst.SaveHistDataPointToGroup;
            histDataPointListController.NotifyObserver(obj);

            obj.Type = DataNotificaitonConst.AddHistDataPoint;
            obj.Body = new EtyHistDataPoint();
            histDataPointListController.NotifyObserver(obj);

            obj.Type = DataNotificaitonConst.ModifyHistDataPoint;
            obj.Body = new KeyValuePair<string, EtyHistDataPoint>(TestConst.HIST_DP_NAME, new EtyHistDataPoint());
            histDataPointListController.NotifyObserver(obj);
        }

//         [Test]
//         public void TestSetHistDataPointRowByEty()
//         {
//             Accessor ctlAccessor = ReflectionAccessor.Wrap(histDataPointListController);
//             DataTable table = (DataTable)ctlAccessor.GetField("m_DataPointTable");
//             DataRow row = table.NewRow();
//             histDataPointListController.SetHistDataPointRowByEty(ref row, new EtyHistDataPoint());
//         }

        [Test]
        public void TestSetHistDataPointEtyByRow()
        {
            Accessor ctlAccessor = ReflectionAccessor.Wrap(histDataPointListController);
            DataTable table = (DataTable)ctlAccessor.GetField("m_DataPointTable");
            DataRow row = table.NewRow();
            row["DATA_PT_DT"] = new DateTime(2013, 1, 1);
            row["DATA_PT_DT_END"] = new DateTime(2013, 1, 2);
            row["ENTITYKEY"] = 0;
            EtyHistDataPoint dp = new EtyHistDataPoint();
            histDataPointListController.SetHistDataPointEtyByRow(ref dp, row);
        }

        [Test]
        public void TestAddHistDataPointToList_ModifyHistDataPoint_DeleteByHistDataPointName()
        {
            EtyHistDataPoint dp = new EtyHistDataPoint();
            dp.DPName = "temp";

            histDataPointListController.AddHistDataPointToList(dp);

            histDataPointListController.ModifyHistDataPoint("temp", dp);
            histDataPointListController.DeleteByHistDataPointName("temp");
        }

        [Test]
        public void TestSaveHistDataPointListToGroup()
        {
            histDataPointListController.SaveHistDataPointListToGroup(TestConst.HIST_GROUP_NAME);
        }

        [Test]
        public void TestGetHistDataPointTable()
        {
            histDataPointListController.GetHistDataPointTable();
        }

        [Test]
        public void TestLoadHistDataPoint()
        {
            histDataPointListController.LoadHistDataPoint(null, null);
        }

        [Test]
        public void TestEditHistDataPoint()
        {
            histDataPointListController.EditHistDataPoint(TestConst.HIST_DP_NAME);
        }

        [Test]
        public void TestCellDoubleClick01()
        {
            DataGridViewCellEventArgs e = new DataGridViewCellEventArgs(-1,0);

            histDataPointListController.CellDoubleClick(null, e);

            e = new DataGridViewCellEventArgs(0, 0);
            histDataPointListController.CellDoubleClick(null, e);
            e = new DataGridViewCellEventArgs(1, 0);
            histDataPointListController.CellDoubleClick(null, e);
        }

        [Test]
        public void TestCellDoubleClick02()
        {
            //Exception caught MessageBoxDialog.Show(,,,)
            DataGridViewCellEventArgs e = new DataGridViewCellEventArgs(1,0);
            histDataPointListController.CellDoubleClick(null, e);
        }

        [Test]
        public void TestSaveHistDataPoint01()
        {
            histDataPointListController.SaveHistDataPoint(null, null);
        }

        [Test]
        public void TestSaveHistDataPoint02()
        {
            //catch exception
            histDataPointListController.SaveHistDataPoint(null, null);
        }

        [Test]
        public void TestAddHistDataPoint()
        {
            Accessor ctlAccessor = ReflectionAccessor.Wrap(histDataPointListController);
            DataTable table = (DataTable)ctlAccessor.GetField("m_DataPointTable");
            DataRow row = table.NewRow();
            row["DATA_PT_DT"] = new DateTime(2013, 1, 1);
            row["DATA_PT_DT_END"] = new DateTime(2013, 1, 2);
            row["ENTITYKEY"] = 0;
            table.Rows.Add(row);
            ctlAccessor.SetField("m_DataPointTable", table);
            histDataPointListController.AddHistDataPoint(null, null);
        }

        [Test]
        public void TestHistDataPointListDataGridView_Sorted()
        {
            histDataPointListController.HistDataPointListDataGridView_Sorted(null, null);
        }

        [Test]
        public void TestPageNumdataPointListDataGridView_ValueChanged()
        {
            histDataPointListController.PageNumdataPointListDataGridView_ValueChanged(null, null);
        }

        [Test]
        public void TestFormClose()
        {
            histDataPointListController.FormClose(null, null);
        }

        [Test]
        public void TestConvertDataTableToList()
        {
            //m_DataPointTable should has rows
            Accessor ctlAccessor = ReflectionAccessor.Wrap(histDataPointListController);
            DataTable table = (DataTable)ctlAccessor.GetField("m_DataPointTable");
            DataRow row = table.NewRow();
            row["DATA_PT_DT"] = new DateTime(2013, 1, 1);
            row["DATA_PT_DT_END"] = new DateTime(2013, 1, 2);
            row["ENTITYKEY"] = 0;
            table.Rows.Add(row);
            ctlAccessor.SetField("m_DataPointTable", table);
            histDataPointListController.ConvertDataTableToList();
        }
    }
}
