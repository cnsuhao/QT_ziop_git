using Dottest.Framework;
using Dottest.Framework.RecordState;
using NUnit.Framework;
using System;
using System.Windows.Forms;
using TrendViewer.View;

namespace TrendViewer.DotTest.Factories.View
{
    [TestFixture(), ObjectFactoryType()]
    public class DataPointGroupFactory
    {
        ///<summary>A caller is responsible for disposing the result</summary>
        [ObjectFactoryMethod]
        [HashCode("+7Xb6A", "+lapiA")]
        public static DataPointGroup CreateDataPointGroup01()
        {
            DataPointGroup dataPointGroup = new DataPointGroup();
            return dataPointGroup;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateDataPointGroup01()
        {
            using (DataPointGroup dataPointGroup = CreateDataPointGroup01())
            {
                Assert.IsNotNull(dataPointGroup);
                #region Record State
                ValueRecorder recorder = new ValueRecorder();
                recorder.Record((string)dataPointGroup.AccessibleDefaultActionDescription);
                recorder.Record((string)dataPointGroup.AccessibleDescription);
                recorder.Record((string)dataPointGroup.AccessibleName);
                recorder.Record((AccessibleRole)dataPointGroup.AccessibleRole);
                recorder.Record((bool)dataPointGroup.AllowDrop);
                recorder.Record((bool)dataPointGroup.AllowTransparency);
                recorder.Record((AnchorStyles)dataPointGroup.Anchor);
                recorder.Record((AutoScaleMode)dataPointGroup.AutoScaleMode);
                recorder.Record((bool)dataPointGroup.AutoScroll);
                recorder.Record((bool)dataPointGroup.AutoSize);
                recorder.Record((AutoSizeMode)dataPointGroup.AutoSizeMode);
                recorder.Record((AutoValidate)dataPointGroup.AutoValidate);
                recorder.Record((ImageLayout)dataPointGroup.BackgroundImageLayout);
                recorder.Record((int)dataPointGroup.Bottom);
                recorder.Record((bool)dataPointGroup.CanFocus);
                recorder.Record((bool)dataPointGroup.CanSelect);
                recorder.Record((bool)dataPointGroup.Capture);
                recorder.Record((bool)dataPointGroup.CausesValidation);
                recorder.Record((string)dataPointGroup.CompanyName);
                recorder.Record((bool)dataPointGroup.ContainsFocus);
                recorder.Record((bool)dataPointGroup.ControlBox);
                recorder.Record((bool)dataPointGroup.Created);
                recorder.Record((DialogResult)dataPointGroup.DialogResult);
                recorder.Record((bool)dataPointGroup.Disposing);
                recorder.Record((DockStyle)dataPointGroup.Dock);
                recorder.Record((bool)dataPointGroup.Enabled);
                recorder.Record((bool)dataPointGroup.Focused);
                recorder.Record((FormBorderStyle)dataPointGroup.FormBorderStyle);
                recorder.Record((bool)dataPointGroup.HasChildren);
                recorder.Record((int)dataPointGroup.Height);
                recorder.Record((bool)dataPointGroup.HelpButton);
                recorder.Record((ImeMode)dataPointGroup.ImeMode);
                recorder.Record((bool)dataPointGroup.InvokeRequired);
                recorder.Record((bool)dataPointGroup.IsAccessible);
                recorder.Record((bool)dataPointGroup.IsDisposed);
                recorder.Record((bool)dataPointGroup.IsHandleCreated);
                recorder.Record((bool)dataPointGroup.IsMdiChild);
                recorder.Record((bool)dataPointGroup.IsMdiContainer);
                recorder.Record((bool)dataPointGroup.IsMirrored);
                recorder.Record((bool)dataPointGroup.IsRestrictedWindow);
                recorder.Record((bool)dataPointGroup.KeyPreview);
                recorder.Record((int)dataPointGroup.Left);
                recorder.Record((bool)dataPointGroup.MaximizeBox);
                recorder.Record((bool)dataPointGroup.MinimizeBox);
                recorder.Record((bool)dataPointGroup.Modal);
                recorder.Record((string)dataPointGroup.Name);
                recorder.Record((double)dataPointGroup.Opacity);
                recorder.Record((string)dataPointGroup.ProductName);
                recorder.Record((string)dataPointGroup.ProductVersion);
                recorder.Record((bool)dataPointGroup.RecreatingHandle);
                recorder.Record((int)dataPointGroup.Right);
                recorder.Record((RightToLeft)dataPointGroup.RightToLeft);
                recorder.Record((bool)dataPointGroup.RightToLeftLayout);
                recorder.Record((bool)dataPointGroup.ShowIcon);
                recorder.Record((bool)dataPointGroup.ShowInTaskbar);
                recorder.Record((SizeGripStyle)dataPointGroup.SizeGripStyle);
                recorder.Record((FormStartPosition)dataPointGroup.StartPosition);
                recorder.Record((bool)ReflectionAccessor.Wrap(dataPointGroup).GetProperty("System.Windows.Forms.Layout.IArrangedElement.ParticipatesInLayout"));
                recorder.Record((int)dataPointGroup.TabIndex);
                recorder.Record((bool)dataPointGroup.TabStop);
                recorder.Record((string)dataPointGroup.Text);
                recorder.Record((int)dataPointGroup.Top);
                recorder.Record((bool)dataPointGroup.TopLevel);
                recorder.Record((bool)dataPointGroup.TopMost);
                recorder.Record((bool)dataPointGroup.UseWaitCursor);
                recorder.Record((string)dataPointGroup.ViewID);
                recorder.Record((string)dataPointGroup.ViewType);
                recorder.Record((bool)dataPointGroup.Visible);
                recorder.Record((int)dataPointGroup.Width);
                recorder.Record((FormWindowState)dataPointGroup.WindowState);
                recorder.FinishRecording();
                #endregion
                #region Assertions
                Assert.IsNull((string)dataPointGroup.AccessibleDefaultActionDescription);
                Assert.IsFalse((bool)dataPointGroup.AllowDrop);
                Assert.AreEqual(global::System.Windows.Forms.AutoScaleMode.Font, (AutoScaleMode)dataPointGroup.AutoScaleMode);
                Assert.AreEqual(global::System.Windows.Forms.AutoValidate.EnablePreventFocusChange, (AutoValidate)dataPointGroup.AutoValidate);
                Assert.IsFalse((bool)dataPointGroup.CanFocus);
                Assert.AreEqual("ST Electronics Ltd", (string)dataPointGroup.CompanyName);
                Assert.IsFalse((bool)dataPointGroup.Created);
                Assert.IsTrue((bool)dataPointGroup.Enabled);
                Assert.IsTrue((bool)dataPointGroup.HasChildren);
                Assert.IsFalse((bool)dataPointGroup.InvokeRequired);
                Assert.IsFalse((bool)dataPointGroup.IsHandleCreated);
                Assert.IsFalse((bool)dataPointGroup.IsRestrictedWindow);
                Assert.IsFalse((bool)dataPointGroup.MaximizeBox);
                Assert.AreEqual(1.0, (double)dataPointGroup.Opacity, 1E-7);
                Assert.IsFalse((bool)dataPointGroup.RecreatingHandle);
                Assert.IsTrue((bool)dataPointGroup.ShowIcon);
                Assert.AreEqual(global::System.Windows.Forms.FormStartPosition.CenterScreen, (FormStartPosition)dataPointGroup.StartPosition);
                Assert.AreEqual("Load Data Configuration", (string)dataPointGroup.Text);
                Assert.IsFalse((bool)dataPointGroup.TopMost);
                Assert.IsFalse((bool)dataPointGroup.Visible);
                #endregion
            }
        }

    }
}
