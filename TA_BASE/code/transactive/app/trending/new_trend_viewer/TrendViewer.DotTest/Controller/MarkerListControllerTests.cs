using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using STEE.ISCS.MVC;
using Entity.Trending;
using System.Collections.Generic;
using System.Data;
using TrendViewer.Model;
using TrendViewer.View;
using DAO.Trending;
using System.Windows.Forms;
using DAO.Trending.Common;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class MarkerListControllerTests
    {
        private MarkerListController markerListController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            if (DBConnectionStrings.GetInstance().GetConnectionStrings().Count < 1)
            {
                DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
                SimpleDatabase.GetInstance().OpenConnection();
            }
            SimpleDatabase.GetInstance().OpenConnection();
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.MarkerList, TestConst.TEST_VIEW_ID);
            markerListController = (MarkerListController)(view.getController());
            markerListController.initMarkerTable();
            MarkerListModel model = new MarkerListModel();
            markerListController.SetMarkerTable(model.GetMarkerListByGrp(TestConst.MARK_GROUP_NAME));
            markerListController.SetGrpName(TestConst.MARK_GROUP_NAME);
        }

        [Test]
        public void TestNotifyObserver()
        {
            NotifyObject obj = new NotifyObject();
            obj.Name = "Another_ViewID";
            markerListController.NotifyObserver(obj);

            obj.Name = TestConst.TEST_VIEW_ID;
            obj.Type = DataNotificaitonConst.SelectMarkGroupToLoad;
            obj.Body = TestConst.MARK_GROUP_NAME;
            markerListController.NotifyObserver(obj);

            obj.Type = DataNotificaitonConst.SaveMarkerToGroup;
            markerListController.NotifyObserver(obj);

            obj.Type = DataNotificaitonConst.AddMarker;
            EtyMarker ety = new EtyMarker();
            ety.MarkerName = "tempMarker";
            obj.Body = ety;
            markerListController.NotifyObserver(obj);

            obj.Type = DataNotificaitonConst.ModifyMarker;
            KeyValuePair<string ,EtyMarker> pair = new KeyValuePair<string ,EtyMarker>("tempName", ety);
            obj.Body = pair;
            markerListController.NotifyObserver(obj);

            markerListController.DeleteByMarkerName("tempMarker");
        }

        [Test]
        public void TestSetMarkerEtyByRow()
        {
            EtyMarker marker = new EtyMarker();

            DataTable table = new DataTable("TRENDVIEWER_MARKER");
            table.Columns.Add("CONFIG_NAME", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_NAME", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_WIDTH", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_VALUE", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_ENABLED", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_BCOLOR", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_FCOLOR", System.Type.GetType("System.String"));
            DataRow row = table.NewRow();

            row["MARKER_WIDTH"] = 3;
            row["MARKER_VALUE"] = 1;
            row["MARKER_BCOLOR"] = "-23296";
            row["MARKER_FCOLOR"] = "-23296";
            row["MARKER_ENABLED"]="Y";

            markerListController.SetMarkerEtyByRow(ref marker, row);

            row["MARKER_ENABLED"]="N";
            markerListController.SetMarkerEtyByRow(ref marker, row);
        }

        [Test]
        public void TestAddMarkerToList_ModifyMarker_DeleteByMarkerName_ConvertDataTableToList_EditMarker()
        {
            string tempMarkerName = "tempName";

            EtyMarker marker = new EtyMarker();
            marker.MarkerName = tempMarkerName;
            markerListController.AddMarkerToList(marker);


            marker.MarkerEnabled = true;
            markerListController.ModifyMarker(tempMarkerName, marker);
            marker.MarkerEnabled = false;
            markerListController.ModifyMarker(tempMarkerName, marker);

            markerListController.ConvertDataTableToList();
            markerListController.EditMarker(tempMarkerName);

            markerListController.DeleteByMarkerName(tempMarkerName);

        }

        [Test]
        public void TestSaveMarkerListToGroup()
        {
            markerListController.SaveMarkerListToGroup(TestConst.MARK_GROUP_NAME);
        }

        [Test]
        public void TestgetMarkerTable()
        {
            markerListController.getMarkerTable();
        }

        [Test]
        public void TestCellDoubleClick01()
        {
            DataGridViewCellEventArgs e = new DataGridViewCellEventArgs(-1,0);
            markerListController.CellDoubleClick(null, e);

            e = new DataGridViewCellEventArgs(0, 0);
            markerListController.CellDoubleClick(null, e);
            e = new DataGridViewCellEventArgs(1, 0);
            markerListController.CellDoubleClick(null, e);
        }

        [Test]
        public void TestCellDoubleClick02()
        {
            //Exception caught MessageBoxDialog.Show(,,,)
            DataGridViewCellEventArgs e = new DataGridViewCellEventArgs(1,0);
            markerListController.CellDoubleClick(null, e);
        }

        [Test]
        public void TestLoadMarker()
        {
            markerListController.LoadMarker(null, null);
        }

        [Test]
        public void TestSaveMarker01()
        {
            markerListController.SaveMarker(null, null);
        }

        [Test]
        public void TestSaveMarker02()
        {
            markerListController.SaveMarker(null, null);
        }

        [Test]
        public void TestAddMarker()
        {
            MarkerListModel model = new MarkerListModel();
            markerListController.SetMarkerTable(model.GetMarkerListByGrp(TestConst.MARK_GROUP_NAME));
            markerListController.SetGrpName(TestConst.MARK_GROUP_NAME);
            markerListController.AddMarker(null, null);
        }

        [Test]
        public void TestSetMarkerTable()
        {
            List<EtyMarker> list = new List<EtyMarker>();
            list.Add(new EtyMarker());
            markerListController.SetMarkerTable(list);
        }

        [Test]
        public void TestFormClose()
        {
            markerListController.FormClose(null, null);
        }



    }
}
