using System;
using Dottest.Framework;

namespace SystemMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.DateTime")]
    public class DateTimeMSS
    {

        [MethodSpecificStubsMethod("System.DateTime.get_Now()")]
        public static System.DateTime get_NowMSSS()
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { }, out result))
            {
                return (System.DateTime)result;
            }
            else
            {
                return System.DateTime.Now;
            }
        }

        [TestSpecificStub("System.DateTime.get_Now()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataPointManagerTests), "TestGetNextSampleTime01")]
        public static System.DateTime get_NowTSSS()
        {
            System.DateTime currenttime = new DateTime(2013, 01, 29, 15, 20, 50);
            return currenttime;
        }

        [MethodSpecificStubsMethod("System.DateTime.Compare(System.DateTime,System.DateTime)")]
        public static System.Int32 CompareMSSS(System.DateTime t1, System.DateTime t2)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { t1, t2 }, out result))
            {
                return (System.Int32)result;
            }
            else
            {
                return System.DateTime.Compare(t1, t2);
            }
        }

        [TestSpecificStub("System.DateTime.Compare(System.DateTime,System.DateTime)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestCanLogDataPT02")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestCanLogDataPT03")]
        public static System.Int32 CompareTSSS(System.DateTime t1, System.DateTime t2)
        {
            string currentmethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentmethod.Equals("TestCanLogDataPT03"))
            {
                return -1;
            }
            else
            {
                return 0;
            }

        }

    }
}

