using System;
using Dottest.Framework;

namespace OPCSampleGrpConfig.ViewMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCSampleGrpConfig")]
    [MethodSpecificStubsClassAttribute("OPCSampleGrpConfig.View.OPCSampleGrpConfigStart")]
    public class OPCSampleGrpConfigStartMSS
    {
        [MethodSpecificStubsMethod("OPCSampleGrpConfig.View.OPCSampleGrpConfigStart.GetEditFields()")]
        public static Entity.Trending.EtyDataLogDPGroupTrend GetEditFieldsMSS(
        OPCSampleGrpConfig.View.OPCSampleGrpConfigStart target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (Entity.Trending.EtyDataLogDPGroupTrend)result;
            }
            else
            {
                return target.GetEditFields();
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.View.OPCSampleGrpConfigStart.GetEditFields()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestSaveCommand01")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestSaveCommand02")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestSaveCommand03")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestSaveCommand04")]
        public static Entity.Trending.EtyDataLogDPGroupTrend GetEditFieldsTSS(
        OPCSampleGrpConfig.View.OPCSampleGrpConfigStart target)
        {
            Entity.Trending.EtyDataLogDPGroupTrend grp = new Entity.Trending.EtyDataLogDPGroupTrend();
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (name.Equals("TestSaveCommand01"))
            {
                return null;
            }
            else if (name.Equals("TestSaveCommand02"))
            {
                grp.SampleGrpName = "testing";
                grp.SampleGrpID = 1;
                grp.NewData = true;
            }
            else if (name.Equals("TestSaveCommand03"))
            {
                grp.SampleGrpName = "testing123445";
                grp.NewData = true;
            }
            return grp;
        }

        [MethodSpecificStubsMethod("OPCSampleGrpConfig.View.OPCSampleGrpConfigStart.PopulateIntervalConfigDataGridView(System.Int32)")]
        public static void PopulateIntervalConfigDataGridViewMSS(
        OPCSampleGrpConfig.View.OPCSampleGrpConfigStart target,
        System.Int32 mode)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, mode }, out result))
            {
                return;
            }
            else
            {
                target.PopulateIntervalConfigDataGridView(mode);
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.View.OPCSampleGrpConfigStart.PopulateIntervalConfigDataGridView(System.Int32)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestSaveCommand03")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestIntervalConfigDataGridView_CellDoubleClick04")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestIntervalConfigDataGridView_CellDoubleClick03")]
        public static void PopulateIntervalConfigDataGridViewTSS(
        OPCSampleGrpConfig.View.OPCSampleGrpConfigStart target,
        System.Int32 mode)
        {
            return;
        }

        [MethodSpecificStubsMethod("OPCSampleGrpConfig.View.OPCSampleGrpConfigStart.GetDataGridValue(System.Int32,System.Int32)")]
        public static System.String GetDataGridValueMSS(
        OPCSampleGrpConfig.View.OPCSampleGrpConfigStart target,
        System.Int32 nRow, System.Int32 nColumn)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, nRow, nColumn }, out result))
            {
                return (System.String)result;
            }
            else
            {
                return target.GetDataGridValue(nRow, nColumn);
            }
        }

        [TestSpecificStub("OPCSampleGrpConfig.View.OPCSampleGrpConfigStart.GetDataGridValue(System.Int32,System.Int32)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestIntervalConfigDataGridView_CellDoubleClick01")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestIntervalConfigDataGridView_CellDoubleClick02")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestIntervalConfigDataGridView_CellDoubleClick03")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestIntervalConfigDataGridView_CellDoubleClick04")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestIntervalConfigDataGridView_CellDoubleClick05")]
        public static System.String GetDataGridValueTSS(
        OPCSampleGrpConfig.View.OPCSampleGrpConfigStart target,
        System.Int32 nRow, System.Int32 nColumn)
        {
            return "1";
        }

    }
}

