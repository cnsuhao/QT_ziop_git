using System;
using Dottest.Framework;

namespace System.ComponentModelMSS
{
    [MethodSpecificStubsAssemblyAttribute("System")]
    [MethodSpecificStubsClassAttribute("System.ComponentModel.TypeDescriptor")]
    public class TypeDescriptorMSS
    {
        [MethodSpecificStubsMethod("System.ComponentModel.TypeDescriptor.GetConverter(System.Object)")]
        public static System.ComponentModel.TypeConverter GetConverterMSSS(System.Object component)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { component }, out result))
            {
                return (System.ComponentModel.TypeConverter)result;
            }
            else
            {
                return System.ComponentModel.TypeDescriptor.GetConverter(component);
            }
        }

        [TestSpecificStub("System.ComponentModel.TypeDescriptor.GetConverter(System.Object)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ConvertHelperTests), "TestErr_ChangeType04")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ConvertHelperTests), "TestErr_ToString03")]
        public static System.ComponentModel.TypeConverter GetConverterTSSS(System.Object component)
        {
            return null;
        }

    }
}

