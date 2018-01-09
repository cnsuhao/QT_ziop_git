using Dottest.Framework;
using Dottest.Framework.RecordState;
using NUnit.Framework;
using OPCDataLogger;
using System;
using System.Windows.Forms;

namespace OPCDataLogger.DotTest.Factories
{
    [TestFixture(), ObjectFactoryType()]
    public class OPCDataLoggerFactory
    {
        ///<summary>A caller is responsible for disposing the result</summary>
        [ObjectFactoryMethod]
        [HashCode("/CpCJA", "+3wuqw")]
        public static OPCDataLogger CreateOPCDataLogger01()
        {
            OPCDataLogger oPCDataLogger = new OPCDataLogger();
            return oPCDataLogger;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateOPCDataLogger01()
        {
            using (OPCDataLogger oPCDataLogger = CreateOPCDataLogger01())
            {
                Assert.IsNotNull(oPCDataLogger);
                #region Record State
                ValueRecorder recorder = new ValueRecorder();
                recorder.Record((string)oPCDataLogger.AccessibleDefaultActionDescription);
                recorder.Record((string)oPCDataLogger.AccessibleDescription);
                recorder.Record((string)oPCDataLogger.AccessibleName);
                recorder.Record((AccessibleRole)oPCDataLogger.AccessibleRole);
                recorder.Record((bool)oPCDataLogger.AllowDrop);
                recorder.Record((bool)oPCDataLogger.AllowTransparency);
                recorder.Record((AnchorStyles)oPCDataLogger.Anchor);
                recorder.Record((AutoScaleMode)oPCDataLogger.AutoScaleMode);
                recorder.Record((bool)oPCDataLogger.AutoScroll);
                recorder.Record((bool)oPCDataLogger.AutoSize);
                recorder.Record((AutoSizeMode)oPCDataLogger.AutoSizeMode);
                recorder.Record((AutoValidate)oPCDataLogger.AutoValidate);
                recorder.Record((ImageLayout)oPCDataLogger.BackgroundImageLayout);
                recorder.Record((int)oPCDataLogger.Bottom);
                recorder.Record((bool)oPCDataLogger.CanFocus);
                recorder.Record((bool)oPCDataLogger.CanSelect);
                recorder.Record((bool)oPCDataLogger.Capture);
                recorder.Record((bool)oPCDataLogger.CausesValidation);
                recorder.Record((string)oPCDataLogger.CompanyName);
                recorder.Record((bool)oPCDataLogger.ContainsFocus);
                recorder.Record((bool)oPCDataLogger.ControlBox);
                recorder.Record((bool)oPCDataLogger.Created);
                recorder.Record((DialogResult)oPCDataLogger.DialogResult);
                recorder.Record((bool)oPCDataLogger.Disposing);
                recorder.Record((DockStyle)oPCDataLogger.Dock);
                recorder.Record((bool)oPCDataLogger.Enabled);
                recorder.Record((bool)oPCDataLogger.Focused);
                recorder.Record((FormBorderStyle)oPCDataLogger.FormBorderStyle);
                recorder.Record((bool)oPCDataLogger.HasChildren);
                recorder.Record((int)oPCDataLogger.Height);
                recorder.Record((bool)oPCDataLogger.HelpButton);
                recorder.Record((ImeMode)oPCDataLogger.ImeMode);
                recorder.Record((bool)oPCDataLogger.InvokeRequired);
                recorder.Record((bool)oPCDataLogger.IsAccessible);
                recorder.Record((bool)oPCDataLogger.IsDisposed);
                recorder.Record((bool)oPCDataLogger.IsHandleCreated);
                recorder.Record((bool)oPCDataLogger.IsMdiChild);
                recorder.Record((bool)oPCDataLogger.IsMdiContainer);
                recorder.Record((bool)oPCDataLogger.IsMirrored);
                recorder.Record((bool)oPCDataLogger.IsRestrictedWindow);
                recorder.Record((bool)oPCDataLogger.KeyPreview);
                recorder.Record((int)oPCDataLogger.Left);
                recorder.Record((bool)oPCDataLogger.MaximizeBox);
                recorder.Record((bool)oPCDataLogger.MinimizeBox);
                recorder.Record((bool)oPCDataLogger.Modal);
                recorder.Record((string)oPCDataLogger.Name);
                recorder.Record((double)oPCDataLogger.Opacity);
                recorder.Record((string)oPCDataLogger.ProductName);
                recorder.Record((string)oPCDataLogger.ProductVersion);
                recorder.Record((bool)oPCDataLogger.RecreatingHandle);
                recorder.Record((int)oPCDataLogger.Right);
                recorder.Record((RightToLeft)oPCDataLogger.RightToLeft);
                recorder.Record((bool)oPCDataLogger.RightToLeftLayout);
                recorder.Record((bool)oPCDataLogger.ShowIcon);
                recorder.Record((bool)oPCDataLogger.ShowInTaskbar);
                recorder.Record((SizeGripStyle)oPCDataLogger.SizeGripStyle);
                recorder.Record((FormStartPosition)oPCDataLogger.StartPosition);
                recorder.Record((bool)ReflectionAccessor.Wrap(oPCDataLogger).GetProperty("System.Windows.Forms.Layout.IArrangedElement.ParticipatesInLayout"));
                recorder.Record((int)oPCDataLogger.TabIndex);
                recorder.Record((bool)oPCDataLogger.TabStop);
                recorder.Record((string)oPCDataLogger.Text);
                recorder.Record((int)oPCDataLogger.Top);
                recorder.Record((bool)oPCDataLogger.TopLevel);
                recorder.Record((bool)oPCDataLogger.TopMost);
                recorder.Record((bool)oPCDataLogger.UseWaitCursor);
                recorder.Record((bool)oPCDataLogger.Visible);
                recorder.Record((int)oPCDataLogger.Width);
                recorder.Record((FormWindowState)oPCDataLogger.WindowState);
                recorder.FinishRecording();
                #endregion
            }
        }

    }
}
