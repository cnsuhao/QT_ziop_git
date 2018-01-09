using System;
using Dottest.Framework;

namespace OPCDataLoggerMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCDataLogger")]
    [MethodSpecificStubsClassAttribute("OPCDataLogger.CheckConfigVars")]
    public class CheckConfigVarsMSS
    {
        [MethodSpecificStubsMethod("OPCDataLogger.CheckConfigVars.GetOPCDTSmplFlagValue()")]
        public static System.String GetOPCDTSmplFlagValueMSS(
        OPCDataLogger.CheckConfigVars target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.String)result;
            }
            else
            {
                return target.GetOPCDTSmplFlagValue();
            }
        }

        [TestSpecificStub("OPCDataLogger.CheckConfigVars.GetOPCDTSmplFlagValue()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestCheckSampleGroupFlag01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestCheckSampleGroupFlag02")]
        public static System.String GetOPCDTSmplFlagValueTSS(
        OPCDataLogger.CheckConfigVars target)
        {
            return "Y";
        }

    }
}

