using System;
using Dottest.Framework;
using DAO.Trending.DotTest.Common;

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
        [TestSpecificStubContext(typeof(DbStatusMonitorThreadTests), "TestRun01" )]
        public static void RemoveTSS(
        System.Collections.IList target,
        System.Object value_1)
        {
            //target.Remove(value_1);
            Exception e = new Exception();
            throw e;
        }

    }
}

