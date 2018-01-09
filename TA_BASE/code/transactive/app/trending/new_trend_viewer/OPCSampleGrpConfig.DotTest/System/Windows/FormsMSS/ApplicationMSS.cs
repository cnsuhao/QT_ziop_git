using System;
using Dottest.Framework;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.Application")]
    public class ApplicationMSS
    {

        [MethodSpecificStubsMethod("System.Windows.Forms.Application.Run(System.Windows.Forms.Form)")]
        public static void RunMSSS(System.Windows.Forms.Form mainForm)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { mainForm }, out result))
            {
                return;
            }
            else
            {
                System.Windows.Forms.Application.Run(mainForm);
            }
        }

        [TestSpecificStub("System.Windows.Forms.Application.Run(System.Windows.Forms.Form)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.ProgramTest), "TestMain01")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.ProgramTest), "TestMain02")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.ProgramTest), "TestException_Main03")]
        public static void RunTSSS(System.Windows.Forms.Form mainForm)
        {
            //System.Windows.Forms.Application.Run(mainForm);
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (name.Equals("TestException_Main03"))
            {
                throw new Exception();
            }
            return;
        }

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
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.ProgramTest), "TestMain01")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.ProgramTest), "TestMain02")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.ProgramTest), "TestException_Main03")]
        public static void SetCompatibleTextRenderingDefaultTSSS(System.Boolean defaultValue)
        {
            //System.Windows.Forms.Application.SetCompatibleTextRenderingDefault(defaultValue);
            return;
        }

    }
}

