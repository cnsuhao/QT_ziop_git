using System;
using Dottest.Framework;

namespace Oracle.DataAccess.ClientMSS
{
    [MethodSpecificStubsAssemblyAttribute("Oracle.DataAccess")]
    [MethodSpecificStubsClassAttribute("Oracle.DataAccess.Client.OracleCommand")]
    public class OracleCommandMSS
    {
        [MethodSpecificStubsMethod("Oracle.DataAccess.Client.OracleCommand.ExecuteNonQuery()")]
        public static System.Int32 ExecuteNonQueryMSS(
        Oracle.DataAccess.Client.OracleCommand target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Int32)result;
            }
            else
            {
                //return target.ExecuteNonQuery();
                return 1;
            }
        }

        [TestSpecificStub("Oracle.DataAccess.Client.OracleCommand.ExecuteNonQuery()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.GeneralFunctionTests), "TestErr_ExecuteOracleNonQuery01")]
        public static System.Int32 ExecuteNonQueryTSS(
        Oracle.DataAccess.Client.OracleCommand target)
        {
            throw new Exception();
        }

        [MethodSpecificStubsMethod("Oracle.DataAccess.Client.OracleCommand.ExecuteReader()")]
        public static Oracle.DataAccess.Client.OracleDataReader ExecuteReaderMSS(
        Oracle.DataAccess.Client.OracleCommand target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (Oracle.DataAccess.Client.OracleDataReader)result;
            }
            else
            {
                return target.ExecuteReader();
            }
        }

        [TestSpecificStub("Oracle.DataAccess.Client.OracleCommand.ExecuteReader()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static Oracle.DataAccess.Client.OracleDataReader ExecuteReaderTSS(
        Oracle.DataAccess.Client.OracleCommand target)
        {
            throw new Exception();
        }
    }
}

