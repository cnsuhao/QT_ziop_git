using Dottest.Framework;
using Dottest.Framework.RecordState;
using NUnit.Framework;
using System;
using System.Windows.Forms;
using TrendViewer.View;

namespace TrendViewer.DotTest.Factories.View
{
    [TestFixture(), ObjectFactoryType()]
    public class MarkerDataFactory
    {
        ///<summary>A caller is responsible for disposing the result</summary>
        [ObjectFactoryMethod]
        [HashCode("+U1Fsg", "/5NYUA")]
        public static MarkerData CreateMarkerData01()
        {
            MarkerData markerData = new MarkerData();
            return markerData;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateMarkerData01()
        {
            using (MarkerData markerData = CreateMarkerData01())
            {
                Assert.IsNotNull(markerData);
                #region Record State
                ValueRecorder recorder = new ValueRecorder();
                recorder.Record((string)markerData.AccessibleDefaultActionDescription);
                recorder.Record((string)markerData.AccessibleDescription);
                recorder.Record((string)markerData.AccessibleName);
                recorder.Record((AccessibleRole)markerData.AccessibleRole);
                recorder.Record((bool)markerData.AllowDrop);
                recorder.Record((bool)markerData.AllowTransparency);
                recorder.Record((AnchorStyles)markerData.Anchor);
                recorder.Record((AutoScaleMode)markerData.AutoScaleMode);
                recorder.Record((bool)markerData.AutoScroll);
                recorder.Record((bool)markerData.AutoSize);
                recorder.Record((AutoSizeMode)markerData.AutoSizeMode);
                recorder.Record((AutoValidate)markerData.AutoValidate);
                recorder.Record((ImageLayout)markerData.BackgroundImageLayout);
                recorder.Record((int)markerData.Bottom);
                recorder.Record((bool)markerData.CanFocus);
                recorder.Record((bool)markerData.CanSelect);
                recorder.Record((bool)markerData.Capture);
                recorder.Record((bool)markerData.CausesValidation);
                recorder.Record((string)markerData.CompanyName);
                recorder.Record((bool)markerData.ContainsFocus);
                recorder.Record((bool)markerData.ControlBox);
                recorder.Record((bool)markerData.Created);
                recorder.Record((DialogResult)markerData.DialogResult);
                recorder.Record((bool)markerData.Disposing);
                recorder.Record((DockStyle)markerData.Dock);
                recorder.Record((bool)markerData.Enabled);
                recorder.Record((bool)markerData.Focused);
                recorder.Record((FormBorderStyle)markerData.FormBorderStyle);
                recorder.Record((bool)markerData.HasChildren);
                recorder.Record((int)markerData.Height);
                recorder.Record((bool)markerData.HelpButton);
                recorder.Record((ImeMode)markerData.ImeMode);
                recorder.Record((bool)markerData.InvokeRequired);
                recorder.Record((bool)markerData.IsAccessible);
                recorder.Record((bool)markerData.IsDisposed);
                recorder.Record((bool)markerData.IsHandleCreated);
                recorder.Record((bool)markerData.IsMdiChild);
                recorder.Record((bool)markerData.IsMdiContainer);
                recorder.Record((bool)markerData.IsMirrored);
                recorder.Record((bool)markerData.IsRestrictedWindow);
                recorder.Record((bool)markerData.KeyPreview);
                recorder.Record((int)markerData.Left);
                recorder.Record((bool)markerData.MaximizeBox);
                recorder.Record((bool)markerData.MinimizeBox);
                recorder.Record((bool)markerData.Modal);
                recorder.Record((string)markerData.Name);
                recorder.Record((double)markerData.Opacity);
                recorder.Record((string)markerData.ProductName);
                recorder.Record((string)markerData.ProductVersion);
                recorder.Record((bool)markerData.RecreatingHandle);
                recorder.Record((int)markerData.Right);
                recorder.Record((RightToLeft)markerData.RightToLeft);
                recorder.Record((bool)markerData.RightToLeftLayout);
                recorder.Record((bool)markerData.ShowIcon);
                recorder.Record((bool)markerData.ShowInTaskbar);
                recorder.Record((SizeGripStyle)markerData.SizeGripStyle);
                recorder.Record((FormStartPosition)markerData.StartPosition);
                recorder.Record((bool)ReflectionAccessor.Wrap(markerData).GetProperty("System.Windows.Forms.Layout.IArrangedElement.ParticipatesInLayout"));
                recorder.Record((int)markerData.TabIndex);
                recorder.Record((bool)markerData.TabStop);
                recorder.Record((string)markerData.Text);
                recorder.Record((int)markerData.Top);
                recorder.Record((bool)markerData.TopLevel);
                recorder.Record((bool)markerData.TopMost);
                recorder.Record((bool)markerData.UseWaitCursor);
                recorder.Record((string)markerData.ViewID);
                recorder.Record((string)markerData.ViewType);
                recorder.Record((bool)markerData.Visible);
                recorder.Record((int)markerData.Width);
                recorder.Record((FormWindowState)markerData.WindowState);
                recorder.FinishRecording();
                #endregion
                #region Assertions
                Assert.IsNull((string)markerData.AccessibleDefaultActionDescription);
                Assert.IsFalse((bool)markerData.AllowDrop);
                Assert.AreEqual(global::System.Windows.Forms.AutoScaleMode.Font, (AutoScaleMode)markerData.AutoScaleMode);
                Assert.AreEqual(global::System.Windows.Forms.AutoValidate.EnablePreventFocusChange, (AutoValidate)markerData.AutoValidate);
                Assert.IsFalse((bool)markerData.CanFocus);
                Assert.AreEqual("ST Electronics Ltd", (string)markerData.CompanyName);
                Assert.IsFalse((bool)markerData.Created);
                Assert.IsTrue((bool)markerData.Enabled);
                Assert.IsTrue((bool)markerData.HasChildren);
                Assert.IsFalse((bool)markerData.InvokeRequired);
                Assert.IsFalse((bool)markerData.IsHandleCreated);
                Assert.IsFalse((bool)markerData.IsRestrictedWindow);
                Assert.IsFalse((bool)markerData.MaximizeBox);
                Assert.AreEqual(1.0, (double)markerData.Opacity, 1E-7);
                Assert.IsFalse((bool)markerData.RecreatingHandle);
                Assert.IsTrue((bool)markerData.ShowIcon);
                Assert.AreEqual(global::System.Windows.Forms.FormStartPosition.CenterScreen, (FormStartPosition)markerData.StartPosition);
                Assert.AreEqual("Edit Data Marker", (string)markerData.Text);
                Assert.IsFalse((bool)markerData.TopMost);
                Assert.IsFalse((bool)markerData.Visible);
                #endregion
            }
        }

    }
}
