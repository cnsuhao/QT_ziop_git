using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using STEE.ISCS.MVC;
using TrendViewer.View;
using DAO.Trending;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class DataPointGroupControllerTests
    {
        private DataPointGroupController dataPointGroupController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.LoadConfigDataPoint,TestConst.TEST_VIEW_ID);

            dataPointGroupController = (DataPointGroupController)(view.getController());

            dataPointGroupController.SetFormType(FormType.Load);

            dataPointGroupController.InitDPGrp();
            DAO.Trending.Common.DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
            DAO.Trending.Common.SimpleDatabase.GetInstance().OpenConnection();
        }

        [Test]
        public void TestNotifyObserver()
        {
            dataPointGroupController.NotifyObserver(null);
        }

        [Test]
        public void TestInitDPGrp01()
        {
            Accessor ctlAccessor = ReflectionAccessor.Wrap(dataPointGroupController);
            ctlAccessor.SetField("m_DataPointCfgNameList", null);
            
            //test : catch exception
            dataPointGroupController.InitDPGrp();
        }

        [Test]
        public void TestInitDPGrp02()
        {
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.LoadConfigDataPoint, TestConst.TEST_VIEW_ID);

            dataPointGroupController = (DataPointGroupController)(view.getController());
            dataPointGroupController.SetFormType(FormType.Load);
            dataPointGroupController.InitDPGrp();
        }

        [Test]
        public void TestSetFormType_GetFormType()
        {
            dataPointGroupController.SetFormType(FormType.Load);
            dataPointGroupController.GetFormType();
        }

        [Test]
        public void TestGetDataPointGrpNames()
        {
            dataPointGroupController.GetDataPointGrpNames();
            dataPointGroupController.SaveDataPointToGrpName(null, null);
        }

        [Test]
        public void TestLoadDataPointByGrpName_SaveDataPointToGrpName_01()
        {
            dataPointGroupController.LoadDataPointByGrpName(null, null);
        }

        [Test]
        public void TestLoadDataPointByGrpName_SaveDataPointToGrpName_02()
        {
            dataPointGroupController.LoadDataPointByGrpName(null, null);
            dataPointGroupController.SaveDataPointToGrpName(null, null);
        }
    }
}
