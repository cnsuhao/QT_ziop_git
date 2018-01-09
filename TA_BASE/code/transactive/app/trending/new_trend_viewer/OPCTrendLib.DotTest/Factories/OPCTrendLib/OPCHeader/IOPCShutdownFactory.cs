using Dottest.Framework;
using NUnit.Framework;
using OPCTrendLib.OPCData;
using OPCTrendLib.OPCHeader;

namespace DotTest.Factories.OPCTrendLib.OPCHeader
{
    [TestFixture(), ObjectFactoryType()]
    public class IOPCShutdownFactory
    {
        [ObjectFactoryMethod]
        [HashCode("/GIUdw", "+pH5ig")]
        internal static IOPCShutdown CreateIOPCShutdown01()
        {
            IOPCShutdown oPCShutdown = new OpcServer();
            return oPCShutdown;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateIOPCShutdown01()
        {
            IOPCShutdown oPCShutdown = CreateIOPCShutdown01();
            Assert.IsNotNull(oPCShutdown);
        }

    }
}
