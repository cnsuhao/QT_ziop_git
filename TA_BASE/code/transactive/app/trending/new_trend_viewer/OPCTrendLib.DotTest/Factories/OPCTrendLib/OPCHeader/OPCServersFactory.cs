using Dottest.Framework;
using NUnit.Framework;
using OPCTrendLib.OPCHeader;

namespace DotTest.Factories.OPCTrendLib.OPCHeader
{
    [TestFixture(), ObjectFactoryType()]
    public class OPCServersFactory
    {
        [ObjectFactoryMethod]
        [HashCode("/6nobg", "/tKWqw")]
        public static OPCServers CreateOPCServers01()
        {
            OPCServers oPCServers = new OPCServers();
            return oPCServers;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateOPCServers01()
        {
            OPCServers oPCServers = CreateOPCServers01();
            Assert.IsNotNull(oPCServers);
        }

    }
}
