using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ModelMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.Model.OPCDataSelectorModel")]
    public class OPCDataSelectorModelMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.Model.OPCDataSelectorModel.GetDataNodeListByServerRootName(System.String)")]
        public static System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity> GetDataNodeListByServerRootNameMSS(
            /*TrendViewer.Model.OPCDataSelectorModel*/ object target,
        System.String servreRootName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, servreRootName }, out result))
            {
                return (System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity>)result;
            }
            else
            {
                return (System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity>)Helper.CallMethod(target, "GetDataNodeListByServerRootName", new object[] { servreRootName });
            }
        }

        [TestSpecificStub("TrendViewer.Model.OPCDataSelectorModel.GetDataNodeListByServerRootName(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataSelectorControllerTests), "TestInitDataNodeList01")]
        public static System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity> GetDataNodeListByServerRootNameTSS(
            /*TrendViewer.Model.OPCDataSelectorModel*/ object target,
        System.String servreRootName)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

