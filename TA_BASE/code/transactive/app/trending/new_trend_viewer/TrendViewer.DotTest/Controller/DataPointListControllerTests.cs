using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using STEE.ISCS.MVC;
using Entity.Trending;
using System.Collections.Generic;
using System.Data;
using TrendViewer.View;
using TrendViewer.Model;
using DAO.Trending;
using System.Windows.Forms;
using DAO.Trending.Common;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class DataPointListControllerTests
    {
        private DataPointListController dataPointListController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            if (DBConnectionStrings.GetInstance().GetConnectionStrings().Count < 1)
            {
                DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
                SimpleDatabase.GetInstance().OpenConnection();
            }
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.DataPointList, TestConst.TEST_VIEW_ID);
            dataPointListController = (DataPointListController)(view.getController());
            dataPointListController.InitDataPointTableColumn();

            DataPointListModel model = new DataPointListModel();
            dataPointListController.FillDataPointTable(model.GetDPListByGrp(TestConst.GROUP_NAME)); //will call setDataPointRowByEty
            dataPointListController.SetGrpName(TestConst.GROUP_NAME);

        }

        [Test]
        public void TestNotifyObserver()
        {
             NotifyObject obj = new NotifyObject();
              obj.Name = "b";
              dataPointListController.NotifyObserver(obj);

              obj.Name = TestConst.TEST_VIEW_ID;
              obj.Type = DataNotificaitonConst.SelectDPGroupToLoad;
              obj.Body = TestConst.GROUP_NAME;
              dataPointListController.NotifyObserver(obj);

              obj.Type =  DataNotificaitonConst.SaveDataPointToGroup;
              dataPointListController.NotifyObserver(obj);

              obj.Type = DataNotificaitonConst.AddDataPoint;
              obj.Body = new EtyDataPoint();
              dataPointListController.NotifyObserver(obj);

              obj.Type = DataNotificaitonConst.ModifyDataPoint;
              KeyValuePair<string, EtyDataPoint> pa = new KeyValuePair<string, EtyDataPoint>("dpName", new EtyDataPoint());
              obj.Body  = pa;
              dataPointListController.NotifyObserver(obj);
        }



        [Test]
        public void TestAddDataPointToList_ModifyDataPoint()
        {
            EtyDataPoint dp = new EtyDataPoint();
            dp.DPName = "ToBeModified";
            dataPointListController.AddDataPointToList(dp);
            dataPointListController.ModifyDataPoint("ToBeModified", dp);
        }

        [Test]
        public void TestConvertDataTableToList_SetDataPointEtyByRow()
        {
            dataPointListController.AddDataPointToList(new EtyDataPoint());
            dataPointListController.ConvertDataTableToList();
        }

        [Test]
        public void TestSaveDataPointListToGroup()
        {
            dataPointListController.SaveDataPointListToGroup(TestConst.GROUP_NAME);
        }

        [Test]
        public void TestGetDataPointTable()
        {
            dataPointListController.GetDataPointTable();
        }

        [Test]
        public void TestCellDoubleClick01()
        {
            DataGridViewCellEventArgs e = new DataGridViewCellEventArgs(-1,0);
            dataPointListController.CellDoubleClick(null, e);

            e = new DataGridViewCellEventArgs(0, 0);
            dataPointListController.CellDoubleClick(null, e);
            e = new DataGridViewCellEventArgs(1, 0);
            dataPointListController.CellDoubleClick(null, e);
        }

        [Test]
        public void TestCellDoubleClick02()
        {
            //Exception caught MessageBoxDialog.Show(,,,)
            DataGridViewCellEventArgs e = new DataGridViewCellEventArgs(1,0);
            //e.ColumnIndex = 1;
            dataPointListController.CellDoubleClick(null, e);
        }

        [Test]
        public void TestLoadDataPoint()
        {
            dataPointListController.LoadDataPoint(null, null);
        }

        [Test]
        public void TestSetGrpName()
        {
            dataPointListController.SetGrpName(TestConst.GROUP_NAME);
        }

        [Test]
        public void TestDeleteByDataPointName()
        {
            EtyDataPoint dp = new EtyDataPoint();
            dp.DPName = "ToBeDel_DP";
            dataPointListController.AddDataPointToList(dp);
            dataPointListController.DeleteByDataPointName("ToBeDel_DP");
        }

        [Test]
        public void TestEditDataPoint()
        {
            dataPointListController.EditDataPoint(TestConst.DP_NAME);
        }

        [Test]
        public void TestSaveDataPoint01()
        {
            dataPointListController.SaveDataPoint(null, null);
        }

        [Test]
        public void TestSaveDataPoint02()
        {
            //test exception
            dataPointListController.SaveDataPoint(null, null);
        }

        [Test]
        public void TestAddDataPoint()
        {
            dataPointListController.AddDataPoint(null, null);
        }

        [Test]
        public void TestDataPointListDataGridView_Sorted()
        {
            dataPointListController.DataPointListDataGridView_Sorted(null, null);
        }

        [Test]
        public void TestPageNumdataPointListDataGridView_ValueChanged()
        {
            dataPointListController.PageNumdataPointListDataGridView_ValueChanged(null, null);
        }

        [Test]
        public void TestFormClose()
        {
            dataPointListController.FormClose(null, null);
        }
    }
}
