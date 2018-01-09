using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.DataGridViewColumnCollection")]
    public class DataGridViewColumnCollectionMSS
    {
        [MethodSpecificStubsMethod("System.Windows.Forms.DataGridViewColumnCollection.Add(System.String,System.String)")]
        public static System.Int32 AddMSS(
        System.Windows.Forms.DataGridViewColumnCollection target,
        System.String columnName, System.String headerText)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, columnName, headerText }, out result))
            {
                return (System.Int32)result;
            }
            else
            {
                return target.Add(columnName, headerText);
            }
        }

        [TestSpecificStub("System.Windows.Forms.DataGridViewColumnCollection.Add(System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCDataSelectorTests), "TestInitializeOpcItemDataGridView02")]
        [TestSpecificStubContext(typeof(MarkerListTests), "TestInitializeDataMarkerListDataGridView02")]
        [TestSpecificStubContext(typeof(HistDataPointListTests), "TestInitializeDataPointListDataGridView02")]
        [TestSpecificStubContext(typeof(DataPointListTests), "TestInitializeDataPointListDataGridView")]
        [TestSpecificStubContext(typeof(FormulaListTests), "TestInitializeDataPointListDataGridView02")]
        public static System.Int32 AddTSS(
        System.Windows.Forms.DataGridViewColumnCollection target,
        System.String columnName, System.String headerText)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

