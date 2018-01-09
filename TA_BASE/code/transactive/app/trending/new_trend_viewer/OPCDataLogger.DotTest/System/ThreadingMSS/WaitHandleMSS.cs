using System;
using Dottest.Framework;

namespace System.ThreadingMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Threading.WaitHandle")]
    public class WaitHandleMSS
    {
        static int kk = 0;
        [MethodSpecificStubsMethod("System.Threading.WaitHandle.WaitOne()")]
        public static System.Boolean WaitOneMSS(
        System.Threading.WaitHandle target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                kk++;
                if(kk == 1)
                {
                    return target.WaitOne();
                }
                else
                {
                    OPCDataLogger.OPCLoggerWriteQuene writeQueue = OPCDataLogger.DotTest.Factories.OPCLoggerWriteQueneFactory.CreateOPCLoggerWriteQuene01();
                    Accessor writeQAccessor = ReflectionAccessor.Wrap(writeQueue);
                    writeQAccessor.SetField("m_terminate", true);
                    kk = 0;
                    return true;
                }
                
            }
        }

        [TestSpecificStub("System.Threading.WaitHandle.WaitOne()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun01" )]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun02")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun03")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun04")]
        public static System.Boolean WaitOneTSS(
        System.Threading.WaitHandle target)
        {
            return target.WaitOne();
        }

    }
}

