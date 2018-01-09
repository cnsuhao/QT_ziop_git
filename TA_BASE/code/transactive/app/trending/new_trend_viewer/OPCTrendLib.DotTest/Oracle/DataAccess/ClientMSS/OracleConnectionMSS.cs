using System;
using Dottest.Framework;

namespace Oracle.DataAccess.ClientMSS
{
    [MethodSpecificStubsAssemblyAttribute("Oracle.DataAccess")]
    [MethodSpecificStubsClassAttribute("Oracle.DataAccess.Client.OracleConnection")]
    public class OracleConnectionMSS
    {
        [MethodSpecificStubsMethod("Oracle.DataAccess.Client.OracleConnection.Open()")]
        public static void OpenMSS(
        Oracle.DataAccess.Client.OracleConnection target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                //target.Open();
            }
        }

        [TestSpecificStub("Oracle.DataAccess.Client.OracleConnection.Open()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static void OpenTSS(
        Oracle.DataAccess.Client.OracleConnection target)
        {
            target.Open();
        }

    }
}

