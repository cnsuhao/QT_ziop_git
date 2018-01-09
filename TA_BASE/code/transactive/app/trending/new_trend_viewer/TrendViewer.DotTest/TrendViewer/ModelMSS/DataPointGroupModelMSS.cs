using System;
using Dottest.Framework;
using TrendViewer.Controller;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ModelMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.Model.DataPointGroupModel")]
    public class DataPointGroupModelMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.Model.DataPointGroupModel.GetAllDPGrp()")]
        public static System.Collections.Generic.List<System.String> GetAllDPGrpMSS(
            /*TrendViewer.Model.DataPointGroupModel*/ object target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Collections.Generic.List<System.String>)result;
            }
            else
            {
                return (System.Collections.Generic.List<System.String>)Helper.CallMethod(target, "GetAllDPGrp", new object[] { });
            }
        }

        [TestSpecificStub("TrendViewer.Model.DataPointGroupModel.GetAllDPGrp()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DataPointGroupControllerTests), "TestInitDPGrp01")]
        public static System.Collections.Generic.List<System.String> GetAllDPGrpTSS(
            /*TrendViewer.Model.DataPointGroupModel*/ object target)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

