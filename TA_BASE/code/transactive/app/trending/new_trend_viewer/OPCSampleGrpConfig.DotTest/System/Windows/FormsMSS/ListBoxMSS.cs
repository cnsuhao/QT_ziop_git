using System;
using Dottest.Framework;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.ListBox")]
    public class ListBoxMSS
    {
        [MethodSpecificStubsMethod("System.Windows.Forms.ListBox.get_SelectedItems()")]
        public static System.Windows.Forms.ListBox.SelectedObjectCollection get_SelectedItemsMSS(
        System.Windows.Forms.ListBox target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Windows.Forms.ListBox.SelectedObjectCollection)result;
            }
            else
            {
                return target.SelectedItems;
            }
        }

        [TestSpecificStub("System.Windows.Forms.ListBox.get_SelectedItems()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.View.OPCDataSelectorTests), "TestRemoveSelectedDataPoints01")]
        public static System.Windows.Forms.ListBox.SelectedObjectCollection get_SelectedItemsTSS(
        System.Windows.Forms.ListBox target)
        {
            target.Items.Add("Testing");
            target.Items.Add("Testing1");
            target.Items.Add("Testing2");
            System.Windows.Forms.ListBox.SelectedObjectCollection colll = new System.Windows.Forms.ListBox.SelectedObjectCollection(target);
            colll.Add("Testing");
            return colll;
        }

        [MethodSpecificStubsMethod("System.Windows.Forms.ListBox.get_Items()")]
        public static System.Windows.Forms.ListBox.ObjectCollection get_ItemsMSS(
        System.Windows.Forms.ListBox target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Windows.Forms.ListBox.ObjectCollection)result;
            }
            else
            {
                return target.Items;
            }
        }

        [TestSpecificStub("System.Windows.Forms.ListBox.get_Items()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.View.OPCDataSelectorTests), "TestGetSelectedDataPoints01")]
        public static System.Windows.Forms.ListBox.ObjectCollection get_ItemsTSS(
        System.Windows.Forms.ListBox target)
        {
            System.Windows.Forms.ListBox.ObjectCollection objColl = new System.Windows.Forms.ListBox.ObjectCollection(target);
            objColl.Add("Testing");
            objColl.Add("Testing1");
            objColl.Add("Testing2");
            return objColl;
        }

    }
}

