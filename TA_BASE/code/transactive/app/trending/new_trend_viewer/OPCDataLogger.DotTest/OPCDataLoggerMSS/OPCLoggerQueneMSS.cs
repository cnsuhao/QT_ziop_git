using System;
using Dottest.Framework;

namespace OPCDataLoggerMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCDataLogger")]
    [MethodSpecificStubsClassAttribute("OPCDataLogger.OPCLoggerQuene")]
    public class OPCLoggerQueneMSS
    {
        static int m_Runttimes = 0;
        [MethodSpecificStubsMethod("OPCDataLogger.OPCLoggerQuene.TestOracleConnection()")]
        public static System.Boolean TestOracleConnectionMSS(
            /*OPCDataLogger.OPCLoggerQuene*/ object target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return (System.Boolean)Helper.CallMethod(target, "TestOracleConnection", new object[] { });
            }
        }

        [TestSpecificStub("OPCDataLogger.OPCLoggerQuene.TestOracleConnection()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun02")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun03")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun04")]
        public static System.Boolean TestOracleConnectionTSS(
            /*OPCDataLogger.OPCLoggerQuene*/ object target)
        {
            return true;
        }

        [MethodSpecificStubsMethod("OPCDataLogger.OPCLoggerQuene.CheckOPCConnection()")]
        public static System.Boolean CheckOPCConnectionMSS(
            /*OPCDataLogger.OPCLoggerQuene*/ object target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return (System.Boolean)Helper.CallMethod(target, "CheckOPCConnection", new object[] { });
            }
        }

        [TestSpecificStub("OPCDataLogger.OPCLoggerQuene.CheckOPCConnection()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun02")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun03")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun04")]
        public static System.Boolean CheckOPCConnectionTSS(
            /*OPCDataLogger.OPCLoggerQuene*/ object target)
        {
            m_Runttimes++;
            if(m_Runttimes >= 2)
            {
                OPCDataLogger.OPCLoggerQuene oPCLoggerQuene = OPCDataLogger.DotTest.Factories.OPCLoggerQueneFactory.CreateOPCLoggerQuene02();
                Accessor oPCLoggerQueneAccessor = ReflectionAccessor.Wrap(oPCLoggerQuene);
                oPCLoggerQueneAccessor.SetField("m_serviceStarted", false);
                m_Runttimes = 0;
            }
           return true;
        }

    }
}

