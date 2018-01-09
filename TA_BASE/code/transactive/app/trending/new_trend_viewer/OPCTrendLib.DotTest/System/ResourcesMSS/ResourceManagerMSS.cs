using System;
using Dottest.Framework;

namespace System.ResourcesMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Resources.ResourceManager")]
    public class ResourceManagerMSS
    {
        [MethodSpecificStubsMethod("System.Resources.ResourceManager.GetString(System.String,System.Globalization.CultureInfo)")]
        public static System.String GetStringMSS(
        System.Resources.ResourceManager target,
        System.String name, System.Globalization.CultureInfo culture)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, name, culture }, out result))
            {
                return (System.String)result;
            }
            else
            {
                //return target.GetString(name, culture);
                return "Testing";
            }
        }

        [TestSpecificStub("System.Resources.ResourceManager.GetString(System.String,System.Globalization.CultureInfo)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static System.String GetStringTSS(
        System.Resources.ResourceManager target,
        System.String name, System.Globalization.CultureInfo culture)
        {
            return target.GetString(name, culture);
        }

    }
}

