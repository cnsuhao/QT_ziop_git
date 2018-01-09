using Dottest.Framework;
using Dottest.Framework.RecordState;
using NUnit.Framework;
using OPCTrendLib;

namespace DotTest.Factories.OPCTrendLib
{
    [TestFixture(), ObjectFactoryType()]
    public class BinaryOpFactory
    {
        [ObjectFactoryMethod]
        [HashCode("++nUMw", "/ZuieQ")]
        internal static BinaryOp CreateBinaryOp01()
        {
            Token type = Token.BitwiseShiftLeft;
            BinaryOp binaryOp = BinaryOp.CreateOp(type);
            return binaryOp;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateBinaryOp01()
        {
            BinaryOp binaryOp = CreateBinaryOp01();
            Assert.IsNotNull(binaryOp);
            #region Record State
            ValueRecorder recorder = new ValueRecorder();
            recorder.Record((OperatorPriority)binaryOp.Priority);
            recorder.FinishRecording();
            #endregion
        }

        [ObjectFactoryMethod]
        [HashCode("++nUMw", "+AhY4A")]
        internal static BinaryOp CreateBinaryOp02()
        {
            CompareResult compareResult = CompareResult.Greater;
            BinaryOp binaryOp = new CompareOp(compareResult);
            return binaryOp;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateBinaryOp02()
        {
            BinaryOp binaryOp = CreateBinaryOp02();
            Assert.IsNotNull(binaryOp);
            #region Record State
            ValueRecorder recorder = new ValueRecorder();
            recorder.Record((OperatorPriority)binaryOp.Priority);
            recorder.FinishRecording();
            #endregion
        }

        [ObjectFactoryMethod]
        [HashCode("++nUMw", "+FDVyQ")]
        internal static BinaryOp CreateBinaryOp03()
        {
            Token type = Token.DateMarker;
            BinaryOp binaryOp = BinaryOp.CreateOp(type);
            return binaryOp;
        }

        [ExpectedException(typeof(System.InvalidOperationException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateBinaryOp03()
        {
            BinaryOp binaryOp = CreateBinaryOp03();
            Assert.IsNotNull(binaryOp);
            #region Record State
            ValueRecorder recorder = new ValueRecorder();
            recorder.Record((OperatorPriority)binaryOp.Priority);
            recorder.FinishRecording();
            #endregion
        }

    }
}
