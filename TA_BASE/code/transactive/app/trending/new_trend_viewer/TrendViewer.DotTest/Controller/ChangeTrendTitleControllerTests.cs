using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using STEE.ISCS.MVC;
using TrendViewer.View;
using DAO.Trending;
using DAO.Trending.Common;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class ChangeTrendTitleControllerTests
    {
        private ChangeTrendTitleController changeTrendTitleController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.ChangeTrendTitle, "Test_ViewID");
            changeTrendTitleController = (ChangeTrendTitleController)(view.getController());
            string[] titles = new string[3];
            titles[0] = "";
            titles[1] = "";
            titles[2] = "";
            changeTrendTitleController.InitData(titles);
            changeTrendTitleController.AttachCallBack(null, view);
            DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
            SimpleDatabase.GetInstance().OpenConnection();
        }


        [Test]
        public void TestNotifyObserver()
        {
            NotifyObject obj = new NotifyObject();
            obj.Name = "aa";
            changeTrendTitleController.NotifyObserver(obj);
        }


        [Test]
        public void TestGetTitles()
        {
            changeTrendTitleController.GetTitles();
        }

        [Test]
        public void TestModifyTitles()
        {
            changeTrendTitleController.ModifyTitles(null, null);
        }
    }
}
