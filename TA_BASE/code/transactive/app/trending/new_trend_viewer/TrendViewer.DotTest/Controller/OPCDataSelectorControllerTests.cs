using Dottest.Framework;
using NUnit.Framework;
using TrendViewer.Controller;
using System.Windows.Forms;
using DAO.Trending;
using STEE.ISCS.MVC;
using TrendViewer.View;
using DAO.Trending.Common;

namespace TrendViewer.DotTest.Controller
{
    [TestFixture()]
    public class OPCDataSelectorControllerTests
    {
        private OPCDataSelectorController oPCDataSelectorController;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            DBConnectionStrings.GetInstance().AddConnectionString(TestConst.CONNECTION_STR);
            SimpleDatabase.GetInstance().OpenConnection();
            ViewManager.GetInstance().RegisterViewFactory(new TrendingViewFactory());
            IView view = ViewManager.GetInstance().GetView(TrendViewConst.OPCDataSelector, TestConst.TEST_VIEW_ID);

            oPCDataSelectorController = (OPCDataSelectorController)(view.getController());
            //oPCDataSelectorController.InitDataNodeList();
        }

        [Test]
        public void TestInitDataNodeList01()
        {
            //Exception caught
            oPCDataSelectorController.InitDataNodeList();
        }

        [Test]
        public void TestInitDataNodeList02()
        {
            //Exception caught
            oPCDataSelectorController.InitDataNodeList();
        }

        [Test]
        public void TeseopcItemDataGridView_CellDoubleClick01()
        {
            //stub: send() do nothing

            DataGridViewCellEventArgs e = new DataGridViewCellEventArgs(1,1);
           // e.RowIndex = 1;
            oPCDataSelectorController.opcItemDataGridView_CellDoubleClick(null, e);
             e = new DataGridViewCellEventArgs(-1, -1);
           // e.RowIndex = -1;
            oPCDataSelectorController.opcItemDataGridView_CellDoubleClick(null, e);
        }

        [Test]
        public void TeseopcItemDataGridView_CellDoubleClick02()
        {
            //stub:getDPInGridView return data
  
            DataGridViewCellEventArgs e = new DataGridViewCellEventArgs(1, 1);
            oPCDataSelectorController.opcItemDataGridView_CellDoubleClick(null, e);
        }

        [Test]
        public void TestgetDataNodeMap()
        {
            oPCDataSelectorController.getDataNodeMap();
        }

        [Test]
        public void TestAfterSelect01()
        {
            oPCDataSelectorController.AfterSelect(null, null);
            // TreeNode selectedNode = m_View.getSelectedNode(); 
            //if ( selectedNode.Nodes.Count >= 1)
        }
        [Test]
        public void TestAfterSelect02()
        {
            oPCDataSelectorController.AfterSelect(null, null);
            // TreeNode selectedNode = m_View.getSelectedNode(); 
            //if ( selectedNode.Nodes.Count < 1)
        }

        [Test]
        public void TestAfterSelect03()
        {
            //exception:
            //m_View == null;
            
            

            oPCDataSelectorController.AfterSelect(null, null);
            
        }
    }
}
