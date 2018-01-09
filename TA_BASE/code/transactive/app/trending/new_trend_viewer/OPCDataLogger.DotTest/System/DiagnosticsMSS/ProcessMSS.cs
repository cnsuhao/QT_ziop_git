using System;
using Dottest.Framework;

namespace System.DiagnosticsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System")]
    [MethodSpecificStubsClassAttribute("System.Diagnostics.Process")]
    public class ProcessMSS
    {
        [MethodSpecificStubsMethod("System.Diagnostics.Process.GetProcessesByName(System.String)")]
        public static System.Diagnostics.Process[] GetProcessesByNameMSSS(System.String processName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { processName }, out result))
            {
                return (System.Diagnostics.Process[])result;
            }
            else
            {
                return System.Diagnostics.Process.GetProcessesByName(processName);
            }
        }

        [TestSpecificStub("System.Diagnostics.Process.GetProcessesByName(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataLoggerTests), "TestOPCDataLoggerConstructor02")]
        public static System.Diagnostics.Process[] GetProcessesByNameTSSS(System.String processName)
        {
            //get current Suite method name
            System.Diagnostics.Process[] processArray = new System.Diagnostics.Process[2];
            System.Diagnostics.Process pp = new System.Diagnostics.Process();
            System.Diagnostics.Process pp2 = new System.Diagnostics.Process();
            processArray[0] = pp;
            processArray[1] = pp2;
            return processArray;
        }

        [MethodSpecificStubsMethod("System.Diagnostics.Process.Kill()")]
        public static void KillMSS(
        System.Diagnostics.Process target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                return;
            }
        }

        [TestSpecificStub("System.Diagnostics.Process.Kill()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataLoggerTests), "TestOPCDataLoggerConstructor02")]
        public static void KillTSS(
        System.Diagnostics.Process target)
        {
            return;
        }

    }
}

