using System;
using Dottest.Framework;

namespace OPCDataLoggerMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCDataLogger")]
    [MethodSpecificStubsClassAttribute("OPCDataLogger.OPCDataPointManager")]
    public class OPCDataPointManagerMSS
    {
        [MethodSpecificStubsMethod("OPCDataLogger.OPCDataPointManager.InitializeDataLogger(System.Boolean)")]
        public static System.Boolean InitializeDataLoggerMSS(
            /*OPCDataLogger.OPCDataPointManager*/ object target,
        System.Boolean bReInitializing)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, bReInitializing }, out result))
            {
                return (System.Boolean)result; 
            }
            else
            {
                return (System.Boolean)Helper.CallMethod(target, "InitializeDataLogger", new object[] { bReInitializing });
            }
        }

        [TestSpecificStub("OPCDataLogger.OPCDataPointManager.InitializeDataLogger(System.Boolean)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestCheckSampleGroupFlag01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun02")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun03")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun04")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun05")]
        public static System.Boolean InitializeDataLoggerTSS(
            /*OPCDataLogger.OPCDataPointManager*/ object target,
        System.Boolean bReInitializing)
        {
           string currentmethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
           if (currentmethod.Equals("TestCheckSampleGroupFlag01"))
           {
               throw new Exception();
           }
           else if(currentmethod.Equals("TestRun05"))
           {
               return false;
           }
           else
               return true;
        }

    }
}

