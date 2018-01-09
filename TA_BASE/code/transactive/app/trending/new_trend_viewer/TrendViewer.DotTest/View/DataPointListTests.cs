using Dottest.Framework;
using Dottest.Framework.RecordState;
using NUnit.Framework;
using System;
using System.Windows.Forms;
using TrendViewer.View;
using System.Data;
using STEE.ISCS.MVC;
using TrendViewer.Controller;

namespace TrendViewer.DotTest.View
{
    [TestFixture()]
    public class DataPointListTests
    {
        private DataPointList dataPointList;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            dataPointList = new DataPointList();
          //  Accessor dataPointListAccessor = ReflectionAccessor.Wrap(dataPointList);
           // dataPointListAccessor.Call("InitializeDataPointListDataGridView", null);
        }
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("+RySHQ", "/+P3gA")]
        public void TestDataPointListConstructor01()
        {
            DataPointList dataPointList = new DataPointList();
            #region Record State
            ValueRecorder recorder = new ValueRecorder();
            recorder.Record((string)dataPointList.AccessibleDefaultActionDescription);
            recorder.Record((string)dataPointList.AccessibleDescription);
            recorder.Record((string)dataPointList.AccessibleName);
            recorder.Record((AccessibleRole)dataPointList.AccessibleRole);
            recorder.Record((bool)dataPointList.AllowDrop);
            recorder.Record((bool)dataPointList.AllowTransparency);
            recorder.Record((AnchorStyles)dataPointList.Anchor);
            recorder.Record((AutoScaleMode)dataPointList.AutoScaleMode);
            recorder.Record((bool)dataPointList.AutoScroll);
            recorder.Record((bool)dataPointList.AutoSize);
            recorder.Record((AutoSizeMode)dataPointList.AutoSizeMode);
            recorder.Record((AutoValidate)dataPointList.AutoValidate);
            recorder.Record((ImageLayout)dataPointList.BackgroundImageLayout);
            recorder.Record((int)dataPointList.Bottom);
            recorder.Record((bool)dataPointList.CanFocus);
            recorder.Record((bool)dataPointList.CanSelect);
            recorder.Record((bool)dataPointList.Capture);
            recorder.Record((bool)dataPointList.CausesValidation);
            recorder.Record((string)dataPointList.CompanyName);
            recorder.Record((bool)dataPointList.ContainsFocus);
            recorder.Record((bool)dataPointList.ControlBox);
            recorder.Record((bool)dataPointList.Created);
            recorder.Record((DialogResult)dataPointList.DialogResult);
            recorder.Record((bool)dataPointList.Disposing);
            recorder.Record((DockStyle)dataPointList.Dock);
            recorder.Record((bool)dataPointList.Enabled);
            recorder.Record((bool)dataPointList.Focused);
            recorder.Record((FormBorderStyle)dataPointList.FormBorderStyle);
            recorder.Record((bool)dataPointList.HasChildren);
            recorder.Record((int)dataPointList.Height);
            recorder.Record((bool)dataPointList.HelpButton);
            recorder.Record((ImeMode)dataPointList.ImeMode);
            recorder.Record((bool)dataPointList.InvokeRequired);
            recorder.Record((bool)dataPointList.IsAccessible);
            recorder.Record((bool)dataPointList.IsDisposed);
            recorder.Record((bool)dataPointList.IsHandleCreated);
            recorder.Record((bool)dataPointList.IsMdiChild);
            recorder.Record((bool)dataPointList.IsMdiContainer);
            recorder.Record((bool)dataPointList.IsMirrored);
            recorder.Record((bool)dataPointList.IsRestrictedWindow);
            recorder.Record((bool)dataPointList.KeyPreview);
            recorder.Record((int)dataPointList.Left);
            recorder.Record((bool)dataPointList.MaximizeBox);
            recorder.Record((bool)dataPointList.MinimizeBox);
            recorder.Record((bool)dataPointList.Modal);
            recorder.Record((string)dataPointList.Name);
            recorder.Record((double)dataPointList.Opacity);
            recorder.Record((string)dataPointList.ProductName);
            recorder.Record((string)dataPointList.ProductVersion);
            recorder.Record((bool)dataPointList.RecreatingHandle);
            recorder.Record((int)dataPointList.Right);
            recorder.Record((RightToLeft)dataPointList.RightToLeft);
            recorder.Record((bool)dataPointList.RightToLeftLayout);
            recorder.Record((bool)dataPointList.ShowIcon);
            recorder.Record((bool)dataPointList.ShowInTaskbar);
            recorder.Record((SizeGripStyle)dataPointList.SizeGripStyle);
            recorder.Record((FormStartPosition)dataPointList.StartPosition);
            recorder.Record((bool)ReflectionAccessor.Wrap(dataPointList).GetProperty("System.Windows.Forms.Layout.IArrangedElement.ParticipatesInLayout"));
            recorder.Record((int)dataPointList.TabIndex);
            recorder.Record((bool)dataPointList.TabStop);
            recorder.Record((string)dataPointList.Text);
            recorder.Record((int)dataPointList.Top);
            recorder.Record((bool)dataPointList.TopLevel);
            recorder.Record((bool)dataPointList.TopMost);
            recorder.Record((bool)dataPointList.UseWaitCursor);
            recorder.Record((string)dataPointList.ViewID);
            recorder.Record((string)dataPointList.ViewType);
            recorder.Record((bool)dataPointList.Visible);
            recorder.Record((int)dataPointList.Width);
            recorder.Record((FormWindowState)dataPointList.WindowState);
            recorder.FinishRecording();
            #endregion
            #region Assertions
            Assert.IsNull((string)dataPointList.AccessibleDefaultActionDescription);
            Assert.IsFalse((bool)dataPointList.AllowDrop);
            Assert.AreEqual(global::System.Windows.Forms.AutoScaleMode.Font, (AutoScaleMode)dataPointList.AutoScaleMode);
            Assert.AreEqual(global::System.Windows.Forms.AutoValidate.EnablePreventFocusChange, (AutoValidate)dataPointList.AutoValidate);
            Assert.IsFalse((bool)dataPointList.CanFocus);
            Assert.AreEqual("ST Electronics Ltd", (string)dataPointList.CompanyName);
            Assert.IsFalse((bool)dataPointList.Created);
            Assert.IsTrue((bool)dataPointList.Enabled);
            Assert.IsTrue((bool)dataPointList.HasChildren);
            Assert.IsFalse((bool)dataPointList.InvokeRequired);
            Assert.IsFalse((bool)dataPointList.IsHandleCreated);
            Assert.IsFalse((bool)dataPointList.IsRestrictedWindow);
            Assert.IsFalse((bool)dataPointList.MaximizeBox);
            Assert.AreEqual(1.0, (double)dataPointList.Opacity, 1E-7);
            Assert.IsFalse((bool)dataPointList.RecreatingHandle);
            Assert.IsTrue((bool)dataPointList.ShowIcon);
            Assert.AreEqual(global::System.Windows.Forms.FormStartPosition.CenterParent, (FormStartPosition)dataPointList.StartPosition);
            Assert.AreEqual("Datapoint Viewer", (string)dataPointList.Text);
            Assert.IsFalse((bool)dataPointList.TopMost);
            Assert.IsFalse((bool)dataPointList.Visible);
            #endregion
        }

        [Test]
        public void TestTranslateCaption()
        {
            dataPointList.TranslateCaption();
        }

        [Test]
        public void TestDisplay01()
        {
            int mode = 1;
            DataTable dataTable = new DataTable("TRENDVIEWER_CONFIG");

            dataTable.Columns.Add("DATA_PT_HOST", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_NAME", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_TYPE", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_COLOR", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_SERVER", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_ENABLED", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_LBL_ENABLED", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_LBL_NAME", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_VISIBLE", System.Type.GetType("System.String"));

            DataRow row = dataTable.NewRow();
            row["DATA_PT_NAME"] = "test_name1";
            row["DATA_PT_HOST"] = "test_host";
            row["DATA_PT_TYPE"] = "Line";
            row["DATA_PT_ENABLED"] = "Y";
            dataTable.Rows.Add(row);

            dataPointList.Display(mode, dataTable);
        }

        [Test]
        public void TestDisplay02()
        {
            //Exception caught
            dataPointList.Display(1, null);
        }

        [Test]
        public void Test_GetDPNameByRowIndex_PopulateDataPointListDataGridView()
        {
            int mode = 1;

            dataPointList.GetDataPointNameByRowIndex(0);
            Accessor dataPointListAccessor = ReflectionAccessor.Wrap(dataPointList);
            DataGridView dataPointListDataGridView = (DataGridView)(dataPointListAccessor.GetField("dataPointListDataGridView"));

             DataTable dataTable = new DataTable("TRENDVIEWER_CONFIG");

            dataTable.Columns.Add("DATA_PT_HOST", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_NAME", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_TYPE", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_COLOR", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_SERVER", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_ENABLED", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_LBL_ENABLED", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_LBL_NAME", System.Type.GetType("System.String"));
            dataTable.Columns.Add("DATA_PT_VISIBLE", System.Type.GetType("System.String"));

            dataPointList.Display(mode, dataTable);

            DataRow row = dataTable.NewRow();
               row["DATA_PT_NAME"] = "test_name1";
               row["DATA_PT_HOST"] = "test_host";
               row["DATA_PT_TYPE"] = "Line";
               row["DATA_PT_ENABLED"]= "Y";
               dataTable.Rows.Add(row);

               dataPointList.GetDataPointNameByRowIndex(0);
              // dataPointListDataGridView.SortedColumn = dataPointListDataGridView.Columns[3];
              
               dataPointList.Display(mode, dataTable);

               row = dataTable.NewRow();
               row["DATA_PT_NAME"] = "test_name2";
               row["DATA_PT_HOST"] = "test_host";
               row["DATA_PT_TYPE"] = "Line";
               row["DATA_PT_ENABLED"] = "N";
               dataTable.Rows.Add(row);
               row = dataTable.NewRow();
               row["DATA_PT_NAME"] = "test_name3";
               row["DATA_PT_HOST"] = "test_host";
               row["DATA_PT_TYPE"] = "Line";
               row["DATA_PT_ENABLED"] = "N";
               dataTable.Rows.Add(row);
               row = dataTable.NewRow();
               row["DATA_PT_NAME"] = "test_name4";
               row["DATA_PT_HOST"] = "test_host";
               row["DATA_PT_TYPE"] = "Line";
               row["DATA_PT_ENABLED"] = "N";
               dataTable.Rows.Add(row);
              row = dataTable.NewRow();
               row["DATA_PT_NAME"] = "test_name5";
               row["DATA_PT_HOST"] = "test_host";
               row["DATA_PT_TYPE"] = "Line";
               row["DATA_PT_ENABLED"] = "N";
               dataTable.Rows.Add(row);
               row = dataTable.NewRow();
               row["DATA_PT_NAME"] = "test_name6";
               row["DATA_PT_HOST"] = "test_host";
               row["DATA_PT_TYPE"] = "Line";
               row["DATA_PT_ENABLED"] = "N";
               dataTable.Rows.Add(row);
               row = dataTable.NewRow();
               row["DATA_PT_NAME"] = "test_name7";
               row["DATA_PT_HOST"] = "test_host";
               row["DATA_PT_TYPE"] = "Line";
               row["DATA_PT_ENABLED"] = "N";
               row["DATA_PT_LBL_ENABLED"] = "N";
               row["DATA_PT_VISIBLE"] = "Y";
               row["DATA_PT_LBL_NAME"] = "abc";
               dataTable.Rows.Add(row);

               
               dataPointList.Display(mode, dataTable);

               row = dataTable.NewRow();
               row["DATA_PT_NAME"] = "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
               row["DATA_PT_HOST"] = "test_host";
               row["DATA_PT_TYPE"] = "Line";
               row["DATA_PT_ENABLED"] = "N";
               row["DATA_PT_LBL_ENABLED"] = "Y";
               row["DATA_PT_VISIBLE"] = "N";
               //DATA_PT_LBL_NAME.size=120:
               row["DATA_PT_LBL_NAME"] = "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
               row["DATA_PT_COLOR"] = System.Drawing.Color.Transparent.ToArgb().ToString();
               dataTable.Rows.Add(row);

               
               //dataPointListDataGridView.SortOrder = SortOrder.Descending;
               //dataPointListDataGridView.SortedColumn.Name = "DATA_PT_LBL_NAME";
               dataPointList.Display(mode, dataTable);
        }
        
        [Test]
        public void TestDataPointViewer_Paint()
        {
            Accessor viewAccessor = ReflectionAccessor.Wrap(dataPointList);
            viewAccessor.Call("DataPointViewer_Paint", null, null);
        }
        [Test]
        public void TestAttachListener()
        {
            IController ctl = new DataPointListController();
            dataPointList.AttachListener(ctl);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("+Tceyw", "+LwQ/g")]
        public void TestfirstdataPointListDataGridView_Click01()
        {
            object sender = null;
            EventArgs e = null;
            Accessor dataPointListAccessor = ReflectionAccessor.Wrap(dataPointList);
            dataPointListAccessor.Call("firstdataPointListDataGridView_Click", sender, e);
            #region Record State
            ValueRecorder recorder = new ValueRecorder();
            recorder.Record((string)dataPointList.AccessibleDefaultActionDescription);
            recorder.Record((string)dataPointList.AccessibleDescription);
            recorder.Record((string)dataPointList.AccessibleName);
            recorder.Record((AccessibleRole)dataPointList.AccessibleRole);
            recorder.Record((bool)dataPointList.AllowDrop);
            recorder.Record((bool)dataPointList.AllowTransparency);
            recorder.Record((AnchorStyles)dataPointList.Anchor);
            recorder.Record((AutoScaleMode)dataPointList.AutoScaleMode);
            recorder.Record((bool)dataPointList.AutoScroll);
            recorder.Record((bool)dataPointList.AutoSize);
            recorder.Record((AutoSizeMode)dataPointList.AutoSizeMode);
            recorder.Record((AutoValidate)dataPointList.AutoValidate);
            recorder.Record((ImageLayout)dataPointList.BackgroundImageLayout);
            recorder.Record((int)dataPointList.Bottom);
            recorder.Record((bool)dataPointList.CanFocus);
            recorder.Record((bool)dataPointList.CanSelect);
            recorder.Record((bool)dataPointList.Capture);
            recorder.Record((bool)dataPointList.CausesValidation);
            recorder.Record((string)dataPointList.CompanyName);
            recorder.Record((bool)dataPointList.ContainsFocus);
            recorder.Record((bool)dataPointList.ControlBox);
            recorder.Record((bool)dataPointList.Created);
            recorder.Record((DialogResult)dataPointList.DialogResult);
            recorder.Record((bool)dataPointList.Disposing);
            recorder.Record((DockStyle)dataPointList.Dock);
            recorder.Record((bool)dataPointList.Enabled);
            recorder.Record((bool)dataPointList.Focused);
            recorder.Record((FormBorderStyle)dataPointList.FormBorderStyle);
            recorder.Record((bool)dataPointList.HasChildren);
            recorder.Record((int)dataPointList.Height);
            recorder.Record((bool)dataPointList.HelpButton);
            recorder.Record((ImeMode)dataPointList.ImeMode);
            recorder.Record((bool)dataPointList.InvokeRequired);
            recorder.Record((bool)dataPointList.IsAccessible);
            recorder.Record((bool)dataPointList.IsDisposed);
            recorder.Record((bool)dataPointList.IsHandleCreated);
            recorder.Record((bool)dataPointList.IsMdiChild);
            recorder.Record((bool)dataPointList.IsMdiContainer);
            recorder.Record((bool)dataPointList.IsMirrored);
            recorder.Record((bool)dataPointList.IsRestrictedWindow);
            recorder.Record((bool)dataPointList.KeyPreview);
            recorder.Record((int)dataPointList.Left);
            recorder.Record((bool)dataPointList.MaximizeBox);
            recorder.Record((bool)dataPointList.MinimizeBox);
            recorder.Record((bool)dataPointList.Modal);
            recorder.Record((string)dataPointList.Name);
            recorder.Record((double)dataPointList.Opacity);
            recorder.Record((string)dataPointList.ProductName);
            recorder.Record((string)dataPointList.ProductVersion);
            recorder.Record((bool)dataPointList.RecreatingHandle);
            recorder.Record((int)dataPointList.Right);
            recorder.Record((RightToLeft)dataPointList.RightToLeft);
            recorder.Record((bool)dataPointList.RightToLeftLayout);
            recorder.Record((bool)dataPointList.ShowIcon);
            recorder.Record((bool)dataPointList.ShowInTaskbar);
            recorder.Record((SizeGripStyle)dataPointList.SizeGripStyle);
            recorder.Record((FormStartPosition)dataPointList.StartPosition);
            recorder.Record((bool)dataPointListAccessor.GetProperty("System.Windows.Forms.Layout.IArrangedElement.ParticipatesInLayout"));
            recorder.Record((int)dataPointList.TabIndex);
            recorder.Record((bool)dataPointList.TabStop);
            recorder.Record((string)dataPointList.Text);
            recorder.Record((int)dataPointList.Top);
            recorder.Record((bool)dataPointList.TopLevel);
            recorder.Record((bool)dataPointList.TopMost);
            recorder.Record((bool)dataPointList.UseWaitCursor);
            recorder.Record((string)dataPointList.ViewID);
            recorder.Record((string)dataPointList.ViewType);
            recorder.Record((bool)dataPointList.Visible);
            recorder.Record((int)dataPointList.Width);
            recorder.Record((FormWindowState)dataPointList.WindowState);
            recorder.FinishRecording();
            #endregion
            #region Assertions
            Assert.IsNull((string)dataPointList.AccessibleDefaultActionDescription);
            Assert.IsFalse((bool)dataPointList.AllowDrop);
            Assert.AreEqual(global::System.Windows.Forms.AutoScaleMode.Font, (AutoScaleMode)dataPointList.AutoScaleMode);
            Assert.AreEqual(global::System.Windows.Forms.AutoValidate.EnablePreventFocusChange, (AutoValidate)dataPointList.AutoValidate);
            Assert.IsFalse((bool)dataPointList.CanFocus);
            Assert.AreEqual("ST Electronics Ltd", (string)dataPointList.CompanyName);
            Assert.IsFalse((bool)dataPointList.Created);
            Assert.IsTrue((bool)dataPointList.Enabled);
            Assert.IsTrue((bool)dataPointList.HasChildren);
            Assert.IsFalse((bool)dataPointList.InvokeRequired);
            Assert.IsFalse((bool)dataPointList.IsHandleCreated);
            Assert.IsFalse((bool)dataPointList.IsRestrictedWindow);
            Assert.IsFalse((bool)dataPointList.MaximizeBox);
            Assert.AreEqual(1.0, (double)dataPointList.Opacity, 1E-7);
            Assert.IsFalse((bool)dataPointList.RecreatingHandle);
            Assert.IsTrue((bool)dataPointList.ShowIcon);
            Assert.AreEqual(global::System.Windows.Forms.FormStartPosition.CenterParent, (FormStartPosition)dataPointList.StartPosition);
            Assert.AreEqual("Datapoint Viewer", (string)dataPointList.Text);
            Assert.IsFalse((bool)dataPointList.TopMost);
            Assert.IsFalse((bool)dataPointList.Visible);
            #endregion
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("+cdhSQ", "+MFGvg")]
        public void TestlastdataPointListDataGridView_Click01()
        {
            object sender = null;
            EventArgs e = null;
            Accessor dataPointListAccessor = ReflectionAccessor.Wrap(dataPointList);
            dataPointListAccessor.Call("lastdataPointListDataGridView_Click", sender, e);
            #region Record State
            ValueRecorder recorder = new ValueRecorder();
            recorder.Record((string)dataPointList.AccessibleDefaultActionDescription);
            recorder.Record((string)dataPointList.AccessibleDescription);
            recorder.Record((string)dataPointList.AccessibleName);
            recorder.Record((AccessibleRole)dataPointList.AccessibleRole);
            recorder.Record((bool)dataPointList.AllowDrop);
            recorder.Record((bool)dataPointList.AllowTransparency);
            recorder.Record((AnchorStyles)dataPointList.Anchor);
            recorder.Record((AutoScaleMode)dataPointList.AutoScaleMode);
            recorder.Record((bool)dataPointList.AutoScroll);
            recorder.Record((bool)dataPointList.AutoSize);
            recorder.Record((AutoSizeMode)dataPointList.AutoSizeMode);
            recorder.Record((AutoValidate)dataPointList.AutoValidate);
            recorder.Record((ImageLayout)dataPointList.BackgroundImageLayout);
            recorder.Record((int)dataPointList.Bottom);
            recorder.Record((bool)dataPointList.CanFocus);
            recorder.Record((bool)dataPointList.CanSelect);
            recorder.Record((bool)dataPointList.Capture);
            recorder.Record((bool)dataPointList.CausesValidation);
            recorder.Record((string)dataPointList.CompanyName);
            recorder.Record((bool)dataPointList.ContainsFocus);
            recorder.Record((bool)dataPointList.ControlBox);
            recorder.Record((bool)dataPointList.Created);
            recorder.Record((DialogResult)dataPointList.DialogResult);
            recorder.Record((bool)dataPointList.Disposing);
            recorder.Record((DockStyle)dataPointList.Dock);
            recorder.Record((bool)dataPointList.Enabled);
            recorder.Record((bool)dataPointList.Focused);
            recorder.Record((FormBorderStyle)dataPointList.FormBorderStyle);
            recorder.Record((bool)dataPointList.HasChildren);
            recorder.Record((int)dataPointList.Height);
            recorder.Record((bool)dataPointList.HelpButton);
            recorder.Record((ImeMode)dataPointList.ImeMode);
            recorder.Record((bool)dataPointList.InvokeRequired);
            recorder.Record((bool)dataPointList.IsAccessible);
            recorder.Record((bool)dataPointList.IsDisposed);
            recorder.Record((bool)dataPointList.IsHandleCreated);
            recorder.Record((bool)dataPointList.IsMdiChild);
            recorder.Record((bool)dataPointList.IsMdiContainer);
            recorder.Record((bool)dataPointList.IsMirrored);
            recorder.Record((bool)dataPointList.IsRestrictedWindow);
            recorder.Record((bool)dataPointList.KeyPreview);
            recorder.Record((int)dataPointList.Left);
            recorder.Record((bool)dataPointList.MaximizeBox);
            recorder.Record((bool)dataPointList.MinimizeBox);
            recorder.Record((bool)dataPointList.Modal);
            recorder.Record((string)dataPointList.Name);
            recorder.Record((double)dataPointList.Opacity);
            recorder.Record((string)dataPointList.ProductName);
            recorder.Record((string)dataPointList.ProductVersion);
            recorder.Record((bool)dataPointList.RecreatingHandle);
            recorder.Record((int)dataPointList.Right);
            recorder.Record((RightToLeft)dataPointList.RightToLeft);
            recorder.Record((bool)dataPointList.RightToLeftLayout);
            recorder.Record((bool)dataPointList.ShowIcon);
            recorder.Record((bool)dataPointList.ShowInTaskbar);
            recorder.Record((SizeGripStyle)dataPointList.SizeGripStyle);
            recorder.Record((FormStartPosition)dataPointList.StartPosition);
            recorder.Record((bool)dataPointListAccessor.GetProperty("System.Windows.Forms.Layout.IArrangedElement.ParticipatesInLayout"));
            recorder.Record((int)dataPointList.TabIndex);
            recorder.Record((bool)dataPointList.TabStop);
            recorder.Record((string)dataPointList.Text);
            recorder.Record((int)dataPointList.Top);
            recorder.Record((bool)dataPointList.TopLevel);
            recorder.Record((bool)dataPointList.TopMost);
            recorder.Record((bool)dataPointList.UseWaitCursor);
            recorder.Record((string)dataPointList.ViewID);
            recorder.Record((string)dataPointList.ViewType);
            recorder.Record((bool)dataPointList.Visible);
            recorder.Record((int)dataPointList.Width);
            recorder.Record((FormWindowState)dataPointList.WindowState);
            recorder.FinishRecording();
            #endregion
            #region Assertions
            Assert.IsNull((string)dataPointList.AccessibleDefaultActionDescription);
            Assert.IsFalse((bool)dataPointList.AllowDrop);
            Assert.AreEqual(global::System.Windows.Forms.AutoScaleMode.Font, (AutoScaleMode)dataPointList.AutoScaleMode);
            Assert.AreEqual(global::System.Windows.Forms.AutoValidate.EnablePreventFocusChange, (AutoValidate)dataPointList.AutoValidate);
            Assert.IsFalse((bool)dataPointList.CanFocus);
            Assert.AreEqual("ST Electronics Ltd", (string)dataPointList.CompanyName);
            Assert.IsFalse((bool)dataPointList.Created);
            Assert.IsTrue((bool)dataPointList.Enabled);
            Assert.IsTrue((bool)dataPointList.HasChildren);
            Assert.IsFalse((bool)dataPointList.InvokeRequired);
            Assert.IsFalse((bool)dataPointList.IsHandleCreated);
            Assert.IsFalse((bool)dataPointList.IsRestrictedWindow);
            Assert.IsFalse((bool)dataPointList.MaximizeBox);
            Assert.AreEqual(1.0, (double)dataPointList.Opacity, 1E-7);
            Assert.IsFalse((bool)dataPointList.RecreatingHandle);
            Assert.IsTrue((bool)dataPointList.ShowIcon);
            Assert.AreEqual(global::System.Windows.Forms.FormStartPosition.CenterParent, (FormStartPosition)dataPointList.StartPosition);
            Assert.AreEqual("Datapoint Viewer", (string)dataPointList.Text);
            Assert.IsFalse((bool)dataPointList.TopMost);
            Assert.IsFalse((bool)dataPointList.Visible);
            #endregion
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("+E9s5w", "+8+hpA")]
        public void TestnextdataPointListDataGridView_Click01()
        {
            object sender = null;
            EventArgs e = null;
            Accessor dataPointListAccessor = ReflectionAccessor.Wrap(dataPointList);
            System.Windows.Forms.NumericUpDown ud = (System.Windows.Forms.NumericUpDown)dataPointListAccessor.GetField("pageNumDataPointListDataGridView");
            ud = new System.Windows.Forms.NumericUpDown();
            ud.Minimum = 1;
            ud.Maximum = 3;
            ud.Value = 2;
            dataPointListAccessor.SetField("pageNumDataPointListDataGridView", ud);
            
            dataPointListAccessor.Call("nextdataPointListDataGridView_Click", sender, e);

        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("+pgHsQ", "+ztXvA")]
        public void TestprevdataPointListDataGridView_Click01()
        {

            object sender = null;
            EventArgs e = null;
            Accessor dataPointListAccessor = ReflectionAccessor.Wrap(dataPointList);
            System.Windows.Forms.NumericUpDown ud = (System.Windows.Forms.NumericUpDown)dataPointListAccessor.GetField("pageNumDataPointListDataGridView");
            ud = new System.Windows.Forms.NumericUpDown();
            ud.Minimum = 1;
            ud.Maximum = 3;
            ud.Value = 2;
            dataPointListAccessor.SetField("pageNumDataPointListDataGridView", ud);
            dataPointListAccessor.Call("prevdataPointListDataGridView_Click", sender, e);

        }

        [Test]
        public void TestDispose()
        {
            Accessor formAccessor = ReflectionAccessor.Wrap(dataPointList);
            bool disposing = true;
            formAccessor.SetField("components", new System.ComponentModel.Container());
            formAccessor.Call("Dispose", disposing);
        }

        [Test]
        public void TestInitializeDataPointListDataGridView()
        {
            //Exception
            Accessor formAccessor = ReflectionAccessor.Wrap(dataPointList);
            formAccessor.Call("InitializeDataPointListDataGridView");
        }


    }
}