using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using STEE.ISCS.MVC;
using TrendViewer.View;
using DAO.Trending;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class MarkerGroupControllerTests
    {

        private MarkerGroupController markerGroupController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            DAO.Trending.Common.DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
            DAO.Trending.Common.SimpleDatabase.GetInstance().OpenConnection();
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.LoadConfigMarker, TestConst.TEST_VIEW_ID);

            markerGroupController = (MarkerGroupController)(view.getController());
            markerGroupController.SetFormType(FormType.Load);
            markerGroupController.InitMarkGrp();
        }

        [Test]
        public void TestSetFormType_GetFormType()
        {
            markerGroupController.SetFormType(markerGroupController.GetFormType());
        }

        [Test]
        public void TestNotifyObserver()
        {
            markerGroupController.NotifyObserver(null);
        }

        [Test]
        public void TestGetMarkerGrpNames()
        {
            markerGroupController.GetMarkerGrpNames();
        }

        [Test]
        public void TestLoadMarkerByGrpName_SaveMarkerToGrpName_01()
        {
            markerGroupController.LoadMarkerByGrpName(null, null);
            markerGroupController.SaveMarkerToGrpName(null, null);
        }

        [Test]
        public void TestLoadMarkerByGrpName_SaveMarkerToGrpName_02()
        {
            markerGroupController.LoadMarkerByGrpName(null, null);
            markerGroupController.SaveMarkerToGrpName(null, null);
        }
    }
}
