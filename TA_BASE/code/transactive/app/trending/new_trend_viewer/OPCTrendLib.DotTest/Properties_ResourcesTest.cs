using Dottest.Framework;
using NUnit.Framework;
using System;

namespace OPCTrendLib.DotTest
{
    [TestFixture()]
    class Properties_ResourceTest
    {

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestResourceConstructor01()
        {   
            
            
            OPCTrendLib.Properties.Resources resource = new OPCTrendLib.Properties.Resources();

            //Post Check
            Assert.IsNotNull(resource);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestGetResourceManager01()
        {
            System.Resources.ResourceManager resourcemanager = OPCTrendLib.Properties.Resources.ResourceManager;


            //Post Check
            Assert.IsNotNull(resourcemanager);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCultural01()
        {
            System.Globalization.CultureInfo culturinfo = System.Globalization.CultureInfo.CurrentCulture;
            OPCTrendLib.Properties.Resources.Culture = culturinfo;
            System.Globalization.CultureInfo culturinfo1 = OPCTrendLib.Properties.Resources.Culture;

            //Post Check
            Assert.AreEqual(culturinfo, culturinfo1);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestUnrecogniseChar01()
        {
            string unrecognisedChar = OPCTrendLib.Properties.Resources.UnrecogniseChar;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestNotSupportGlobalFunction01()
        {
            string unrecognisedChar = OPCTrendLib.Properties.Resources.NotSupportGlobalFunction;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestNotHexChar01()
        {
            string unrecognisedChar = OPCTrendLib.Properties.Resources.NotHexChar;
        }



    }
}
