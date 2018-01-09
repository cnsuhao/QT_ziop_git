using System;
using Dottest.Framework;

namespace SystemMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Activator")]
    public class ActivatorMSS
    {
        [MethodSpecificStubsMethod("System.Activator.CreateInstance(System.Type)")]
        public static System.Object CreateInstanceMSSS(System.Type type)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { type }, out result))
            {
                return (System.Object)result;
            }
            else
            {
                return System.Activator.CreateInstance(type);
            }
        }

        [TestSpecificStub("System.Activator.CreateInstance(System.Type)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCHeader.OPCServerListTests), "TestListAllServer01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCHeader.OPCServerListTests), "TestListAllServer02")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCHeader.OPCServerListTests), "TestListAllServer03")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCHeader.OPCServerListTests), "TestListAllServerData01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestConnect02")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestConnect03")]
        public static System.Object CreateInstanceTSSS(System.Type type)
        {
            Guid clsid = new Guid("13486D51-4821-11D2-A494-3CB306C10000");
            Type typeFromCLSID = Type.GetTypeFromCLSID(clsid);
            string currentMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentMethod.Equals("TestListAllServer01"))
            {
                //OPCTrendLib.OPCHeader.OPCServerList opcServerList = new OPCTrendLib.OPCHeader.OPCServerList();
                //return opcServerList;
                return null;
            }
            else if(currentMethod.Equals("TestConnect02"))
            {
                return null;
            }
            else if (currentMethod.Equals("TestConnect03"))
            {
                OPCTrendLib.OPCDataInterface.OPCServerComClass opcserver = new OPCTrendLib.OPCDataInterface.OPCServerComClass();
                return opcserver;
            }
            else
            {
                if (type.Equals(typeFromCLSID))
                {
                    OPCTrendLib.OPCHeader.OPCServerListC opcserver = new OPCTrendLib.OPCHeader.OPCServerListC();
                    return opcserver;
                }
            }
            return System.Activator.CreateInstance(type);
        }
    }
}

