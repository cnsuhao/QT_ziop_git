using System;
using Dottest.Framework;

namespace System.ComponentModelMSS
{
    [MethodSpecificStubsAssemblyAttribute("System")]
    [MethodSpecificStubsClassAttribute("System.ComponentModel.TypeConverter")]
    public class TypeConverterMSS
    {
        [MethodSpecificStubsMethod("System.ComponentModel.TypeConverter.CanConvertTo(System.Type)")]
        public static System.Boolean CanConvertToMSS(
        System.ComponentModel.TypeConverter target,
        System.Type destinationType)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, destinationType }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.CanConvertTo(destinationType);
            }
        }

        [TestSpecificStub("System.ComponentModel.TypeConverter.CanConvertTo(System.Type)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ConvertHelperTests), "TestErr_ToString04")]
        public static System.Boolean CanConvertToTSS(
        System.ComponentModel.TypeConverter target,
        System.Type destinationType)
        {
            return false;
        }

    }
}

