using Dottest.Framework;
using Dottest.Framework.RecordState;
using NUnit.Framework;
using OPCTrendLib;

namespace DotTest.OPCTrendLib
{
    [TestFixture()]
    public class BitwiseAndTests
    {
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("+aAfRA", "/sdhfA")]
        public void TestBitwiseAndConstructor01()
        {
            BitwiseAnd bitwiseAnd = new BitwiseAnd();
            #region Record State
            ValueRecorder recorder = new ValueRecorder();
            recorder.Record((OperatorPriority)bitwiseAnd.Priority);
            recorder.FinishRecording();
            #endregion
        }

    }
}
