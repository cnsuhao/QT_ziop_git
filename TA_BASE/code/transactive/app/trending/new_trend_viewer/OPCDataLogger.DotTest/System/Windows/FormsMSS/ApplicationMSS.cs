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
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.ProgramTests), "TestMain01")]
        public static void RunTSSS(System.Windows.Forms.Form mainForm)
        {
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
                return;
            }
        }

        [TestSpecificStub("System.Windows.Forms.Application.SetCompatibleTextRenderingDefault(System.Boolean)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static void SetCompatibleTextRenderingDefaultTSSS(System.Boolean defaultValue)
        {
            return;
        }

        [MethodSpecificStubsMethod("System.Windows.Forms.Application.Exit()")]
        public static void ExitMSSS()
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { }, out result))
            {
                return;
            }
            else
            {
                return;
            }
        }

        [TestSpecificStub("System.Windows.Forms.Application.Exit()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static void ExitTSSS()
        {
            return;
        }

        /*[MethodSpecificStubsMethod("System.Windows.Forms.Application.get_OpenForms()")]
        public static System.Windows.Forms.FormCollection get_OpenFormsMSSS()
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { }, out result))
            {
                return (System.Windows.Forms.FormCollection)result;
            }
            else
            {
                return System.Windows.Forms.Application.OpenForms;
            }
        }

        [TestSpecificStub("System.Windows.Forms.Application.get_OpenForms()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataLoggerTests), "TestcloseApplicationToolStripMenuItem_Click01")]
        public static System.Windows.Forms.FormCollection get_OpenFormsTSSS()
        {
            try
            {
                System.Windows.Forms.Form frm = new System.Windows.Forms.Form();
                System.Collections.ArrayList ff = new System.Collections.ArrayList();
                ff.Add(frm);
                System.Collections.ICollection cc = ff;
                return (System.Windows.Forms.FormCollection)cc;
            }
            catch (System.Exception ex)
            {
            	int i=0;
            }

            return null;

        }
         * */

    }
}

