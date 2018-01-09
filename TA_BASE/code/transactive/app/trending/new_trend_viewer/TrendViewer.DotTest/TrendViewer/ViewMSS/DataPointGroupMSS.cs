using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.View.DataPointGroup")]
    public class DataPointGroupMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.View.DataPointGroup.GetConfigName()")]
        public static System.String GetConfigNameMSS(
        TrendViewer.View.DataPointGroup target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.String)result;
            }
            else
            {
                return target.GetConfigName();
            }
        }

        [TestSpecificStub("TrendViewer.View.DataPointGroup.GetConfigName()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DataPointGroupControllerTests), "TestLoadDataPointByGrpName_SaveDataPointToGrpName_02")]
        public static System.String GetConfigNameTSS(
        TrendViewer.View.DataPointGroup target)
        {
            return "NotEmpty";
        }

    }
}

