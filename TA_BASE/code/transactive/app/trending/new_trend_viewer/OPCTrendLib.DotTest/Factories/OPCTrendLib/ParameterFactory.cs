using Dottest.Framework;
using NUnit.Framework;
using OPCTrendLib;
//using STEE.ISCS.Log.DotTest.Factories;
using System;

namespace DotTest.Factories.OPCTrendLib
{
    [TestFixture(), ObjectFactoryType()]
    public class ParameterFactory
    {
        [ObjectFactoryMethod]
        [HashCode("++Z5TA", "/aTQ6g")]
        public static Parameter CreateParameter01()
        {
            Parameter parameter = new Parameter();
            return parameter;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateParameter01()
        {
            Parameter parameter = CreateParameter01();
            Assert.IsNotNull(parameter);
        }

        [ObjectFactoryMethod]
        [HashCode("++Z5TA", "+9sSOw")]
        public static Parameter CreateParameter02()
        {
            //object value1 = LogLevelFactory.CreateLogLevel01();
            Parameter parameter = new Parameter("PARAM");
            return parameter;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateParameter02()
        {
            Parameter parameter = CreateParameter02();
            Assert.IsNotNull(parameter);
        }

    }
}
