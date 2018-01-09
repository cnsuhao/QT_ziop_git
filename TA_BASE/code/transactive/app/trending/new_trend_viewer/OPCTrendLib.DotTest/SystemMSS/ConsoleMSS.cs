using System;
using Dottest.Framework;

namespace SystemMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Console")]
    public class ConsoleMSS
    {
        [MethodSpecificStubsMethod("System.Console.WriteLine(System.String)")]
        public static void WriteLineMSSS(System.String value_1)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { value_1 }, out result))
            {
                return;
            }
            else
            {
                //System.Console.WriteLine(value_1);
            }
        }

        [TestSpecificStub("System.Console.WriteLine(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static void WriteLineTSSS(System.String value_1)
        {
            System.Console.WriteLine(value_1);
        }

    }
}

