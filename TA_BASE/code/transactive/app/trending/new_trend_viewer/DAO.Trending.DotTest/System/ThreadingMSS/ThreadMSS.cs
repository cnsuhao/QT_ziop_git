using System;
using Dottest.Framework;
using DAO.Trending.DotTest.Common;

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
                target.Start();
            }
        }

        [TestSpecificStub("System.Threading.Thread.Start()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DbStatusMonitorThreadTests), "TestRun02")]
        public static void StartTSS(
        System.Threading.Thread target)
        {
            target.Start();
//             Exception e = new Exception();
//             throw e;
        }

    }
}

