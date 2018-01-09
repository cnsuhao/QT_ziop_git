using System;
using Dottest.Framework;
using DAO.Trending.DotTest.Common;

namespace System.DataMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Data")]
    [MethodSpecificStubsClassAttribute("System.Data.IDbConnection")]
    public class IDbConnectionMSS
    {
        [MethodSpecificStubsMethod("System.Data.IDbConnection.CreateCommand()")]
        public static System.Data.IDbCommand CreateCommandMSS(
        System.Data.IDbConnection target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Data.IDbCommand)result;
            }
            else
            {
                return target.CreateCommand();
            }
        }

        [TestSpecificStub("System.Data.IDbConnection.CreateCommand()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DatabaseSessionTests), "TestCreateCommand")]
        public static System.Data.IDbCommand CreateCommandTSS(
        System.Data.IDbConnection target)
        {
           // return target.CreateCommand();
            return null;
        }

    }
}

