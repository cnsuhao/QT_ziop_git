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
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataLoggerTests), "TestRestoreMenuStrip_Click02")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestCheckSampleGroupFlag01")]
        public static System.Boolean RemoveAllOPCItemTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            throw new Exception();
        }

        [MethodSpecificStubsMethod("STEE.ISCS.OPCProxy.OPCServerProxy.AddOPCItem(System.String)")]
        public static System.Boolean AddOPCItemMSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target,
        System.String opcID)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, opcID }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.AddOPCItem(opcID);
            }
        }

        [TestSpecificStub("STEE.ISCS.OPCProxy.OPCServerProxy.AddOPCItem(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataPointManagerTests), "TestConnectDPToOPC02")]
        public static System.Boolean AddOPCItemTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target,
        System.String opcID)
        {
            throw new Exception();
        }

        [MethodSpecificStubsMethod("STEE.ISCS.OPCProxy.OPCServerProxy.InitializeServer(System.String,System.String)")]
        public static void InitializeServerMSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target,
        System.String serverName, System.String groupName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, serverName, groupName }, out result))
            {
                return;
            }
            else
            {
                target.InitializeServer(serverName, groupName);
            }
        }

        [TestSpecificStub("STEE.ISCS.OPCProxy.OPCServerProxy.InitializeServer(System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataPointManagerTests), "TestInitializeDataLogger02")]
        public static void InitializeServerTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target,
        System.String serverName, System.String groupName)
        {
            return;
        }

        [MethodSpecificStubsMethod("STEE.ISCS.OPCProxy.OPCServerProxy.ReconnectAndAddToOPC()")]
        public static System.Boolean ReconnectAndAddToOPCMSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.ReconnectAndAddToOPC();
            }
        }

        [TestSpecificStub("STEE.ISCS.OPCProxy.OPCServerProxy.ReconnectAndAddToOPC()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCDataPointManagerTests), "TestInitializeDataLogger02")]
        public static System.Boolean ReconnectAndAddToOPCTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            return true;
        }

        [MethodSpecificStubsMethod("STEE.ISCS.OPCProxy.OPCServerProxy.IsOPCServerConnected()")]
        public static System.Boolean IsOPCServerConnectedMSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.IsOPCServerConnected();
            }
        }

        [TestSpecificStub("STEE.ISCS.OPCProxy.OPCServerProxy.IsOPCServerConnected()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestCheckOPCConnection02")]
        public static System.Boolean IsOPCServerConnectedTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            return true;
        }

        [MethodSpecificStubsMethod("STEE.ISCS.OPCProxy.OPCServerProxy.GetDataItemValue(System.String)")]
        public static System.Object GetDataItemValueMSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target,
        System.String itemID)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, itemID }, out result))
            {
                return (System.Object)result;
            }
            else
            {
                return target.GetDataItemValue(itemID);
            }
        }

        [TestSpecificStub("STEE.ISCS.OPCProxy.OPCServerProxy.GetDataItemValue(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestGetOPCValue02")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestGetOPCValue03")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestGetOPCValue04")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun01")]
        [TestSpecificStubContext(typeof(OPCDataLogger.DotTest.OPCLoggerQueneTests), "TestRun03")]
        public static System.Object GetDataItemValueTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target,
        System.String itemID)
        {
            string currentMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentMethod.Equals("TestGetOPCValue02"))
            {
                //boolean
                return true;
            }
            else if (currentMethod.Equals("TestGetOPCValue03"))
            {
                //numeric
                return 22;
            }
            else if (currentMethod.Equals("TestGetOPCValue04"))
            {
                //invalid type
                return "sytt";
            }
            else
                return 0;

        }

    }
}

