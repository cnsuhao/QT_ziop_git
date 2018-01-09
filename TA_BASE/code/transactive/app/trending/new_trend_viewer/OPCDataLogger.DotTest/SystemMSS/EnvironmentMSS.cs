using System;
using Dottest.Framework;

namespace SystemMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Environment")]
    public class EnvironmentMSS
    {
        [MethodSpecificStubsMethod("System.Environment.Exit(System.Int32)")]
        public static void ExitMSSS(System.Int32 exitCode)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { exitCode }, out result))
            {
                return;
            }
            else
            {
                return;
            }
        }

        [TestSpecificStub("System.Environment.Exit(System.Int32)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static void ExitTSSS(System.Int32 exitCode)
        {
            return;
        }

    }
}

