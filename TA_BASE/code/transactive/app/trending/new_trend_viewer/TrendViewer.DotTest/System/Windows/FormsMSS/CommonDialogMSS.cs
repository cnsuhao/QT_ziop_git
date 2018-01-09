using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.CommonDialog")]
    public class CommonDialogMSS
    {

        [MethodSpecificStubsMethod("System.Windows.Forms.CommonDialog.ShowDialog()")]
        public static System.Windows.Forms.DialogResult ShowDialogMSS(
        System.Windows.Forms.CommonDialog target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Windows.Forms.DialogResult)result;
            }
            else
            {
                return target.ShowDialog();
            }
        }

        [TestSpecificStub("System.Windows.Forms.CommonDialog.ShowDialog()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(MarkerDataTests), "TestbackColorCmd_Click02")]
        [TestSpecificStubContext(typeof(MarkerDataTests), "TestfontColorCmd_Click02")]
        [TestSpecificStubContext(typeof(HistDataPointDataTests), "TestcolorCmd_Click02")]
        [TestSpecificStubContext(typeof(DataPointDataTests), "TestcolorCmd_Click02")]
        [TestSpecificStubContext(typeof(FormulaDataTests), "TestcolorCmd_Click02")]
        public static System.Windows.Forms.DialogResult ShowDialogTSS(
        System.Windows.Forms.CommonDialog target)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

