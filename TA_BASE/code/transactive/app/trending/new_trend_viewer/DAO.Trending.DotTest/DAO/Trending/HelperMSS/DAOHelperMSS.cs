using System;
using Dottest.Framework;
using DAO.Trending.DotTest;

namespace DAO.Trending.HelperMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.Helper.DAOHelper")]
    public class DAOHelperMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.Helper.DAOHelper.CheckLocationAndAddSQL(System.String)")]
        public static System.String CheckLocationAndAddSQLMSSS(System.String LocationColName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { LocationColName }, out result))
            {
                return (System.String)result;
            }
            else
            {
                return DAO.Trending.Helper.DAOHelper.CheckLocationAndAddSQL(LocationColName);
            }
        }

        [TestSpecificStub("DAO.Trending.Helper.DAOHelper.CheckLocationAndAddSQL(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(EntityDAOTests), "TestGetAllDataNodesByServerRootName")]
        [TestSpecificStubContext(typeof(EntityDAOTests), "TestGetDataNodeChildrenByPkey")]
        [TestSpecificStubContext(typeof(DatalogDPGroupTrendDAOTests), "TestGetAllOPCGrpsBySort01")]
        [TestSpecificStubContext(typeof(DatalogDPGroupTrendDAOTests), "TestGetDataPointByName")]
        [TestSpecificStubContext(typeof(DatalogDPTrendDAOTests), "TestGetDataPointByName")]
        [TestSpecificStubContext(typeof(DatalogDPTrendDAOTests), "TestGetDataPointByName02")]
        [TestSpecificStubContext(typeof(DatalogDPGroupTrendDAOTests), "TestGetAllOPCGrpsBySortNPage01")]
        public static System.String CheckLocationAndAddSQLTSSS(System.String LocationColName)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (name.Equals("TestGetAllOPCGrpsBySort01") || name.Equals("TestGetAllOPCGrpsBySortNPage01"))
            {
                return " LOCATION_KEY =1 ";
            }
            else
                return " LOCATIONKEY =1 ";


        }

        [MethodSpecificStubsMethod("DAO.Trending.Helper.DAOHelper.CreateEnqueneParameters(System.Collections.Generic.List<System.String>)")]
        public static System.Collections.Generic.List<DAO.Trending.Helper.SqlParameter> CreateEnqueneParametersMSSS(System.Collections.Generic.List<System.String> parameterValues)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { parameterValues }, out result))
            {
                return (System.Collections.Generic.List<DAO.Trending.Helper.SqlParameter>)result;
            }
            else
            {
                return DAO.Trending.Helper.DAOHelper.CreateEnqueneParameters(parameterValues);
            }
        }

        [TestSpecificStub("DAO.Trending.Helper.DAOHelper.CreateEnqueneParameters(System.Collections.Generic.List<System.String>)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DatalogDPGroupTrendDAOTests), "TestDeleteOPCSampleGroupById02")]
        public static System.Collections.Generic.List<DAO.Trending.Helper.SqlParameter> CreateEnqueneParametersTSSS(System.Collections.Generic.List<System.String> parameterValues)
        {
             Exception e= new Exception();
            throw e;
         }

    }


}

