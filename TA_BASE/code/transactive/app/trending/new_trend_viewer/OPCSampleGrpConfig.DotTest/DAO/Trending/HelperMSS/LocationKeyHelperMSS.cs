using System;
using Dottest.Framework;

namespace DAO.Trending.HelperMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.Helper.LocationKeyHelper")]
    public class LocationKeyHelperMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.Helper.LocationKeyHelper.get_IsOCC()")]
        public static System.Boolean get_IsOCCMSS(
        DAO.Trending.Helper.LocationKeyHelper target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.IsOCC;
            }
        }

        [TestSpecificStub("DAO.Trending.Helper.LocationKeyHelper.get_IsOCC()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestCanAddDataPoint03")]
        public static System.Boolean get_IsOCCTSS(
        DAO.Trending.Helper.LocationKeyHelper target)
        {
            return true;
        }

    }
}

