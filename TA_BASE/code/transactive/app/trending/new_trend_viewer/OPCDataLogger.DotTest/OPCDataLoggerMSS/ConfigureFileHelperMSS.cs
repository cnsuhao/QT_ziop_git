using System;
using Dottest.Framework;

namespace OPCDataLoggerMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCDataLogger")]
    [MethodSpecificStubsClassAttribute("OPCDataLogger.ConfigureFileHelper")]
    public class ConfigureFileHelperMSS
    {
        [MethodSpecificStubsMethod("OPCDataLogger.ConfigureFileHelper.get_TimeIntevalMS()")]
        public static System.String get_TimeIntevalMSMSS(
            /*OPCDataLogger.ConfigureFileHelper*/ object target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.String)result;
            }
            else
            {
                return (System.String)Helper.CallMethod(target, "get_TimeIntevalMS", new object[] { });
            }
        }

        [TestSpecificStub("OPCDataLogger.ConfigureFileHelper.get_TimeIntevalMS()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataPointManagerTests), "TestGetInstance01")]
        public static System.String get_TimeIntevalMSTSS(
            /*OPCDataLogger.ConfigureFileHelper*/ object target)
        {
            return "1000";
        }

    }
}

