using System;
using Dottest.Framework;
using DAO.Trending.DotTest;

namespace System.DataMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Data")]
    [MethodSpecificStubsClassAttribute("System.Data.IDataReader")]
    public class IDataReaderMSS
    {
        [MethodSpecificStubsMethod("System.Data.IDataReader.Read()")]
        public static System.Boolean ReadMSS(
        System.Data.IDataReader target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.Read();
            }
        }

        [TestSpecificStub("System.Data.IDataReader.Read()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(EntityDAOTests), "TestGetAllDataNodesByServerRootName02")]
        [TestSpecificStubContext(typeof(EntityDAOTests), "TestGetDataNodeChildrenByPkey02")]
        [TestSpecificStubContext(typeof(EntityDAOTests), "TestFindDataPointEtyKeyByName02")]
        [TestSpecificStubContext(typeof(EntityDAOTests), "TestGetLocationNameFromKey02")]
        [TestSpecificStubContext(typeof(EntityDAOTests), "TestGetLocationKeyByEtyName02")]
        [TestSpecificStubContext(typeof(EntityDAOTests), "TestGetDataPointByDNPkey02")]
        [TestSpecificStubContext(typeof(EntityDAOTests), "TestGetHistDPLogList02")]
         [TestSpecificStubContext(typeof(DatalogDPGroupTrendDAOTests), "TestGetAllOPCGrpsBySort02")]
        [TestSpecificStubContext(typeof(DatalogDPGroupTrendDAOTests), "TestGetOPCGrpByID02")]
        [TestSpecificStubContext(typeof(DatalogDPGroupTrendDAOTests), "TestGetAllOPCGrpsBySortNPage02")]
        [TestSpecificStubContext(typeof(DatalogDPGroupTrendDAOTests), "TestGetRowCount02")]
        [TestSpecificStubContext(typeof(DatalogDPGroupTrendDAOTests), "TestGetAllOPCGrpIdAndName02")]
        [TestSpecificStubContext(typeof(DatalogDPTrendDAOTests), "TestGetOPCDataPointByGrpId02")]
        [TestSpecificStubContext(typeof(DatalogDPTrendDAOTests), "TestGetDataPointByName02")]
        [TestSpecificStubContext(typeof(DatalogDPTrendDAOTests), "TestGetGrpIDByDPName02")]
        [TestSpecificStubContext(typeof(TrendLogDAOTests), "TestGetHistDPLogList02")]
        [TestSpecificStubContext(typeof(FormulaerDAOTests), "TestGetAllformulaGrpNames_GetFormulasByCfgName02")]
        [TestSpecificStubContext(typeof(HistDataPointDAOTests), "TestExceptions")]
        [TestSpecificStubContext(typeof(HistDataPointDAOTests), "TestException")]
        [TestSpecificStubContext(typeof(MarkerDAOTests), "TestExceptions")]
        [TestSpecificStubContext(typeof(DataPointDAOTests), "TestException")]
        [TestSpecificStubContext(typeof(DataLogDPLogTrendDAOTests), "TestGetHistDPLogList02")]
        public static System.Boolean ReadTSS(
        System.Data.IDataReader target)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

