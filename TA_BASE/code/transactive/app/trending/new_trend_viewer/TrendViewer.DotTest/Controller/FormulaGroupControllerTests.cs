using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using TrendViewer.View;
using STEE.ISCS.MVC;
using DAO.Trending;
using DAO.Trending.Common;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class FormulaGroupControllerTests
    {
        private FormulaGroupController formulaGroupController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            if (DBConnectionStrings.GetInstance().GetConnectionStrings().Count < 1)
            {
                DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
                SimpleDatabase.GetInstance().OpenConnection();
            }
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.LoadConfigFormula, TestConst.TEST_VIEW_ID);

            formulaGroupController = (FormulaGroupController)(view.getController());

            formulaGroupController.SetFormType(FormType.Load);

            formulaGroupController.InitFormulaGrp();


        }

        [Test]
        public void TestGetFormType()
        {
            formulaGroupController.GetFormType();
        }

        [Test]
        public void TestNotifyObserver()
        {
            formulaGroupController.NotifyObserver(null);
        }

        [Test]
        public void TestGetFormulaGrpNames()
        {
            formulaGroupController.GetFormulaGrpNames();
        }

        [Test]
        public void TestLoadFormulaByGrpName_SaveFormulaToGrpName_01()
        {
            // grpName = m_View.GetConfigName(); grpName is empty
            formulaGroupController.LoadFormulaByGrpName(null, null);
            formulaGroupController.SaveFormulaToGrpName(null, null);
        }

        [Test]
        public void TestLoadFormulaByGrpName_SaveFormulaToGrpName_02()
        {
            // grpName = m_View.GetConfigName(); grpName is not empty
            formulaGroupController.LoadFormulaByGrpName(null, null);
            formulaGroupController.SaveFormulaToGrpName(null, null);
        }
    }
}
