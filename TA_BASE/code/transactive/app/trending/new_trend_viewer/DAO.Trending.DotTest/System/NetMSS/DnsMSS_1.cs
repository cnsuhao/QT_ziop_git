using System;
using Dottest.Framework;
using DAO.Trending.DotTest.Helper;

namespace System.NetMSS
{
    [MethodSpecificStubsAssemblyAttribute("System")]
    [MethodSpecificStubsClassAttribute("System.Net.Dns")]
    public class DnsMSS
    {
        [MethodSpecificStubsMethod("System.Net.Dns.GetHostName()")]
        public static System.String GetHostNameMSSS()
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { }, out result))
            {
                return (System.String)result;
            }
            else
            {
                return System.Net.Dns.GetHostName();
            }
        }

        [TestSpecificStub("System.Net.Dns.GetHostName()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(LocationKeyHelperTests), "Test_AbNormalFlow")]
        public static System.String GetHostNameTSSS()
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

