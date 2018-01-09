using System;
using Dottest.Framework;

namespace DAO.Trending.CommonMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.Common.SimpleDatabase")]
    public class SimpleDatabaseMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.Common.SimpleDatabase.BeginTransaction()")]
        public static void BeginTransactionMSS(
        DAO.Trending.Common.SimpleDatabase target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.BeginTransaction();
            }
        }

        [TestSpecificStub("DAO.Trending.Common.SimpleDatabase.BeginTransaction()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Model.OPCDataSelectorModelTests), "TestBeginTransaction01")]
        public static void BeginTransactionTSS(
        DAO.Trending.Common.SimpleDatabase target)
        {
            return;
        }

        [MethodSpecificStubsMethod("DAO.Trending.Common.SimpleDatabase.CommitTransaction()")]
        public static void CommitTransactionMSS(
        DAO.Trending.Common.SimpleDatabase target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.CommitTransaction();
            }
        }

        [TestSpecificStub("DAO.Trending.Common.SimpleDatabase.CommitTransaction()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Model.OPCDataSelectorModelTests), "TestCommitTransaction01")]
        public static void CommitTransactionTSS(
        DAO.Trending.Common.SimpleDatabase target)
        {
            return;
        }

        [MethodSpecificStubsMethod("DAO.Trending.Common.SimpleDatabase.RollbackTransaction()")]
        public static void RollbackTransactionMSS(
        DAO.Trending.Common.SimpleDatabase target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.RollbackTransaction();
            }
        }

        [TestSpecificStub("DAO.Trending.Common.SimpleDatabase.RollbackTransaction()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Model.OPCDataSelectorModelTests), "TestRollBackTransaction01")]
        public static void RollbackTransactionTSS(
        DAO.Trending.Common.SimpleDatabase target)
        {
            return;
        }

    }
}

