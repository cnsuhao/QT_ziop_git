using System;
using Dottest.Framework;

namespace STEE.ISCS.MulLanguageMSS
{
    [MethodSpecificStubsAssemblyAttribute("STEE.ISCS.MulLanguage")]
    [MethodSpecificStubsClassAttribute("STEE.ISCS.MulLanguage.MessageBoxForm")]
    public class MessageBoxFormMSS
    {
        [MethodSpecificStubsMethod("STEE.ISCS.MulLanguage.MessageBoxForm.DisableCloseIfMultipleButtonsAndNoCancelButton()")]
        public static void DisableCloseIfMultipleButtonsAndNoCancelButtonMSS(
            /*STEE.ISCS.MulLanguage.MessageBoxForm*/ object target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                Helper.CallMethod(target, "DisableCloseIfMultipleButtonsAndNoCancelButton", new object[] { });
            }
        }

        [TestSpecificStub("STEE.ISCS.MulLanguage.MessageBoxForm.DisableCloseIfMultipleButtonsAndNoCancelButton()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(STEE.ISCS.MulLanguage.DotTest.MessageBoxFormTests), "TestBuildMessageBox04")]
        public static void DisableCloseIfMultipleButtonsAndNoCancelButtonTSS(
            /*STEE.ISCS.MulLanguage.MessageBoxForm*/ object target)
        {
            throw new Exception();
        }

    }
}

