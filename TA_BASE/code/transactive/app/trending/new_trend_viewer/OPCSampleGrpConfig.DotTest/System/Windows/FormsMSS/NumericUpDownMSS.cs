using System;
using Dottest.Framework;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.NumericUpDown")]
    public class NumericUpDownMSS
    {
        [MethodSpecificStubsMethod("System.Windows.Forms.NumericUpDown.get_Value()")]
        public static System.Decimal get_ValueMSS(
        System.Windows.Forms.NumericUpDown target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Decimal)result;
            }
            else
            {
                return target.Value;
            }
        }

        [TestSpecificStub("System.Windows.Forms.NumericUpDown.get_Value()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.View.OPCSampleGrpConfigStartTests), "TestprevIntervalConfigDataGridView_Click01")]
        public static System.Decimal get_ValueTSS(
        System.Windows.Forms.NumericUpDown target)
        {
            return (decimal)2;
        }

        [MethodSpecificStubsMethod("System.Windows.Forms.NumericUpDown.set_Value(System.Decimal)")]
        public static void set_ValueMSS(
        System.Windows.Forms.NumericUpDown target,
        System.Decimal value_1)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, value_1 }, out result))
            {
                return;
            }
            else
            {
                target.Value = value_1;
            }
        }

        [TestSpecificStub("System.Windows.Forms.NumericUpDown.set_Value(System.Decimal)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static void set_ValueTSS(
        System.Windows.Forms.NumericUpDown target,
        System.Decimal value_1)
        {
            target.Value = value_1;
        }

        [MethodSpecificStubsMethod("System.Windows.Forms.NumericUpDown.get_Maximum()")]
        public static System.Decimal get_MaximumMSS(
        System.Windows.Forms.NumericUpDown target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Decimal)result;
            }
            else
            {
                return target.Maximum;
            }
        }

        [TestSpecificStub("System.Windows.Forms.NumericUpDown.get_Maximum()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        //[TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.View.OPCSampleGrpConfigStartTests), "TestnextIntervalConfigDataGridView_Click01")]
        public static System.Decimal get_MaximumTSS(
        System.Windows.Forms.NumericUpDown target)
        {
            return (decimal)3;
        }

        [MethodSpecificStubsMethod("System.Windows.Forms.NumericUpDown.set_Maximum(System.Decimal)")]
        public static void set_MaximumMSS(
        System.Windows.Forms.NumericUpDown target,
        System.Decimal value_1)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, value_1 }, out result))
            {
                return;
            }
            else
            {
                target.Maximum = value_1;
            }
        }

        [TestSpecificStub("System.Windows.Forms.NumericUpDown.set_Maximum(System.Decimal)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        public static void set_MaximumTSS(
        System.Windows.Forms.NumericUpDown target,
        System.Decimal value_1)
        {
            target.Maximum = value_1;
        }

    }
}

