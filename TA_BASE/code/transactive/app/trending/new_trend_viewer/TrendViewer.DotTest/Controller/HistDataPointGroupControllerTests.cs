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
    public class HistDataPointGroupControllerTests
    {

        private HistDataPointGroupController histDataPointGroupController;
        
        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.LoadConfigHistDataPoint, TestConst.TEST_VIEW_ID);

            histDataPointGroupController = (HistDataPointGroupController)(view.getController());

            histDataPointGroupController.SetFormType(FormType.Load);

            histDataPointGroupController.InitHistDPGrp();
            ((HistDataPointGroup)(view)).SetFormType(FormType.Load);
            ((HistDataPointGroup)view).FillConfigNameBox(histDataPointGroupController.GetHistDataPointGrpNames());
            ((HistDataPointGroup)view).SetCurrentGrpName(TestConst.HIST_GROUP_NAME);
            DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
            SimpleDatabase.GetInstance().OpenConnection();
        }

        [Test]
        public void TestSetFormType_GetFormType()
        {
            histDataPointGroupController.SetFormType(histDataPointGroupController.GetFormType());
        }

        [Test]
        public void TestNotifyObserver()
        {
            histDataPointGroupController.NotifyObserver(null);
        }

        [Test]
        public void TestGetHistDataPointGrpNames()
        {
            histDataPointGroupController.GetHistDataPointGrpNames();
        }

        [Test]
        public void TestLoadHistDataPointByGrpName_SaveHistDataPointToGrpName_01()
        {
//             Accessor ctlAccessor = ReflectionAccessor.Wrap(histDataPointGroupController);
//             HistDataPointGroup frm = (HistDataPointGroup)ctlAccessor.GetField("m_View");
//             frm.SetCurrentGrpName("");
//             ctlAccessor.SetField("m_View", frm);
//             //m_View.GetConfigName() == ""

            histDataPointGroupController.LoadHistDataPointByGrpName(null, null);
            histDataPointGroupController.SaveHistDataPointToGrpName(null, null);
        }

        [Test]
        public void TestLoadHistDataPointByGrpName_SaveHistDataPointToGrpName_02()
        {
//             Accessor ctlAccessor = ReflectionAccessor.Wrap(histDataPointGroupController);
//             HistDataPointGroup frm = (HistDataPointGroup)ctlAccessor.GetField("m_View");
//             frm.SetCurrentGrpName("aaa");
//             ctlAccessor.("m_View", frm);

            histDataPointGroupController.LoadHistDataPointByGrpName(null, null);
            histDataPointGroupController.SaveHistDataPointToGrpName(null, null);
        }
    }
}
