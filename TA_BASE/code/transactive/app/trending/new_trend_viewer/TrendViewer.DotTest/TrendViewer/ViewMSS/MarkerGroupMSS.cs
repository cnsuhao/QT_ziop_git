using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.View.MarkerGroup")]
    public class MarkerGroupMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.View.MarkerGroup.GetConfigName()")]
        public static System.String GetConfigNameMSS(
        TrendViewer.View.MarkerGroup target)
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

        [TestSpecificStub("TrendViewer.View.MarkerGroup.GetConfigName()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(MarkerGroupControllerTests), "TestLoadMarkerByGrpName_SaveMarkerToGrpName_02")]
        public static System.String GetConfigNameTSS(
        TrendViewer.View.MarkerGroup target)
        {
            return "NotEmptyName";
        }

    }
}

