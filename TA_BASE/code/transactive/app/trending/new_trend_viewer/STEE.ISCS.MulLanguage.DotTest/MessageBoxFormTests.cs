using Dottest.Framework;
using Dottest.Framework.RecordState;
using NUnit.Framework;
using System;
using STEE.ISCS.MulLanguage;
using System.Windows.Forms;
using System.Drawing;

namespace STEE.ISCS.MulLanguage.DotTest
{
    [TestFixture()]
    public class MessageBoxFormTests
    {
        [TearDown]
        public void FixtureTearDown()
        {

        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("/UNeBA", "/Tsl2Q")]
        public void TestDispose01()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            bool disposing = true;
            messageBoxAccessor.SetField("components", new System.ComponentModel.Container());
            messageBoxAccessor.Call("Dispose", disposing);
        }
        
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("/SdNJQ", "/n30LA")]
        public void TestMessageBeep01()
        {
            uint type = uint.MaxValue;
            StaticAccessor messageBoxFormAccessor = ReflectionAccessor.Wrap(typeof(MessageBoxForm));
            bool b = (bool)messageBoxFormAccessor.Call("MessageBeep", type);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("/SdNJQ", "+WKUSw")]
        public void TestMessageBeep02()
        {
            uint type = 0;
            StaticAccessor messageBoxFormAccessor = ReflectionAccessor.Wrap(typeof(MessageBoxForm));
            bool b = (bool)messageBoxFormAccessor.Call("MessageBeep", type);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("/PoCXQ", "+MK9Pg")]
        public void TestMessageBoxFormConstructor01()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
           Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
           messageBoxAccessor.Call("CleanUp");
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildMessageBox01()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            messageBoxAccessor.SetField("m_icon", SystemIcons.Error);
            messageBoxForm.BuildButtons(MessageBoxButtons.OKCancel);
            messageBoxForm.BuildMessageBox("SampleMessageBox","Hi!!",2,true);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildMessageBox02()
        {
            //test - 3 buttons
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            messageBoxAccessor.SetField("m_icon", SystemIcons.Error);
            messageBoxForm.BuildButtons(MessageBoxButtons.AbortRetryIgnore);
            messageBoxForm.BuildMessageBox("SampleMessageBox", "Hi!!", 3, true);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildMessageBox03()
        {
            //Test 1 button and no icons
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            messageBoxForm.BuildButtons(MessageBoxButtons.OK);
            messageBoxForm.BuildMessageBox("SampleMessageBox", "Hi!!", 1, false);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildMessageBox04()
        {
            //Test exception thrown
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            messageBoxForm.BuildButtons(MessageBoxButtons.OK);
            messageBoxForm.BuildMessageBox("SampleMessageBox", "Hi!!", 1, false);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestDisableCloseIfMultipleButtonsAndNoCancelButton01()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            messageBoxAccessor.SetField("m_button2", new Button());
            messageBoxAccessor.SetField("m_button1", new Button());
            messageBoxAccessor.GetFieldAccessor("m_button1").SetProperty("Tag", 1);
            //test --- exception throw
            messageBoxAccessor.Call("DisableCloseIfMultipleButtonsAndNoCancelButton");
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildMessageBox05()
        {
            //Test --- long display name
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            messageBoxForm.BuildButtons(MessageBoxButtons.OK);
            messageBoxForm.BuildMessageBox("Sample Test","long display test 21ebufe873487bnvufr7y843ythg878738294019240ujf9ery8gt93853845tjrgf98y9358123-18024019u030jf89r8948597yrhf89734589349cdlotrhig89595viufr8yvhbasijodiq29384923850nver789t3459760894309jvniofugh948956865089234cvm90c57-27u8rtu8345ytnog8y35t05y8u1-8-431nc4r8051-8mctyhwerhg875078-51-yjcn589mc8694jcxcm7yy7huahfrr89thunncuhiqy89014804178-517yuhjw8o7y9025870257uhifvrwohbv789057180808080808080808080808080808080808080808080gh8r7fghhhhhhhhhhhhhhhhhhhhhhhhhhwotyr895872222222000000000hv8eghb8787", 1, false);            
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestShowButton01()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            messageBoxAccessor.Call("ShowButton", new Button(), "OK", 1);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCalculateButtons01()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test procedure Call
            int numberOfButtons = (int)messageBoxAccessor.Call("CalculateButtons", MessageBoxButtons.OKCancel);
            //Post Condition Check
            Assert.AreEqual(2, numberOfButtons);

            //test --- 
            //Test procedure Call
            numberOfButtons = (int)messageBoxAccessor.Call("CalculateButtons", MessageBoxButtons.OK);
            //Post Condition Check
            Assert.AreEqual(1, numberOfButtons);

            //test --- 
            //Test procedure Call
            numberOfButtons = (int)messageBoxAccessor.Call("CalculateButtons", MessageBoxButtons.AbortRetryIgnore);
            //Post Condition Check
            Assert.AreEqual(3, numberOfButtons);

            //test --- 
            //Test procedure Call
            numberOfButtons = (int)messageBoxAccessor.Call("CalculateButtons", (MessageBoxButtons)99);
            //Post Condition Check
            Assert.AreEqual(1, numberOfButtons);



        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildIcon01()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildIcon(MessageBoxIcon.Asterisk);
            //Post Condition Check
            Assert.AreEqual(SystemIcons.Asterisk, messageBoxAccessor.GetField("m_icon"));
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildIcon02()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildIcon(MessageBoxIcon.Error);
            //Post Condition Check
            Assert.AreEqual(SystemIcons.Error, messageBoxAccessor.GetField("m_icon"));
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildIcon03()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildIcon(MessageBoxIcon.Exclamation);
            //Post Condition Check
            Assert.AreEqual(SystemIcons.Exclamation, messageBoxAccessor.GetField("m_icon"));
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildIcon04()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildIcon(MessageBoxIcon.Hand);
            //Post Condition Check
            Assert.AreEqual(SystemIcons.Error, messageBoxAccessor.GetField("m_icon"));
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildIcon05()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildIcon(MessageBoxIcon.Information);
            //Post Condition Check
            Assert.AreEqual(SystemIcons.Asterisk, messageBoxAccessor.GetField("m_icon"));
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildIcon06()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildIcon(MessageBoxIcon.None);
            //Post Condition Check
           Assert.IsNull(messageBoxAccessor.GetField("m_icon"));
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildIcon07()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildIcon(MessageBoxIcon.Question);
            //Post Condition Check
            Assert.AreEqual(SystemIcons.Question, messageBoxAccessor.GetField("m_icon"));
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildIcon08()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildIcon(MessageBoxIcon.Stop);
            //Post Condition Check
            Assert.AreEqual(SystemIcons.Error, messageBoxAccessor.GetField("m_icon"));
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildIcon09()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildIcon(MessageBoxIcon.Warning);
            //Post Condition Check
            Assert.AreEqual(SystemIcons.Exclamation, messageBoxAccessor.GetField("m_icon"));
        }

        [ExpectedException(typeof(System.ComponentModel.InvalidEnumArgumentException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildIcon10()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildIcon((MessageBoxIcon)66);
            //Post Condition Check
            
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildButtons01()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildButtons(MessageBoxButtons.OKCancel);
            //Post Condition Check
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button1"));
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button2"));
            Assert.IsNull(messageBoxAccessor.GetField("m_button3"));
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildButtons02()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildButtons(MessageBoxButtons.OK);
            //Post Condition Check
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button1"));
            Assert.IsNull(messageBoxAccessor.GetField("m_button2"));
            Assert.IsNull(messageBoxAccessor.GetField("m_button3"));
        }



        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildButtons03()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildButtons(MessageBoxButtons.AbortRetryIgnore);
            //Post Condition Check
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button1"));
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button2"));
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button3"));
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildButtons04()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildButtons(MessageBoxButtons.RetryCancel);
            //Post Condition Check
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button1"));
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button2"));
            Assert.IsNull(messageBoxAccessor.GetField("m_button3"));
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildButtons05()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildButtons(MessageBoxButtons.YesNo);
            //Post Condition Check
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button1"));
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button2"));
            Assert.IsNull(messageBoxAccessor.GetField("m_button3"));
        }



        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildButtons06()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildButtons(MessageBoxButtons.YesNoCancel);
            //Post Condition Check
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button1"));
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button2"));
            Assert.IsNotNull(messageBoxAccessor.GetField("m_button3"));
        }

        [ExpectedException(typeof(System.ComponentModel.InvalidEnumArgumentException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildButtons07()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            //Test Procedure Call
            messageBoxForm.BuildButtons((MessageBoxButtons)44);
            //Post Condition Check
            Assert.IsNull(messageBoxAccessor.GetField("m_button1"));
            Assert.IsNull(messageBoxAccessor.GetField("m_button2"));
            Assert.IsNull(messageBoxAccessor.GetField("m_button3"));
        }   

        [ExpectedException(typeof(System.ComponentModel.InvalidEnumArgumentException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestSelectDefaultButton01()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            messageBoxAccessor.SetField("m_button3", new Button());
            messageBoxAccessor.SetField("m_button2", new Button());
            messageBoxAccessor.SetField("m_button1", new Button());
            //test Procedure Call
            messageBoxForm.SelectDefaultButton(MessageBoxDefaultButton.Button2);
            //test Procedure Call
            messageBoxForm.SelectDefaultButton(MessageBoxDefaultButton.Button1);
            //test Procedure Call
            messageBoxForm.SelectDefaultButton(MessageBoxDefaultButton.Button3);


            ///testing scenario
            messageBoxAccessor.SetField("m_button3", null);
            //test Procedure Call
            messageBoxForm.SelectDefaultButton(MessageBoxDefaultButton.Button2);
            //test Procedure Call
            messageBoxForm.SelectDefaultButton(MessageBoxDefaultButton.Button1);

            ///testing scenario
            messageBoxAccessor.SetField("m_button2", null);
            //test Procedure Call
            messageBoxForm.SelectDefaultButton(MessageBoxDefaultButton.Button1);

            //default case testing
            messageBoxForm.SelectDefaultButton((MessageBoxDefaultButton)444);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void Testbtn_Click01()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            Accessor messageBoxAccessor = ReflectionAccessor.Wrap(messageBoxForm);
            Button senderbutton = new Button();
            senderbutton.Tag = 1;
            //Test Procedure Call
            messageBoxAccessor.Call("btn_Click", senderbutton,null);
            //Post Condition Check
            Assert.AreEqual((DialogResult)senderbutton.Tag,messageBoxAccessor.GetField("m_ReturnButton"));

        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestGetResultButton01()
        {
            MessageBoxForm messageBoxForm = new MessageBoxForm();
            messageBoxForm.GetResultButton();
        }


    }
}
