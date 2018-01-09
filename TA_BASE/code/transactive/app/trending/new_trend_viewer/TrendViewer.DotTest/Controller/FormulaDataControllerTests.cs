using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using TrendViewer.View;
using STEE.ISCS.MVC;
using System.Collections.Generic;
using DAO.Trending;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class FormulaDataControllerTests
    {
        private FormulaDataController formulaDataController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.EditFormulaData, TestConst.TEST_VIEW_ID);
            ((FormulaData)view).SetFormType(FormType.Edit);
            formulaDataController = (FormulaDataController)(view.getController());

            formulaDataController.SetFormType(FormType.Edit);

            //formulaDataController.InitFormulaData(dataRow, otherFormulaNameList);

            //view.ShowView(true);
            DAO.Trending.Common.DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
            DAO.Trending.Common.SimpleDatabase.GetInstance().OpenConnection();
        }

        [Test]
        public void TestGetFormType()
        {
            formulaDataController.GetFormType();
        }

        [Test]
        public void TestGetFormula()
        {
            formulaDataController.GetFormula();
        }

        [Test]
        public void TestNotifyObserver()
        {
            formulaDataController.NotifyObserver(null);
        }

        [Test]
        public void TestInitFormulaData1()
        {
            formulaDataController.SetFormType(FormType.Add);
            List<string> list = new List<string>();
            list.Add("formula1");
            list.Add("formula2");
            formulaDataController.InitFormulaData(list);

            formulaDataController.SetFormType(FormType.Edit);
            formulaDataController.InitFormulaData(list);
        }


        [Test]
        public void TestFormulaNameValid()
        {
            formulaDataController.SetFormType(FormType.Add);
            List<string> list = new List<string>();
            list.Add("formula1");
            list.Add("formula2");
            formulaDataController.InitFormulaData(list);

            formulaDataController.FormulaNameValid("formula1");
            formulaDataController.FormulaNameValid("formula3");
        }

        [Test]
        public void TestAddFormulaData_ModifyFormulaData_01()
        {
            //formulaName is empty
            formulaDataController.AddFormulaData(null, null);
            formulaDataController.ModifyFormula(null, null);
        }

        [Test]
        public void TestAddFormulaData_ModifyFormulaData_02()
        {
            //formulaName is not empty, FormulaNameValid(formulaName) is false
            formulaDataController.AddFormulaData(null, null);
            formulaDataController.ModifyFormula(null, null);
        }

        [Test]
        public void TestAddFormulaData_ModifyFormulaData_03()
        {
            //formulaName is not empty, FormulaNameValid(formulaName) is true
            formulaDataController.AddFormulaData(null, null);
            formulaDataController.ModifyFormula(null, null);
        }
    }
}
