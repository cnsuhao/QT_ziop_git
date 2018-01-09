using Dottest.Framework;
using Dottest.Framework.RecordState;
using NUnit.Framework;
using OPCSampleGrpConfig.View;
using System;
using System.Windows.Forms;

namespace OPCSampleGrpConfig.DotTest.Factories.View
{
    [TestFixture(), ObjectFactoryType()]
    public class OPCSampleGrpConfigStartFactory
    {
        ///<summary>A caller is responsible for disposing the result</summary>
        [ObjectFactoryMethod]
        [HashCode("+9jAiQ", "/Glsgw")]
        public static OPCSampleGrpConfigStart CreateOPCSampleGrpConfigStart01()
        {
            OPCSampleGrpConfigStart oPCSampleGrpConfigStart = new OPCSampleGrpConfigStart();
            return oPCSampleGrpConfigStart;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateOPCSampleGrpConfigStart01()
        {
            using (OPCSampleGrpConfigStart oPCSampleGrpConfigStart = CreateOPCSampleGrpConfigStart01())
            {
                Assert.IsNotNull(oPCSampleGrpConfigStart);
                #region Record State
                ValueRecorder recorder = new ValueRecorder();
                recorder.Record((string)oPCSampleGrpConfigStart.AccessibleDefaultActionDescription);
                recorder.Record((string)oPCSampleGrpConfigStart.AccessibleDescription);
                recorder.Record((string)oPCSampleGrpConfigStart.AccessibleName);
                recorder.Record((AccessibleRole)oPCSampleGrpConfigStart.AccessibleRole);
                recorder.Record((bool)oPCSampleGrpConfigStart.AllowDrop);
                recorder.Record((bool)oPCSampleGrpConfigStart.AllowTransparency);
                recorder.Record((AnchorStyles)oPCSampleGrpConfigStart.Anchor);
                recorder.Record((AutoScaleMode)oPCSampleGrpConfigStart.AutoScaleMode);
                recorder.Record((bool)oPCSampleGrpConfigStart.AutoScroll);
                recorder.Record((bool)oPCSampleGrpConfigStart.AutoSize);
                recorder.Record((AutoSizeMode)oPCSampleGrpConfigStart.AutoSizeMode);
                recorder.Record((AutoValidate)oPCSampleGrpConfigStart.AutoValidate);
                recorder.Record((ImageLayout)oPCSampleGrpConfigStart.BackgroundImageLayout);
                recorder.Record((int)oPCSampleGrpConfigStart.Bottom);
                recorder.Record((bool)oPCSampleGrpConfigStart.CanFocus);
                recorder.Record((bool)oPCSampleGrpConfigStart.CanSelect);
                recorder.Record((bool)oPCSampleGrpConfigStart.Capture);
                recorder.Record((bool)oPCSampleGrpConfigStart.CausesValidation);
                recorder.Record((string)oPCSampleGrpConfigStart.CompanyName);
                recorder.Record((bool)oPCSampleGrpConfigStart.ContainsFocus);
                recorder.Record((bool)oPCSampleGrpConfigStart.ControlBox);
                recorder.Record((bool)oPCSampleGrpConfigStart.Created);
                recorder.Record((DialogResult)oPCSampleGrpConfigStart.DialogResult);
                recorder.Record((bool)oPCSampleGrpConfigStart.Disposing);
                recorder.Record((DockStyle)oPCSampleGrpConfigStart.Dock);
                recorder.Record((bool)oPCSampleGrpConfigStart.Enabled);
                recorder.Record((bool)oPCSampleGrpConfigStart.Focused);
                recorder.Record((FormBorderStyle)oPCSampleGrpConfigStart.FormBorderStyle);
                recorder.Record((bool)oPCSampleGrpConfigStart.HasChildren);
                recorder.Record((int)oPCSampleGrpConfigStart.Height);
                recorder.Record((bool)oPCSampleGrpConfigStart.HelpButton);
                recorder.Record((ImeMode)oPCSampleGrpConfigStart.ImeMode);
                recorder.Record((bool)oPCSampleGrpConfigStart.InvokeRequired);
                recorder.Record((bool)oPCSampleGrpConfigStart.IsAccessible);
                recorder.Record((bool)oPCSampleGrpConfigStart.IsDisposed);
                recorder.Record((bool)oPCSampleGrpConfigStart.IsHandleCreated);
                recorder.Record((bool)oPCSampleGrpConfigStart.IsMdiChild);
                recorder.Record((bool)oPCSampleGrpConfigStart.IsMdiContainer);
                recorder.Record((bool)oPCSampleGrpConfigStart.IsMirrored);
                recorder.Record((bool)oPCSampleGrpConfigStart.IsRestrictedWindow);
                recorder.Record((bool)oPCSampleGrpConfigStart.KeyPreview);
                recorder.Record((int)oPCSampleGrpConfigStart.Left);
                recorder.Record((bool)oPCSampleGrpConfigStart.MaximizeBox);
                recorder.Record((bool)oPCSampleGrpConfigStart.MinimizeBox);
                recorder.Record((bool)oPCSampleGrpConfigStart.Modal);
                recorder.Record((string)oPCSampleGrpConfigStart.Name);
                recorder.Record((double)oPCSampleGrpConfigStart.Opacity);
                recorder.Record((string)oPCSampleGrpConfigStart.ProductName);
                recorder.Record((string)oPCSampleGrpConfigStart.ProductVersion);
                recorder.Record((bool)oPCSampleGrpConfigStart.RecreatingHandle);
                recorder.Record((int)oPCSampleGrpConfigStart.Right);
                recorder.Record((RightToLeft)oPCSampleGrpConfigStart.RightToLeft);
                recorder.Record((bool)oPCSampleGrpConfigStart.RightToLeftLayout);
                recorder.Record((bool)oPCSampleGrpConfigStart.ShowIcon);
                recorder.Record((bool)oPCSampleGrpConfigStart.ShowInTaskbar);
                recorder.Record((SizeGripStyle)oPCSampleGrpConfigStart.SizeGripStyle);
                recorder.Record((FormStartPosition)oPCSampleGrpConfigStart.StartPosition);
                recorder.Record((bool)ReflectionAccessor.Wrap(oPCSampleGrpConfigStart).GetProperty("System.Windows.Forms.Layout.IArrangedElement.ParticipatesInLayout"));
                recorder.Record((int)oPCSampleGrpConfigStart.TabIndex);
                recorder.Record((bool)oPCSampleGrpConfigStart.TabStop);
                recorder.Record((string)oPCSampleGrpConfigStart.Text);
                recorder.Record((int)oPCSampleGrpConfigStart.Top);
                recorder.Record((bool)oPCSampleGrpConfigStart.TopLevel);
                recorder.Record((bool)oPCSampleGrpConfigStart.TopMost);
                recorder.Record((bool)oPCSampleGrpConfigStart.UseWaitCursor);
                recorder.Record((string)oPCSampleGrpConfigStart.ViewID);
                recorder.Record((string)oPCSampleGrpConfigStart.ViewType);
                recorder.Record((bool)oPCSampleGrpConfigStart.Visible);
                recorder.Record((int)oPCSampleGrpConfigStart.Width);
                recorder.Record((FormWindowState)oPCSampleGrpConfigStart.WindowState);
                recorder.FinishRecording();
                #endregion
                #region Assertions
                Assert.IsNull((string)oPCSampleGrpConfigStart.AccessibleDefaultActionDescription);
                Assert.IsFalse((bool)oPCSampleGrpConfigStart.AllowDrop);
                Assert.AreEqual(global::System.Windows.Forms.AutoScaleMode.Font, (AutoScaleMode)oPCSampleGrpConfigStart.AutoScaleMode);
                Assert.AreEqual(global::System.Windows.Forms.AutoValidate.EnablePreventFocusChange, (AutoValidate)oPCSampleGrpConfigStart.AutoValidate);
                Assert.IsFalse((bool)oPCSampleGrpConfigStart.CanFocus);
                Assert.AreEqual("ST Electronics Ltd", (string)oPCSampleGrpConfigStart.CompanyName);
                Assert.IsFalse((bool)oPCSampleGrpConfigStart.Created);
                Assert.IsTrue((bool)oPCSampleGrpConfigStart.Enabled);
                Assert.IsTrue((bool)oPCSampleGrpConfigStart.HasChildren);
                Assert.IsFalse((bool)oPCSampleGrpConfigStart.InvokeRequired);
                Assert.IsFalse((bool)oPCSampleGrpConfigStart.IsHandleCreated);
                Assert.IsFalse((bool)oPCSampleGrpConfigStart.IsRestrictedWindow);
                Assert.IsFalse((bool)oPCSampleGrpConfigStart.MaximizeBox);
                Assert.AreEqual(1.0, (double)oPCSampleGrpConfigStart.Opacity, 1E-7);
                Assert.IsFalse((bool)oPCSampleGrpConfigStart.RecreatingHandle);
                Assert.IsTrue((bool)oPCSampleGrpConfigStart.ShowIcon);
                Assert.AreEqual(global::System.Windows.Forms.FormStartPosition.CenterParent, (FormStartPosition)oPCSampleGrpConfigStart.StartPosition);
                Assert.AreEqual("OPC DataLogger Configuration", (string)oPCSampleGrpConfigStart.Text);
                Assert.IsFalse((bool)oPCSampleGrpConfigStart.TopMost);
                Assert.IsFalse((bool)oPCSampleGrpConfigStart.Visible);
                #endregion
            }
        }

    }
}