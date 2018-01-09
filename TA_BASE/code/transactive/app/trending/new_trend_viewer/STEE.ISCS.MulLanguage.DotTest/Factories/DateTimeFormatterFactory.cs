using Dottest.Framework;
using NUnit.Framework;
using STEE.ISCS.MulLanguage;

namespace STEE.ISCS.MulLanguage.DotTest.Factories
{
    [TestFixture(), ObjectFactoryType()]
    public class DateTimeFormatterFactory
    {
        [ObjectFactoryMethod]
        [HashCode("+OvmYg", "+Rthvw")]
        public static DateTimeFormatter CreateDateTimeFormatter01()
        {
            DateTimeFormatter dateTimeFormatter = DateTimeFormatter.getInstance();
            return dateTimeFormatter;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateDateTimeFormatter01()
        {
            DateTimeFormatter dateTimeFormatter = CreateDateTimeFormatter01();
            Assert.IsNotNull(dateTimeFormatter);
        }

    }
}
