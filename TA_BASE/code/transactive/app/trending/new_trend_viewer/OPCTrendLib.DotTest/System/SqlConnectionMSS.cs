using System;
using Dottest.Framework;

namespace System.Data.SqlClientMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Data")]
    [MethodSpecificStubsClassAttribute("System.Data.SqlClient.SqlConnection")]
    public class SqlConnectionMSS
    {
        [MethodSpecificStubsMethod("System.Data.SqlClient.SqlConnection.Open()")]
        public static void OpenMSS(
        System.Data.SqlClient.SqlConnection target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                //target.Open();
                return;
            }
        }

        [TestSpecificStub("System.Data.SqlClient.SqlConnection.Open()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static void OpenTSS(
        System.Data.SqlClient.SqlConnection target)
        {
            target.Open();
        }

        [MethodSpecificStubsMethod("System.Data.SqlClient.SqlConnection.SqlConnection(System.String)")]
        public static System.Data.SqlClient.SqlConnection SqlConnection_ctor_MSS(System.String connectionString)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { connectionString }, out result))
            {
                return (System.Data.SqlClient.SqlConnection)result;
            }
            else
            {
                //return new System.Data.SqlClient.SqlConnection(connectionString);
                System.Data.SqlClient.SqlConnection sqlConnection = new System.Data.SqlClient.SqlConnection();
                sqlConnection.ConnectionString = "Data Source=MSSQL1;";
                return sqlConnection;
            }
        }

        [TestSpecificStub("System.Data.SqlClient.SqlConnection.SqlConnection(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static System.Data.SqlClient.SqlConnection SqlConnection_ctor_TSS(System.String connectionString)
        {
            return new System.Data.SqlClient.SqlConnection(connectionString);
        }

    }
}

