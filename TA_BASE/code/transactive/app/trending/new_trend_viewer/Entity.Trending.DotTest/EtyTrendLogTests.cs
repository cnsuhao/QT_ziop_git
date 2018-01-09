using Dottest.Framework;
using Dottest.Framework.RecordState;
using Entity.Trending;
using NUnit.Framework;
using System;

namespace Entity.Trending.DotTest
{
    [TestFixture()]
    public class EtyTrendLogTests
    {
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("/ACs+g", "+CSY8w")]
        public void TestEtyTrendLog01()
        {
            EtyTrendLog etyTrendLog = new EtyTrendLog();

            string s = etyTrendLog.Data_PT_Host;
            etyTrendLog.Data_PT_Host=s;

            s = etyTrendLog.Data_PT_Name;
            etyTrendLog.Data_PT_Name=s;

            s = etyTrendLog.Data_PT_Server;
            etyTrendLog.Data_PT_Server=s;

            DateTime dt = etyTrendLog.Data_PT_Time;
            etyTrendLog.Data_PT_Time=dt;

            double d = etyTrendLog.Data_PT_Value;
            etyTrendLog.Data_PT_Value=d;

        }

    }
}
