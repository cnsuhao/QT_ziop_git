using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.View.HistDataPointGroup")]
    public class HistDataPointGroupMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.View.HistDataPointGroup.GetConfigName()")]
        public static System.String GetConfigNameMSS(
        TrendViewer.View.HistDataPointGroup target)
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

        [TestSpecificStub("TrendViewer.View.HistDataPointGroup.GetConfigName()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(HistDataPointGroupControllerTests), "TestLoadHistDataPointByGrpName_SaveHistDataPointToGrpName_02")]
        [TestSpecificStubContext(typeof(HistDataPointGroupControllerTests), "TestLoadHistDataPointByGrpName_SaveHistDataPointToGrpName_01")]
        public static System.String GetConfigNameTSS(
        TrendViewer.View.HistDataPointGroup target)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (name.Equals("TestLoadHistDataPointByGrpName_SaveHistDataPointToGrpName_01"))
            {
                return "";
            }else
            {
                return "NotEmptyName";
            }
            
        }

    }
}

