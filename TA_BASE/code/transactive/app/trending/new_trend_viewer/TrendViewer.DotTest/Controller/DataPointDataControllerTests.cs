using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using STEE.ISCS.MVC;
using TrendViewer.Model;
using TrendViewer.View;
using Entity.Trending;
using System.Collections.Generic;
using System.Data;
using System.Data;
using DAO.Trending;
using DAO.Trending.Common;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class DataPointDataControllerTests
    {
        private DataPointDataController dataPointDataController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.AddDataPointData, TestConst.TEST_VIEW_ID);
            dataPointDataController = (DataPointDataController)(view.getController());
            dataPointDataController.AttachCallBack(null, view);
            if (DBConnectionStrings.GetInstance().GetConnectionStrings().Count<1)
            {
                DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
                SimpleDatabase.GetInstance().OpenConnection();
            }

        }



        [Test]
        public void TestSet_And_GetFormType()
        {
           dataPointDataController.SetFormType(FormType.Add);
           FormType ret = dataPointDataController.GetFormType();


           dataPointDataController.SetFormType(FormType.Edit);
           ret = dataPointDataController.GetFormType();

        }


 
        [Test]
        public void TestGetDataPoint()
        {
            dataPointDataController.GetDataPoint();
        }


        [Test]
        public void TestNotifyObserver()
        {
            NotifyObject notifyObj = new NotifyObject();
            notifyObj.Name = "dldl";
            dataPointDataController.NotifyObserver(notifyObj);

            notifyObj.Name = TestConst.TEST_VIEW_ID;
            notifyObj.Type = DataNotificaitonConst.DPSelected;
            EtyEntity ety = new EtyEntity();
            ety.Name = "bb";
            notifyObj.Body = ety;
            dataPointDataController.NotifyObserver(notifyObj);
        }

        [Test]
        public void TestInit1()
        {
           List<string>  dpNameList = new List<string>();
           dpNameList.Add("Name1");
           dpNameList.Add("Name2");
           dataPointDataController.SetFormType(FormType.Add);
           dataPointDataController.Init(dpNameList);
           dataPointDataController.SetFormType(FormType.Edit);
           dataPointDataController.Init(dpNameList);
        }

        [Test]
        public void TestInit2()
         {

             System.Data.DataTable dataPointTable = new System.Data.DataTable("TRENDVIEWER_CONFIG");

            dataPointTable.Columns.Add("DATA_PT_HOST", System.Type.GetType("System.String"));
            dataPointTable.Columns.Add("DATA_PT_NAME", System.Type.GetType("System.String"));
            dataPointTable.Columns.Add("DATA_PT_TYPE", System.Type.GetType("System.String"));
            dataPointTable.Columns.Add("DATA_PT_COLOR", System.Type.GetType("System.String"));
            dataPointTable.Columns.Add("DATA_PT_SERVER", System.Type.GetType("System.String"));
            dataPointTable.Columns.Add("DATA_PT_ENABLED", System.Type.GetType("System.String"));
            dataPointTable.Columns.Add("DATA_PT_LBL_ENABLED", System.Type.GetType("System.String"));
            dataPointTable.Columns.Add("DATA_PT_LBL_NAME", System.Type.GetType("System.String"));
            dataPointTable.Columns.Add("DATA_PT_VISIBLE", System.Type.GetType("System.String"));

            System.Data.DataRow row = dataPointTable.NewRow();

            row["DATA_PT_NAME"] = "test_name";
            row["DATA_PT_HOST"] = "test_host";
            row["DATA_PT_TYPE"] = "Line";
            row["DATA_PT_ENABLED"]= "Y";

            List<string>  dpNameList = new List<string>();
            dpNameList.Add("Name1");
            dpNameList.Add("Name2");

            dataPointDataController.Init(row, dpNameList);
        }

        [Test]
        public void TestDataPointNameValid()
        {
            List<string>  dpNameList = new List<string>();
            dpNameList.Add("Name1");
            dpNameList.Add("Name2");
            dataPointDataController.SetFormType(FormType.Add);
            dataPointDataController.Init(dpNameList);
            dataPointDataController.DataPointNameValid("a");
            dataPointDataController.DataPointNameValid("Name2");
        }

        [Test]
        public void TestAddDataPointData_ModifyDataPoint_01()
        {
            dataPointDataController.AddDataPointData(null, null);
            dataPointDataController.ModifyDataPoint(null, null);
        }

        [Test]
        public void TestAddDataPointData_ModifyDataPoint_02()
        {
            dataPointDataController.AddDataPointData(null, null);
            dataPointDataController.ModifyDataPoint(null, null);
        }

        [Test]
        public void TestBrowseDP()
        {
            dataPointDataController.BrowseDP(null, null);
        }



    }
}