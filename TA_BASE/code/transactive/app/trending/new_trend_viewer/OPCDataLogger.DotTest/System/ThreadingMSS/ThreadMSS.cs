using System;
using Dottest.Framework;

namespace System.ThreadingMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Threading.Thread")]
    public class ThreadMSS
    {
        [MethodSpecificStubsMethod("System.Threading.Thread.Start()")]
        public static void StartMSS(
        System.Threading.Thread target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                //target.Start();
                return;
            }
        }

        [TestSpecificStub("System.Threading.Thread.Start()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataLoggerTests), "")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataPointManagerTests), "TestInitializeOPCSetting01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerWriteQueneTests), "TestRun01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun02")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun03")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun04")]
        public static void StartTSS(
        System.Threading.Thread target)
        {
            string currentmethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentmethod.Equals("TestInitializeOPCSetting01"))
            {
                throw new Exception();
            }
            else if (currentmethod.Equals("TestRun01") ||
                currentmethod.Equals("TestRun02") ||
                currentmethod.Equals("TestRun03") ||
                currentmethod.Equals("TestRun04"))
            {
                target.Start();
            }
            // target.Start();
        }

        [MethodSpecificStubsMethod("System.Threading.Thread.Abort()")]
        public static void AbortMSS(
        System.Threading.Thread target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                //target.Abort();
                return;
            }
        }

        [TestSpecificStub("System.Threading.Thread.Abort()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun02")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun03")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun04")]
        public static void AbortTSS(
        System.Threading.Thread target)
        {
            //target.Abort();
            OPCDataLogger.DotTest.OPCLoggerQueneTests.m_OPCLoggerQuenesignal.Set();
        }

        [MethodSpecificStubsMethod("System.Threading.Thread.Join()")]
        public static void JoinMSS(
        System.Threading.Thread target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                return;
            }
        }

        [TestSpecificStub("System.Threading.Thread.Join()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestTerminateAndWait01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerWriteQueneTests), "TestTerminateAndWait01")]
        public static void JoinTSS(
        System.Threading.Thread target)
        {
            target.Join();
        }

    }
}

