using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace TrendingHelperMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendingHelper")]
    [MethodSpecificStubsClassAttribute("TrendingHelper.ConfigureFileHelper")]
    public class ConfigureFileHelperMSS
    {
        [MethodSpecificStubsMethod("TrendingHelper.ConfigureFileHelper.GetInstance()")]
        public static TrendingHelper.ConfigureFileHelper GetInstanceMSSS()
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { }, out result))
            {
                return (TrendingHelper.ConfigureFileHelper)result;
            }
            else
            {
                return TrendingHelper.ConfigureFileHelper.GetInstance();
            }
        }

        [TestSpecificStub("TrendingHelper.ConfigureFileHelper.GetInstance()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DataPointDataTests), "TestDisplay02")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestInitializeTrendViewer02")]
        public static TrendingHelper.ConfigureFileHelper GetInstanceTSSS()
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

