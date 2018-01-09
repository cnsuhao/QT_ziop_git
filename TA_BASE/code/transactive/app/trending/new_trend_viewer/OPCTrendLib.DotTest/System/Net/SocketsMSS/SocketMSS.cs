using System;
using Dottest.Framework;

namespace System.Net.SocketsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System")]
    [MethodSpecificStubsClassAttribute("System.Net.Sockets.Socket")]
    public class SocketMSS
    {
        [MethodSpecificStubsMethod("System.Net.Sockets.Socket.SendTo(System.Byte[],System.Net.EndPoint)")]
        public static System.Int32 SendToMSS(
        System.Net.Sockets.Socket target,
        System.Byte[] buffer, System.Net.EndPoint remoteEP)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, buffer, remoteEP }, out result))
            {
                return (System.Int32)result;
            }
            else
            {
                return target.SendTo(buffer, remoteEP);
            }
        }

        [TestSpecificStub("System.Net.Sockets.Socket.SendTo(System.Byte[],System.Net.EndPoint)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.GeneralFunctionTests), "TestErr_BroadcastPacket01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.GeneralFunctionTests), "TestErr_BroadcastPacketSingleTarget01")]
        public static System.Int32 SendToTSS(
        System.Net.Sockets.Socket target,
        System.Byte[] buffer, System.Net.EndPoint remoteEP)
        {
            throw new Exception();
        }

    }
}

