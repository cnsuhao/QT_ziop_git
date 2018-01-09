using Dottest.Framework;
using Dottest.Framework.RecordState;
using NUnit.Framework;
using OPCTrendLib;
using System;

namespace DotTest.Factories.OPCTrendLib
{
    [TestFixture(), ObjectFactoryType()]
    public class ConditionalOpExecutorFactory
    {
        [ObjectFactoryMethod]
        [HashCode("+XKJtQ", "/WFmig")]
        internal static ConditionalOpExecutor CreateConditionalOpExecutor01()
        {
            ConditionalOpExecutor conditionalOpExecutor = new ConditionalOpExecutor();
            return conditionalOpExecutor;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateConditionalOpExecutor01()
        {
            ConditionalOpExecutor conditionalOpExecutor = CreateConditionalOpExecutor01();
            Assert.IsNotNull(conditionalOpExecutor);
            #region Record State
            ValueRecorder recorder = new ValueRecorder();
            recorder.Record((int)ReflectionAccessor.Wrap(conditionalOpExecutor).GetProperty("CurrentIndex"));
            recorder.FinishRecording();
            #endregion
        }

    }
}
