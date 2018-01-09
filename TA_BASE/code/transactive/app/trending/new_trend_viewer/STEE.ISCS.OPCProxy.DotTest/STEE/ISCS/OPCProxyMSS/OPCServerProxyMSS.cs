using System;
using Dottest.Framework;

namespace STEE.ISCS.OPCProxyMSS
{
    [MethodSpecificStubsAssemblyAttribute("STEE.ISCS.OPCProxy")]
    [MethodSpecificStubsClassAttribute("STEE.ISCS.OPCProxy.OPCServerProxy")]
    public class OPCServerProxyMSS
    {
        [MethodSpecificStubsMethod("STEE.ISCS.OPCProxy.OPCServerProxy.RemoveAllOPCItem()")]
        public static System.Boolean RemoveAllOPCItemMSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.RemoveAllOPCItem();
            }
        }

        [TestSpecificStub("STEE.ISCS.OPCProxy.OPCServerProxy.RemoveAllOPCItem()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCProxy.DotTest.OPCServerProxyTests), "TestReleaseInstance01")]
        public static System.Boolean RemoveAllOPCItemTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            throw new Exception();
        }

    }
}

