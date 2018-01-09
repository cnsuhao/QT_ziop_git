using System;
using Dottest.Framework;
using DAO.Trending.DotTest;

namespace DAO.Trending.CommonMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.Common.SimpleDatabase")]
    public class SimpleDatabaseMSS
    {

        [MethodSpecificStubsMethod("DAO.Trending.Common.SimpleDatabase.ExecuteEnqueneProcedure(System.Collections.Generic.List<DAO.Trending.Helper.SqlParameter>)")]
        public static System.Boolean ExecuteEnqueneProcedureMSS(
        DAO.Trending.Common.SimpleDatabase target,
        System.Collections.Generic.List<DAO.Trending.Helper.SqlParameter> parameters)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, parameters }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.ExecuteEnqueneProcedure(parameters);
            }
        }

        [TestSpecificStub("DAO.Trending.Common.SimpleDatabase.ExecuteEnqueneProcedure(System.Collections.Generic.List<DAO.Trending.Helper.SqlParameter>)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DatalogDPGroupTrendDAOTests), "TestDeleteOPCSampleGroupById02")]
        public static System.Boolean ExecuteEnqueneProcedureTSS(
        DAO.Trending.Common.SimpleDatabase target,
        System.Collections.Generic.List<DAO.Trending.Helper.SqlParameter> parameters)
        {
            Exception e = new Exception();
            throw e;
        }

        [MethodSpecificStubsMethod("DAO.Trending.Common.SimpleDatabase.ExecuteNonQuery(System.String)")]
        public static System.Boolean ExecuteNonQueryMSS(
        DAO.Trending.Common.SimpleDatabase target,
        System.String query)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, query }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.ExecuteNonQuery(query);
            }
        }

        [TestSpecificStub("DAO.Trending.Common.SimpleDatabase.ExecuteNonQuery(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(FormulaerDAOTests), "TestSaveFormulaListToGrp02")]
        [TestSpecificStubContext(typeof(HistDataPointDAOTests), "TestSave02")]
        [TestSpecificStubContext(typeof(DataPointDAOTests), "TestSave02")]
        [TestSpecificStubContext(typeof(MarkerDAOTests), "TestSave02")]
        public static System.Boolean ExecuteNonQueryTSS(
        DAO.Trending.Common.SimpleDatabase target,
        System.String query)
        {
            return false;
        }

        [MethodSpecificStubsMethod("DAO.Trending.Common.SimpleDatabase.ExecuteNonQueryWithParams(System.String,System.Collections.Generic.List<DAO.Trending.Helper.SqlParameter>)")]
        public static System.Boolean ExecuteNonQueryWithParamsMSS(
        DAO.Trending.Common.SimpleDatabase target,
        System.String query, System.Collections.Generic.List<DAO.Trending.Helper.SqlParameter> parameters)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, query, parameters }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.ExecuteNonQueryWithParams(query, parameters);
            }
        }

        [TestSpecificStub("DAO.Trending.Common.SimpleDatabase.ExecuteNonQueryWithParams(System.String,System.Collections.Generic.List<DAO.Trending.Helper.SqlParameter>)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
         [TestSpecificStubContext(typeof(HistDataPointDAOTests), "TestInsertHistDPListToGrp02" )]
        public static System.Boolean ExecuteNonQueryWithParamsTSS(
        DAO.Trending.Common.SimpleDatabase target,
        System.String query, System.Collections.Generic.List<DAO.Trending.Helper.SqlParameter> parameters)
        {
            return false;
        }

    }
}

