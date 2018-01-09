using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using STEE.ISCS.MVC;
using Entity.Trending;
using System.Data;
using TrendViewer.View;
using TrendViewer.Model;
using System.Collections.Generic;
using DAO.Trending;
using DAO.Trending.Common;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class FormulaListControllerTests
    {
        private FormulaListController formulaListController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            if (DBConnectionStrings.GetInstance().GetConnectionStrings().Count < 1)
            {
                DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
                SimpleDatabase.GetInstance().OpenConnection();
            }
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.FormulaList, TestConst.TEST_VIEW_ID);
            formulaListController = (FormulaListController)(view.getController());
            formulaListController.initFormulaTable();

            FormulaListModel model = new FormulaListModel();
            formulaListController.SetFormulaTable(model.GetFormulaListByGrp(TestConst.FORMULA_GROUP_NAME));
            formulaListController.SetGrpName(TestConst.FORMULA_GROUP_NAME);

        }

        [Test]
        public void TestNotifyObserver()
        {
            NotifyObject obj = new NotifyObject();
            obj.Name = "otherName";

            formulaListController.NotifyObserver(obj);

            obj.Name = TestConst.TEST_VIEW_ID;
            obj.Type = DataNotificaitonConst.SelectFormulaGroupToLoad;
            obj.Body = TestConst.FORMULA_GROUP_NAME;
            formulaListController.NotifyObserver(obj);

            obj.Type = DataNotificaitonConst.SaveFormulaToGroup;
            obj.Body = TestConst.FORMULA_GROUP_NAME;
            formulaListController.NotifyObserver(obj);

            obj.Type = DataNotificaitonConst.AddFormula;
            obj.Body = new EtyFormula();
            formulaListController.NotifyObserver(obj);

            obj.Type = DataNotificaitonConst.ModifyFormula;
            KeyValuePair<string, EtyFormula> pair = new KeyValuePair<string, EtyFormula>("", new EtyFormula());
            obj.Body = pair;
            formulaListController.NotifyObserver(obj);
        }

        [Test]
        public void TestModifyFormula()
        {
            string formulaName = TestConst.FORMULA_LBL_NAME;
            EtyFormula data = new EtyFormula();
            formulaListController.ModifyFormula(formulaName, data);
        }

        [Test]
        public void TestSetFormulaEtyByRow()
        {
            EtyFormula data = new EtyFormula();
            Accessor ctlAccessor = ReflectionAccessor.Wrap(formulaListController);
            DataTable table = (DataTable)ctlAccessor.GetField("m_FormulaTable");
            DataRow row = table.NewRow();
            formulaListController.SetFormulaEtyByRow(ref data, row);
        }

        [Test]
        public void TestSaveFormulaListToGroup()
        {
            formulaListController.SaveFormulaListToGroup(TestConst.FORMULA_GROUP_NAME);
        }

        [Test]
        public void TestLoadFormula()
        {
            formulaListController.LoadFormula(null, null);
        }

        [Test]
        public void TestAddFormulaToList_DeleteByFormulaLblName_EditFormula()
        {
            EtyFormula ety = new EtyFormula();
            ety.DPLblName = "tempName";
            formulaListController.AddFormulaToList(ety);
            formulaListController.EditFormula("tempName");
            formulaListController.DeleteByFormulaLblName("tempName");
        }

        [Test]
        public void TestAddFormula()
        {
            formulaListController.AddFormula(null, null);
        }

        [Test]
        public void TestSaveFormula01()
        {
            formulaListController.SaveFormula(null, null);
        }

        [Test]
        public void TestSaveFormula02()
        {
            formulaListController.SaveFormula(null, null);
        }

        [Test]
        public void TestFormClose()
        {
            formulaListController.FormClose(null, null);
        }

        [Test]
        public void TestConvertDataTableToList()
        {
            formulaListController.ConvertDataTableToList();
        }
    }
}
