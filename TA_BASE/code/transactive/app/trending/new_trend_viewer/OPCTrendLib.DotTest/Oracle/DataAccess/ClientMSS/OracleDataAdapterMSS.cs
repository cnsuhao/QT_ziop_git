using System;
using Dottest.Framework;

namespace Oracle.DataAccess.ClientMSS
{
    [MethodSpecificStubsAssemblyAttribute("Oracle.DataAccess")]
    [MethodSpecificStubsClassAttribute("Oracle.DataAccess.Client.OracleDataAdapter")]
    public class OracleDataAdapterMSS
    {
        [MethodSpecificStubsMethod("Oracle.DataAccess.Client.OracleDataAdapter.Fill(System.Data.DataSet,System.Int32,System.Int32,System.String,System.Data.IDbCommand,System.Data.CommandBehavior)")]
        public static System.Int32 FillMSS(
        Oracle.DataAccess.Client.OracleDataAdapter target,
        System.Data.DataSet dataSet, System.Int32 startRecord, System.Int32 maxRecords, System.String srcTable, System.Data.IDbCommand command, System.Data.CommandBehavior behavior)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, dataSet, startRecord, maxRecords, srcTable, command, behavior }, out result))
            {
                return (System.Int32)result;
            }
            else
            {
                return (System.Int32)Helper.CallMethod(target, "Fill", new object[] { dataSet, startRecord, maxRecords, srcTable, command, behavior });
            }
        }

        [TestSpecificStub("Oracle.DataAccess.Client.OracleDataAdapter.Fill(System.Data.DataSet,System.Int32,System.Int32,System.String,System.Data.IDbCommand,System.Data.CommandBehavior)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.GeneralFunctionTests), "TestGetSQLQuery11")]
        public static System.Int32 FillTSS(
        Oracle.DataAccess.Client.OracleDataAdapter target,
        System.Data.DataSet dataSet, System.Int32 startRecord, System.Int32 maxRecords, System.String srcTable, System.Data.IDbCommand command, System.Data.CommandBehavior behavior)
        {
            //            return (System.Int32)Helper.CallMethod(target, "Fill", new object[] { dataSet, startRecord, maxRecords, srcTable, command, behavior });
            return 1;
        }

    }
}

