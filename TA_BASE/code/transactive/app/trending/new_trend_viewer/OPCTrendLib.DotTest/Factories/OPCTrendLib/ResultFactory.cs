using Dottest.Framework;
using NUnit.Framework;
using OPCTrendLib;
using STEE.ISCS.Log.DotTest.Factories;
using System;

namespace DotTest.Factories.OPCTrendLib
{
    [TestFixture(), ObjectFactoryType()]
    public class ResultFactory
    {
        [ObjectFactoryMethod]
        [HashCode("+x4VNQ", "/9+rTA")]
        public static Result CreateResult01()
        {
            //object value1 = LogLevelFactory.CreateLogLevel01();
            Result result = new Result("RES");
            return result;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateResult01()
        {
            Result result = CreateResult01();
            Assert.IsNotNull(result);
        }

    }
}
