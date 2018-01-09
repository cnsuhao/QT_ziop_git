using System;
using Dottest.Framework;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.Control")]
    public class ControlMSS
    {
        [MethodSpecificStubsMethod("System.Windows.Forms.Control.get_Handle()")]
        public static System.IntPtr get_HandleMSS(
        System.Windows.Forms.Control target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.IntPtr)result;
            }
            else
            {
                return target.Handle;
            }
        }

        [TestSpecificStub("System.Windows.Forms.Control.get_Handle()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(STEE.ISCS.MulLanguage.DotTest.MessageBoxFormTests), "TestDisableCloseIfMultipleButtonsAndNoCancelButton01" )]
        public static System.IntPtr get_HandleTSS(
        System.Windows.Forms.Control target)
        {
            throw new Exception();
        }

    }
}

