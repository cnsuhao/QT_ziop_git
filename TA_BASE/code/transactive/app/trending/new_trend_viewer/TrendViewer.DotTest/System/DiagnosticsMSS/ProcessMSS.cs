using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace System.DiagnosticsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System")]
    [MethodSpecificStubsClassAttribute("System.Diagnostics.Process")]
    public class ProcessMSS
    {
        [MethodSpecificStubsMethod("System.Diagnostics.Process.Start(System.String)")]
        public static System.Diagnostics.Process StartMSSS(System.String fileName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { fileName }, out result))
            {
                return (System.Diagnostics.Process)result;
            }
            else
            {
                return System.Diagnostics.Process.Start(fileName);
            }
        }

        [TestSpecificStub("System.Diagnostics.Process.Start(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewControllerTests), "TestDataLoggerConfigurationToolStripMenuItem_Click02")]
        public static System.Diagnostics.Process StartTSSS(System.String fileName)
        {
            //return System.Diagnostics.Process.Start(fileName);
            return null;
        }

    }
}

