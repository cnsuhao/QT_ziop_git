using Dottest.Framework;
using NUnit.Framework;


using System;

namespace STEE.ISCS.MulLanguage.DotTest
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
            Properties.Resources resource = new STEE.ISCS.MulLanguage.Properties.Resources();

            //Post Check
            Assert.IsNotNull(resource);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestGetResourceManager01()
        {
            System.Resources.ResourceManager resourcemanager = STEE.ISCS.MulLanguage.Properties.Resources.ResourceManager;


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
            Properties.Resources.Culture = culturinfo;
            System.Globalization.CultureInfo culturinfo1 = Properties.Resources.Culture;

            //Post Check
            Assert.AreEqual(culturinfo, culturinfo1);
        }




    }
}
