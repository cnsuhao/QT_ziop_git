using System;
using Dottest.Framework;
using TrendViewer.DotTest.Controller;

namespace TrendViewer.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.View.FormulaGroup")]
    public class FormulaGroupMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.View.FormulaGroup.GetConfigName()")]
        public static System.String GetConfigNameMSS(
        TrendViewer.View.FormulaGroup target)
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

        [TestSpecificStub("TrendViewer.View.FormulaGroup.GetConfigName()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
         [TestSpecificStubContext(typeof(FormulaGroupControllerTests), "TestLoadFormulaByGrpName_SaveFormulaToGrpName_02" )]
        public static System.String GetConfigNameTSS(
        TrendViewer.View.FormulaGroup target)
        {
            return "Not_Empty_Name";
        }

    }
}

