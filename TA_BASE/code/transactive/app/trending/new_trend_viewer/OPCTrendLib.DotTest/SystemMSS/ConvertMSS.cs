using System;
using Dottest.Framework;

namespace SystemMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Convert")]
    public class ConvertMSS
    {
        [MethodSpecificStubsMethod("System.Convert.ChangeType(System.Object,System.Type)")]
        public static System.Object ChangeTypeMSSS(System.Object value_1, System.Type conversionType)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { value_1, conversionType }, out result))
            {
                return (System.Object)result;
            }
            else
            {
                return System.Convert.ChangeType(value_1, conversionType);
            }
        }

        [TestSpecificStub("System.Convert.ChangeType(System.Object,System.Type)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ConvertHelperTests), "TestErr_ToString01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ConvertHelperTests), "TestErr_ToString02")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ConvertHelperTests), "TestErr_ToString03")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ConvertHelperTests), "TestErr_ToString04")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ConvertHelperTests), "TestErr_ToString05")]
        public static System.Object ChangeTypeTSSS(System.Object value_1, System.Type conversionType)
        {
            throw new Exception(); 
        }

    }
}

