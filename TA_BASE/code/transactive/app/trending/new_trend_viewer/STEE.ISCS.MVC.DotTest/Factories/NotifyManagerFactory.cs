using Dottest.Framework;
using NUnit.Framework;
using STEE.ISCS.MVC;

namespace STEE.ISCS.MVC.DotTest.Factories
{
    [TestFixture(), ObjectFactoryType()]
    public class NotifyManagerFactory
    {
        [ObjectFactoryMethod]
        [HashCode("+ORU8w", "+xq7XQ")]
        public static NotifyManager CreateNotifyManager01()
        {
            NotifyManager notifyManager = NotifyManager.GetInstance();
            return notifyManager;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateNotifyManager01()
        {
            NotifyManager notifyManager = CreateNotifyManager01();
            Assert.IsNotNull(notifyManager);
        }

    }
}
