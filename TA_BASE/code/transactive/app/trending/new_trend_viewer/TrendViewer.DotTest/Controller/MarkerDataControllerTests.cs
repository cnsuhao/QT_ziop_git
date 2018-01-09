using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using STEE.ISCS.MVC;
using System.Data;
using System.Collections.Generic;
using TrendViewer.View;
using Entity.Trending;
using DAO.Trending;
using DAO.Trending.Common;
using System.Windows.Forms;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class MarkerDataControllerTests
    {
        private MarkerDataController markerDataController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            if (DBConnectionStrings.GetInstance().GetConnectionStrings().Count < 1)
            {
                DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
                SimpleDatabase.GetInstance().OpenConnection();
            }
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.AddMarkerData, TestConst.TEST_VIEW_ID);

            markerDataController = (MarkerDataController)(view.getController());
            //((MarkerData)view).SetFormType(FormType.Add);
            markerDataController.SetFormType(FormType.Add);

            List<string> markerNameList = new List<string>();

            markerNameList.Add("makerName1");
            markerNameList.Add("makerName2");

            markerDataController.InitMarkerData(markerNameList);

        }

        [Test]
        public void TestSetFormType_GetFormType()
        {
            markerDataController.SetFormType(markerDataController.GetFormType());
        }

        [Test]
        public void TestGetMarker()
        {
            markerDataController.GetMarker();
        }

        [Test]
        public void TestNotifyObserver()
        {
            markerDataController.NotifyObserver(null);

        }

        [Test]
        public void TestInitMarkerData01()
        {
            List<string> markerNameList = new List<string>();
            markerNameList.Add("makerName1");
            markerNameList.Add("makerName2");

            markerDataController.SetFormType(FormType.Edit);
            markerDataController.InitMarkerData(markerNameList);

            markerDataController.SetFormType(FormType.Add);
            markerDataController.InitMarkerData(markerNameList);
        }

        [Test]
        public void TestInitMarkerData02()
        {
            DataTable table = new DataTable("TRENDVIEWER_MARKER");

            table.Columns.Add("CONFIG_NAME", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_NAME", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_WIDTH", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_VALUE", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_ENABLED", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_BCOLOR", System.Type.GetType("System.String"));
            table.Columns.Add("MARKER_FCOLOR", System.Type.GetType("System.String"));

            DataRow row = table.NewRow();
            row["MARKER_WIDTH"] = 2.2;
            row["MARKER_VALUE"] = 5;
            row["MARKER_BCOLOR"] = "-16777216";
            row["MARKER_FCOLOR"] = "-16777216";

            List<string> list = new List<string>();
            list.Add("makerName1");
            list.Add("makerName2");
            markerDataController.InitMarkerData(row, list);
        }

        [Test]
        public void TestMarkerNameValid()
        {
            markerDataController.MarkerNameValid("makerName1");
            markerDataController.MarkerNameValid("makerName4");
        }

        [Test]
        public void TestAddMarkerData_ModifyMarker_01()
        {
            
            Accessor ctlAccessor = ReflectionAccessor.Wrap(markerDataController);
            List<string> list = (List<string>)ctlAccessor.GetField("m_otherMarkerNames");
            list.Clear();
            list.Add("makerName1");
            list.Add("makerName2");
            ctlAccessor.SetField("m_otherMarkerNames", list);

            //m_View.GetMarkerName() == ""
            TrendViewer.View.MarkerData frm = (TrendViewer.View.MarkerData)ctlAccessor.GetField("m_View");

            Accessor frmAccessor = ReflectionAccessor.Wrap(frm);
            TextBox tb = (TextBox)frmAccessor.GetField("markerNameBox");
            tb.Text = "";
            markerDataController.AddMarkerData(null, null);
            markerDataController.ModifyMarker(null, null);

            //m_View.GetMarkerName() != "", MarkerNameValid(markerName)
            tb.Text = "notEmptyName";
            markerDataController.AddMarkerData(null, null);
            markerDataController.ModifyMarker(null, null);

            //m_View.GetMarkerName() != "", !MarkerNameValid(markerName)
            tb.Text = "makerName1";
            markerDataController.AddMarkerData(null, null);
            markerDataController.ModifyMarker(null, null);
        }

    }
}
