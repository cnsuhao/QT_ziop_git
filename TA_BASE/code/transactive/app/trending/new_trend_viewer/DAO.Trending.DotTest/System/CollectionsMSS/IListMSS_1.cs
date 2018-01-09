using System;
using Dottest.Framework;
using DAO.Trending.DotTest.Common;
using DAO.Trending.DotTest;

namespace System.CollectionsMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Collections.IList")]
    public class IListMSS
    {
        [MethodSpecificStubsMethod("System.Collections.IList.Remove(System.Object)")]
        public static void RemoveMSS(
        System.Collections.IList target,
        System.Object value_1)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, value_1 }, out result))
            {
                return;
            }
            else
            {
                target.Remove(value_1);
            }
        }

        [TestSpecificStub("System.Collections.IList.Remove(System.Object)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DbStatusMonitorThreadTests), "TestRun02")]
        public static void RemoveTSS(
        System.Collections.IList target,
        System.Object value_1)
        {
            Exception e = new Exception();
            throw e;
        }

        [MethodSpecificStubsMethod("System.Collections.IList.Add(System.Object)")]
        public static System.Int32 AddMSS(
        System.Collections.IList target,
        System.Object value_1)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, value_1 }, out result))
            {
                return (System.Int32)result;
            }
            else
            {
                return target.Add(value_1);
            }
        }

        [TestSpecificStub("System.Collections.IList.Add(System.Object)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DatalogDPGroupTrendDAOTests), "TestDeleteOPCSampleGroupById02")]
        public static System.Int32 AddTSS(
        System.Collections.IList target,
        System.Object value_1)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

