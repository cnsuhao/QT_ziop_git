using System;
using Dottest.Framework;

namespace System.Data.SqlClientMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Data")]
    [MethodSpecificStubsClassAttribute("System.Data.SqlClient.SqlCommand")]
    public class SqlCommandMSS
    {
        [MethodSpecificStubsMethod("System.Data.SqlClient.SqlCommand.ExecuteNonQuery()")]
        public static System.Int32 ExecuteNonQueryMSS(
        System.Data.SqlClient.SqlCommand target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Int32)result;
            }
            else
            {
               // return target.ExecuteNonQuery();
                return 1;
            }
        }

        [TestSpecificStub("System.Data.SqlClient.SqlCommand.ExecuteNonQuery()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.GeneralFunctionTests), "TestErr_ExecuteSQLNonQuery01" )]
        public static System.Int32 ExecuteNonQueryTSS(
        System.Data.SqlClient.SqlCommand target)
        {
            throw new Exception();
        }
    }
}

