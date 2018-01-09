using System;
using Dottest.Framework;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.DataGridView")]
    public class DataGridViewMSS
    {
        [MethodSpecificStubsMethod("System.Windows.Forms.DataGridView.get_SortedColumn()")]
        public static System.Windows.Forms.DataGridViewColumn get_SortedColumnMSS(
        System.Windows.Forms.DataGridView target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Windows.Forms.DataGridViewColumn)result;
            }
            else
            {
                return target.SortedColumn;
            }
        }

        [TestSpecificStub("System.Windows.Forms.DataGridView.get_SortedColumn()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.View.OPCSampleGrpConfigStartTests), "TestLoadPageNumber01")]
        public static System.Windows.Forms.DataGridViewColumn get_SortedColumnTSS(
        System.Windows.Forms.DataGridView target)
        {
            System.Windows.Forms.DataGridViewColumn columnsortt = new System.Windows.Forms.DataGridViewColumn();
            columnsortt.Name = OPCSampleGrpConfig.View.OPCSampleGrpConfigStart.OPCSAMPLEGRPDESC_COL_NAME;
            return columnsortt;
        }

        [MethodSpecificStubsMethod("System.Windows.Forms.DataGridView.get_SelectedRows()")]
        public static System.Windows.Forms.DataGridViewSelectedRowCollection get_SelectedRowsMSS(
        System.Windows.Forms.DataGridView target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Windows.Forms.DataGridViewSelectedRowCollection)result;
            }
            else
            {
                return target.SelectedRows;
            }
        }

        [TestSpecificStub("System.Windows.Forms.DataGridView.get_SelectedRows()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.View.OPCDataSelectorTests), "TestAddSelectedDataPoints01")]
        public static System.Windows.Forms.DataGridViewSelectedRowCollection get_SelectedRowsTSS(
        System.Windows.Forms.DataGridView target)
        {
            System.Windows.Forms.DataGridViewSelectedRowCollection selectedRows = target.SelectedRows; ;
            // System.Windows.Forms.DataGridViewSelectedRowCollection selectedRows = new System.Windows.Forms.DataGridViewSelectedRowCollection;
            //Accessor acc = ReflectionAccessor.Wrap(selectedRows);
            target.Rows.Add();
            System.Windows.Forms.DataGridViewRow row1 = target.Rows[0];
            row1.Cells[0].Value = "1";
            //System.Windows.Forms.DataGridViewRow[] array = new System.Windows.Forms.DataGridViewRow[1];
            // array.SetValue(row1, 0);

            // acc.Call("Insert", 0, row1);
            selectedRows.Insert(0, row1);
            return selectedRows;
        } 

    }
}

