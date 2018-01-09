using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace STEE.ISCS.MVCMSS
{
    [MethodSpecificStubsAssemblyAttribute("STEE.ISCS.MVC")]
    [MethodSpecificStubsClassAttribute("STEE.ISCS.MVC.NotifyManager")]
    public class NotifyManagerMSS
    {
        [MethodSpecificStubsMethod("STEE.ISCS.MVC.NotifyManager.Send(System.String,System.String,System.Object)")]
        public static void SendMSS(
        STEE.ISCS.MVC.NotifyManager target,
        System.String type, System.String name, System.Object body)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, type, name, body }, out result))
            {
                return;
            }
            else
            {
                target.Send(type, name, body);
            }
        }

        [TestSpecificStub("STEE.ISCS.MVC.NotifyManager.Send(System.String,System.String,System.Object)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(ChangeTrendTitleControllerTests), "TestModifyTitles")]
        [TestSpecificStubContext(typeof(DataPointGroupControllerTests), "TestLoadDataPointByGrpName_SaveDataPointToGrpName_02")]
        [TestSpecificStubContext(typeof(DataPointListControllerTests), "TestFormClose")]
        [TestSpecificStubContext(typeof(FormulaDataControllerTests), "TestAddFormulaData_ModifyFormulaData_01")]
        [TestSpecificStubContext(typeof(FormulaDataControllerTests), "TestAddFormulaData_ModifyFormulaData_02")]
        [TestSpecificStubContext(typeof(FormulaDataControllerTests), "TestAddFormulaData_ModifyFormulaData_03")]
        public static void SendTSS(
        STEE.ISCS.MVC.NotifyManager target,
        System.String type, System.String name, System.Object body)
        {
            return;
        }

    }
}

