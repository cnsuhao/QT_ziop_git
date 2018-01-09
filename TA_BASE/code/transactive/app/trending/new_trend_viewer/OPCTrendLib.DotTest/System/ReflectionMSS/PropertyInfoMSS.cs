using System;
using Dottest.Framework;

namespace System.ReflectionMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Reflection.PropertyInfo")]
    public class PropertyInfoMSS
    {
        [MethodSpecificStubsMethod("System.Reflection.PropertyInfo.GetValue(System.Object,System.Object[])")]
        public static System.Object GetValueMSS(
        System.Reflection.PropertyInfo target,
        System.Object obj, System.Object[] index)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, obj, index }, out result))
            {
                return (System.Object)result;
            }
            else
            {
                return target.GetValue(obj, index);
            }
        }

        [TestSpecificStub("System.Reflection.PropertyInfo.GetValue(System.Object,System.Object[])")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.IndexerOpTests), "TestEval04")]
        public static System.Object GetValueTSS(
        System.Reflection.PropertyInfo target,
        System.Object obj, System.Object[] index)
        {
            return 2;
        }

    }
}

