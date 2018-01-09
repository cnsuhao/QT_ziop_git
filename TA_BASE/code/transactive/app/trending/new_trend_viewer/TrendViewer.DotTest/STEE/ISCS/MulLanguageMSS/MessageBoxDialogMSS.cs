using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;
using TrendViewer.DotTest.Controller;
using System.Windows.Forms;

namespace STEE.ISCS.MulLanguageMSS
{
    [MethodSpecificStubsAssemblyAttribute("STEE.ISCS.MulLanguage")]
    [MethodSpecificStubsClassAttribute("STEE.ISCS.MulLanguage.MessageBoxDialog")]
    public class MessageBoxDialogMSS
    {


        [MethodSpecificStubsMethod("STEE.ISCS.MulLanguage.MessageBoxDialog.Show(System.String,System.String,System.Windows.Forms.MessageBoxButtons,System.Windows.Forms.MessageBoxIcon)")]
        public static System.Windows.Forms.DialogResult ShowMSSS(System.String text, System.String caption, System.Windows.Forms.MessageBoxButtons buttons, System.Windows.Forms.MessageBoxIcon icon)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { text, caption, buttons, icon }, out result))
            {
                return (System.Windows.Forms.DialogResult)result;
            }
            else
            {
                return STEE.ISCS.MulLanguage.MessageBoxDialog.Show(text, caption, buttons, icon);
            }
        }

        [TestSpecificStub("STEE.ISCS.MulLanguage.MessageBoxDialog.Show(System.String,System.String,System.Windows.Forms.MessageBoxButtons,System.Windows.Forms.MessageBoxIcon)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(FormulaDataTests), "TestcheckValidDataPointsToolStripMenuItem_Click02")]
        [TestSpecificStubContext(typeof(HistDataPointGroupTests), "TestokCmd_Click02")]
        [TestSpecificStubContext(typeof(DataPointListControllerTests), "TestCellDoubleClick02")]
        [TestSpecificStubContext(typeof(HistDataPointListControllerTests), "TestCellDoubleClick02")]
        [TestSpecificStubContext(typeof(MarkerListControllerTests), "TestCellDoubleClick02")]
        [TestSpecificStubContext(typeof(FormulaListTests), "TestdataPointListDataGridView_CellDoubleClick01")]
        [TestSpecificStubContext(typeof(FormulaListTests), "TestdataPointListDataGridView_CellDoubleClick02")]
        [TestSpecificStubContext(typeof(DataPointListControllerTests), "TestCellDoubleClick01")]
        [TestSpecificStubContext(typeof(HistDataPointListControllerTests), "TestCellDoubleClick01")]
        [TestSpecificStubContext(typeof(MarkerListControllerTests), "TestCellDoubleClick01")]
        public static System.Windows.Forms.DialogResult ShowTSSS(System.String text, System.String caption, System.Windows.Forms.MessageBoxButtons buttons, System.Windows.Forms.MessageBoxIcon icon)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if(name.Equals("TestdataPointListDataGridView_CellDoubleClick01")
                || name.Equals("TestCellDoubleClick01"))
            {
                return DialogResult.Yes;
            }
            else{
                Exception e = new Exception();
                throw e;
            }

        }

    }
}

