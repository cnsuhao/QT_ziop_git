using System;
using Dottest.Framework;

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
                return "occ-mft-01";
            }
        }

        [TestSpecificStub("System.Net.Dns.GetHostName()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestCanAddDataPoint03")]
        public static System.String GetHostNameTSSS()
        {
            return "occ-mft-01";//"dt19-mft-01";
        }

    }
}

