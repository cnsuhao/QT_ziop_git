using System;
using Dottest.Framework;

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
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestIntervalConfigDataGridView_CellDoubleClick03")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestIntervalConfigDataGridView_CellDoubleClick04")]
        public static System.Windows.Forms.DialogResult ShowTSSS(System.String text, System.String caption, System.Windows.Forms.MessageBoxButtons buttons, System.Windows.Forms.MessageBoxIcon icon)
        {
            return System.Windows.Forms.DialogResult.Yes;
        }

    }
}

