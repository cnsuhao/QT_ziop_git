using System;
using Dottest.Framework;

namespace OPCTrendLib.OPCDataMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCTrendLib")]
    [MethodSpecificStubsClassAttribute("OPCTrendLib.OPCData.OpcServer")]
    public class OpcServerMSS
    {
        static int s_diconnecttimes = 0;
        [MethodSpecificStubsMethod("OPCTrendLib.OPCData.OpcServer.Disconnect()")]
        public static void DisconnectMSS(
        OPCTrendLib.OPCData.OpcServer target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.Disconnect();
            }
        }

        [TestSpecificStub("OPCTrendLib.OPCData.OpcServer.Disconnect()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer04")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer05")]
        public static void DisconnectTSS(
        OPCTrendLib.OPCData.OpcServer target)
        {
            string currentMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentMethod.Equals("TestInitializeServer04"))
            {

                s_diconnecttimes++;
                if (s_diconnecttimes == 1)
                {
                    throw new System.Runtime.InteropServices.COMException("Interface not found");
                }
                else
                {
                    return;
                }
            }
            return;
        }

        [MethodSpecificStubsMethod("OPCTrendLib.OPCData.OpcServer.Connect(System.String)")]
        public static void ConnectMSS(
        OPCTrendLib.OPCData.OpcServer target,
        System.String clsidOPCserver)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, clsidOPCserver }, out result))
            {
                return;
            }
            else
            {
                target.Connect(clsidOPCserver);
            }
        }

        [TestSpecificStub("OPCTrendLib.OPCData.OpcServer.Connect(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer01")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer03")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer05")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC03")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC04")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC06")]
        public static void ConnectTSS(
        OPCTrendLib.OPCData.OpcServer target,
        System.String clsidOPCserver)
        {
            string currentMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
           /* if (currentMethod.Equals("TestInitializeServer02"))
            {
                throw new System.Runtime.InteropServices.COMException("Interface not found");
            }
            else*/
                return;
        }

        [MethodSpecificStubsMethod("OPCTrendLib.OPCData.OpcServer.GetStatus(out OPCTrendLib.OPCDataInterface.SERVERSTATUS)")]
        public static void GetStatusMSS(
        OPCTrendLib.OPCData.OpcServer target,
        out OPCTrendLib.OPCDataInterface.SERVERSTATUS serverStatus)
        {
            serverStatus = null; object result;
            // NOTE: out and ref parameters are not passed from a test specific stub
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, serverStatus }, out result))
            {
                string currentMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
                if (currentMethod.Equals("TestReconnectAndAddToOPC02"))
                {
                    throw new Exception("Error 0x333 The RPC server is unavailable.");
                }
                else if (currentMethod.Equals("TestIsOPCServerConnected01"))
                {
                    serverStatus = new OPCTrendLib.OPCDataInterface.SERVERSTATUS();
                    serverStatus.eServerState = OPCTrendLib.OPCDataInterface.OPCSERVERSTATE.OPC_STATUS_SUSPENDED;
                    serverStatus.ftStartTime = DateTime.Now.ToFileTime();
                }
                else
                {
                    serverStatus = new OPCTrendLib.OPCDataInterface.SERVERSTATUS();
                    serverStatus.eServerState = OPCTrendLib.OPCDataInterface.OPCSERVERSTATE.OPC_STATUS_RUNNING;
                    serverStatus.ftStartTime = DateTime.Now.ToFileTime();
                }
                return;
            }
            else
            {
                target.GetStatus(out serverStatus);
            }
        }

        [TestSpecificStub("OPCTrendLib.OPCData.OpcServer.GetStatus(out OPCTrendLib.OPCDataInterface.SERVERSTATUS)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer01")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer03")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer05")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC01")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC02")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC03")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC04")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC06")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestIsOPCServerConnected01")]        
        public static void GetStatusTSS(
        OPCTrendLib.OPCData.OpcServer target,
        out OPCTrendLib.OPCDataInterface.SERVERSTATUS serverStatus)
        {
            serverStatus = new OPCTrendLib.OPCDataInterface.SERVERSTATUS();
            serverStatus.eServerState = OPCTrendLib.OPCDataInterface.OPCSERVERSTATE.OPC_STATUS_RUNNING;
            serverStatus.ftStartTime = DateTime.Now.ToFileTime();
            return;
        }

        [MethodSpecificStubsMethod("OPCTrendLib.OPCData.OpcServer.AddGroup(System.String,System.Boolean,System.Int32)")]
        public static OPCTrendLib.OPCData.OpcGroup AddGroupMSS(
        OPCTrendLib.OPCData.OpcServer target,
        System.String groupName, System.Boolean setActive, System.Int32 requestedUpdateRate)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, groupName, setActive, requestedUpdateRate }, out result))
            {
                return (OPCTrendLib.OPCData.OpcGroup)result;
            }
            else
            {
                return target.AddGroup(groupName, setActive, requestedUpdateRate);
            }
        }

        [TestSpecificStub("OPCTrendLib.OPCData.OpcServer.AddGroup(System.String,System.Boolean,System.Int32)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer01")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer05")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC03")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC04")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC06")]
        public static OPCTrendLib.OPCData.OpcGroup AddGroupTSS(
        OPCTrendLib.OPCData.OpcServer target,
        System.String groupName, System.Boolean setActive, System.Int32 requestedUpdateRate)
        {
            //return target.AddGroup(groupName, setActive, requestedUpdateRate);
             OPCTrendLib.OPCDataInterface.IOPCServer server = null;
             OPCTrendLib.OPCData.OpcGroup group = new OPCTrendLib.OPCData.OpcGroup(ref server, false, groupName, setActive, requestedUpdateRate);
            return group;  
        }

        [MethodSpecificStubsMethod("OPCTrendLib.OPCData.OpcServer.SetClientName(System.String)")]
        public static void SetClientNameMSS(
        OPCTrendLib.OPCData.OpcServer target,
        System.String name)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, name }, out result))
            {
                return;
            }
            else
            {
                target.SetClientName(name);
            }
        }

        [TestSpecificStub("OPCTrendLib.OPCData.OpcServer.SetClientName(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer01")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestInitializeServer05")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC03")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC04")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC06")]
        public static void SetClientNameTSS(
        OPCTrendLib.OPCData.OpcServer target,
        System.String name)
        {
            return;
        }

    }
}

