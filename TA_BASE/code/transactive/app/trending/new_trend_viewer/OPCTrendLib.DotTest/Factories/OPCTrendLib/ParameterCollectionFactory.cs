using Dottest.Framework;
using NUnit.Framework;
using OPCTrendLib;

namespace DotTest.Factories.OPCTrendLib
{
    [TestFixture(), ObjectFactoryType()]
    public class ParameterCollectionFactory
    {
        [ObjectFactoryMethod]
        [HashCode("+aY/cA", "/L3yPA")]
        public static ParameterCollection CreateParameterCollection01()
        {
            ParameterCollection parameterCollection = new ParameterCollection();
            return parameterCollection;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateParameterCollection01()
        {
            ParameterCollection parameterCollection = CreateParameterCollection01();
            Assert.IsNotNull(parameterCollection);
        }

    }
}