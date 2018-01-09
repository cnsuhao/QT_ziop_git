using System;
using Dottest.Framework;
using TrendViewer.DotTest;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.Application")]
    public class ApplicationMSS
    {
        [MethodSpecificStubsMethod("System.Windows.Forms.Application.SetCompatibleTextRenderingDefault(System.Boolean)")]
        public static void SetCompatibleTextRenderingDefaultMSSS(System.Boolean defaultValue)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { defaultValue }, out result))
            {
                return;
            }
            else
            {
                System.Windows.Forms.Application.SetCompatibleTextRenderingDefault(defaultValue);
            }
        }

        [TestSpecificStub("System.Windows.Forms.Application.SetCompatibleTextRenderingDefault(System.Boolean)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(ProgramTests), "TestMain01")]
        [TestSpecificStubContext(typeof(ProgramTests), "TestMain02")]
        public static void SetCompatibleTextRenderingDefaultTSSS(System.Boolean defaultValue)
        {
            return;
        }

    }
}

