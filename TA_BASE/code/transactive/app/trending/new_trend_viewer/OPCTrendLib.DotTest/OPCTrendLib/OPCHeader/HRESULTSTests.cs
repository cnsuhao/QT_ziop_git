using Dottest.Framework;
using NUnit.Framework;
using OPCTrendLib.OPCHeader;

namespace DotTest.OPCTrendLib.OPCHeader
{
    [TestFixture()]
    public class HRESULTSTests
    {
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestFailed01()
        {
            //Test Procedure Call
            bool b = HRESULTS.Failed(HRESULTS.OPC_E_INVALIDITEMID);
            //Post Condition Check
            Assert.IsTrue(b);

            b = HRESULTS.Failed(HRESULTS.S_OK);
            //Post Condition Check
            Assert.IsFalse(b);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestSucceeded01()
        {
            //Test Procedure Call
            bool b = HRESULTS.Succeeded(HRESULTS.OPC_E_INVALIDITEMID);
            //Post Condition Check
            Assert.IsFalse(b);

            b = HRESULTS.Succeeded(HRESULTS.S_OK);
            //Post Condition Check
            Assert.IsTrue(b);
        }

    }
}
